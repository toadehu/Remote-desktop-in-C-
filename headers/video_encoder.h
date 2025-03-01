#pragma once

#include <stdint.h>

typedef unsigned char byte;

#ifdef __AVX2__
#include <immintrin.h>

#endif

#ifdef HAVE_X86
#include <emmintrin.h>
#include <immintrin.h>

/*I think this does NOT work*/
void ARGBToYUV420_SSE2 (char **argb, int width, int height, char **yuv) {
    int plane_size = width * height;
    char *y_plane = *yuv;
    char *u_plane = *yuv + plane_size;
    char *v_plane = *yuv + plane_size + (plane_size >> 2);
    char *orig_argb_ptr = *argb;
    int argb_index = 0;

    const __m128i shuffle_mask = _mm_set_epi8(15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
    const __m128i y_coeffs = _mm_set_epi16(0, 25, 129, 66, 0, 25, 129, 66);
    const __m128i u_coeffs = _mm_set_epi16(0, 112, -74, -38, 0, 112, -74, -38);
    const __m128i v_coeffs = _mm_set_epi16(0, -18, -94, 112, 0, -18, -94, 112);
    const __m128i y_offset = _mm_set1_epi16(16);
    const __m128i uv_offset = _mm_set1_epi16(128);

    int i,j;
    for (j = 0; j < height; j+=1) {
        /*printf("Current index:%d\n", argb_index);*/
        for (i = 0; i < width; i += 8) { /* Process 8 pixels per iteration*/
            /*printf("precrisis, addres where segfault happens is:%d or it may be:%d\n", argb_index + (i + 0), argb_index + (i + 4));*/
            /*printf("actual offset should be:%d, actual size: %d\n", j * width * 4 + i + 4, width * height * 4);*/
            __m128i argb0 = _mm_lddqu_si128((__m128i *)(*argb + argb_index + i));
            __m128i argb1 = _mm_lddqu_si128((__m128i *)(*argb + argb_index +  i + 4));

            /* Shuffle RGB values*/
            __m128i b0g0r0a0 = _mm_shuffle_epi8(argb0, shuffle_mask);
            __m128i b1g1r1a1 = _mm_shuffle_epi8(argb1, shuffle_mask);

            /* Calculate Y, U, V values*/
            __m128i y0 = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, y_coeffs), y_offset), 8);
            __m128i y1 = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b1g1r1a1, y_coeffs), y_offset), 8);
            __m128i u = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, u_coeffs), uv_offset), 8);
            __m128i v = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, v_coeffs), uv_offset), 8);

            /* Pack Y values and store them*/
            _mm_storeu_si128((__m128i *)(y_plane + i), _mm_packus_epi16(y0, y1));

            /* Handle chroma subsampling*/
            if (j % 2 == 0 && i % 16 == 0) {
                u = _mm_packus_epi16(u, u);
                v = _mm_packus_epi16(v, v);

                /* Average U and V values for subsampling*/
                __m128i uv_avg = _mm_avg_epu8(u, _mm_shuffle_epi32(u, _MM_SHUFFLE(1, 0, 3, 2)));
                __m128i vv_avg = _mm_avg_epu8(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));

                /* Store U and V values*/
                _mm_storel_epi64((__m128i *)(u_plane + (i >> 1)), uv_avg);
                _mm_storel_epi64((__m128i *)(v_plane + (i >> 1)), vv_avg);
            }
        }
        argb_index += (width * 4);
        /**argb += (width << 1);*/
        y_plane += width;
        if (!(j&1)) {
            u_plane += (width / 2);
            v_plane += (width / 2);
        }
    }
    *argb = orig_argb_ptr;

    printf("Total theortical size:%d\n", width * height * 4);
}

