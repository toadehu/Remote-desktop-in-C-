#pragma once

#include <stdint.h>
#include "x264.h"

#ifdef HAVE_X86
#include <emmintrin.h>
#include <immintrin.h>

//I think this does NOT work
void ARGBToYUV420_SSE2 (unsigned char **argb, int width, int height, unsigned char **yuv) {
    int plane_size = width * height;
    unsigned char *y_plane = *yuv;
    unsigned char *u_plane = *yuv + plane_size;
    unsigned char *v_plane = *yuv + plane_size + (plane_size >> 2);
    unsigned char *orig_argb_ptr = *argb;
    int argb_index = 0;

    const __m128i shuffle_mask = _mm_set_epi8(15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
    const __m128i y_coeffs = _mm_set_epi16(0, 25, 129, 66, 0, 25, 129, 66);
    const __m128i u_coeffs = _mm_set_epi16(0, 112, -74, -38, 0, 112, -74, -38);
    const __m128i v_coeffs = _mm_set_epi16(0, -18, -94, 112, 0, -18, -94, 112);
    const __m128i y_offset = _mm_set1_epi16(16);
    const __m128i uv_offset = _mm_set1_epi16(128);

    for (int j = 0; j < height; j+=1) {
        //printf("Current index:%d\n", argb_index);
        for (int i = 0; i < width; i += 8) { // Process 8 pixels per iteration
            //printf("precrisis, addres where segfault happens is:%d or it may be:%d\n", argb_index + (i + 0), argb_index + (i + 4));
            //printf("actual offset should be:%d, actual size: %d\n", j * width * 4 + i + 4, width * height * 4);
            __m128i argb0 = _mm_lddqu_si128((__m128i *)(*argb + argb_index + i));
            __m128i argb1 = _mm_lddqu_si128((__m128i *)(*argb + argb_index +  i + 4));

            // Shuffle RGB values
            __m128i b0g0r0a0 = _mm_shuffle_epi8(argb0, shuffle_mask);
            __m128i b1g1r1a1 = _mm_shuffle_epi8(argb1, shuffle_mask);

            // Calculate Y, U, V values
            __m128i y0 = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, y_coeffs), y_offset), 8);
            __m128i y1 = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b1g1r1a1, y_coeffs), y_offset), 8);
            __m128i u = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, u_coeffs), uv_offset), 8);
            __m128i v = _mm_srai_epi16(_mm_add_epi16(_mm_madd_epi16(b0g0r0a0, v_coeffs), uv_offset), 8);

            // Pack Y values and store them
            _mm_storeu_si128((__m128i *)(y_plane + i), _mm_packus_epi16(y0, y1));

            // Handle chroma subsampling
            if (j % 2 == 0 && i % 16 == 0) {
                u = _mm_packus_epi16(u, u);
                v = _mm_packus_epi16(v, v);

                // Average U and V values for subsampling
                __m128i uv_avg = _mm_avg_epu8(u, _mm_shuffle_epi32(u, _MM_SHUFFLE(1, 0, 3, 2)));
                __m128i vv_avg = _mm_avg_epu8(v, _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));

                // Store U and V values
                _mm_storel_epi64((__m128i *)(u_plane + (i >> 1)), uv_avg);
                _mm_storel_epi64((__m128i *)(v_plane + (i >> 1)), vv_avg);
            }
        }
        argb_index += (width * 4);
        //*argb += (width << 1);
        y_plane += width;
        if (!(j&1)) {
            u_plane += (width / 2);
            v_plane += (width / 2);
        }
    }
    *argb = orig_argb_ptr;

    printf("Total theortical size:%d\n", width * height * 4);
}

