/*
	SERVER
*/

#include "framework.h"

#include <setjmp.h>

/* jmp_buf */ 

#define HEADER_SIZE 20

int port;

void update_versace_size(SDL_Rect* rect, SDL_Rect win_rect, void *extra)
{
	rect -> w = win_rect.w * 0.35;
	rect -> h = win_rect.h * 0.35;
	rect -> x = (win_rect.w >> 6);
	rect -> y = (win_rect.h >> 1) + (win_rect.h >> 3);
}

void versace_button_on_click(){/*To be completed*/}

struct screen_data
{
	int screen_width, screen_height;
	int image_width, image_height, image_dim, type, quality, no_blocks;
	int buf_size[5];
	unsigned char *screen_bits, *converted_bits, *dst, *jpeg_arr, *offsets;
	struct OpenCLFunctionWrapper *openclwrap;
};

/**
 * TODO: Move all of this to a nice little header
 */
void process_inputs(TCP_SOCKET* sock, inputs* inp)
{
	/* More than 1 input/ms is impossible big polling ratez are 1000hz*/
	char buffer[400];
	int rez = TCP_Socket_receive_data_once(sock, 0, buffer, 400);
	int i;
	uint32_t key, mods;
	int32_t x,y,relx,rely;
	char click_type;
	for (i = 0; i < rez;)
	{
		switch (buffer[i])
		{
		case mouse_input_move:
			i += 4;
			x = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			y = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			relx = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			rely = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			uint32_t realX, realY;
			realX = x * (float)((float)inp->screen_w / (float)relx);
			realY = y * (float)((float)inp->screen_h / (float)rely);
			printf("realX: %d, realY: %d\n", realX, realY);
			/*set_mouse_pos(inp, realX, realY);*/
			break;
		case mouse_input_click:
			click_type = buffer[i + 1];
			i += 4;
			x = ntohl(*(uint32_t*)((char*)buffer + i));
			if (x < 0)
			{
				x *= -1;
			}
			i += 4;
			y = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			relx = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			rely = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			realX = inp->screen_w * (float)((float)x / (float)relx);
			realY = inp->screen_h * (float)((float)y / (float)rely);
			printf("X:%d, Y:%d, winX:%d, winY:%d\n", x, y, relx, rely);
			printf("realX: %d, realY: %d\n", realX, realY);
			set_mouse_pos(inp, realX, realY);
			if (click_type == (char)CLICK_LEFT)
			{
				printf("Left click\n");
				send_Lclick(inp);
			}
			else if (click_type == (char)CLICK_RIGHT)
			{
				send_Rclick(inp);
			}
			else if (click_type == (char)CLICK_MIDDLE)
			{
				send_Mclick(inp);
			}
			/* This is a double click */
			else if (click_type == (char)mouse_input_click)
			{
				send_Lclick(inp);
				send_Lclick(inp);
			}
			/* No support for OEM1 and OEM2*/
			break;
		case mouse_input_scroll:
			send_mouse_scroll(inp, ntohl(*(uint32_t*)((char*)buffer + i)));
			i += 12;
			break;
		case keybd_input:
			i += 4;
			key = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			mods = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			send_key(inp, key, mods, SDL_INPUT);
			break;
		case mouse_input_hold:
			printf("HOlD\n");
			i += 4;
			x = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			y = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			relx = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			rely = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			realX = inp->screen_w * (float)((float)x / (float)relx);
			realY = inp->screen_h * (float)((float)y / (float)rely);
			set_mouse_pos(inp, realX, realY);
			send_Lhold(inp);
			break;
		case mouse_input_release:
			i += 4;
			x = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			y = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			relx = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			rely = ntohl(*(uint32_t*)((char*)buffer + i));
			i += 4;
			realX = inp->screen_w * (float)((float)x / (float)relx);
			realY = inp->screen_h * (float)((float)y / (float)rely);
			set_mouse_pos(inp, realX, realY);
			send_Lrelease(inp);
			break;
		default:
			i+=12;
			printf("Unknown input\n");
			break;
		}
		printf("i:%d\n", i);
	}
}

