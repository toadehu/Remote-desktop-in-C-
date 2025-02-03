/*
	CLIENT
*/

#include "framework.h"

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

const float zoom_ratio = 0.75;

typedef struct state_machine
{
	/* number of zooms, max 3 */
	int zoom;
	int zooms_x[3], zooms_y[3];

	/* 0 for mouse out, 1 for mouse in */
	int mouse_in;
}state_machine;

state_machine *state_m;

typedef unsigned char byte;

/* How often should the key presses buffer be send, in milliseconds */
const int keybufferinterval = 25; 
 
char* serverIp, * user;
int ipver, port;

int keypresses_size;
char* keypresses;

struct screen_data video_dec;

void real_rect(int srcW, int srcH, int destW, int destH, int *goodW, int *goodH);

/* This could be a funny macro though */
void add_pos_to_buffer(GRAPHICS_RENDERER* renderer, int screen_width, int screen_height)
{
	int relativeX, relativeY, windowW, windowH;
	SDL_GetMouseState(&relativeX, &relativeY);
	real_rect(renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h,
				 screen_width, screen_height, &windowW, &windowH);

	*((int*)((char*)keypresses + keypresses_size)) = htonl(relativeX);
	keypresses_size += 4;
	*((int*)((char*)keypresses + keypresses_size)) = htonl(relativeY);
	keypresses_size += 4;
	*((int*)((char*)keypresses + keypresses_size)) = htonl(windowW);
	keypresses_size += 4;	
	*((int*)((char*)keypresses + keypresses_size)) = htonl(windowH);
	keypresses_size += 4;
}

int old_w = 0, old_h = 0;

char* buffer = NULL, *screen_bits = NULL, *yuv_buffer = NULL, *resized_screen_bits = NULL;
char* zoom_screen_bits = NULL;

int resized_size = 0, buffer_size = 0, screen_width = 0, screen_height = 0;

basic_video_dec* basic_dec = NULL;

void convert_to_aspect_ratio(int Xcoord, int Ycoord, int srcW, int srcH, int destW, int destH, int *rightX, int *rightY)
{
	float aspect_ratio = (float)srcW / (float)srcH;
	float dest_aspect_ratio = (float)destW / (float)destH;
	float new_w, new_h, Wdiff, Hdiff;

	float epsilon = 0.0001;
	if (abs(aspect_ratio - dest_aspect_ratio) < epsilon)
	{
		*rightX = Xcoord;
		*rightY = Ycoord;
		return;
	}
	if (aspect_ratio < dest_aspect_ratio)
	{
		new_h = (float)(destH - (int)destH%18);
		Hdiff = (float)srcH - new_h;
		new_w = (float)srcW * dest_aspect_ratio;
		Wdiff = (float)srcW - new_w;
	}
	else
	{
		new_w = (float)(destW - (int)destW%16);
		Wdiff = (float)srcW - new_w;
		new_h = (float)srcH * dest_aspect_ratio;
		Hdiff = (float)srcH - new_h;
	}
	
	*rightX = (int)Xcoord + (int)(Wdiff/2);
	*rightY = (int)Ycoord + (int)(Hdiff/2);
}

/** @brief Gets and writes the real size of the rectangle that we are actuall drawing unto
 * 
 * @param srcW the source rectangle width
 * @param srcH the source rectangle height
 * @param destW the destination s rectangle width (server screen width)
 * @param destH the destination s rectangle height (server screen height)
 * @param goodW pointer to where the actual width will be written
 * @param goodH pointer to where the actuial width will be written
*/
void real_rect(int srcW, int srcH, int destW, int destH, int *goodW, int *goodH)
{
	float aspect_ratio = (float)srcW / (float)srcH;
	float dest_aspect_ratio = (float)destW / (float)destH;float epsilon = 0.0001;
	if (abs(aspect_ratio - dest_aspect_ratio) < epsilon)
	{
		*goodW = srcW;
		*goodH = srcH;
		return;
	}
	if (aspect_ratio < dest_aspect_ratio)
	{
		*goodH = (float)(destH - (int)destH%18);
		*goodW = (float)srcW * dest_aspect_ratio;
	}
	else
	{
		*goodW = (float)(destW - (int)destW%16);
		*goodH = (float)srcH * dest_aspect_ratio;
	}
	
}

