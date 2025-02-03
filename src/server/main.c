/*
	SERVER
*/

#include "framework.h"


#define HEADER_SIZE 20

int port;

struct screen_data video_enc;

int main (int argc, char *argv[])
{
	int i;
	for (i = 1; i < argc; i++)
	{
		switch (argv[i][1])
		{
		case 'h':
			printf("Usage: -p PORT\n");
			exit(0);
		case 'p':
			if (strlen(argv[i]) != 2)
			{
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR@user -p PORT\n");
				exit(1);
			}
			port = atoi(argv[i + 1]);
			i++;
			break;
		default:
			printf("Invalid argument\nCorrect usage: -ip4/6 IPADDR@user -p PORT\n");
			exit(1);

		}
	}

  /* Lazy placeholder that should be enough for 4k 16:10 at 10bpp or whatever cool people use */
  video_enc.image_dim = 0;
  video_enc.buf_size = malloc(5 * sizeof(int));

	/* Dry run to get the parameners at startup*/
	video_enc.screen_bits = NULL;
  video_enc.buf_size[0] = 0;
	capture_one_display(0, (char**)&video_enc.screen_bits, &video_enc.buf_size[0], 
	&video_enc.screen_width, &video_enc.screen_height);

	video_enc.kernels = 3;
	video_enc.openclKernels = (struct OpenCLFunctionWrapper**)malloc(sizeof(struct OpenCLFunctionWrapper*) * video_enc.kernels);
	memset(video_enc.buf_size, 0, 5 * sizeof(int));

	video_enc.image_height = 1200;
	video_enc.image_width = 1920;
	video_enc.image_dim = video_enc.image_width * video_enc.image_height;

	/* Yes I want page aligned memory. I will not take chances with OpenCL crying about it with error code -130184012047 */
	video_enc.converted_bits = __aligned_malloc(video_enc.image_dim * 8, 4096);
	video_enc.dst = __aligned_malloc(video_enc.image_dim * 8, 4096);
	video_enc.jpeg_arr = __aligned_malloc(video_enc.image_dim * 8, 4096);
	video_enc.offsets = __aligned_malloc(video_enc.image_dim, 4096);

  memset(video_enc.offsets, 0, video_enc.image_dim);

	video_enc.type = YUV420;
	if (video_enc.type == YUV420)
	{
		video_enc.no_blocks = (((video_enc.image_width+7)/8) * ((video_enc.image_height+7)/8)) * 1.5;
	}
	video_enc.quality = DEFAULT_QUALITY;

	video_enc.openclKernels[0] = createOpenCLWrapperStruct("../image_mainp.cl", KERNEL_FROM_FILE,
      "GPU_resize", NULL, 0, 0, 0);

	set_no_buffers(video_enc.openclKernels[0], 2);
	create_and_set_buf(video_enc.openclKernels[0], video_enc.screen_bits,
	 4 * video_enc.screen_width * video_enc.screen_height, sizeof(void*), 0, READWRITE, FROM_MEMORY);


	set_kernel_arg(video_enc.openclKernels[0], 1, sizeof(int), video_enc.screen_width);
	set_kernel_arg(video_enc.openclKernels[0], 2, sizeof(int), video_enc.screen_height);
	set_kernel_arg(video_enc.openclKernels[0], 3, sizeof(int), 4);
	create_and_set_buf(video_enc.openclKernels[0], video_enc.dst,
	 3 * video_enc.image_dim, sizeof(void*), 4, READWRITE, 0);
	set_kernel_arg(video_enc.openclKernels[0], 5, sizeof(int), video_enc.image_width);
	set_kernel_arg(video_enc.openclKernels[0], 6, sizeof(int), video_enc.image_height);
	set_kernel_arg(video_enc.openclKernels[0], 7, sizeof(int), 3);
	set_kernel_arg(video_enc.openclKernels[0], 8, sizeof(int), 2);
	set_kernel_arg(video_enc.openclKernels[0], 9, sizeof(float), 1.0);

	set_dimension_and_values(video_enc.openclKernels[0], 2, 
	video_enc.image_width, video_enc.image_height, 1);


	video_enc.openclKernels[1] = createSharedContextOpenCLWrapper(video_enc.openclKernels[0], 
														"../image_mainp.cl", KERNEL_FROM_FILE, 
													"GPU_ARGBToYUV420", NULL, 0);

	set_no_buffers(video_enc.openclKernels[1], 2);
	copy_buf_from_other(video_enc.openclKernels[0], video_enc.openclKernels[1],
	video_enc.dst, sizeof(void*), 0, 0);
	set_kernel_arg(video_enc.openclKernels[1], 1, sizeof(int), video_enc.image_width);
	set_kernel_arg(video_enc.openclKernels[1], 2, sizeof(int), video_enc.image_height);
	set_kernel_arg(video_enc.openclKernels[1], 3, sizeof(int), 3);
	create_and_set_buf(video_enc.openclKernels[1], video_enc.converted_bits,
  video_enc.image_dim * 2, sizeof(void*), 4, READWRITE, 0);
	set_dimension_and_values(video_enc.openclKernels[1], 1, video_enc.image_dim, 1, 1);

	video_enc.openclKernels[2] = createSharedContextOpenCLWrapper(video_enc.openclKernels[1], 
														"../jpegKernel.cl", KERNEL_FROM_FILE, 
													"do_jpeg", NULL, 0);
																	
	set_no_buffers(video_enc.openclKernels[2], 4);
  copy_buf_from_other(video_enc.openclKernels[1], video_enc.openclKernels[2], 
  video_enc.converted_bits, sizeof(void*), 0, 0);
  copy_buf_from_other(video_enc.openclKernels[0], video_enc.openclKernels[2], 
	video_enc.dst, sizeof(void*), 1, 0);
  create_and_set_buf(video_enc.openclKernels[2], video_enc.jpeg_arr, sizeof(short) * video_enc.image_dim * 4, sizeof(void*), 2, READWRITE, TO_MEMORY);
  create_and_set_buf(video_enc.openclKernels[2], video_enc.offsets, sizeof(short)* (video_enc.no_blocks + 128), sizeof(void*), 3, READWRITE, TO_MEMORY); /* +1 is the bare minimum so why not a bit more */
  set_kernel_arg(video_enc.openclKernels[2], 4, sizeof(int), video_enc.image_width);
  set_kernel_arg(video_enc.openclKernels[2], 5, sizeof(int), video_enc.image_height);
  set_kernel_arg(video_enc.openclKernels[2], 6, sizeof(int), video_enc.type); /* 1 is YUV420, 4 is used as ARGB8 in the source code 3 is RGB8 */
  set_kernel_arg(video_enc.openclKernels[2], 7, sizeof(int), video_enc.quality); /* This is the quality */
  set_dimension_and_values(video_enc.openclKernels[2], 3, (video_enc.image_width+7)/8, (video_enc.image_height+7)/8, 3);

  /*
	call_function(video_enc.openclKernels[0], 0);
	clW_get_last_error();
	call_function(video_enc.openclKernels[1], 0);
	clW_get_last_error();

	call_function(video_enc.openclKernels[2], 0);
	clW_get_last_error();
  */


	printf("PORT = %d\n", port);

	char* sending_buffer = __aligned_malloc(1024 * 1024 * 20, 16);

	int sending_buffer_size = 1024 * 1024 * 20;

	size_t st, en;

	/*Is the main loop active?*/
	bool loop = 1;

	/*create the video encoder*/

	struct TCP_Socket* sock = TCP_SOCKET_create(port, 0, true, 1, 0);

	if (sock == NULL)
	{
		printf("Failed to create socket\n");
		exit(1);
	}

  allow_select(sock);
  
  while (loop)
  {
    //int rez = 69;//find_hot_socket_with_timeout(sock, 100);

	  /* check if there is data coming from the client */
	  //if (rez == 0)
	  //{
	  //	process_inputs(sock, inp);
	  //}

    int temp_scrW, temp_scrH;

    st = clock();	

	  capture_one_display(0, &video_enc.screen_bits, &video_enc.buf_size[0], &temp_scrW, &temp_scrH);

    if (temp_scrW != video_enc.screen_width || temp_scrH != video_enc.screen_height)
      printf("Screen changed from %dX%d to %dX%d\n", video_enc.screen_width, video_enc.screen_height, temp_scrW, temp_scrH);


    clW_get_last_error();

    printf("Calling kernel 1\n");
	  call_function(video_enc.openclKernels[0], 0);
    printf("Calling kernel 2\n");
	  call_function(video_enc.openclKernels[1], 0);
    printf("Calling kernel 3\n");
	  call_function(video_enc.openclKernels[2], 0);
	  clW_get_last_error();

	  /* Assembling the RLE encoded array. TODO: move this to the GPU soon enough. 
	  Reusing screen_bits since we don't care about the previous values, they have been used already */
	  long long int cptr = 0;
    //short *rle_arr, *jpeg_arr;
	  for (int i = 0; i < video_enc.no_blocks; ++i)
	  {
		  for (int j = 0; j < ((short*)video_enc.offsets)[i+1]; ++j)
		  {
			  ((short*)video_enc.screen_bits)[cptr++] = ((short*)video_enc.jpeg_arr)[i * 128 + j];
		  }
	  }
    printf("cptr is: %d\n", cptr);
	  memcpy(video_enc.jpeg_arr, video_enc.offsets, 4 * video_enc.no_blocks + 2);
	  ((int*)video_enc.offsets)[0] = 0;
	  for (int i = 0; i < video_enc.no_blocks; ++i)
	  {
      if (((short*)video_enc.jpeg_arr)[i+1] < 0)
        printf("Negative value at: %d, value: %d, previous values: %d %d\n", i+1, ((int*)video_enc.offsets)[i+1], ((int*)video_enc.offsets)[i], ((short*)video_enc.jpeg_arr)[i]);
		  ((int*)video_enc.offsets)[i+1] = ((int*)video_enc.offsets)[i] + ((short*)video_enc.jpeg_arr)[i+1];
	  }

	  if (sending_buffer_size < 20 + 2 * ((int*)video_enc.offsets)[video_enc.no_blocks])
	  {
		  sending_buffer = (char*)realloc(sending_buffer, 4096 * 8 + 
		  2 * ((int*)video_enc.offsets)[video_enc.no_blocks] + sizeof(int) * video_enc.no_blocks);
		  sending_buffer_size = 4096 * 8 + 2 * ((int*)video_enc.offsets)[video_enc.no_blocks];
  	}

	  int frame_size = 0;
	  /* Add the size of the raw data */
	  frame_size += 2 * ((int*)video_enc.offsets)[video_enc.no_blocks];
	  frame_size += video_enc.no_blocks * sizeof(int);

    if (frame_size > sending_buffer_size)
    {
      printf("Buffer too smol\n");
      sending_buffer = __aligned_realloc(sending_buffer, frame_size, frame_size + 4096 * 10, 1024);
      sending_buffer_size = frame_size + 4096 * 10;
    }

	  sending_buffer[0] = new_frame;
	  sending_buffer[1] = jpeg_no_huff;


	  *((int*)((char*)sending_buffer + 4)) = htonl(2 * ((int*)video_enc.offsets)[video_enc.no_blocks]);
	  *((int*)((char*)sending_buffer + 8)) = htonl(frame_size);
	  *((int*)((char*)sending_buffer + 12)) = htonl(sizeof(int) * video_enc.no_blocks);
	  *(uint16_t*)((char*)sending_buffer + 16) = htons(video_enc.image_width);
	  *(uint16_t*)((char*)sending_buffer + 18) = htons(video_enc.image_height);
	  memcpy(sending_buffer + 20, video_enc.screen_bits, 2 * ((int*)video_enc.offsets)[video_enc.no_blocks]);
	  memcpy(sending_buffer + 20 + 2 * ((int*)video_enc.offsets)[video_enc.no_blocks],
	  video_enc.offsets, sizeof(int) * video_enc.no_blocks);
	  printf("frame size is: %d, other numbers: %d %d\n", 
	  frame_size, 2 * ((int*)video_enc.offsets)[video_enc.no_blocks], sizeof(int) * video_enc.no_blocks);

	  int sent = TCP_Socket_send_data(sock, 0, sending_buffer, frame_size + 20);
	  if (sent == -1)
	  {
		  /* The client disconnected :(*/
		  printf("The client disconnected\n"); 
		  TCP_Socket_accept_new_connection(sock, 0);
		  printf("Accepted new connection\n"); 
	  }
	  else if (sent != 2 * ((int*)video_enc.offsets)[video_enc.no_blocks] + 20)
	  {
		  int sent2 = TCP_Socket_send_data(sock, 0, sending_buffer, frame_size - sent);
		  if (sent2 < 0)
		  {
			  printf("The client disconnected\n");
		  	TCP_Socket_accept_new_connection(sock, 0);
		  }
	  }

	  en = clock();

  	printf("Pipeline %ld \n", en-st);
	  /* calculate to 24fps*/
    //SDL_Delay(10);
  }

	return 0;
}