int main (int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

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


	struct screen_data video_enc;

	video_enc.screen_bits = NULL;
	video_enc.converted_bits = NULL;
	memset(video_enc.buf_size, 0, 5 * sizeof(int));
	/* Dry run to get the parameners at startup*/
	capture_one_display(0, &video_enc.screen_bits, &video_enc.buf_size[0], 
	&video_enc.screen_width, &video_enc.screen_height);

	video_enc.image_height = 1080;
	video_enc.image_width = 1920;
	video_enc.image_dim = video_enc.image_width * video_enc.image_width;

	/* Yes I want page aligned memory. I will not take chances with OpenCL crying about it with error code -130184012047 */
	video_enc.converted_bits = __aligned_malloc(video_enc.image_dim * 4, 4096);
	video_enc.dst = __aligned_malloc(video_enc.image_dim * 6, 4096);
	video_enc.jpeg_arr = __aligned_malloc(video_enc.image_dim * 8, 4096);
	video_enc.offsets = __aligned_malloc(video_enc.image_dim, 4096);

	video_enc.type = YUV420;
	if (video_enc.type == YUV420)
	{
		video_enc.no_blocks = (((video_enc.image_width+7)/8) * ((video_enc.image_height+7)/8)) * 1.5;
	}
	video_enc.quality = 75;

	video_enc.openclwrap = createOpenCLWrapperStruct("../jpegKernel.cl", KERNEL_FROM_FILE, 
							"do_jpeg", NULL, 0, 0, ENABLE_PROFILING);

	set_no_buffers(video_enc.openclwrap, 5);
    create_and_set_buf(video_enc.openclwrap, video_enc.converted_bits, video_enc.image_dim * 2, sizeof(void*), 0, READONLY, FROM_MEMORY);
    create_and_set_buf(video_enc.openclwrap, video_enc.dst, video_enc.image_dim * 1, sizeof(void*), 1, WRITEONLY, TO_MEMORY);
    create_and_set_buf(video_enc.openclwrap, video_enc.jpeg_arr, video_enc.image_dim * 6, sizeof(void*), 2, READWRITE, TO_MEMORY);
    create_and_set_buf(video_enc.openclwrap, video_enc.offsets, video_enc.image_dim / 8, sizeof(void*), 3, READWRITE, TO_MEMORY);
    set_kernel_arg(video_enc.openclwrap, 4, sizeof(int), video_enc.image_width);
    set_kernel_arg(video_enc.openclwrap, 5, sizeof(int), video_enc.image_height);
    set_kernel_arg(video_enc.openclwrap, 6, sizeof(int), video_enc.type); /* 1 is YUV420, 4 is used as ARGB8 in the source code 3 is RGB8 */
    set_kernel_arg(video_enc.openclwrap, 7, sizeof(int), video_enc.quality); /* This is the quality */
    set_dimension_and_values(video_enc.openclwrap, 3, (video_enc.image_width+7)/8, (video_enc.image_height+7)/8, 3);

	inputs* inp;
	inp = create_inputs_struct(FULL_LAYOUT | MOUSE);

	printf("PORT = %d\n", port);

	GRAPHICS_RENDERER *renderer = create_graphics_renderer(800, 600, (char*)"Test", (char*)"img.jpg", SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);
	SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);

	/*GRAPHICS_RENDERER *la_lautari = create_graphics_renderer(800, 600, "Lautari", SDL_WINDOW_SHOWN);*/

	button_element *versace_button = create_new_button_element(renderer -> renderer, (char*)"button.png", (char*)"flopa.png", renderer -> win_rect.w / 8, renderer -> win_rect.h / 8,
	  renderer -> win_rect.w / 8, renderer -> win_rect.h / 8, update_versace_size);

	versace_button -> on_click = versace_button_on_click;

	renderer_add_button(renderer, versace_button);

	renderer_update_rects(renderer);

	char* sending_buffer = __aligned_malloc(1024 * 1024 * 20, 1024);

	int sending_buffer_size = 1024 * 1024 * 20;

	size_t st, en;

	/*Is the main loop active?*/
	bool loop = 1;

	/*create the video encoder*/

	struct TCP_Socket* sock = TCP_SOCKET_create(port, 0, true, 1, 0);

	allow_select(sock);

	if (sock == NULL)
	{
		printf("Failed to create socket\n");
		//exit(1);
	}

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					SDL_DestroyWindow(renderer -> window);
					SDL_Quit();
					loop = 0;
					break;
				case SDL_WINDOWEVENT:
			       	if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);
						/*adjust the size of the texture*/
						renderer_update_rects(renderer);
                        /*update_rectangle_size(&renderer -> images[0][0] -> rect, renderer -> win_rect.w, renderer -> win_rect.h);*/
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					renderer_handle_click(renderer, event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						renderer_handle_click_released(renderer, event.button.x, event.button.y);
					}
					break;
			}
		}

		int rez = 69;//find_hot_socket_with_timeout(sock, 100);

		/* check if there is data coming from the client */
		if (rez == 0)
		{
			process_inputs(sock, inp);
		}

		/*clear the screen*/
		SDL_RenderClear(renderer -> renderer);
		
		/*render the images*/
		renderer_draw_images(renderer);

		/*SDL_RenderCopy(renderer -> renderer, renderer -> images[0][0] -> texture, NULL, &renderer -> images[0][0] -> rect);*/

		/* update the screen*/
		SDL_RenderPresent(renderer -> renderer);	

		capture_one_display(0, &video_enc.screen_bits, &video_enc.buf_size[0], &video_enc.screen_width, &video_enc.screen_height);

		st = clock();	

		/* TODO: fix this shit and move all this stuff to a nice kernel because wtf is this */
		resize_image_bilinear(video_enc.screen_bits, video_enc.dst, 
		video_enc.screen_width, video_enc.screen_height, video_enc.image_width, video_enc.image_height, 4);

		ARGBToYUV420(&video_enc.dst, video_enc.image_width, video_enc.image_height, &video_enc.converted_bits);

		int64_t ret = call_function(video_enc.openclwrap, 0 | HIGH_PERF_CLOCK);

		/* Assembling the RLE encoded array. TODO: move this to the GPU soon enough. 
		Reusing screen_bits since we don't care about the previous values, they have been used already */
		int cptr = 0;
		for (int i = 0; i < video_enc.no_blocks; ++i)
		{
			for (int j = 0; j < ((short*)video_enc.offsets)[i+1]; ++j)
			{
				((short*)video_enc.screen_bits)[cptr++] = ((short*)video_enc.jpeg_arr)[i * 128 + j];
			}
		}
		memcpy(video_enc.jpeg_arr, video_enc.offsets, 2 * video_enc.no_blocks + 2);
		((int*)video_enc.offsets)[0] = 0;
		for (int i = 0; i < video_enc.no_blocks; ++i)
		{
			((int*)video_enc.offsets)[i+1] = ((int*)video_enc.offsets)[i] + ((short*)video_enc.jpeg_arr)[i+1];
		}

		if (sending_buffer_size < 20 + 2 * ((int*)video_enc.offsets)[video_enc.no_blocks])
		{
			sending_buffer = (char*)realloc(sending_buffer, 4096 * 8 + 
			2 * ((int*)video_enc.offsets)[video_enc.no_blocks] + sizeof(int) * video_enc.no_blocks);
			sending_buffer_size = 4096 * 8 + 2 * ((int*)video_enc.offsets)[video_enc.no_blocks];
		}

		sending_buffer[0] = new_frame;
		sending_buffer[1] = jpeg_no_huff;
		int frame_size = 0;
		/* Add the size of the raw data */
		frame_size += 2 * ((int*)video_enc.offsets)[video_enc.no_blocks];
		frame_size += video_enc.no_blocks * sizeof(int);
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

		printf("Pipeline %ld and kernel time: %ld\n", en-st, ret);
		/* calculate to 24fps*/
		SDL_Delay(200);
	}

	return 0;
}