void ARGBToYUV420_AVX2(char **argb, int width, int height, char **yuv) {
    int plane_size = width * height;
    char *y_plane = *yuv;
    char *u_plane = *yuv + plane_size;
    char *v_plane = *yuv + plane_size + (plane_size >> 2);

    int argb_index = 0;

    const __m256i shuffle_mask = _mm256_set_epi8(15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
    const __m256i y_coeffs = _mm256_set_epi16(0, 25, 129, 66, 0, 25, 129, 66, 0, 25, 129, 66, 0, 25, 129, 66);
    const __m256i u_coeffs = _mm256_set_epi16(0, 112, -74, -38, 0, 112, -74, -38, 0, 112, -74, -38, 0, 112, -74, -38);
    const __m256i v_coeffs = _mm256_set_epi16(0, -18, -94, 112, 0, -18, -94, 112, 0, -18, -94, 112, 0, -18, -94, 112);
    const __m256i y_offset = _mm256_set1_epi16(16);
    const __m256i uv_offset = _mm256_set1_epi16(128);

    /* Set the lower (0) and upper (255) bounds*/
    __m256i lower_bound = _mm256_set1_epi8(0);
    __m256i upper_bound = _mm256_set1_epi8(255);

    int i,j;
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i += 16) { /* Process 16 pixels per iteration*/
            __m256i argb0 = _mm256_lddqu_si256((__m256i *)(*argb + argb_index + (i + 0)));
            __m256i argb1 = _mm256_lddqu_si256((__m256i *)(*argb + argb_index + (i + 8)));

            /* Shuffle RGB values*/
            __m256i b0g0r0a0 = _mm256_shuffle_epi8(argb0, shuffle_mask);
            __m256i b1g1r1a1 = _mm256_shuffle_epi8(argb1, shuffle_mask);

            /* Calculate Y, U, V values*/
             __m256i y0 = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, y_coeffs), y_offset), 8);
            __m256i y1 = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b1g1r1a1, y_coeffs), y_offset), 8);
            __m256i u = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, u_coeffs), uv_offset), 8);
            __m256i v = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, v_coeffs), uv_offset), 8);


            /* Clamp the packed_y_values between 0 and 255*/
            y0= _mm256_max_epu8(y0, lower_bound); /* Ensure values are not below 0*/
            y0 = _mm256_min_epu8(y0, upper_bound); /* Ensure values are not above 255*/
            y1= _mm256_max_epu8(y1, lower_bound); /* Ensure values are not below 0*/
            y1 = _mm256_min_epu8(y1, upper_bound); /* Ensure values are not above 255*/
            u = _mm256_max_epu8(u, lower_bound); /* Ensure values are not below 0*/
            u = _mm256_min_epu8(u, upper_bound); /* Ensure values are not above 255*/
            v= _mm256_max_epu8(v, lower_bound); /* Ensure values are not below 0*/
            v = _mm256_min_epu8(v, upper_bound); /* Ensure values are not above 255*/

            /* Pack Y values and store them*/
            _mm256_storeu_si256((__m256i *)(y_plane + i), _mm256_packus_epi16(y0, y1));

            /* Handle chroma subsampling*/
            if (j % 2 == 0 && i % 32 == 0) {
                u = _mm256_packus_epi16(u, u);
                v = _mm256_packus_epi16(v, v);

                /* Average U and V values for subsampling*/
                __m256i uv_avg = _mm256_avg_epu8(u, _mm256_permute4x64_epi64(u, _MM_SHUFFLE(1, 0, 3, 2)));
                __m256i vv_avg = _mm256_avg_epu8(v, _mm256_permute4x64_epi64(v, _MM_SHUFFLE(1, 0, 3, 2)));

                /* Store U and V values*/
                _mm256_storeu2_m128i((__m128i *)(v_plane + (i >> 1)), (__m128i *)(u_plane + (i >> 1)), _mm256_unpacklo_epi64(uv_avg, vv_avg));
            }
        }
        argb_index += (width * 4);
        y_plane += width;
        if (j % 2 == 1) {
            u_plane += (width / 2);
            v_plane += (width / 2);
        }
    }
}

#elif __arm__
/*to do*/
#endif

void ARGBToYUV420(unsigned char **argb, int width, int height, unsigned char **yuv) 
{
    int plane_size = width * height;
    unsigned char *y_plane = *yuv;
    unsigned char *u_plane = *yuv + plane_size;
    unsigned char *v_plane = *yuv + plane_size + (plane_size  / 4);

    int argb_index = 0;
    int y_index = 0, uv_index = 0;

    int i,j;
    for (j = 0; j < height; j+=1) 
    {
        for (i = 0; i < width; i+=1) 
        {
            int b = (byte)*(*argb + argb_index);
            argb_index+=1;
            int g = (byte)*(*argb + argb_index);
            argb_index+=1;
            int r = (byte)*(*argb + argb_index);
            argb_index+=2; /* Skip the alpha channel*/

            int y = (0.299 * r + 0.587 * g + 0.114 * b) + 16;

            y_plane[y_index] = (char)(y < 0 ? 0 : (y > 255 ? 255 : y));
            y_index+=1;

            if (j % 2 == 0 && i % 2 == 0) 
            {
                int u = (128 -0.14713 * r - 0.28886 * g + 0.436 * b);
                int v = (128 + 0.615 * r - 0.51499 * g - 0.10001 * b);
                u_plane[uv_index] = (char)(u < 0 ? 0 : (u > 255 ? 255 : u));
                v_plane[uv_index] = (char)(v < 0 ? 0 : (v > 255 ? 255 : v));
                uv_index+=1;
            }
        }
    }
}

#ifdef _MSC_VER
#include <intrin.h> /* For Microsoft Visual C++ */
#else
#include <cpuid.h> /* For GCC and Clang */
#endif

bool checkAVX2Support()
{
#ifdef _MSC_VER
    int info[4];
    __cpuid(info, 0); /* Get the highest supported function ID */
    if (info[0] < 7)
        return false;

    __cpuidex(info, 7, 0); /* Get the feature flags */
    return (info[1] & (1 << 5)) != 0; /* Check the AVX2 bit (bit 5 of ECX) */
#else
    unsigned int eax, ebx, ecx, edx;
    if (__get_cpuid(0, &eax, &ebx, &ecx, &edx) && eax >= 7)
    {
        __cpuid_count(7, 0, eax, ebx, ecx, edx);
        return (ebx & (1 << 5)) != 0; /* Check the AVX2 bit (bit 5 of EBX) */
    }
    return false;
#endif
}

#ifdef __AVX2__