void ARGBToYUV420_AVX2(unsigned char **argb, int width, int height, unsigned char **yuv) {
    int plane_size = width * height;
    unsigned char *y_plane = *yuv;
    unsigned char *u_plane = *yuv + plane_size;
    unsigned char *v_plane = *yuv + plane_size + (plane_size >> 2);

    int argb_index = 0;

    const __m256i shuffle_mask = _mm256_set_epi8(15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, 12, 9, 6, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
    const __m256i y_coeffs = _mm256_set_epi16(0, 25, 129, 66, 0, 25, 129, 66, 0, 25, 129, 66, 0, 25, 129, 66);
    const __m256i u_coeffs = _mm256_set_epi16(0, 112, -74, -38, 0, 112, -74, -38, 0, 112, -74, -38, 0, 112, -74, -38);
    const __m256i v_coeffs = _mm256_set_epi16(0, -18, -94, 112, 0, -18, -94, 112, 0, -18, -94, 112, 0, -18, -94, 112);
    const __m256i y_offset = _mm256_set1_epi16(16);
    const __m256i uv_offset = _mm256_set1_epi16(128);

    // Set the lower (0) and upper (255) bounds
    __m256i lower_bound = _mm256_set1_epi8(0);
    __m256i upper_bound = _mm256_set1_epi8(255);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i += 16) { // Process 16 pixels per iteration
            __m256i argb0 = _mm256_lddqu_si256((__m256i *)(*argb + argb_index + (i + 0)));
            __m256i argb1 = _mm256_lddqu_si256((__m256i *)(*argb + argb_index + (i + 8)));

            // Shuffle RGB values
            __m256i b0g0r0a0 = _mm256_shuffle_epi8(argb0, shuffle_mask);
            __m256i b1g1r1a1 = _mm256_shuffle_epi8(argb1, shuffle_mask);

            // Calculate Y, U, V values
             __m256i y0 = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, y_coeffs), y_offset), 8);
            __m256i y1 = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b1g1r1a1, y_coeffs), y_offset), 8);
            __m256i u = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, u_coeffs), uv_offset), 8);
            __m256i v = _mm256_srai_epi16(_mm256_add_epi16(_mm256_madd_epi16(b0g0r0a0, v_coeffs), uv_offset), 8);


            // Clamp the packed_y_values between 0 and 255
            y0= _mm256_max_epu8(y0, lower_bound); // Ensure values are not below 0
            y0 = _mm256_min_epu8(y0, upper_bound); // Ensure values are not above 255
            y1= _mm256_max_epu8(y1, lower_bound); // Ensure values are not below 0
            y1 = _mm256_min_epu8(y1, upper_bound); // Ensure values are not above 255
            u = _mm256_max_epu8(u, lower_bound); // Ensure values are not below 0
            u = _mm256_min_epu8(u, upper_bound); // Ensure values are not above 255
            v= _mm256_max_epu8(v, lower_bound); // Ensure values are not below 0
            v = _mm256_min_epu8(v, upper_bound); // Ensure values are not above 255

            // Pack Y values and store them
            _mm256_storeu_si256((__m256i *)(y_plane + i), _mm256_packus_epi16(y0, y1));

            // Handle chroma subsampling
            if (j % 2 == 0 && i % 32 == 0) {
                u = _mm256_packus_epi16(u, u);
                v = _mm256_packus_epi16(v, v);

                // Average U and V values for subsampling
                __m256i uv_avg = _mm256_avg_epu8(u, _mm256_permute4x64_epi64(u, _MM_SHUFFLE(1, 0, 3, 2)));
                __m256i vv_avg = _mm256_avg_epu8(v, _mm256_permute4x64_epi64(v, _MM_SHUFFLE(1, 0, 3, 2)));

                // Store U and V values
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
//to do
#endif

typedef struct video_encoder
{
    x264_param_t param;
    x264_t *encoder;
    x264_picture_t picture_in, picture_out;
    int frame_number;
} video_encoder;

void ARGBToYUV420(unsigned char **argb, int width, int height, unsigned char **yuv) {
    int plane_size = width * height;
    unsigned char *y_plane = *yuv;
    unsigned char *u_plane = *yuv + plane_size;
    unsigned char *v_plane = *yuv + plane_size + (plane_size >> 2);

    int argb_index = 0;
    int y_index = 0, uv_index = 0;

#ifdef _____HAVE_X11 //this means we have a bit depth of 24 so there is a need for new code
    for (int j = 0; j < height; j+=1) {
        for (int i = 0; i < width; i+=1) 
        {
            int b = *(*argb + argb_index);
            argb_index+=1;
            int g = *(*argb + argb_index);
            argb_index+=1;
            int r = *(*argb + argb_index);
            argb_index+=1; //Here there is no alpha channel

            int y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            int u = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            int v = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

            y_plane[y_index] = (unsigned char)(y < 0 ? 0 : (y > 255 ? 255 : y));
            y_index+=1;

            if (j % 2 == 0 && i % 2 == 0) {
                u_plane[uv_index] = (unsigned char)(u < 0 ? 0 : (u > 255 ? 255 : u));
                v_plane[uv_index] = (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
                uv_index+=1;
            }
        }
    }
#else
    for (int j = 0; j < height; j+=1) {
        for (int i = 0; i < width; i+=1) {
            int b = *(*argb + argb_index);
            argb_index+=1;
            int g = *(*argb + argb_index);
            argb_index+=1;
            int r = *(*argb + argb_index);
            argb_index+=2; // Skip the alpha channel

            int y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            int u = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            int v = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

            y_plane[y_index] = (unsigned char)(y < 0 ? 0 : (y > 255 ? 255 : y));
            y_index+=1;

            if (j % 2 == 0 && i % 2 == 0) {
                u_plane[uv_index] = (unsigned char)(u < 0 ? 0 : (u > 255 ? 255 : u));
                v_plane[uv_index] = (unsigned char)(v < 0 ? 0 : (v > 255 ? 255 : v));
                uv_index+=1;
            }
        }
    }

#endif

    //printf("fin:%d\n", argb_index);
}

void YUV420ToARGB(unsigned char **yuv, int width, int height, unsigned char ** argb, int bit_depth)
{
    int plane_size = width * height / 4;
    int argb_index = 0;
    unsigned char* y_plane = *yuv;
    unsigned char* u_plane = *yuv + plane_size;
    unsigned char* v_plane = *yuv + plane_size + (plane_size>>2);
    for (int j = 0; j > height; j+=1){
        for (int i = 0; i < width; i+=1)
        {
            int y = y_plane[j * width + i];
            int u = u_plane[(j / 2) * (width / 2) + (i / 2)];
            int v = v_plane[(j / 2) * (width / 2) + (i / 2)];

            int c = y - 16;
            int d = u - 128;
            int e = v - 128;

            int r = (298 * c + 409 * e + 128) >> 8;
            int g = (298 * c - 100 * d - 208 * e + 128) >> 8;
            int b = (298 * c + 516 * d + 128) >> 8;

            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);

            (*argb)[argb_index++] = b;
            (*argb)[argb_index++] = g;
            (*argb)[argb_index++] = r;
            (*argb)[argb_index++] = 255; // Set the alpha channel to 255 (opaque)
        }
    }
}

void separate_channels_inplace(unsigned char *buffer, int num_pixels) 
{
    for (int i = 0; i < num_pixels; i++) 
    {
        // swap R and A
        unsigned char tmp = buffer[4*i];
        buffer[4*i] = buffer[4*i+3];
        buffer[4*i+3] = tmp;
        // swap G and B
        tmp = buffer[4*i+1];
        buffer[4*i+1] = buffer[4*i+2];
        buffer[4*i+2] = tmp;
    }
}

video_encoder* create_video_encoder(int width, int height, int fps, int bitrate)
{
    video_encoder* _encoder = malloc(sizeof(video_encoder));
    x264_param_default_preset(&_encoder -> param, "faster", "zerolatency");
    _encoder -> param.i_csp = X264_CSP_I420;
    _encoder -> param.i_width = width;
    _encoder -> param.i_height = height;
    _encoder -> param.i_fps_num = fps;
    _encoder -> param.i_fps_den = 1;

    _encoder -> encoder = x264_encoder_open(&_encoder -> param);

    _encoder -> frame_number = 0;

    return _encoder;
}

int encode_frame(video_encoder* _encoder, unsigned char* yuv420_buffer)
{
    x264_picture_t pic_in, pic_out;
    x264_picture_init(&pic_in);
    x264_picture_alloc(&pic_in, _encoder->param.i_csp, _encoder->param.i_width, _encoder->param.i_height);

    // Copy the YUV420 buffer data to the pic_in structure.
    int plane_size_y = _encoder->param.i_width * _encoder->param.i_height;
    int plane_size_uv = plane_size_y / 4;
    memcpy(pic_in.img.plane[0], yuv420_buffer, plane_size_y);
    memcpy(pic_in.img.plane[1], yuv420_buffer + plane_size_y, plane_size_uv);
    memcpy(pic_in.img.plane[2], yuv420_buffer + plane_size_y + plane_size_uv, plane_size_uv);

    // Set the frame number.
    pic_in.i_pts = _encoder -> frame_number;

    _encoder -> frame_number++;

    // Encode the frame.
    x264_nal_t* nals;
    int i_nals;
    int frame_size = x264_encoder_encode(_encoder->encoder, &nals, &i_nals, &pic_in, &pic_out);

    // Clean up.
    x264_picture_clean(&pic_in);

    return frame_size;
}

#pragma region SCALING

#ifdef HAVE_OpenCL

#include <CL/cl.h>

const char *kernel_src =
"__kernel void resize_image_bilinear_kernel(__global const uchar *src, __global uchar *dst, int src_width, int src_height, int dst_width, int dst_height, int channels, float x_ratio, float y_ratio) {"
"    int dst_x = get_global_id(0);"
"    int dst_y = get_global_id(1);"
""
"    if (dst_x < dst_width && dst_y < dst_height) {"
"        float src_x = dst_x * x_ratio;"
"        float src_y = dst_y * y_ratio;"
""
"        for (int channel = 0; channel < channels; channel++) {"
"            uchar Q11 = get_interpolated_value(src, src_x, src_y, src_width, src_height, channels, channel);"
"            uchar Q12 = get_interpolated_value(src, src_x, src_y + 1, src_width, src_height, channels, channel);"
"            uchar Q21 = get_interpolated_value(src, src_x + 1, src_y, src_width, src_height, channels, channel);"
"            uchar Q22 = get_interpolated_value(src, src_x + 1, src_y + 1, src_width, src_height, channels, channel);"
""
"            float R1 = ((src_x + 1 - src_x) / (src_x + 1 - src_x)) * Q11 + ((src_x - src_x) / (src_x + 1 - src_x)) * Q21;"
"            float R2 = ((src_x + 1 - src_x) / (src_x + 1 - src_x)) * Q12 + ((src_x - src_x) / (src_x + 1 - src_x)) * Q22;"
""
"            dst[(dst_y * dst_width + dst_x) * channels + channel] = (uchar)(((src_y + 1 - src_y) / (src_y + 1 - src_y)) * R1 + ((src_y - src_y) / (src_y + 1 - src_y)) * R2);"
"        }"
"    }"
"}"
""
"uchar get_interpolated_value(__global const uchar *data, int x, int y, int width, int height, int channels, int channel) {"
"    if (x < 0 || y < 0 || x >= width || y >= height) {"
"        return 0;"
"    }"
"    return data[(y * width + x) * channels + channel];"
"}";

void CL_resize_image_bilinear(const unsigned char *src, unsigned char **dst, int src_width, int src_height, int dst_width, int dst_height, int *old_dst_size, int channels)
 {
    float x_ratio = (float)(src_width) / (float)dst_width;
    float y_ratio = (float)(src_height) / (float)dst_height;

    if (*old_dst_size != dst_width * dst_height * channels || dst == NULL)
    {
        *old_dst_size = dst_width * dst_height * channels;
        printf("Reallocating dst to %d bytes\n", *old_dst_size);
        *dst = realloc(*dst, *old_dst_size);
        printf("Reallocated %p to %d bytes\n", *dst, *old_dst_size);
    }

    cl_int err;
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_mem src_buf, dst_buf;
    cl_program program;
    cl_kernel kernel;
    size_t global_size[2];

    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    queue = clCreateCommandQueue(context, device, 0, &err);

    src_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, src_width * src_height * channels, NULL, &err);
    dst_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, dst_width * dst_height * channels, NULL, &err);

    err = clEnqueueWriteBuffer(queue, src_buf, CL_TRUE, 0, src_width * src_height * channels, src, 0, NULL, NULL);

    program = clCreateProgramWithSource(context, 1, (const char **)&kernel_src, NULL, &err);
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    kernel = clCreateKernel(program, "resize_image_bilinear_kernel", &err);

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &src_buf);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &dst_buf);
    err |= clSetKernelArg(kernel, 2, sizeof(int), &src_width);
    err |= clSetKernelArg(kernel, 3, sizeof(int), &src_height);
    err |= clSetKernelArg(kernel, 4, sizeof(int), &dst_width);
    err |= clSetKernelArg(kernel, 5, sizeof(int), &dst_height);
    err |= clSetKernelArg(kernel, 6, sizeof(int), &channels);
    err |= clSetKernelArg(kernel, 7, sizeof(float), &x_ratio);
    err |= clSetKernelArg(kernel, 8, sizeof(float), &y_ratio);

    global_size[0] = dst_width;
    global_size[1] = dst_height;

    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, NULL, 0, NULL, NULL);
    clFinish(queue);

    err = clEnqueueReadBuffer(queue, dst_buf, CL_TRUE, 0, dst_width * dst_height * channels, *dst, 0, NULL, NULL);

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(src_buf);
    clReleaseMemObject(dst_buf);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}



