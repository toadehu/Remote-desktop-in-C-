#include "framework.h"

/* Define a struct to hold arguments for capture_screen function*/
struct CaptureScreenArgs {
    char** screen_bits;
    size_t* buffer_size;
    int* screen_width;
    int* screen_height;
};

/* Define a struct to hold arguments for resize_image_bilinear function*/
struct ResizeImageArgs {
    char* screen_bits;
    char** resized_screen_bits;
    int screen_width;
    int screen_height;
    int target_width;
    int target_height;
    size_t* resized_size;
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

		resize_image_bilinear(cap_and_resize_args -> resize_args.screen_bits, cap_and_resize_args -> resize_args.resized_screen_bits,
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

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

	serverIp = (char*)malloc(20);
	user = (char*)malloc(128);

	int i;

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
				port = atoi(argv[i+1]);
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
				char *arobas = strchr(argv[i + 1], '@');
				if (arobas == NULL) 
				{
					printf("Invalid argument\nCorrect usage: -ip4/6 IPADDR@user -p PORT\n");
					exit(1);
				}
				size_t serverIpLen = arobas - argv[i + 1];
                serverIp = (char *) malloc((serverIpLen + 1) * sizeof(char));
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

    GRAPHICS_RENDERER* renderer = create_graphics_renderer(1280, 720, "Test", SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);
	SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);

    renderer -> ms = 50;

	char* screen_bits = NULL;

    char* resized_screen_bits = NULL;

    int resized_size = 0;

    int buffer_size = 0, screen_width, screen_height;

	capture_screen(&screen_bits, &buffer_size, &screen_width, &screen_height);

    /*This should mean that the buffer is never reallocated, so there is no data racing issues*/
    resized_screen_bits = (char*)__aligned_malloc(buffer_size, 4096);
    resized_size = buffer_size;

    printf("Screen size: %d x %d\n", screen_width, screen_height);

    renderer_update_bg(renderer, screen_bits, IMAGE_FROM_RGB32);

    /*Is the main loop active?*/
	bool loop = 1, sock_connected = 0;

	TCP_SOCKET* sock = TCP_SOCKET_create(4001, INADDR_LOOPBACK, false, false, CLIENT);

#pragma region Initiate the thread
	/*Initiate struct to move computations on a parallel thread*/
    struct CaptureScreenArgs capture_args;
    struct ResizeImageArgs resize_args;
	struct CapAndResizeArgs capture_and_resize_args;

	capture_args.screen_bits = &screen_bits;
	capture_args.buffer_size = &buffer_size;
	capture_args.screen_width = &screen_width;
	capture_args.screen_height = &screen_height;

	resize_args.screen_bits = screen_bits;
	resize_args.resized_screen_bits = &resized_screen_bits;
	resize_args.screen_width = screen_width;
	resize_args.screen_height = screen_height;
	resize_args.target_width = renderer -> images[0][0] -> rect.w;
	resize_args.target_height = renderer -> images[0][0] -> rect.h;
	resize_args.resized_size = &resized_size;
	resize_args.channels = 4;

	capture_and_resize_args.cap_args = capture_args;
	capture_and_resize_args.resize_args = resize_args;

	/*Start the thread*/
	pthread_t comp_thread;

	/*pthread_create(&comp_thread, NULL, capture_and_resize, &capture_and_resize_args);*/
#pragma endregion

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
						SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);
						/*adjust the size of the texture*/
						renderer_update_rects(renderer);
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

        capture_screen(&screen_bits, &buffer_size, &screen_width, &screen_height);

        clock_t start = clock();

        resize_image_bilinear(screen_bits, &resized_screen_bits, screen_width, screen_height,
         renderer -> images[0][0] -> rect.w, renderer -> images[0][0] -> rect.h, &resized_size, 4);

        /*resize_image_nearest_neighbor(screen_bits, resized_screen_bits, screen_width, screen_height,*/
        /* renderer -> images[0][0] -> rect.w, renderer -> images[0][0] -> rect.h, &resized_size, 4);*/

        clock_t end = clock();

        printf("Time: %f\n", (double)(1000 * (end - start)) / CLOCKS_PER_SEC);

        renderer_update_bg(renderer, resized_screen_bits, IMAGE_FROM_RGB32);

		/*clear the screen*/
		SDL_RenderClear(renderer -> renderer);
		
		/*render the images*/
		renderer_draw_images(renderer);

		/*SDL_RenderCopy(renderer -> renderer, renderer -> images[0][0] -> texture, NULL, &renderer -> images[0][0] -> rect);*/

		/* update the screen*/
		SDL_RenderPresent(renderer -> renderer);
        
		SDL_Delay(renderer -> ms);
	}
}
