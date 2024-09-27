__constant float C[8][8] = {
    {0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  0.3535533905932738 },
 { 0.4903926402016152,  0.4157348061512726, 0.27778511650980114,  0.09754516100806417,  -0.09754516100806417,  -0.27778511650980114,  -0.4157348061512726,  -0.4903926402016152 },
 { 0.46193976625564337,  0.19134171618254492, -0.19134171618254492, -0.46193976625564337,  -0.46193976625564337,  -0.19134171618254492,  0.19134171618254492,  0.46193976625564337 },
 { 0.4157348061512726, -0.09754516100806417,  -0.4903926402016152,  -0.27778511650980114,  0.27778511650980114,  0.4903926402016152,  0.09754516100806417,  -0.4157348061512726 },
  { 0.3535533905932738,  -0.3535533905932738,  -0.3535533905932738,  0.3535533905932738,  0.3535533905932738,  -0.3535533905932738,  -0.3535533905932738,  0.3535533905932738 },
 { 0.27778511650980114,  -0.4903926402016152,  0.09754516100806417,  0.4157348061512726,  -0.4157348061512726,  -0.09754516100806417,  0.4903926402016152,  -0.27778511650980114 },
 { 0.19134171618254492,  -0.46193976625564337,  0.46193976625564337,  -0.19134171618254492,  -0.19134171618254492,  0.46193976625564337,  -0.46193976625564337,  0.19134171618254492 },
 { 0.09754516100806417,  -0.27778511650980114,  0.4157348061512726,  -0.4903926402016152,  0.4903926402016152,  -0.4157348061512726,  0.27778511650980114,  -0.09754516100806417 } };

__constant float alpha[8] = { 0.5, 0.4903926402016152, 0.46193976625564337, 0.4157348061512726, 0.3535533905932738, 0.27778511650980114, 0.19134171618254492, 0.09754516100806417 };

__constant float LQM2[8][8] = {{ 16, 11, 10, 16, 24, 40, 51, 61 },
  { 12, 12, 14, 19, 26, 58, 60, 55 },
  { 14, 13, 16, 24, 40, 57, 69, 56 },
  { 14, 17, 22, 29, 51, 87, 80, 62 },
  { 18, 22, 37, 56, 68, 109, 103, 77 },
  { 24, 35, 55, 64, 81, 104, 113, 92 },
  { 49, 64, 78, 87, 103, 121, 120, 101 },
  { 72, 92, 95, 98, 112, 100, 103, 99 }};

__constant float LQM[8][8] = {{ 8,  6,  6,  7,  6,  5,  8,  7},
 { 7,  7,  9,  9,  8, 10, 12, 20},
 {13, 12, 11, 11, 12, 25, 18, 19},
 {15, 20, 29, 26, 31, 30, 29, 26},
 {28, 28, 32, 36, 46, 39, 32, 34},
 {44, 35, 28, 28, 40, 55, 41, 44},
 {48, 49, 52, 52, 52, 31, 39, 57},
 {61, 56, 50, 60, 46, 51, 52, 50}};

__constant float CQM2[8][8] = {{ 17, 18, 24, 47, 99, 99, 99, 99 },
  { 18, 21, 26, 66, 99, 99, 99, 99 },
  { 24, 26, 56, 99, 99, 99, 99, 99 },
  { 47, 66, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 },
  { 99, 99, 99, 99, 99, 99, 99, 99 }};

__constant float CQM[8][8] = {{ 9,  9,  9, 12, 11, 12, 24, 13},
 {13, 24, 50, 33, 28, 33, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50},
 {50, 50, 50, 50, 50, 50, 50, 50}};


__constant uchar _posX[64] = { 0, 0, 1, 2, 1, 0, 0, 1, 2, 3, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 4, 5, 6, 7, 7, 6, 5, 6, 7, 7 };
__constant uchar _posY[64] = { 0, 1, 0, 0, 1, 2, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 5, 6, 7, 7, 6, 7 };

