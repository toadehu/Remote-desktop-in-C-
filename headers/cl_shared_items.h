/*
 *  This header contains shared datastructures and constants between the client and server
 *
 *
 */

struct screen_data
{
	int screen_width, screen_height;
	int image_width, image_height, image_dim, type, no_blocks;
  int quality;
	int *buf_size;
	unsigned char *screen_bits, *converted_bits, *dst, *jpeg_arr, *offsets;
	struct OpenCLFunctionWrapper **openclKernels;
	int kernels;
};

#define DEFAULT_QUALITY 80