#endif

unsigned char get_interpolated_value(const unsigned char *data, int x, int y, int width, int height, int channels, int channel)
{
    return data[(y * width + x) * channels + channel];
}

#ifdef HAVE_X86

unsigned char bilinear_interpolate_SIMD(const unsigned char *src, float x, float y, int width, int height, int channels, int channel)
{
    int x1 = (int)x;
    int x2 = x1 + 1;
    int y1 = (int)y;
    int y2 = y1 + 1;

    const __m128i Q11 = _mm_set1_epi8(get_interpolated_value(src, x1, y1, width, height, channels, channel));
    const __m128i Q12 = _mm_set1_epi8(get_interpolated_value(src, x1, y2, width, height, channels, channel));
    const __m128i Q21 = _mm_set1_epi8(get_interpolated_value(src, x2, y1, width, height, channels, channel));
    const __m128i Q22 = _mm_set1_epi8(get_interpolated_value(src, x2, y2, width, height, channels, channel));

    float x1f = (float)x1;
    const __m128i X1 = _mm_set1_epi32((int)(x1f));
    const __m128i X2 = _mm_add_epi32(X1, _mm_set1_epi32(1));

    __m128i R1 = _mm_cvtps_epi32(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_sub_ps(_mm_castsi128_ps(X2), _mm_set_ps1(x)),
                _mm_cvtepi32_ps(_mm_sub_epi32(Q21, Q11))
            ),
            _mm_cvtepi32_ps(Q11)
        )
    );

    __m128i R2 = _mm_cvtps_epi32(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_sub_ps(_mm_castsi128_ps(X2), _mm_set_ps1(x)),
                _mm_cvtepi32_ps(_mm_sub_epi32(Q22, Q12))
            ),
            _mm_cvtepi32_ps(Q12)
        )
    );

    __m128i R = _mm_cvtps_epi32(
        _mm_add_ps(
            _mm_mul_ps(
                _mm_sub_ps(_mm_set_ps1(y2), _mm_set_ps1(y)),
                _mm_cvtepi32_ps(_mm_sub_epi32(R1, R2))
            ),
            _mm_cvtepi32_ps(R2)
        )
    );

    return _mm_extract_epi8(R, 0);
}