void YUV420ToARGB_AVX2(char* yuv, int width, int height, char* argb)
{
    int plane_size = width * height;
    int argb_index = 0;
    char* y_plane = yuv;
    char* u_plane = yuv + plane_size;
    char* v_plane = yuv + plane_size + plane_size / 4;

    const __m256i y_offset = _mm256_set1_epi8(-16);
    const __m256i uv_offset = _mm256_set1_epi8(-128);
    const __m256i coef_r = _mm256_set1_epi16(298);
    const __m256i coef_g = _mm256_set1_epi16(-100);
    const __m256i coef_g2 = _mm256_set1_epi16(-208);
    const __m256i coef_b = _mm256_set1_epi16(516);
    const __m256i coef_const = _mm256_set1_epi16(128);
    const __m256i clamp_min = _mm256_setzero_si256();
    const __m256i clamp_max = _mm256_set1_epi16(255);

    int i,j;
    for (j = 0; j < height; j++)
    {
        int vert_offset = (j >> 1) * (width >> 1);
        int vert_offset2 = j * width;

        for (i = 0; i < width; i += 16)
        {
            /* Load Y, U, and V values */
            __m256i y_values = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)(y_plane + vert_offset2 + i)));
            __m256i u_values = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)(u_plane + vert_offset + (i >> 1))));
            __m256i v_values = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)(v_plane + vert_offset + (i >> 1))));


            /* Subtract offsets */
            y_values = _mm256_add_epi16(y_values, y_offset);
            u_values = _mm256_add_epi16(u_values, uv_offset);
            v_values = _mm256_add_epi16(v_values, uv_offset);

            /* Calculate RGB values */
            __m256i r_values = _mm256_srai_epi16(_mm256_add_epi16(_mm256_mullo_epi16(coef_r, y_values),
                _mm256_mullo_epi16(coef_b, v_values)), 8);
            __m256i g_values = _mm256_srai_epi16(_mm256_add_epi16(_mm256_mullo_epi16(coef_r, y_values),
                _mm256_add_epi16(_mm256_mullo_epi16(coef_g, u_values),
                    _mm256_mullo_epi16(coef_g2, v_values))), 8);
            __m256i b_values = _mm256_srai_epi16(_mm256_add_epi16(_mm256_mullo_epi16(coef_r, y_values),
                _mm256_mullo_epi16(coef_b, u_values)), 8);

            /* Add constant offset */
            r_values = _mm256_add_epi16(r_values, coef_const);
            g_values = _mm256_add_epi16(g_values, coef_const);
            b_values = _mm256_add_epi16(b_values, coef_const);

            /* Clamp values */
            r_values = _mm256_min_epi16(_mm256_max_epi16(r_values, clamp_min), clamp_max);
            g_values = _mm256_min_epi16(_mm256_max_epi16(g_values, clamp_min), clamp_max);
            b_values = _mm256_min_epi16(_mm256_max_epi16(b_values, clamp_min), clamp_max);

            /* Convert RGB values to bytes */
            __m256i rg_values = _mm256_packus_epi16(r_values, g_values);
            __m256i ba_values = _mm256_packus_epi16(b_values, _mm256_set1_epi16(255));

            /* Interleave RGB and BA values */ 
            __m256i ar_values = _mm256_permute2x128_si256(rg_values, ba_values, 0x20);
            __m256i gb_values = _mm256_permute2x128_si256(rg_values, ba_values, 0x31);

            /* Store the interleaved results */
            _mm256_storeu_si256((__m256i*)(argb + argb_index), ar_values);
            _mm256_storeu_si256((__m256i*)(argb + argb_index + 32), gb_values);

            argb_index += 64;
        }
    }
}

#endif


void YUV420ToARGB(char *yuv, int width, int height, char *argb)
{
    int plane_size = width * height;
    int argb_index = 0;
    char* y_plane = yuv;
    char* u_plane = yuv + plane_size;
    char* v_plane = yuv + plane_size + plane_size / 4;
    int y_ind = 0, u_ind = 0;
    int i,j;
    for (j = 0; j < height; j+=1){
        for (i = 0; i < width; i+=1)
        {
            int y = (byte)y_plane[y_ind++];
            int u = (byte)u_plane[u_ind];
            int v = (byte)v_plane[u_ind];
            u_ind += (!(i&1)) & (!(j&1));

            u -= 128;
            v -= 128;
            
            int r = round(y               + 1.402 * v);
            int g = round(y - 0.344 * u - 0.714 * v);
            int b = round(1.147 * y + 1.773 * u);

            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);

            argb[argb_index++] = b;
            argb[argb_index++] = g;
            argb[argb_index++] = r;
            argb[argb_index++] = 255;
        }
    }
}