bool is_point_inside_rect(int x, int y, int rectX, int rectY, int rectW, int rectH)
{
	if (x >= rectX && x <= rectX + rectW && y >= rectY && y <= rectY + rectH)
	{
		return true;
	}
	return false;
}

void init_decoder(uint16_t w, uint16_t h)
{
	basic_dec = basic_create_video_dec(w, h, 0, VIDEO_YUV420, RLE_TWO_PASS);
	if (basic_dec == NULL)
	{
		printf("ERROR\n");
		exit(0);
	}
	screen_bits = (char*)__aligned_malloc(w * h * 4, 1024);
	yuv_buffer = (char*)__aligned_malloc(w * h * 3 / 2, 1024);
	int res_w = 1920, res_h = 1200;
	old_w = w;
	old_h = h;
	resized_screen_bits = (char*)__aligned_malloc(res_w * res_h * 4, 1024);
	resized_size = res_w * res_h * 4;
}

inline void update_bufs(void ***bufs, int no_bufs, int *old_sizes, int *new_sizes)
{
  for (int i = 0; i < no_bufs; ++i)
  {
    char **cur_buf = (char**)bufs[i];
    /* No explicit alginment, 1024 is good, and should not lead to big fragmentation */
    *cur_buf = (char*)__aligned_realloc(*cur_buf, old_sizes[i], new_sizes[i], 1024);
  }
}

void receive_and_procees_data(TCP_SOCKET* sock, GRAPHICS_RENDERER* renderer)
{
	int rec = TCP_Socket_receive_data_fixed(sock, buffer, 20);

	if (rec != 20)
	{
		printf("Expected 20 bytes, received %d bytes for the header\n", rec);
		printf("Something bad happened, either the connection is compromised or the server has shut down... exiting\n");
		exit(1);
	}
	if (buffer[0] != new_frame)
	{
  /* More cases to handle will come later */ 
  }

  int old_buf_size[3] = {screen_width * screen_height * 4, screen_width * screen_height * 3 / 2, screen_width * screen_height * 4};
  void **bufs[3] = {&screen_bits, &yuv_buffer, &zoom_screen_bits};

	char encoding_type = buffer[1];
	/* Get the flags and information from the header */
	int first_size = ntohl(*(int*)((char*)buffer + 4));
	int encoded_image_size = ntohl(*(int*)((char*)buffer + 8));
	int second_pass_size = ntohl(*(int*)((char*)buffer + 12));
	uint16_t w = ntohs(*(uint16_t*)((char*)buffer + 16));
	uint16_t h = ntohs(*(uint16_t*)((char*)buffer + 18));		
  int new_buf_sizes[3] = {w * h * 4, w * h * 3 / 2, w * h * 4};

	
  if (screen_width != (int)w || screen_height != (int)h)
	{
    update_bufs(bufs, 3, old_buf_size, new_buf_sizes); 
		screen_width = (int)w;
		screen_height = (int)h;
	}

	printf("first size is: %d and second size is: %d\n", first_size, second_pass_size);
	rec = TCP_Socket_receive_data_fixed(sock, (char*)video_dec.converted_bits, first_size);
	rec += TCP_Socket_receive_data_fixed(sock, (char*)video_dec.offsets, second_pass_size);
			
	/* Yeah no realloc because I am lazy TODO: Fix this piece of shit*/
	if (screen_width != video_dec.image_width || screen_height != video_dec.image_height)
	{
		printf("UNIMPLEMENTED EXIT\n");
		exit(1);
	}
			
	long long int ret = call_function(video_dec.openclKernels[0], 0);
	if (ret == -1)
	{
		printf("Error calling the OpenCL kernel:\n");
		clW_get_last_error();
    printf("Exiting...\n");
		exit(2);
	}
  printf("Survived first kernel\n");
  call_function(video_dec.openclKernels[1], 0);
  clW_get_last_error();

  call_function(video_dec.openclKernels[2], 0);
  clW_get_last_error();

  printf("top pixel: %d %d %d %d\n", resized_screen_bits[0], resized_screen_bits[1], resized_screen_bits[2], (unsigned char)resized_screen_bits[3]);
  screen_bits = video_dec.converted_bits;
  return;

	/*convert the image to RGB32*/	
  if (renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4 > resized_size)
	{
		resized_screen_bits = (char*)__aligned_realloc(resized_screen_bits, resized_size, renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4, 1024);
		resized_size = renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4;
	}
	old_w = renderer->images[0][0]->rect.w;
	old_h = renderer->images[0][0]->rect.h;

	if (screen_width != renderer->images[0][0]->rect.w || screen_height != renderer->images[0][0]->rect.h)
	{
		if (renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4 > resized_size)
		{
			resized_screen_bits = (char*)__aligned_realloc(resized_screen_bits, resized_size, renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4, 1024);
			resized_size = renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4;
		}
		resize_image_bilinear((const unsigned char*)screen_bits, (unsigned char*)resized_screen_bits, screen_width, screen_height,
		renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h, 4);
	}
  else
	{
    memcpy(resized_screen_bits, screen_bits, screen_width * screen_height * 4);
	}
}