__constant int YUV420 = 1; /* Y Cb Cr with 4 : 2 : 0 subsamping (channels Cb and Cr are down to 1/4 size) */
__constant int YUV444 = 2; /* Y Cb Cr with no subsampling */
__constant int RGB8 = 3; /* RGB with 8 bits per pixel */
__constant int ARGB8 = 4; /* ARGB with 8 bits per pixel */


void apply_jpeg(__global const uchar* src,
                __global short *dst,
                __global short *global_rle,
		        __global short *offsets,
                int width, int height, int channels,
                int phi, int type)
{
    float block[8][8];
    int i,j;
    int pos_w, pos_h;
    pos_w = 8 * get_global_id(0);
    pos_h = 8 * get_global_id(1);

    int offset = 0;
    if (type != YUV420)
    {
	    offset = (width) * (height) * get_global_id(2);
    }
    else
    {
      if (get_global_id(2))
    {
      height /= 2;
      width /= 2;
      if (get_global_id(2) == 1)
      {
        offset = width * height * 4;
      }
      else /* channel == 2 */
      {
        offset = width * height * 5;
      }
    }}
	if (get_global_id(0) == 0 && get_global_id(1) == 0 && get_global_id(2) == 2)
	{
		printf("offset is: %d\n", offset);
	}

    for (i = pos_h; i < pos_h + 8; ++i)
    {
      for (j = pos_w; j < pos_w + 8; ++j)
      {
        block[i - pos_h][j - pos_w] = (float)src[i * width + j + offset];
      }
    }
    /* Time to DCT along the "horizontal" axis */
    float DCT_temp[8] = {};
    #pragma unroll
    for (i = 0; i < 8; ++i)
    {
        DCT_temp[0] = (block[i][0] + block[i][1] + block[i][2] + block[i][3] + block[i][4] + block[i][5] + block[i][6] + block[i][7]) * C[0][0];
        DCT_temp[1] = (block[i][0] - block[i][7]) * C[1][0] + (block[i][1] - block[i][6]) * C[1][1] + (block[i][2] - block[i][5]) * C[1][2] + (block[i][3] - block[i][4]) * C[1][3];
        DCT_temp[2] = (block[i][0] + block[i][7] - block[i][3] - block[i][4]) * C[2][0] + (block[i][1] + block[i][6] - block[i][2] - block[i][5]) * C[2][1];
        DCT_temp[3] = (block[i][0] - block[i][7]) * C[3][0] + (block[i][1] - block[i][6]) * C[3][1] + (block[i][2] - block[i][5]) * C[3][2] + (block[i][3] - block[i][4]) * C[3][3];
        DCT_temp[4] = (block[i][0] - block[i][1] - block[i][2] + block[i][3] + block[i][4] - block[i][5] - block[i][6] + block[i][7]) * C[4][0];
        DCT_temp[5] = (block[i][0] - block[i][7]) * C[5][0] + (block[i][1] - block[i][6]) * C[5][1] + (block[i][2] - block[i][5]) * C[5][2] + (block[i][3] - block[i][4]) * C[5][3];
        DCT_temp[6] = (block[i][0] + block[i][7] - block[i][3] - block[i][4]) * C[6][0] + (block[i][1] + block[i][6] - block[i][2] - block[i][5]) * C[6][1];
        DCT_temp[7] = (block[i][0] - block[i][7]) * C[7][0] + (block[i][1] - block[i][6]) * C[7][1] + (block[i][2] - block[i][5]) * C[7][2] + (block[i][3] - block[i][4]) * C[7][3];

        // Write back the results for the horizontal pass
        for (j = 0; j < 8; ++j)
        {
            block[i][j] = DCT_temp[j];
        }
    }

    /* Time to DCT along the vertical axis */
    #pragma unroll
    for (i = 0; i < 8; ++i)
    {
        DCT_temp[0] = (block[0][i] + block[1][i] + block[2][i] + block[3][i] + block[4][i] + block[5][i] + block[6][i] + block[7][i]) * C[0][0];
        DCT_temp[1] = (block[0][i] - block[7][i]) * C[1][0] + (block[1][i] - block[6][i]) * C[1][1] + (block[2][i] - block[5][i]) * C[1][2] + (block[3][i] - block[4][i]) * C[1][3];
        DCT_temp[2] = (block[0][i] + block[7][i] - block[3][i] - block[4][i]) * C[2][0] + (block[1][i] + block[6][i] - block[2][i] - block[5][i]) * C[2][1];
        DCT_temp[3] = (block[0][i] - block[7][i]) * C[3][0] + (block[1][i] - block[6][i]) * C[3][1] + (block[2][i] - block[5][i]) * C[3][2] + (block[3][i] - block[4][i]) * C[3][3];
        DCT_temp[4] = (block[0][i] - block[1][i] - block[2][i] + block[3][i] + block[4][i] - block[5][i] - block[6][i] + block[7][i]) * C[4][0];
        DCT_temp[5] = (block[0][i] - block[7][i]) * C[5][0] + (block[1][i] - block[6][i]) * C[5][1] + (block[2][i] - block[5][i]) * C[5][2] + (block[3][i] - block[4][i]) * C[5][3];
        DCT_temp[6] = (block[0][i] + block[7][i] - block[3][i] - block[4][i]) * C[6][0] + (block[1][i] + block[6][i] - block[2][i] - block[5][i]) * C[6][1];
        DCT_temp[7] = (block[0][i] - block[7][i]) * C[7][0] + (block[1][i] - block[6][i]) * C[7][1] + (block[2][i] - block[5][i]) * C[7][2] + (block[3][i] - block[4][i]) * C[7][3];

        // Write back the results for the vertical pass
        for (j = 0; j < 8; ++j)
        {
            block[j][i] = DCT_temp[j];
        }
    }

    /* Now it't time to quantize */
    if (phi < 95) //din topor
    {
      int S = 200 - 2 * phi;
      if (phi < 50)
      {
        S = 5000 / phi;
      }
      if (get_global_id(2))
      {
      #pragma unroll
        for (i = 0; i < 8; ++i)
        {
          for (j = 0; j < 8; ++j)
          {
            float val = block[i][j], div = CQM[i][j];
            div = div * S + 50.0;
            div /= 100.0;
            block[i][j] = (val / div);
          }
        }
      }
      else
      {
      #pragma unroll
        for (i = 0; i < 8; ++i)
        {
          for (j = 0; j < 8; ++j)
          {
            float val = block[i][j], div = LQM[i][j];
            div = div * S + 50.0;
            div /= 100.0;
            block[i][j] = (val / div);
          }
        }
      }
    }

    /* Error Diffusion */
    #pragma unroll
    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            float quantized_value = round(block[i][j]);
            float error = block[i][j] - quantized_value;
            block[i][j] = quantized_value;

            // Distribute the error to neighboring pixels within the 8x8 block
            if (j < 7) // Right neighbor (i, j+1)
            {
                block[i][j + 1] += error * 7.0f / 16.0f;
            }
            if (i < 7) // Below neighbors
            {
                if (j > 0) // Bottom-left (i+1, j-1)
                {
                    block[i + 1][j - 1] += error * 3.0f / 16.0f;
                }
                block[i + 1][j] += error * 5.0f / 16.0f; // Directly below (i+1, j)
                if (j < 7) // Bottom-right (i+1, j+1)
                {
                    block[i + 1][j + 1] += error * 1.0f / 16.0f;
                }
            }
            //if (block[i][j] < -50)
            //  block[i][j] = -50.0;
            //else if (block[i][j] > 255)
            //  block[i][j] = 255.0;
        }
    }

    /* TODO Perform RLE and final encode */
    float cur = block[0][0];
    int ap = 1, pos = offset/64 + (width/8) * get_global_id(1) + get_global_id(0);
    __global short *tempRLE = global_rle + pos * 128;
    __private int rleSize = 0;
    __private int sanity_check = 0;

    for (i = 1; i < 64; ++i)
    {
        float cval = block[_posX[i]][_posY[i]];

        if (cval != cur)
        {
            sanity_check += ap;
            tempRLE[rleSize] = ap;
            tempRLE[rleSize+1] = (short)((int)cur);
	          rleSize = rleSize + 2;
            ap = 0;
            cur = cval;
        }
        ap++;
    }

    sanity_check+=ap;
    tempRLE[rleSize++] = ap;
    tempRLE[rleSize++] = (short)cur;
    offsets[pos + 1] = rleSize;
    /* Now Final encode will be done in another function */

}


