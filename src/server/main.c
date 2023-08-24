/*
	SERVER
*/

#include "framework.h"

#include <setjmp.h>

/* jmp_buf */ 

#define HEADER_SIZE 20

int port;

void update_versace_size(SDL_Rect* rect, SDL_Rect win_rect)
{
	rect -> w = win_rect.w * 0.35;
	rect -> h = win_rect.h * 0.35;
	rect -> x = (win_rect.w >> 6);
	rect -> y = (win_rect.h >> 1) + (win_rect.h >> 3);
}

void versace_button_on_click()
{
	/*To be completed*/
}

void process_inputs(TCP_SOCKET* sock, inputs* inp)
{
	/* More than 1 input/ms is impossible big polling ratez are 1000hz*/
	char buffer[400];
	int rez = TCP_Socket_receive_data_once(sock, 0, buffer, 400);
	int i;
	uint32_t key, mods;
	int32_t x,y,relx,rely;
	uint32_t realX, realY;
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

	char* screen_bits = NULL;
	char* yuv_buffer = NULL;
	char* sending_buffer = NULL;

	int buffer_size = 0, screen_width, screen_height, sending_buffer_size = 0;

	/*Is the main loop active?*/
	bool loop = 1;

	/*create the video encoder*/
	basic_video_enc* basic_enc = NULL;

	struct TCP_Socket* sock = TCP_SOCKET_create(port, 0, true, 1, 0);

	allow_select(sock);

RECONNECT_CLIENT:
	if (sock == NULL)
	{
		printf("Failed to create socket\n");
		exit(1);
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

		int rez = find_hot_socket_with_timeout(sock, 100);
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

		capture_screen(&screen_bits, &buffer_size, &screen_width, &screen_height);

		if (yuv_buffer == NULL)
		{
			yuv_buffer = (char *)__aligned_malloc(((screen_width * screen_height * 3) / 2), 4096);
			sending_buffer = (char *)__aligned_malloc(((screen_width * screen_height * 3) / 2) + 20, 4096); /* This will be large enough for anything */
			sending_buffer_size = ((screen_width * screen_height * 3) / 2) + 20;
			basic_enc = basic_create_video_enc(screen_width, screen_height, 0, VIDEO_YUV420, RLE_TWO_PASS);

			if (basic_enc == NULL)
			{
				printf("Failed to create encoder\n");
				exit(1);
			}
		}

#ifdef _DEBUG
		printf("Screen size: %d, %d\n", screen_width, screen_height);
#endif

		ARGBToYUV420(&screen_bits, screen_width, screen_height, &yuv_buffer);

#ifdef _DEBUG
		printf("YUV conversion done \n");
#endif

		if (basic_enc == NULL)
		{
			printf("Failed to create encoder\n");
			exit(1);
		}

		int encoding_flags, sz = 0;

#ifdef _DEBUG
		clock_t start = clock();

		sz = basic_encode_next_frame(basic_enc, yuv_buffer, &encoding_flags);

		clock_t end = clock();

		printf("Time for encode %d, size of image: %d\n", (end - start) * 1000 / CLOCKS_PER_SEC, sz);

#else
		sz = basic_encode_next_frame(basic_enc, yuv_buffer, &encoding_flags);
#endif
		if (sending_buffer_size < sz + 20)
		{
			free(sending_buffer);
			sending_buffer = (char *)__aligned_malloc(sz + 20000, 4096);
			sending_buffer_size = sz + 20000;
		}
		int encode_rez = basic_copy_frame(basic_enc, sending_buffer, sending_buffer_size, 20, encoding_flags);
		if (encode_rez)
		{
			sending_buffer = (char *)__aligned_realloc(sending_buffer, sending_buffer_size, sz + 512 * 1024, 1024);
		}
		sending_buffer[0] = new_frame;
		*((int*)((char*)sending_buffer + 4)) = htonl(encoding_flags);
		*((int*)((char*)sending_buffer + 8)) = htonl(sz);
		*((int*)((char*)sending_buffer + 12)) = htonl(basic_enc -> pass2_size);
		*(uint16_t*)((char*)sending_buffer + 16) = htons(screen_width);
		*(uint16_t*)((char*)sending_buffer + 18) = htons(screen_height);

		int sent = TCP_Socket_send_data(sock, 0, sending_buffer, sz + 20);
		if (sent == -1)
		{
			/* The client disconnected :(*/
			/* printf("The client disconnected\n"); */
			TCP_Socket_accept_new_connection(sock, 0);
			/* printf("Accepted new connection\n"); */
		}
		else if (sent != sz + 20)
		{
			int sent2 = TCP_Socket_send_data(sock, 0, sending_buffer, sz + 20 - sent);
			if (sent2 < 0)
			{
				printf("The client disconnected\n");
				TCP_Socket_accept_new_connection(sock, 0);
			}
			sent2 == sz + 20 - sent ? printf("Data sent\n") : printf("The connection is compromised, expected to send%d, actually sent: %d\n", sz + 20, sent2); exit(1);
		}

		/* calculate to 24fps*/
		SDL_Delay(1);
	}

	return 0;
}
