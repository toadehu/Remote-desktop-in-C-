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
	int res_w = 1920, res_h = 1080;
	old_w = w;
	old_h = h;
	resized_screen_bits = (char*)__aligned_malloc(res_w * res_h * 4, 1024);
	resized_size = res_w * res_h * 4;
}

void receive_and_procees_data(TCP_SOCKET* sock, GRAPHICS_RENDERER* renderer)
{
	int rec = TCP_Socket_receive_data_fixed(sock, buffer, 20);
	if (rec == -1)
	{
		printf("Server has shut down, exiting\n");
		exit(1);
	}

	if (rec != 20)
	{
		printf("Expected 20 bytes, received %d bytes for the header\n", rec);
		printf("Something catastrophic happened, the connection is probably compromised or the server has shut down... exiting\n");
		exit(1);
	}
	if (buffer[0] == new_frame)
	{

		/* Get the flags and information from the header */
		int encoding_flags = ntohl(*(int*)((char*)buffer + 4));
		int encoded_image_size = ntohl(*(int*)((char*)buffer + 8));
		printf("encoded_image_size: %d\n", encoded_image_size);
		int second_pass_size = ntohl(*(int*)((char*)buffer + 12));
		uint16_t w = ntohs(*(uint16_t*)((char*)buffer + 16));
		uint16_t h = ntohs(*(uint16_t*)((char*)buffer + 18));

		/* Create the initial decoder and other buffers */
		if (basic_dec == NULL)
		{
			init_decoder(w, h);
			int res_w = 1920, res_h = 1080;
			renderer->win_rect.w > w ? res_w = renderer->win_rect.w + 300 : w + 300;
			renderer->win_rect.h > h ? res_h = renderer->win_rect.h + 300 : h + 300; /* This is to have some room so the reallocation doesn't happen */
			resized_screen_bits = (char*)__aligned_malloc(res_w * res_h * 4, 1024);
			resized_size = res_w * res_h * 4;
		}

		if (screen_width != (int)w)
		{
			if (screen_width < (int)w || screen_height < (int)h)
			{
				screen_bits = (char*)__aligned_realloc(screen_bits, screen_width * screen_height * 4, (int)w * (int)h * 4, 1024);
				yuv_buffer = (char*)__aligned_realloc(yuv_buffer, screen_width * screen_height * 3 / 2, (int)w * (int)h * 3 / 2, 1024);
				zoom_screen_bits = (char*)__aligned_realloc(zoom_screen_bits, screen_width * screen_height * 4, (int)w * (int)h * 4, 1024);
			}
			screen_width = (int)w;
			screen_height = (int)h;
		}

		if (encoded_image_size > buffer_size)
		{
			buffer = (char*)__aligned_realloc(buffer, buffer_size, buffer_size * 2 > encoded_image_size ? buffer_size * 2 : encoded_image_size + buffer_size, 4096);
			buffer_size = buffer_size * 2 > encoded_image_size ? buffer_size * 2 : encoded_image_size + buffer_size;/*This is to limit the number of calls to realloc, becuase it will waste at most ~4MB of ram, which is acceptable*/
		}

		rec = TCP_Socket_receive_data_fixed(sock, buffer, encoded_image_size);

		VALIDATE(rec == encoded_image_size)

		basic_decode_next_frame(basic_dec, buffer, encoded_image_size, second_pass_size, encoding_flags);

		/* Because I always update the buffer incase it is too small, I shouldn't run into issues with the buffer being too small */
		basic_copy_frame_d(basic_dec, yuv_buffer, screen_width * screen_height * 3 / 2, 0, encoding_flags);
			
		clock_t st = clock();
		/*convert the image to RGB32*/
		YUV420ToARGB(yuv_buffer, screen_width, screen_height, screen_bits);

		if (renderer->images[0][0]->rect.w != old_w || renderer->images[0][0]->rect.h != old_h)
		{
			if (renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4 > resized_size)
			{
				resized_screen_bits = (char*)__aligned_realloc(resized_screen_bits, resized_size, renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4, 1024);
				resized_size = renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4;
			}
			old_w = renderer->images[0][0]->rect.w;
			old_h = renderer->images[0][0]->rect.h;
		}

		if (screen_width != renderer->images[0][0]->rect.w || screen_height != renderer->images[0][0]->rect.h)
		{
			if (renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4 > resized_size)
			{
				resized_screen_bits = (char*)__aligned_realloc(resized_screen_bits, resized_size, renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4, 1024);
				resized_size = renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4;
			}
			
			else
			{
				resize_image_bilinear((const unsigned char*)screen_bits, (unsigned char*)resized_screen_bits, screen_width, screen_height,
				renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h, 4);
			}
			clock_t en = clock();
			printf("Second resize: %ld\n", en - st);

			/* If the image is smaller than 1/3 of the source image we apply the sharpening kernel */
			if (renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 3 < screen_width * screen_height)
			{
				apply_sharpening((byte*)resized_screen_bits, renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h, 4);
			}
		}
		else
			memcpy(resized_screen_bits, screen_bits, screen_width * screen_height * 4);
	}

}