/**
  @brief Performs jpeg encoding
  @param src source image
  @param dst destination array
  @param global_rle local memory to store the RLE values before final coding
  @param width image width
  @param height image height
  @param type image type (YUV420, YUV444, RGB8 or ARGB8 ONLY no 10bpp yet or ever)
  @param phi quality from 0 to 100, see https://fotoforensics.com/tutorial-estq.php for more context
 */
__kernel void do_jpeg(
    __global const uchar *src, /* Source image */
    __global short *dst, /* Destination encoded image */
    __global short *global_rle, /* here we store the RLE values, only the GPU/Accelerator has access to it*/
    __global short *offsets,
    int width, int height,
    int type, int phi)
{
    int gid_x = get_global_id(0);
    int gid_y = get_global_id(1);
    int gid_ch = get_global_id(2);
    int channels = 3;
    if (type == ARGB8)
    {
      channels = 4;
    }
    /* This looks wrong TODO rewrite */
    if (type == YUV420 && gid_ch) /* In this case the 2nd and 3rd (1 and 2) channels are only half of the resolution so we need to account for that */
    {
        if (gid_x < (width / 16) && gid_y < (height / 16))
        {
          apply_jpeg(src, dst, global_rle, offsets,
          width, height, channels,
          phi, type);
        }
    }
    else
    {
    apply_jpeg(src, dst, global_rle, offsets,
      width, height, channels,
      phi, type);
    }
}