void YUV420ToRGB24(char* yuv, int width, int height, char* argb)
{
    int plane_size = width * height;
    int argb_index = 0;
    char* y_plane = yuv;
    char* u_plane = yuv + plane_size;
    char* v_plane = yuv + plane_size + plane_size / 4;
    int i, j;
    for (j = 0; j < height; j += 1) {
        int vert_offset = (j >> 1) * (width >> 1);
        int vert_offset2 = j * width;
        for (i = 0; i < width; i += 1)
        {
            int y = (byte)y_plane[vert_offset2 + i];
            int u = (byte)u_plane[vert_offset + (i / 2)];
            int v = (byte)v_plane[vert_offset + (i / 2)];

            int c = y - 16;
            int d = u - 128;
            int e = v - 128;

            int r = (298 * c + 409 * e + 128) >> 8;
            int g = (298 * c - 100 * d - 208 * e + 128) >> 8;
            int b = (298 * c + 516 * d + 128) >> 8;

            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);

            (argb)[argb_index++] = b;
            (argb)[argb_index++] = g;
            (argb)[argb_index++] = r;
        }
    }
}

void separate_channels_inplace(char *buffer, int num_pixels) 
{
    int i;
    for (i = 0; i < num_pixels; i++) 
    {
        /* swap R and A*/
        char tmp = buffer[4*i];
        buffer[4*i] = buffer[4*i+3];
        buffer[4*i+3] = tmp;
        /* swap G and B*/
        tmp = buffer[4*i+1];
        buffer[4*i+1] = buffer[4*i+2];
        buffer[4*i+2] = tmp;
    }
}


byte bilinear_interpolate(const byte *src, float x, float y, int width, int height, int channels, int channel)
{
    int x1 = (int)x;
    int x2 = x1 + 1;
    if (x2 >= width) x2--;
    int y1 = (int)y;
    int y2 = y1 + 1;
    if (y2 >= height) y2--;

    float x_frac = x - x1;
    float y_frac = y - y1;

    byte Q11 = src[(y1 * width + x1) * channels + channel];
    byte Q12 = src[(y2 * width + x1) * channels + channel];
    byte Q21 = src[(y1 * width + x2) * channels + channel];
    byte Q22 = src[(y2 * width + x2) * channels + channel];

    float R1 = (1 - x_frac) * Q11 + x_frac * Q21;
    float R2 = (1 - x_frac) * Q12 + x_frac * Q22;

    return (byte)((1 - y_frac) * R1 + y_frac * R2);
}

