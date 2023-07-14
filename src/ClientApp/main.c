/*
	CLIENT
*/

#include "framework.h"

#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

typedef unsigned char u_char;

/* Define a struct to hold arguments for capture_screen function*/
struct CaptureScreenArgs {
    char** screen_bits;
    int* buffer_size;
    int* screen_width;
    int* screen_height;
};

/* Define a struct to hold arguments for resize_image_bilinear function*/
struct ResizeImageArgs {
    char* screen_bits;
    char* resized_screen_bits;
    int screen_width;
    int screen_height;
    int target_width;
    int target_height;
    int* resized_size;
    int channels;
};

struct CapAndResizeArgs{
	struct CaptureScreenArgs cap_args;
	struct ResizeImageArgs resize_args;
};

char* serverIp, *user;
int ipver,port;

void capture_and_resize(void* args)
{
	while (true)
	{
		struct CapAndResizeArgs* cap_and_resize_args = (struct CapAndResizeArgs*)args;

		capture_screen(cap_and_resize_args -> cap_args.screen_bits, cap_and_resize_args -> cap_args.buffer_size,
		cap_and_resize_args -> cap_args.screen_width, cap_and_resize_args -> cap_args.screen_height);

		clock_t start = clock();

		resize_image_bilinear((const unsigned char*)cap_and_resize_args -> resize_args.screen_bits, (unsigned char*)cap_and_resize_args -> resize_args.resized_screen_bits,
		cap_and_resize_args -> resize_args.screen_width, cap_and_resize_args -> resize_args.screen_height,
		cap_and_resize_args -> resize_args.target_width, cap_and_resize_args -> resize_args.target_height,
		cap_and_resize_args -> resize_args.resized_size, cap_and_resize_args -> resize_args.channels);

		/*resize_image_nearest_neighbor(screen_bits, resized_screen_bits, screen_width, screen_height,*/
		/* renderer -> images[0][0] -> rect.w, renderer -> images[0][0] -> rect.h, &resized_size, 4);*/

		clock_t end = clock();

		printf("Time: %f\n", (double)(1000 * (end - start)) / CLOCKS_PER_SEC);

		SDL_Delay(1000 / 24);
	}
}