/**
  @brief Assembles the final array, after RLE is performed
  @param src the source array
  @param dst the destination array
  @param offsets the offsets array
  @param chunk_size the number of elements to process per thread/work-item
  @param no_blocks the total number of blocks
*/
__kernel void assemble_array( __global const uchar *src,
                              __global uchar *dst,
                              __global int *offsets,
                              int chunk_size, int no_blocks)
{
    int gid_x = get_global_id(0);
    int index = gid_x * chunk_size;
    int end = index + chunk_size;
    if (end > no_blocks)
      end = no_blocks;
    int dst_offset = offsets[index] - offsets[0];
    int src_start = index * 128;
    for (; index < end; ++index)
    {
      int start_cpy = src_start;
      while (src[start_cpy])
      {
          dst[dst_offset++] = src[start_cpy++];
          dst[dst_offset++] = src[start_cpy++];
          //start_cpy++;
      }
      src_start += 128;
    }
}

/* Opposite of get corner id
*
*/
unsigned int get_block_id(int width, int height, int channel, int type, int corner_id) {
  unsigned int ret = 0;
  unsigned int regular_channel = (width / 8) * (height / 8);

  if (type == YUV420) {
    if (corner_id >= regular_channel) {
	corner_id -= regular_channel;
      unsigned int sub_width = width / 16;
      unsigned int sub_height = height / 16;
      unsigned int uv_corner_id = corner_id;
      if (uv_corner_id >= sub_width * sub_height) {
        corner_id -= (sub_width * sub_height);
      }
      // Pack y into the lower 16 bits and x into the higher 16 bits
      ret = ((corner_id % sub_width) << 16) + (corner_id / sub_width);
    }

    else {
      // Pack y into the lower 16 bits and x into the higher 16 bits
      ret = ((corner_id % (width / 8)) << 16) + (corner_id / (width / 8));
    }
  } 
  else {
    int w = width / 8;
    // Pack y into the lower 16 bits and x into the higher 16 bits
    ret = ((corner_id % w) << 16);
    if (corner_id > regular_channel)
      corner_id -= regular_channel;
    if (corner_id > regular_channel)
      corner_id -= regular_channel;
    ret += (corner_id / w);
  }
  return ret;
}