int main(int argc, char* argv[])
{
	state_m = (state_machine*)malloc(sizeof(state_machine));

	memset(state_m, 0, sizeof(state_machine));

	serverIp = (char*)malloc(20);
	user = (char*)malloc(128);

	/* bool have_AVX = checkAVX2Support(); */

	int i;
	size_t serverIpLen;
	char* arobas;
	for (i = 1; i < argc; i++)
	{
		switch (argv[i][1])
		{
		case 'h':
			printf("Usage: -ip4/6 IPADDR@user -p PORT\n");
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
		case 'i':
			if (argv[i][2] != 'p')
			{
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR@user -p PORT\n");
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
				printf("Invalid argument\n Correct usage: -ip4/6 IPADDR@user -p PORT\n");
				exit(1);
			}
			arobas = strchr(argv[i + 1], '@');
			if (arobas == NULL)
			{
				printf("Invalid argument\nCorrect usage: -ip4/6 IPADDR@user -p PORT\n");
				exit(1);
			}
			serverIpLen = arobas - argv[i + 1];
			serverIp = (char*)malloc((serverIpLen + 1) * sizeof(char));
			strncpy(serverIp, argv[i + 1], serverIpLen);
			serverIp[serverIpLen] = '\0';
			strcpy(user, arobas + 1);
			i++;
			break;
		default:
			printf("Invalid argument\nCorrect usage: -ip4/6 IPADDR@user -p PORT\n");
			exit(1);

		}
	}

	printf("IP Address: %s\n", serverIp);
	printf("User: %s\n", user);
	printf("Port: %d\n", port);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	GRAPHICS_RENDERER* renderer = create_graphics_renderer(1280, 720, (char*)"Test\0", NULL, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | BACKGROUND_FROM_DATA, SDL_RENDERER_ACCELERATED);
	SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);

	old_w = renderer->images[0][0]->rect.w;
	old_h = renderer->images[0][0]->rect.h;

	renderer->ms = 5;

	buffer = (char*)__aligned_malloc(720 * 1280, 1024);
	buffer_size = 720 * 1280;

	/*Is the main loop active?*/
	bool loop = 1;

	struct TCP_Socket* sock = TCP_SOCKET_create(port > 0 ? port : 4002, INADDR_LOOPBACK, false, false, CLIENT | BIG_BUFFER);


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
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					/*SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);*/
					/*adjust the size of the texture*/
					/*renderer_update_rects(renderer);*/
					/*update_rectangle_size(&renderer -> images[0][0] -> rect, renderer -> win_rect.w, renderer -> win_rect.h);*/
				}
				if (event.window.event == SDL_WINDOWEVENT_ENTER)
				{
					state_m->mouse_in = 1;
				}
				else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
				{
					state_m->mouse_in = 0;
				}
				break;
			case SDL_KEYDOWN:
				if ( ((event.key.keysym.sym == SDLK_Z) && ((event.key.keysym.mod & (KMOD_CTRL)) != 0) && ((event.key.keysym.mod & (KMOD_ALT)) != 0)))
				{
					/* Mouse has left the window no zooming to be done */
					if (state_m->mouse_in == 0)
					{
						break;
					}
					
					if (state_m->zoom < 3)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						state_m->zoom++;
						int convX, convY;
						convX = (int)((float)mouseX / ((float)(renderer->images[0][0]->rect.w) * (float)screen_width));
						convY = (int)((float)mouseY / ((float)(renderer->images[0][0]->rect.h) * (float)screen_height));
						get_zoom_coords(screen_width, screen_height, convX, convY,
						 zoom_ratio, &state_m->zooms_x[state_m->zoom-1], &state_m->zooms_y[state_m->zoom-1]);
					}
					break;
				}

				else if (event.key.keysym.sym == SDLK_Q && event.key.keysym.mod & (KMOD_CTRL) && event.key.keysym.mod & (KMOD_ALT))
				{
					/* Mouse has left the window no zooming to be done */
					if (state_m->mouse_in == 0)
					{
						break;
					}
					if (state_m->zoom > 0)
					{
						SDL_GetMouseState(&mouseX, &mouseY);
						state_m->zoom--;
					}
					break;
				}
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
				printf("Holding on\n");
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
					printf("Something catastrophic happened, the connection is probably compromised or the server has shut down... exiting\n");
					exit(1);
				}
				keypresses_size = 0;
			}
		}

		receive_and_procees_data(sock, renderer);

		clock_t start = clock();
		
		if (resized_size >= renderer->images[0][0]->rect.w * renderer->images[0][0]->rect.h * 4)
		{
			renderer_update_bg(renderer, resized_screen_bits, IMAGE_FROM_RGB32);
		}
		
		/*clear the screen*/
		SDL_RenderClear(renderer->renderer);

		SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);
		/*adjust the size of the texture*/
		renderer_update_rects(renderer);

		/*render the images*/
		renderer_draw_images(renderer);

		/* update the screen*/
		SDL_RenderPresent(renderer->renderer);
		clock_t end = clock();

		printf("Time: %ld, Clocks per second: %ld\n", (end - start), CLOCKS_PER_SEC);
	}
	return 0;
}