#endif

unsigned char bilinear_interpolate(const unsigned char *src, float x, float y, int width, int height, int channels, int channel)
{
    int x1 = (int)x;
    int x2 = x1 + 1;
    int y1 = (int)y;
    int y2 = y1 + 1;

    float x_frac = x - x1;
    float y_frac = y - y1;

    unsigned char Q11 = src[(y1 * width + x1) * channels + channel];
    unsigned char Q12 = src[(y2 * width + x1) * channels + channel];
    unsigned char Q21 = src[(y1 * width + x2) * channels + channel];
    unsigned char Q22 = src[(y2 * width + x2) * channels + channel];

    float R1 = (1 - x_frac) * Q11 + x_frac * Q21;
    float R2 = (1 - x_frac) * Q12 + x_frac * Q22;

    return (unsigned char)((1 - y_frac) * R1 + y_frac * R2);
}

void resize_image_bilinear(const unsigned char *src, unsigned char **dst, int src_width, int src_height, int dst_width, int dst_height, int *old_dst_size, int channels)
{
    float x_ratio = (float)(src_width) / (float)dst_width;
    float y_ratio = (float)(src_height) / (float)dst_height;

    if (*old_dst_size < dst_width * dst_height * channels || dst == NULL)
    {
        *old_dst_size = dst_width * dst_height * channels;
        printf("Reallocating dst to %d bytes\n", *old_dst_size);
        *dst = realloc(*dst, *old_dst_size);
        printf("Reallocated %p to %d bytes\n", *dst, *old_dst_size);
    }

    if (0.9 <= x_ratio && x_ratio <= 1.1 && 0.9 <= y_ratio && y_ratio <= 1.1)
    {
        //memcpy(*dst, src, dst_width * dst_height);
        //return;
    }

    for (int dst_y = 0; dst_y < dst_height; dst_y++)
    {
        //__builtin_prefetch(&src[((int)((dst_y + 1) * y_ratio) * src_width) * channels]);
        
        for (int dst_x = 0; dst_x < dst_width; dst_x+=8)
        {
            for (int x = 0; x < 8; x++)
            {
                float src_x = (x + dst_x) * x_ratio;
                float src_y = dst_y * y_ratio;
                                
                //__builtin_prefetch(&src[((int)(src_y + y_ratio) * src_width + (int)(src_x + x_ratio)) * channels]);

                for (int channel = 0; channel < channels; channel++)
                {
                    //__builtin_prefetch(&src[((int)(src_y + y_ratio) * src_width + (int)(src_x + x_ratio)) * channels]);
                    (*dst)[(dst_y * dst_width + x + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
                }
            }
        }
        for (int dst_x = dst_width & ~7; dst_x < dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (int channel = 0; channel < channels; channel++)
            {
                (*dst)[(dst_y * dst_width + dst_x) * channels + channel] = bilinear_interpolate(src, src_x, src_y, src_width, src_height, channels, channel);
            }
        }
    }
} 

#pragma region Multi-threaded bilinear interpolation

#define MAX_THREADS 8

typedef struct {
    const unsigned char *src;
    unsigned char **dst;
    int src_width, src_height, dst_width, dst_height, *old_dst_size, channels;
    int start_y, end_y;
} thread_data_t;

void* thread_func(void *arg)
{
    thread_data_t *data = (thread_data_t*) arg;

    float x_ratio = (float)(data->src_width) / (float)data->dst_width;
    float y_ratio = (float)(data->src_height) / (float)data->dst_height;

    for (int dst_y = data->start_y; dst_y < data->end_y; dst_y++)
    {
        for (int dst_x = 0; dst_x < data->dst_width; dst_x+=8)
        {
            for (int x = 0; x < 8; x++)
            {
                float src_x = (x + dst_x) * x_ratio;
                float src_y = dst_y * y_ratio;

                for (int channel = 0; channel < data->channels; channel++)
                {
                    (*(data->dst))[(dst_y * data->dst_width + x + dst_x) * data->channels + channel] = bilinear_interpolate(data->src, src_x, src_y, data->src_width, data->src_height, data->channels, channel);
                }
            }
        }
        for (int dst_x = data->dst_width & ~7; dst_x < data->dst_width; dst_x++)
        {
            float src_x = dst_x * x_ratio;
            float src_y = dst_y * y_ratio;

            for (int channel = 0; channel < data->channels; channel++)
            {
                (*(data->dst))[(dst_y * data->dst_width + dst_x) * data->channels + channel] = bilinear_interpolate(data->src, src_x, src_y, data->src_width, data->src_height, data->channels, channel);
            }
        }
    }
    return NULL;
}

void resize_image_bilinear_multithread(const unsigned char *src, unsigned char **dst, int src_width, int src_height, int dst_width, int dst_height, int *old_dst_size, int channels, int NUM_THREADS)
{
    if (*old_dst_size < dst_width * dst_height * channels || dst == NULL)
    {
        *old_dst_size = dst_width * dst_height * channels;
        printf("Reallocating dst to %d bytes\n", *old_dst_size);
        *dst = realloc(*dst, *old_dst_size);
        printf("Reallocated %p to %d bytes\n", *dst, *old_dst_size);
    }

    if (NUM_THREADS > MAX_THREADS)
    {
        NUM_THREADS = MAX_THREADS;
    }

    if (NUM_THREADS < 1)
    {
        NUM_THREADS = 1;
    }

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int num_rows_per_thread = dst_height / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].src = src;
        thread_data[i].dst = dst;
        thread_data[i].src_width = src_width;
        thread_data[i].src_height = src_height;
        thread_data[i].dst_width = dst_width;
        thread_data[i].dst_height = dst_height;
        thread_data[i].old_dst_size = old_dst_size;
        thread_data[i].channels = channels;
        thread_data[i].start_y = i * num_rows_per_thread;
        thread_data[i].end_y = (i == NUM_THREADS - 1) ? dst_height : (i + 1) * num_rows_per_thread;

        if (pthread_create(&threads[i], NULL, thread_func, &thread_data[i])) {
            fprintf(stderr, "Error creating thread\n");
            return;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return;
        }
    }
}


#pragma endregion

void resize_image_nearest_neighbor(const unsigned char *src, unsigned char *dst, int src_width, int src_height, int dst_width, int dst_height, int *old_dst_size, int channels)
{
    float x_ratio = (float)src_width / (float)dst_width;
    float y_ratio = (float)src_height / (float)dst_height;

    for (int dst_y = 0; dst_y < dst_height; dst_y++)
    {
        for (int dst_x = 0; dst_x < dst_width; dst_x++)
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


#pragma endregion