void apply_gaussian_blur(float block[8][8]) {
    float blurred_block[8][8] = {0};
    int i, j, di, dj;
    float gaussian_kernel[3][3] = {
        {0.0625, 0.125, 0.0625},
        {0.125,  0.25,  0.125},
        {0.0625, 0.125, 0.0625}
    };

    // Apply 3x3 Gaussian kernel across the 8x8 block
    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j) {
            float sum = 0.0;
            for (di = -1; di <= 1; ++di) {
                for (dj = -1; dj <= 1; ++dj) {
                    int x = i + di;
                    int y = j + dj;
		    /*if (x < 0)
		    	x = 1;
		    if (x >= 8)
		    	x = 6;
		    if (y < 0)
		    	y = 1;
		    if (y >= 8)
		    	y = 6;
                    */
		    // Ensure we don't go out of bounds (border handling)
                    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                        sum += block[x][y] * gaussian_kernel[di+1][dj+1];
                    }
		    else
		    	sum += block[i][j] * gaussian_kernel[di + 1][dj + 1];
                }
            }
            blurred_block[i][j] = sum;
        }
    }

    // Copy the result back to the original block
    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j) {
            block[i][j] = blurred_block[i][j];
        }
    }
}

void apply_sharpening(float block[8][8]) {
    float sharpened_block[8][8] = {0};
    int i, j, di, dj;
    float sharpen_kernel[3][3] = {
        { 0,  -1,  0},
        {-1,   5, -1},
        { 0,  -1,  0}
    };

    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j) {
            float sum = 0.0;
            for (di = -1; di <= 1; ++di) {
                for (dj = -1; dj <= 1; ++dj) {
                    int x = i + di;
                    int y = j + dj;
                    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                        sum += block[x][y] * sharpen_kernel[di+1][dj+1];
                    }
                }
            }
            sharpened_block[i][j] = sum;
        }
    }

    // Copy the result back to the original block
    for (i = 0; i < 8; ++i) {
        for (j = 0; j < 8; ++j) {
            block[i][j] = sharpened_block[i][j];
        }
    }
}