int main(int argc, char* argv[])
{
	state_m = (state_machine*)malloc(sizeof(state_machine));

	memset(state_m, 0, sizeof(state_machine));

  /* This whole video_dec block should and will eventually go into it's own header */

  video_dec.buf_size = (int*)malloc(sizeof(int) * 5);
	memset(video_dec.buf_size, 0, 5 * sizeof(int));
	/* Setting up default parametes. */
	video_dec.image_height = 1200;
	video_dec.image_width = 1920;
	video_dec.image_dim = video_dec.image_width * video_dec.image_width;

	/* Yes I want page aligned memory. I will not take chances with OpenCL crying about it with error code -1001 */
	video_dec.converted_bits = (unsigned char*)__aligned_malloc(video_dec.image_dim * 4, 4096);
	video_dec.dst = (unsigned char*)__aligned_malloc(video_dec.image_dim * 6, 4096);
	video_dec.jpeg_arr = (unsigned char*)__aligned_malloc(video_dec.image_dim * 8, 4096);
	video_dec.offsets = (unsigned char*)__aligned_malloc(video_dec.image_dim * 2, 4096);
	video_dec.screen_bits = (unsigned char*)__aligned_malloc(video_dec.image_dim * 5, 4096);

	/* More default parameters */
	video_dec.type = YUV420;
	video_dec.no_blocks = (((video_dec.image_width+7)/8) * ((video_dec.image_height+7)/8)) * 1.5;

	printf("No_blocks: %d\n", video_dec.no_blocks);

	video_dec.quality = DEFAULT_QUALITY;

  video_dec.kernels = 3;

  video_dec.openclKernels = (struct OpenCLFunctionWrapper**)malloc(sizeof(struct OpenCLFunctionWrapper*) * video_dec.kernels);

	video_dec.openclKernels[0] = createOpenCLWrapperStruct("../jpegKernel.cl", KERNEL_FROM_FILE, 
							"decode_jpeg", NULL, 0, 0, 0);

	set_no_buffers(video_dec.openclKernels[0], 6);
	create_and_set_buf(video_dec.openclKernels[0], video_dec.converted_bits, video_dec.image_dim * 4, sizeof(void*), 0, READWRITE, FROM_MEMORY);
  create_and_set_buf(video_dec.openclKernels[0], video_dec.offsets, sizeof(int) * video_dec.no_blocks, sizeof(void*), 1, READWRITE, FROM_MEMORY);
  set_kernel_arg(video_dec.openclKernels[0], 2, sizeof(int), video_dec.no_blocks);
	create_and_set_buf(video_dec.openclKernels[0], video_dec.jpeg_arr, video_dec.image_dim * 3 / 2, sizeof(void*), 3, READWRITE, 0);
  set_kernel_arg(video_dec.openclKernels[0], 4, sizeof(int), video_dec.image_width);
  set_kernel_arg(video_dec.openclKernels[0], 5, sizeof(int), video_dec.image_height);
  set_kernel_arg(video_dec.openclKernels[0], 6, sizeof(int), video_dec.type); /* 1 is YUV420, 4 is used as ARGB8 in the source code 3 is RGB8 */
  set_kernel_arg(video_dec.openclKernels[0], 7, sizeof(int), video_dec.quality); /* This is the quality */
  set_dimension_and_values(video_dec.openclKernels[0], 1, video_dec.no_blocks , 1, 1);

  video_dec.openclKernels[1] = createSharedContextOpenCLWrapper(video_dec.openclKernels[0], "../image_mainp.cl", KERNEL_FROM_FILE, "GPU_YUV420ToRGB", NULL, 0);
  set_no_buffers(video_dec.openclKernels[1], 2);
  copy_buf_from_other(video_dec.openclKernels[0], video_dec.openclKernels[1], video_dec.jpeg_arr, sizeof(void*), 0, 0);
  set_kernel_arg(video_dec.openclKernels[1], 1, sizeof(int), video_dec.image_width);
  set_kernel_arg(video_dec.openclKernels[1], 2, sizeof(int), video_dec.image_height);
  create_and_set_buf(video_dec.openclKernels[1], video_dec.converted_bits, video_dec.image_dim * 3,  sizeof(void*), 3, READWRITE, TO_MEMORY);
  set_kernel_arg(video_dec.openclKernels[1], 4, sizeof(int), 3);
  set_dimension_and_values(video_dec.openclKernels[1], 1, video_dec.image_dim, 1, 1);

  video_dec.openclKernels[2] = createSharedContextOpenCLWrapper_fromSameFile(video_dec.openclKernels[1], "GPU_resize"); 
  set_no_buffers(video_dec.openclKernels[2], 2);
  copy_buf_from_other(video_dec.openclKernels[1], video_dec.openclKernels[2], video_dec.converted_bits, sizeof(void*), 0, 0);
  set_kernel_arg(video_dec.openclKernels[2], 1, sizeof(int), video_dec.image_width);
  set_kernel_arg(video_dec.openclKernels[2], 2, sizeof(int), video_dec.image_height);
  set_kernel_arg(video_dec.openclKernels[2], 3, sizeof(int), 3);
  create_and_set_buf(video_dec.openclKernels[2], video_dec.screen_bits, 1600 * 1000 * 4, sizeof(void*), 4, READWRITE, TO_MEMORY);
  set_kernel_arg(video_dec.openclKernels[2], 5, sizeof(int), 1600);
  set_kernel_arg(video_dec.openclKernels[2], 6, sizeof(int), 1000);
  set_kernel_arg(video_dec.openclKernels[2], 7, sizeof(int), 3);
  set_kernel_arg(video_dec.openclKernels[2], 8, sizeof(int), 3);
  set_kernel_arg(video_dec.openclKernels[2], 9, sizeof(int), 3);
  set_dimension_and_values(video_dec.openclKernels[2], 2, 1600, 1000, 1);  

	resized_screen_bits = video_dec.screen_bits; //(char*)__aligned_malloc(video_dec.image_dim * 4, 1024);
  resized_size = 1600 * 1000 * 4;

	serverIp = (char*)malloc(20);
	user = (char*)malloc(128);

	int i;
	size_t serverIpLen;
	char* arobas;
	for (i = 1; i < argc; i++)
	{
		switch (argv[i][1])
		{
		case 'h':
			printf("Usage: -ip4/6 IPADDR -p PORT\n");
			exit(0);
		case 'p':
			if (strlen(argv[i]) != 2)
			{
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR -p PORT\n");
				exit(1);
			}
			port = atoi(argv[i + 1]);
			i++;
			break;
		case 'i':
			if (argv[i][2] != 'p')
			{
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR -p PORT\n");
				exit(1);
			}
			if (argv[i][3] == '4')
			{
				ipver = 4;
			}
			else if (argv[i][3] == '6')
			{
				ipver = 6;
			}
			else
			{
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR -p PORT\n");
				exit(1);
			}
			serverIpLen = strlen(argv[i + 1]);
			serverIp = (char*)malloc((serverIpLen + 1) * sizeof(char));
			strncpy(serverIp, argv[i + 1], serverIpLen);
			serverIp[serverIpLen] = '\0';
			i++;
			break;
		default:
			printf("Invalid argument\nCorrect usage: -ip4/6 IPADDR -p PORT\n");
			exit(1);

		}
	}

	printf("IP Address: %s\n", serverIp);
	printf("Port: %d\n", port);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	GRAPHICS_RENDERER* renderer = create_graphics_renderer(1600, 1000, (char*)"Test\0", NULL, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | BACKGROUND_FROM_DATA, SDL_RENDERER_ACCELERATED);
	SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);

	old_w = renderer->images[0][0]->rect.w;
	old_h = renderer->images[0][0]->rect.h;

	renderer->ms = 5;

	buffer = (char*)__aligned_malloc(720 * 1280, 1024);
	buffer_size = 720 * 1280;

	/*Is the main loop active?*/
	bool loop = 1;

	struct TCP_Socket* sock = TCP_SOCKET_create(port > 0 ? port : 2222, INADDR_LOOPBACK, false, false, CLIENT | BIG_BUFFER);


	int connection_ret = -1;
	while (connection_ret == -1 || connection_ret == 9999999)
	{
		printf("Connecting to the server...\n");
		connection_ret = TCP_SOCKET_connect_from_string(sock, serverIp, port, 0);

		printf("connect returned: %d\n", connection_ret);

		if (connection_ret == -1 || connection_ret == 9999999)
		{
			printf("Connecting to the server failed, retry, change remote server or quit? (r,c,q)\n");
			char inp;
			PROCESS_INP:
			(void)scanf("%c", &inp);
			if (inp == 'r')
			{
				continue;
			}
			else if (inp == 'c')
			{
				printf("Please input the new remote host using the format IPADDR@user PORT\n");
				/*scanf();*/
			}
			else if (inp == 'q')
			{
				exit(0);
			}
			else
			{
				printf("Invalid input, try again\n");
				/* (void)scanf("%c", &inp); */
				goto PROCESS_INP;
			}
		}
	}

	/* There should never be more than 1000bytes of key presses in a given interval */
	keypresses = (char*)malloc(1000);
	keypresses_size = 0;

	int mouseX, mouseY;
	SDL_KeyboardEvent key;
	SDL_Keycode keycode;
	Uint16 modifiers;

	clock_t buffer_start, buffer_current;
	buffer_start = clock();

	/* Is the left mouse button being held? */
	int left_hold = 0, sent_release = 0;

	/*Main loop*/
	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				SDL_DestroyWindow(renderer->window);
				SDL_Quit();
				loop = 0;
				break;
			case SDL_KEYDOWN:
				key = event.key;

				/* Get the key code */
				keycode = key.keysym.sym;

				/* Check for any modifiers */
				modifiers = key.keysym.mod;

				/* To preserve alignment I waste 3 bytes, but I will find a use for them later*/
				keypresses[keypresses_size] = (char)keybd_input;
				keypresses_size += 4;

				/* I want to preserve the alignment to 4 bytes per field. This makes everything nice and easy*/
				*((int*)((char*)keypresses + keypresses_size)) = htonl(keycode);
				keypresses_size += 4;
				*((int*)((char*)keypresses + keypresses_size)) = htonl(modifiers);
				keypresses_size += 4;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouseX, &mouseY);
				
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (event.button.clicks > 1)
					{
						keypresses[keypresses_size] = (char)mouse_input_click;
						keypresses_size += 1;
						keypresses[keypresses_size] = (char)mouse_input_click;
						keypresses_size += 3;
					}
					left_hold = 1;
					break;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT && event.button.clicks)
				{
					keypresses[keypresses_size] = (char)mouse_input_click;
					keypresses_size += 1;
					keypresses[keypresses_size] = (char)CLICK_RIGHT;
					keypresses_size += 3;
				}
				else if (event.button.button == SDL_BUTTON_MIDDLE && event.button.clicks)
				{
					keypresses[keypresses_size] = (char)mouse_input_click;
					keypresses_size += 1;
					keypresses[keypresses_size] = (char)CLICK_MIDDLE;
					keypresses_size += 3;
				}
				else if (event.button.button == SDL_BUTTON_X1 && event.button.clicks)
				{
					keypresses[keypresses_size] = (char)mouse_input_click;
					keypresses_size += 1;
					keypresses[keypresses_size] = (char)CLICK_OEM1;
					keypresses_size += 3;
				}
				else if (event.button.button == SDL_BUTTON_X2 && event.button.clicks)
				{
					keypresses[keypresses_size] = (char)mouse_input_click;
					keypresses_size += 1;
					keypresses[keypresses_size] = (char)CLICK_OEM2;
					keypresses_size += 3;
				}
				/* No click just a hold */
				else 
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						keypresses[keypresses_size] = mouse_input_hold;
						keypresses_size += 4;
					}

				add_pos_to_buffer(renderer, screen_width, screen_height);
				
				break;
			case SDL_MOUSEWHEEL:
				keypresses[keypresses_size] = (char)mouse_input_scroll;
				keypresses_size += 4;
				*((int*)((char*)keypresses + keypresses_size)) = htonl(event.wheel.y);
				keypresses_size += 4;
				*((int*)((char*)keypresses + keypresses_size)) = htonl(event.wheel.x);
				keypresses_size += 4;

				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					left_hold = 0;
				}
				break;
			}
		}
		
		buffer_current = clock();
		if (1000 * (buffer_current - buffer_start) > CLOCKS_PER_SEC * keybufferinterval)
		{
			buffer_start = buffer_current;
			if (left_hold == 1)
			{
				keypresses[keypresses_size] = mouse_input_hold;
				keypresses_size += 4;
				add_pos_to_buffer(renderer, screen_width, screen_height);
				sent_release = 0;
			}
			else if(left_hold == 0)
			{
				if (sent_release == 0)
				{
					keypresses[keypresses_size] = mouse_input_release;
					keypresses_size += 4;
					add_pos_to_buffer(renderer, screen_width, screen_height);
					sent_release = 1;
				}
			}
			/* We send the input buffer */
			if (keypresses_size)
			{
				/* Send the keypresses */
				int sent = TCP_Socket_send_data(sock, -1, keypresses, keypresses_size);
				if (sent == -1)
				{
					printf("Server has shut down, exiting\n");
					exit(1);
				}
				if (sent != keypresses_size)
				{
					printf("Expected %d bytes, sent %d bytes for the keypresses\n", keypresses_size, sent);
					exit(1);
				}
				keypresses_size = 0;
			}
		}

		clock_t start = clock();
		
		if (resized_size >= renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4)
		{
      printf("Updating bg size\n");
			renderer_update_bg(renderer, resized_screen_bits, IMAGE_FROM_RGB32);
		}

		receive_and_procees_data(sock, renderer);

		SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);
		/*adjust the size of the texture*/
		renderer_update_rects(renderer);
		
		/*clear the screen*/
		SDL_RenderClear(renderer->renderer);
			
		/*render the images*/
		renderer_draw_images(renderer);


		/* update the screen*/
		SDL_RenderPresent(renderer->renderer);
		clock_t end = clock();

		printf("Time: %ld, Clocks per second: %ld\n", (end - start), CLOCKS_PER_SEC);
		SDL_Delay(10);
	}
	return 0;
}