void resize_image_bilinear(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    float x_ratio = (float)(src_width) / (float)dst_width;
    float y_ratio = (float)(src_height) / (float)dst_height;

    if (src_height == dst_height && src_width == dst_width)
    {
        return;
    }

    int dst_y, dst_x, x, channel;
    for (dst_y = 0; dst_y < dst_height; dst_y++)
    {
        
        for (dst_x = 0; dst_x < dst_width; dst_x+=8)
        {
            for (x = 0; x < 8; x++)
            {
                float src_x = (x + dst_x) * x_ratio;
                float src_y = dst_y * y_ratio;

                for (channel = 0; channel < channels; channel++)
                {
                    if (channel == 3)
                    {
                        dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                        continue;
                    }
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y,
                        src_width, src_height, channels, channel);
                }
            }
        }
        for (dst_x = dst_width & ~7; dst_x < dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                    continue;
                }
                dst[(dst_y * dst_width + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
} 

void resize_image_bilinear_blocks(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{    
    float x_ratio = (float)(src_width) / (float)dst_width;
    float y_ratio = (float)(src_height) / (float)dst_height;

    const int BLOCK_SIZE = 8;

    int block_start_y, block_start_x, dst_y, dst_x, channel;
    for (block_start_y = 0; block_start_y < src_height; block_start_y += BLOCK_SIZE) 
    {
        for (block_start_x = 0; block_start_x < src_width; block_start_x += BLOCK_SIZE) 
        {
            int block_end_x = block_start_x + BLOCK_SIZE > src_width ? src_width : block_start_x + BLOCK_SIZE;
            int block_end_y = block_start_y + BLOCK_SIZE > src_height ? src_height : block_start_y + BLOCK_SIZE;
            
            for (dst_y = block_start_y; dst_y < block_end_y; dst_y++)
            {
                for (dst_x = block_start_x; dst_x < block_end_x; dst_x++)
                {
                    float src_x = dst_x * x_ratio;
                    float src_y = dst_y * y_ratio;

                    int base_index = dst_y * dst_width + dst_x ;

                    for (channel = 0; channel < channels; channel++)
                    {
                        if (channel == 3)
                        {
                            dst[base_index * channels + channel] = 255;
                        }
                        else
                        {
                            dst[base_index * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
                        }
                    }
                }
            }
        }
    }
}

void bilinear_resize_plane(const byte* src, byte* dst, int src_width, int src_height, int dst_width, int dst_height, float x_ratio, float y_ratio) 
{
    int dst_x, dst_y;
    for (dst_y = 0; dst_y < dst_height; dst_y++)
    {
        for (dst_x = 0; dst_x < dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;
            
            int x1 = (int)src_x;
            int y1 = (int)src_y;
            int x2 = (x1 + 1 < src_width) ? x1 + 1 : x1;
            int y2 = (y1 + 1 < src_height) ? y1 + 1 : y1;
            
            byte p1 = src[y1 * src_width + x1];
            byte p2 = src[y1 * src_width + x2];
            byte p3 = src[y2 * src_width + x1];
            byte p4 = src[y2 * src_width + x2];

            float weight_x = src_x - (float)x1;
            float weight_y = src_y - (float)y1;

            /* Bilinear interpolation formula */
            byte interpolated_value =
                p1 * (1 - weight_x) * (1 - weight_y) +
                p2 * weight_x * (1 - weight_y) +
                p3 * (1 - weight_x) * weight_y +
                p4 * weight_x * weight_y;

            dst[dst_y * dst_width + dst_x] = interpolated_value;
        }
    }
}

/**
 * @brief This is intended to speedup both the interpolation and converstion to ARGB process as a whole. It might shave off a few milliseconds. (which is really significant)
*/
void resize_image_bilinear_YUV420(const byte *src, byte **dst, int src_width, int src_height, int dst_width, int dst_height)
{
    float x_ratio = (float)(src_width) / (float)dst_width;
    float y_ratio = (float)(src_height) / (float)dst_height;

    int src_plane_size = src_width * src_height;
    int dst_plane_size = dst_width * dst_height;

    /* Perform bilinear resizing separately for each plane
       Note: bilinear_interpolate_YUV420 should also be implemented */
    bilinear_resize_plane(&src[0], &(*dst)[0], src_width, src_height, dst_width, dst_height, x_ratio, y_ratio);
    bilinear_resize_plane(&src[src_plane_size], &(*dst)[dst_plane_size], src_width/2, src_height/2, dst_width/2, dst_height/2, x_ratio/2, y_ratio/2);
    bilinear_resize_plane(&src[src_plane_size*5/4], &(*dst)[dst_plane_size*5/4], src_width/2, src_height/2, dst_width/2, dst_height/2, x_ratio/2, y_ratio/2);
}

void resize_image_bilinear_preserve_aspect(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    /* Calculate aspect ratios */
    double src_aspect_ratio = (double)src_width / (double)src_height;
    double dst_aspect_ratio = (double)dst_width / (double)dst_height;

    int target_width, target_height, x_offset = 0, y_offset = 0;
    /* The destination aspect ratio is wider than the source */
    if (dst_aspect_ratio > src_aspect_ratio) 
    {
        target_height = dst_height- dst_height % 18;
        target_width = (int)(dst_height * src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }
    /* The destination aspect ratio is taller than the source */ 
    else
    {
        target_width = dst_width - dst_width % 16;
        target_height = (int)(dst_width / src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }

    float x_ratio = (float)(src_width) / (float)target_width;
    float y_ratio = (float)(src_height) / (float)target_height;

    /* Initialize the destination to black */
    memset(dst, 0, dst_width * dst_height * channels);
    /*
    int i;
    for (i = 0; i < dst_width * dst_height * channels; i++) {
        dst[i] = (i % channels == 3) ? 255 : 0;
    }*/

    int dst_y, dst_x, x, channel;
    for (dst_y = 0; dst_y < target_height; dst_y++)
    {
        for (dst_x = 0; dst_x < target_width; dst_x+=8)
        {
            int ind = (dst_y + y_offset) * dst_width + dst_x + x_offset;;
            for (x = 0; x < 8 && (x + dst_x) < target_width; x++)
            {
                float src_x = (x + dst_x) * x_ratio;
                float src_y = dst_y * y_ratio;

                for (channel = 0; channel < channels; channel++)
                {
                    if (channel == 3)
                    {
                        dst[(ind + x) * channels + channel] = 255;
                        continue;
                    }
                    dst[(ind + x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
                }
            }
        }

        for (dst_x = target_width & ~7; dst_x < target_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[((dst_y + y_offset) * dst_width + dst_x + x_offset) * channels + channel] = 255;
                    continue;
                }
                dst[((dst_y + y_offset) * dst_width + dst_x + x_offset) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
}

/**
 * @brief This is marginally faster than resize_image_bilinear_preserve_aspect 
*/
void resize_image_bilinear_blocks_preserve_aspect(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    /* Calculate aspect ratios */
    double src_aspect_ratio = (double)src_width / (double)src_height;
    double dst_aspect_ratio = (double)dst_width / (double)dst_height;

    int target_width, target_height, x_offset = 0, y_offset = 0;
    
    /* The destination aspect ratio is wider than the source */
    if (dst_aspect_ratio > src_aspect_ratio) 
    {
        target_height = dst_height - dst_height % 18;
        target_width = (int)(dst_height * src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }
    /* The destination aspect ratio is taller than the source */ 
    else
    {
        target_width = dst_width - dst_width % 16;
        target_height = (int)(dst_width / src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }

    float x_ratio = (float)(src_width) / (float)target_width;
    float y_ratio = (float)(src_height) / (float)target_height;

    /* Initialize the destination to black */
    memset(dst, 0, dst_width * dst_height * channels);

    const int BLOCK_SIZE = 8;

    int block_start_y, block_start_x, dst_y, dst_x, channel;
    for (block_start_y = 0; block_start_y < target_height; block_start_y += BLOCK_SIZE) 
    {
        for (block_start_x = 0; block_start_x < target_width; block_start_x += BLOCK_SIZE) 
        {
            int block_end_x = block_start_x + BLOCK_SIZE > target_width ? target_width : block_start_x + BLOCK_SIZE;
            int block_end_y = block_start_y + BLOCK_SIZE > target_height ? target_height : block_start_y + BLOCK_SIZE;
            
            for (dst_y = block_start_y; dst_y < block_end_y; dst_y++)
            {
                for (dst_x = block_start_x; dst_x < block_end_x; dst_x++)
                {
                    float src_x = dst_x * x_ratio;
                    float src_y = dst_y * y_ratio;

                    int base_index = (dst_y + y_offset) * dst_width + dst_x + x_offset;

                    for (channel = 0; channel < channels; channel++)
                    {
                        if (channel == 3)
                        {
                            dst[base_index * channels + channel] = 255;
                        }
                        else
                        {
                            dst[base_index * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
                        }
                    }
                }
            }
        }
    }
}

void apply_sharpening(byte *image, int width, int height, int channels) {
    /* Define the sharpening kernel */
    int kernel[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };

    /* Buffer to store adjusted pixel values */
    byte *copy = (byte *)malloc(width * height * channels);

    /* Make a copy of the original image */
    memcpy(copy, image, width * height * channels);

    int y,x,c;
    for (y = 1; y < height - 1; y+=1) 
    {
        for (x = 1; x < width - 1; x+=1) 
        {
            for (c = 0; c < channels; c+=1) 
            {
                 /* Skip alpha channel */
                if (c == 3) 
                {
                    continue;
                }

                int sum = 0, ky,kx;
                for (ky = -1; ky <= 1; ky+=1) 
                {
                    for (kx = -1; kx <= 1; kx+=1) 
                    {
                        int pixel = copy[((y + ky) * width + (x + kx)) * channels + c];
                        sum += pixel * kernel[ky + 1][kx + 1];
                    }
                }

                /* Clamp the value to [0, 255] */
                image[(y * width + x) * channels + c] = (sum < 0) ? 0 : (sum > 255) ? 255 : sum;
            }
        }
    }

    free(copy);
}

void resize_image_nearest_neighbor(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    float x_ratio = (float)src_width / (float)dst_width;
    float y_ratio = (float)src_height / (float)dst_height;

    int dst_y, dst_x;

    for (dst_y = 0; dst_y < dst_height; dst_y++)
    {
        for (dst_x = 0; dst_x < dst_width; dst_x++)
        {
            int src_x = (int)(dst_x * x_ratio);
            int src_y = (int)(dst_y * y_ratio);

            memcpy(
                &dst[(dst_y * dst_width + dst_x) * channels],
                &src[(src_y * src_width + src_x) * channels],
                channels
            );
        }
    }
}

void resize_image_nearest_neighbor_preserve_aspect(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    /* Calculate aspect ratios */
    double src_aspect_ratio = (double)src_width / (double)src_height;
    double dst_aspect_ratio = (double)dst_width / (double)dst_height;

    int target_width, target_height, x_offset = 0, y_offset = 0;
    /* The destination aspect ratio is wider than the source */
    if (dst_aspect_ratio > src_aspect_ratio) 
    {
        target_height = dst_height- dst_height % 18;
        target_width = (int)(dst_height * src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }
    /* The destination aspect ratio is taller than the source */ 
    else
    {
        target_width = dst_width - dst_width % 16;
        target_height = (int)(dst_width / src_aspect_ratio);
        x_offset = (dst_width - target_width) / 2;
        y_offset = (dst_height - target_height) / 2;
    }

    float x_ratio = (float)(src_width) / (float)target_width;
    float y_ratio = (float)(src_height) / (float)target_height;

    memset(dst, 0, dst_width * dst_height * channels);
    int dst_y, dst_x, channel;

    for (dst_y = 0; dst_y < target_height; dst_y++)
    {
        for (dst_x = 0; dst_x < target_width; dst_x++)
        {
            int src_x = (int)(dst_x * x_ratio);
            int src_y = (int)(dst_y * y_ratio);

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[((dst_y + y_offset) * dst_width + dst_x + x_offset) * channels + channel] = 255;
                    continue;
                }
                dst[((dst_y + y_offset) * dst_width + dst_x + x_offset) * channels + channel] = src[(src_y * src_width + src_x) * channels + channel];
            }
        }
    }
}

/**
 * @brief Gets the good zoom coordinates for the given zoom ratio
 * 
 * @param src_width The width of the source image
 * @param src_height The height of the source image
 * @param zoom_x The x coordinate of the center point
 * @param zoom_y The y coordinate of the center point
 * @param zoom_ratio The zoom ratio
 * @param x The x coordinate of the center point after zooming
*/
void get_zoom_coords(int src_width, int src_height, int zoom_x, int zoom_y, float zoom_ratio, int* x, int *y)
{
    int dst_width = src_width * zoom_ratio, dst_height = src_height * zoom_ratio;
    /* Adjust centers to the minimum positions */
    if (zoom_x < dst_width/2)
    {
        zoom_x = dst_width/2;
    }
    else if (zoom_x > src_width - dst_width/2)
    {
        zoom_x = src_width - dst_width/2;
    }
    if (zoom_y < dst_height/2)
    {
        zoom_y = dst_height/2;
    }
    else if (zoom_y > src_height - dst_height/2)
    {
        zoom_y = src_height - dst_height/2;
    }
    *x=zoom_x;
    *y=zoom_y;
}

/**
 * @brief Zooms in the image at the given center point by the given zoom ratio
 * This functions assumes 4 channels (ARGB)
 * 
 * @param src The source image
 * @param dst The destination image
 * @param src_width The width of the source image
 * @param src_height The height of the source image
 * @param zoom_x The x coordinate of the center point
 * @param zoom_y The y coordinate of the center point
 * @param zoom_ratio The zoom ratio
*/
void zoom_image(const byte *src, byte** dst, int src_width, int src_height, int zoom_x, int zoom_y, float zoom_ratio)
{
    int dst_width = src_width * zoom_ratio, dst_height = src_height * zoom_ratio;
    /* Adjust centers to the minimum positions */
    if (zoom_x < dst_width/2)
    {
        zoom_x = dst_width/2;
    }
    else if (zoom_x > src_width - dst_width/2)
    {
        zoom_x = src_width - dst_width/2;
    }
    if (zoom_y < dst_height/2)
    {
        zoom_y = dst_height/2;
    }
    else if (zoom_y > src_height - dst_height/2)
    {
        zoom_y = src_height - dst_height/2;
    }
    int x_start = zoom_x - dst_width/2, y_start = zoom_y - dst_height/2;
    int x_end = zoom_x + dst_width/2, y_end = zoom_y + dst_height/2;
    int i;
    printf("x_start = %d, x_end = %d, y_start = %d, y_end = %d\n", x_start, x_end, y_start, y_end);
    for (i = x_start; i < x_end; i+=1)
    {
        printf("i = %d\n", i);
        memcpy(&dst[(i - x_start) * dst_height], &src[(i * src_height + y_start) * 4], dst_height * 4);
    }
}

const int FIXED_POINT = 10;

byte bilinear_interpolate_fixed_point(const byte *src, int x, int y, int width, int height, int channels, int channel)
{
    /* Bitmask to essentially compute x % 2^FIXED_POINT but in a fancy way */
    int x1 = x & (~((1<<FIXED_POINT)-1));
    int y1 = y & (~((1<<FIXED_POINT)-1));

    int _x1 = x1 >> FIXED_POINT;
    int _y1 = y1 >> FIXED_POINT;

    int _x2 = _x1 + 1;
    if (_x2 >= width) _x2--;
    int _y2 = _y1 + 1;
    if (_y2 >= height) _y2--;

    int x_frac = x - x1;
    int y_frac = y - y1;

    byte Q11 = src[(_y1 * width + _x1) * channels + channel];
    byte Q12 = src[(_y2 * width + _x1) * channels + channel];
    byte Q21 = src[(_y1 * width + _x2) * channels + channel];
    byte Q22 = src[(_y2 * width + _x2) * channels + channel];


    int R1 = ((((1<<FIXED_POINT)-1) - x_frac) * Q11 + x_frac * Q21) >> FIXED_POINT;
    int R2 = ((((1<<FIXED_POINT)-1) - x_frac) * Q12 + x_frac * Q22) >> FIXED_POINT;

    return (byte)(((((1<<FIXED_POINT)-1) - y_frac) * R1 + y_frac * R2) >> FIXED_POINT);
}

/* This might look goofy, but it is about 10-20% faster than using floatin point arithmetics, so that's good */
void resize_image_bilinear_fixed_point(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    /*int interpolate_width = src_width << FIXED_POINT, interpolate_height = src_height << FIXED_POINT;*/
    int x_ratio = (src_width << FIXED_POINT) / dst_width;
    int y_ratio = (src_height << FIXED_POINT) / dst_height;

    int dst_y, dst_x, x, channel;
    for (dst_y = 0; dst_y < dst_height; dst_y++)
    {
        int src_y = dst_y * y_ratio;
        for (dst_x = 0; dst_x < dst_width; dst_x+=8)
        {
            for (x = 0; x < 8; x++)
            {
                int src_x = (x + dst_x) * x_ratio;

                for (channel = 0; channel < channels; channel++)
                {
                    if (channel == 3)
                    {
                        dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                        continue;
                    }
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = bilinear_interpolate_fixed_point(src, src_x, src_y,
                        src_width, src_height, channels, channel);
                }
            }
        }
        for (dst_x = dst_width & ~7; dst_x < dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                    continue;
                }
                dst[(dst_y * dst_width + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
} 


/* Nothing here is faster than the code above, but it was worth trying */
const int BLOCK_SIZE = 8;

void bilinear_interpolate_block(const byte *src, int src_width, int src_height, int dst_width, int dst_height, int x_ratio, int y_ratio, int start_dst_x, int start_dst_y, int end_dst_x, int end_dst_y, byte *dst, int channels)
{
    int dst_y, dst_x, channel;

    for (dst_y = start_dst_y; dst_y < end_dst_y; dst_y++)
    {
        int src_y = dst_y * y_ratio;
        
        for (dst_x = start_dst_x; dst_x < end_dst_x; dst_x++)
        {
            int src_x = dst_x * x_ratio;

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[(dst_y * dst_width + dst_x) * channels + channel] = 255;
                    continue;
                }
                dst[(dst_y * dst_width + dst_x) * channels + channel] = bilinear_interpolate_fixed_point(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
}

void resize_image_bilinear_fixed_point_block(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    int x_ratio = (src_width << FIXED_POINT) / dst_width;
    int y_ratio = (src_height << FIXED_POINT) / dst_height;

    int block_start_y, block_start_x;
    for (block_start_y = 0; block_start_y < dst_height; block_start_y += BLOCK_SIZE)
    {
        for (block_start_x = 0; block_start_x < dst_width; block_start_x += BLOCK_SIZE)
        {
            int block_end_x = (block_start_x + BLOCK_SIZE > dst_width) ? dst_width : block_start_x + BLOCK_SIZE;
            int block_end_y = (block_start_y + BLOCK_SIZE > dst_height) ? dst_height : block_start_y + BLOCK_SIZE;
            
            bilinear_interpolate_block(src, src_width, src_height, dst_width, dst_height, x_ratio, y_ratio, block_start_x, block_start_y, block_end_x, block_end_y, dst, channels);
        }
    }
}

uint32_t expand_bits(uint32_t v) {
    v = (v * 0x00010001u) & 0xFF0000FFu;
    v = (v * 0x00000101u) & 0x0F00F00Fu;
    v = (v * 0x00000011u) & 0xC30C30C3u;
    v = (v * 0x00000005u) & 0x49249249u;
    return v;
}

uint32_t morton_encode(uint32_t x, uint32_t y) {
    return (expand_bits(x) << 1) + expand_bits(y);
}

void resize_image_bilinear_fixed_point_Z(const byte *src, byte *dst, int src_width, int src_height, int dst_width, int dst_height, int channels)
{
    int x_ratio = (src_width << FIXED_POINT) / dst_width;
    int y_ratio = (src_height << FIXED_POINT) / dst_height;

    int num_blocks_x = (dst_width + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int num_blocks_y = (dst_height + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int block_idx;
    uint32_t i;
    /* Iterate over blocks in Morton order */
    for (block_idx = 0; block_idx < num_blocks_x * num_blocks_y; block_idx++)
    {
        uint32_t block_x = 0, block_y = 0;
        /* Decode Morton order into block_x and block_y */
        for (i = 0; i < (sizeof(uint32_t) * 8) / 2; i++)
        {
            block_x |= (block_idx & (1U << (i * 2))) >> i;
            block_y |= (block_idx & (2U << (i * 2))) >> (i + 1);
        }

        if (block_x >= num_blocks_x || block_y >= num_blocks_y)
            continue;  /* Out of bounds */

        int block_start_x = block_x * BLOCK_SIZE;
        int block_start_y = block_y * BLOCK_SIZE;
        int block_end_x = (block_start_x + BLOCK_SIZE > dst_width) ? dst_width : block_start_x + BLOCK_SIZE;
        int block_end_y = (block_start_y + BLOCK_SIZE > dst_height) ? dst_height : block_start_y + BLOCK_SIZE;

        bilinear_interpolate_block(src, src_width, src_height, dst_width, dst_height,
         x_ratio, y_ratio, block_start_x, block_start_y, block_end_x, block_end_y, dst, channels);
    }
}

/**
 * @brief Resize an image using bilinear interpolation, starting at dst_total_offset, and going for compute_bytes worth of data(
 * make sure to allign everything to the number of channels)
 * 
 * @param src Source image bytes
 * @param dst Destination image bytes
 * @param src_width Source image width
 * @param src_height Source image height
 * @param dst_width Destination image width
 * @param dst_height Destination image height
 * @param channels Number of channels in the image
 * @param dst_total_offset Offset in the destination image to start writing to
 * @param compute_bytes Number of bytes to compute and write in the destination 
*/
void resize_image_bilinear_fixed_point_offset(const byte *src, byte *dst, int src_width, int src_height,
 int dst_width, int dst_height, int channels, int dst_total_offset, int compute_bytes)
{
    int x_ratio = (src_width << FIXED_POINT) / dst_width;
    int y_ratio = (src_height << FIXED_POINT) / dst_height;

    /* This is so much boilerplate  but might prove to be useful when doing multithreading */
    int start_x, start_y;
    start_x = dst_total_offset % (dst_width * channels);
    start_y = dst_total_offset / (dst_height * channels);

    int dst_y, dst_x, x, channel;
    for (dst_y = start_y; dst_y < dst_height; dst_y++)
    {
        int src_y = dst_y * y_ratio;
        for (dst_x = start_x; dst_x < dst_width; dst_x+=8)
        {
            for (x = 0; x < 8; x++)
            {
                int src_x = (x + dst_x) * x_ratio;

                for (channel = 0; channel < channels; channel++)
                {
                    if (channel == 3)
                    {
                        dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                        continue;
                    }
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = bilinear_interpolate_fixed_point(src, src_x, src_y,
                        src_width, src_height, channels, channel);
                }
            }
        }
        for (dst_x = dst_width & ~7; dst_x < dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (channel = 0; channel < channels; channel++)
            {
                if (channel == 3)
                {
                    dst[(dst_y * dst_width + x + dst_x) * channels + channel] = 255;
                    continue;
                }
                dst[(dst_y * dst_width + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
}