void decode_block(__global short* src,
                  __global uchar* dst,
                  __global int* blk_pos,
		              int width, int height,
                  int blk_id, int type, int phi)
{
	__global short* read_pos = src + blk_pos[get_global_id(0)];
	int write_offset = blk_id * 64;
	__global uchar* write_pos = dst + write_offset;

	float cur_block[8][8];

	__constant float (*Q)[8][8] = &LQM;
	//int blk_type = get_blk_type(width, height, blk_id); /* Mersi Fuca Lechete */
	if (blk_id >= (((width+7)/8)*(height+7)/8))
	{
		Q = &CQM;
	}
	float S = 200 - 2 * phi;
	if (phi < 50)
	{
		S = 5000.0 / (float)phi;
	}
	int i,j, no_copies = 0, val_copy=69;
	
	int contl = 0;

  	int diff = 0;


	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; ++j)
		{
			if (no_copies == 0)
			{
        			diff++;
				no_copies = *read_pos;
				contl += no_copies;
				read_pos++;
				val_copy = *read_pos;
				read_pos++;
			}

			no_copies--;
			int px = _posX[i * 8 + j], py = _posY[i * 8 + j];
      			float val = ((float)(*Q)[px][py] * (float)S + 50.0) / 100.0;

			cur_block[px][py] = (float)(val_copy * val);
	  	}
	}


	/*	double val0f = (v[0] + v[4]) * alpha[4] + v[2] * alpha[2] + v[6] * alpha[6];
            double val0s = v[1] * alpha[1] + v[3] * alpha[3] + v[5] * alpha[5] + v[7] * alpha[7];
            double val1f = (v[0] - v[4]) * alpha[4] + v[2] * alpha[6] + v[6] * (-alpha[2]);
            double val1s = v[1] * alpha[3] + v[3] * (-alpha[7]) + v[5] * (-alpha[1]) + v[7] * (-alpha[5]);
            double val2f = (v[0] - v[4]) * alpha[4] + v[2] * (-alpha[6]) + v[6] * (alpha[2]);
            double val2s = v[1] * alpha[5] + v[3] * (-alpha[1]) + v[5] * alpha[7] + v[7] * alpha[3];
            double val3f = (v[0] + v[4]) * alpha[4] + v[2] * (-alpha[2]) + v[6] * (-alpha[6]);
            double val3s = v[1] * alpha[7] + v[3] * (-alpha[5]) + v[5] * alpha[3] + v[7] * (-alpha[1]);
            v[0] = val0f + val0s;
            v[1] = val1f + val1s;
            v[2] = val2f + val2s;
            v[3] = val3f + val3s;
            v[4] = val3f - val3s;
            v[5] = val2f - val2s;
            v[6] = val1f - val1s;
            v[7] = val0f - val0s;*/
	float DCT_temp[8];
	for (i = 0; i < 8; ++i) 
  {
    DCT_temp[0] =  (cur_block[i][0] + cur_block[i][4]) * alpha[4] + cur_block[i][2] * alpha[2] + cur_block[i][6] * alpha[6];

    DCT_temp[1] = cur_block[i][1] * alpha[1] + cur_block[i][3] * alpha[3] + cur_block[i][5] * alpha[5] + cur_block[i][7] * alpha[7];
    
    DCT_temp[2] =  (cur_block[i][0] - cur_block[i][4]) * alpha[4] + cur_block[i][2] * alpha[6] - cur_block[i][6] * alpha[2];

    DCT_temp[3] =  cur_block[i][1] * alpha[3] - cur_block[i][3] * alpha[7] - cur_block[i][5] * alpha[1] - cur_block[i][7] * alpha[5];

    DCT_temp[4] = (cur_block[i][0] - cur_block[i][4]) * alpha[4] - cur_block[i][2] * alpha[6] + cur_block[i][6] * alpha[2];

    DCT_temp[5] = cur_block[i][1] * alpha[5] - cur_block[i][3] * alpha[1] + cur_block[i][5] * alpha[7] + cur_block[i][7] * alpha[3];

    DCT_temp[6] =  (cur_block[i][0] + cur_block[i][4]) * alpha[4] - cur_block[i][2] * alpha[2] - cur_block[i][6] * alpha[6];

    DCT_temp[7] = cur_block[i][1] * alpha[7] - cur_block[i][3] * alpha[5] + cur_block[i][5] * alpha[3] - cur_block[i][7] * alpha[1];


    cur_block[i][0] = DCT_temp[0] + DCT_temp[1];
    cur_block[i][1] = DCT_temp[2] + DCT_temp[3];
    cur_block[i][2] = DCT_temp[4] + DCT_temp[5];
    cur_block[i][3] = DCT_temp[6] + DCT_temp[7];
    cur_block[i][4] = DCT_temp[6] - DCT_temp[7];
    cur_block[i][5] = DCT_temp[4] - DCT_temp[5];
    cur_block[i][6] = DCT_temp[2] - DCT_temp[3];
    cur_block[i][7] = DCT_temp[0] - DCT_temp[1];
  }

  for (i = 0; i < 8; ++i) 
  {
    DCT_temp[0] =  (cur_block[0][i] + cur_block[4][i]) * alpha[4] + cur_block[2][i] * alpha[2] + cur_block[6][i] * alpha[6];

    DCT_temp[1] = cur_block[1][i] * alpha[1] + cur_block[3][i] * alpha[3] + cur_block[5][i] * alpha[5] + cur_block[7][i] * alpha[7];
    
    DCT_temp[2] =  (cur_block[0][i] - cur_block[4][i]) * alpha[4] + cur_block[2][i] * alpha[6] - cur_block[6][i] * alpha[2];

    DCT_temp[3] =  cur_block[1][i] * alpha[3] - cur_block[3][i] * alpha[7] - cur_block[5][i] * alpha[1] - cur_block[7][i] * alpha[5];

    DCT_temp[4] = (cur_block[0][i] - cur_block[4][i]) * alpha[4] - cur_block[2][i] * alpha[6] + cur_block[6][i] * alpha[2];

    DCT_temp[5] = cur_block[1][i] * alpha[5] - cur_block[3][i] * alpha[1] + cur_block[5][i] * alpha[7] + cur_block[7][i] * alpha[3];

    DCT_temp[6] =  (cur_block[0][i] + cur_block[4][i]) * alpha[4] - cur_block[2][i] * alpha[2] - cur_block[6][i] * alpha[6];

    DCT_temp[7] = cur_block[1][i] * alpha[7] - cur_block[3][i] * alpha[5] + cur_block[5][i] * alpha[3] - cur_block[7][i] * alpha[1];
 
    cur_block[0][i] = DCT_temp[0] + DCT_temp[1];
    cur_block[1][i] = DCT_temp[2] + DCT_temp[3];
    cur_block[2][i] = DCT_temp[4] + DCT_temp[5];
    cur_block[3][i] = DCT_temp[6] + DCT_temp[7];
    cur_block[4][i] = DCT_temp[6] - DCT_temp[7];
    cur_block[5][i] = DCT_temp[4] - DCT_temp[5];
    cur_block[6][i] = DCT_temp[2] - DCT_temp[3];
    cur_block[7][i] = DCT_temp[0] - DCT_temp[1];  
      barrier(CLK_LOCAL_MEM_FENCE);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    //apply_gaussian_blur(cur_block);
    //apply_sharpening(cur_block);


	/* We are now ready to do the task of writing the image data at long last */
	int blk_per_ch0 = (width / 8) * (height / 8), blk_per_ch1;
	blk_per_ch1 = blk_per_ch0;
	if (type == YUV420)
		blk_per_ch1 = (width/ 16) * (height / 16);
  int offset = 0;
  unsigned int corner = get_block_id(width, height, 3, type, blk_id);
  if (blk_id >= blk_per_ch0)
  {  
  	offset += width * height;
      if (type == YUV420)
      {
        width/=2;
        height/=2;
      }
  }
  if (blk_id >= blk_per_ch0 + blk_per_ch1)
  {
      offset += width * height;
  }
  int x = (corner>>16), y = corner & 0xFFFF;
  for (i=0; i < 8; ++i)
  {
      for(j=0; j < 8; ++j)
      {
          int pos = offset + (y * 8 + i) * width + j + x * 8;
          dst[pos] = (uchar)(255 + (int)cur_block[i][j]);
      }
  }
}

/**
 * @brief Function to decode the jpeg image
 * @param src source image (encoded)
 * @param dst destination image (decoded)
 * @param width original image width
 * @param height origianl image height
 * @param type image type (YUV420, YUV444, RGB8 or ARGB8 ONLY no 10bpp yet)
 * @param phi quality from 0 to 100, see https://fotoforensics.com/tutorial-estq.php for more context must be same as used for encoding or it will lead to terrible results
 */
__kernel void decode_jpeg(__global short* src, /* This is the encoded global_rle array from do_jpeg */
		__global int* blk_pos,  const int nr_blk,
                __global uchar* dst,
                int width, int height, int type, int phi)
{

  	int blk_id = get_global_id(0);
    if (blk_id < nr_blk)
    {
      decode_block(src, dst, blk_pos, width, height, blk_id, type, phi);
    }
}