int SDL_main(int argc, char* argv[])
{
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

    GRAPHICS_RENDERER* renderer = create_graphics_renderer(1280, 720, (char*)"Test\0", SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);
	SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);

    renderer -> ms = 50;

	char* buffer = NULL;

	char* screen_bits = NULL;
	char* yuv_buffer = NULL;

    char* resized_screen_bits = NULL;

    int resized_size = 0;

    int buffer_size = 0, screen_width = 0, screen_height = 0;

	buffer = (char*)__aligned_malloc(720 * 1280, 1024);
	buffer_size = 720 * 1280;

    /*Is the main loop active?*/
	bool loop = 1;

	struct TCP_Socket* sock = TCP_SOCKET_create(port > 0 ? port : 4002, INADDR_LOOPBACK, false, false, CLIENT | BIG_BUFFER);

	basic_video_dec *basic_dec = NULL;

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
			scanf("%c", &inp);
			PROCESS_INP:
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
				scanf("%c", &inp);
				goto PROCESS_INP;
			}
		}
	}

	/*Main loop*/
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
						/*SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);*/
						/*adjust the size of the texture*/
						/*renderer_update_rects(renderer);*/
                        /*update_rectangle_size(&renderer -> images[0][0] -> rect, renderer -> win_rect.w, renderer -> win_rect.h);*/
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
                        
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
                        
					}
					break;
			}
		}

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
			clock_t start = clock();

			/* Get the flags and information from the header */
			int encoding_flags = ntohl(*(int*)((char*)buffer + 4));
			int encoded_image_size = ntohl(*(int*)((char*)buffer + 8));
			printf("encoded_image_size: %d\n", encoded_image_size);
			int second_pass_size = ntohl(*(int*)((char*)buffer + 12));
			uint16_t w = ntohs(*(uint16_t*)((char*)buffer + 16));
			uint16_t h = ntohs(*(uint16_t*)((char*)buffer + 18));

			//Create the initial decoder and other buffers
			if (basic_dec == NULL)
			{
				basic_dec = basic_create_video_dec(w, h, 0, VIDEO_YUV420, RLE_TWO_PASS);
				screen_bits = (char*)__aligned_malloc(w * h * 4, 1024);
				yuv_buffer = (char*)__aligned_malloc(w * h * 3 / 2, 1024);
				int res_w = 1920, res_h = 1080;
				renderer -> win_rect.w > w ? res_w = renderer -> win_rect.w + 300 : w + 300;
				renderer -> win_rect.h > h ? res_h = renderer -> win_rect.h + 300 : h + 300; /* This is to have some room so the reallocation doesn't happen */
				resized_screen_bits = (char*)__aligned_malloc(res_w * res_h * 4, 1024);
				resized_size = res_w * res_h * 4;
			}

			if (screen_width != (int)w)
			{
				if (screen_width < (int)w || screen_height < (int)h)
				{
					screen_bits = (char*)__aligned_realloc(screen_bits, screen_width * screen_height * 4, (int)w * (int)h * 4, 1024);
					yuv_buffer = (char*)__aligned_realloc(yuv_buffer, screen_width * screen_height * 3 / 2, (int)w * (int)h * 3 / 2, 1024);
				}
				screen_width = (int)w;
				screen_height = (int)h;
			}

			if  (encoded_image_size > buffer_size)
			{
				buffer = (char*)__aligned_realloc(buffer, buffer_size, buffer_size * 2 > encoded_image_size ? buffer_size * 2 : encoded_image_size + buffer_size, 4096); 
				buffer_size = buffer_size * 2 > encoded_image_size ? buffer_size * 2 : encoded_image_size + buffer_size;/*This is to limit the number of calls to realloc, becuase it will waste at most ~4MB of ram, which is acceptable*/
			}

			rec = TCP_Socket_receive_data_fixed(sock, buffer, encoded_image_size);

			if (rec != encoded_image_size)
			{
				printf("Expected %d bytes, received %d bytes\n", encoded_image_size, rec);
				printf("Something catastrophic happened, the connection is probably compromised or the server has shut down... exiting\n");
				exit(1);
			}


			basic_decode_next_frame(basic_dec, buffer, encoded_image_size, second_pass_size, encoding_flags);

			/* Because I always update the buffer incase it is too small, I shouldn't run into issues with the buffer being too small */
			basic_copy_frame_d(basic_dec, yuv_buffer, screen_width * screen_height * 3 / 2, 0, encoding_flags);

			printf("First U pixel bits: %d %d %d %d\n", yuv_buffer[screen_width * screen_height + 0], yuv_buffer[screen_width * screen_height + 1], yuv_buffer[screen_width * screen_height + 2], yuv_buffer[screen_width * screen_height + 3]);

			printf("Copied frame\n");

			/*convert the image to RGB32*/

			YUV420ToARGB(yuv_buffer, screen_width, screen_height, screen_bits);

			printf("First pixel bits: %d %d %d %d\n", (u_char)screen_bits[0], (u_char)screen_bits[1], (u_char)screen_bits[2], (u_char)screen_bits[3]);

			printf("Converted frame to ARGB\n");

			resize_image_nearest_neighbor((const unsigned char*)screen_bits, (unsigned char*)resized_screen_bits, screen_width, screen_height,
				renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h, 4);

			//resize_image_bilinear((const unsigned char*)screen_bits, (unsigned char*) resized_screen_bits, screen_width, screen_height,
			//	renderer->images[0][0]->rect.w, renderer->images[0][0]->rect.h, &resized_size, 4);

			/*resize_image_nearest_neighbor(screen_bits, resized_screen_bits, screen_width, screen_height,
			renderer -> images[0][0] -> rect.w, renderer -> images[0][0] -> rect.h, &resized_size, 4);*/

			clock_t end = clock();

			printf("Time: %f\n", (double)(1000 * (end - start)) / CLOCKS_PER_SEC);

			renderer_update_bg(renderer, resized_screen_bits, IMAGE_FROM_RGB32);

			/*clear the screen*/
			SDL_RenderClear(renderer->renderer);

			/*render the images*/
			renderer_draw_images(renderer);
		}
		/*SDL_RenderCopy(renderer -> renderer, renderer -> images[0][0] -> texture, NULL, &renderer -> images[0][0] -> rect);*/

		SDL_GetWindowSize(renderer->window, &renderer->win_rect.w, &renderer->win_rect.h);
		/*adjust the size of the texture*/
		renderer_update_rects(renderer);

		/* update the screen*/
		SDL_RenderPresent(renderer -> renderer);
        
		SDL_Delay(renderer -> ms);
	}
	return 0;
}
