#include "framework.h"

void update_versace_size(SDL_Rect* rect, SDL_Rect win_rect)
{
	rect -> w = win_rect.w * 0.35;
	rect -> h = win_rect.h * 0.35;
	rect -> x = (win_rect.w >> 6);
	rect -> y = (win_rect.h >> 1) + (win_rect.h >> 3);
}

void versace_button_on_click()
{
	//To be completed
}

int main (int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	GRAPHICS_RENDERER *renderer = create_graphics_renderer(800, 600, "Test", SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	SDL_GetWindowSize(renderer -> window, &renderer -> win_rect.w, &renderer -> win_rect.h);

	//GRAPHICS_RENDERER *la_lautari = create_graphics_renderer(800, 600, "Lautari", SDL_WINDOW_SHOWN);

	button_element *versace_button = create_new_button_element(renderer -> renderer, "button.png", "flopa.jpg", renderer -> win_rect.w / 8, renderer -> win_rect.h / 8,
	  renderer -> win_rect.w / 8, renderer -> win_rect.h / 8, update_versace_size);

	versace_button -> on_click = versace_button_on_click;

	renderer_add_button(renderer, versace_button);

	renderer_update_rects(renderer);

	unsigned char* screen_bits = NULL;
	unsigned char* yuv_buffer = NULL;

	int buffer_size = 0, screen_width, screen_height;

	//Is the main loop active?
	bool loop = 1;

	//create the video encoder
	basic_video_enc* basic_enc = NULL;

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
						//adjust the size of the texture
						renderer_update_rects(renderer);
                        //update_rectangle_size(&renderer -> images[0][0] -> rect, renderer -> win_rect.w, renderer -> win_rect.h);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						renderer_handle_click(renderer, event.button.x, event.button.y);
	
						//Start the capture
						capture_screen(&screen_bits, &buffer_size, &screen_width, &screen_height);

						if (yuv_buffer == NULL)
							yuv_buffer = (unsigned char *)__aligned_malloc((screen_width * screen_height * 3) / 2, 1024);
						
						ARGBToYUV420(&screen_bits, screen_width, screen_height, &yuv_buffer);
						
						#ifdef _DEBUG
						printf("Writing test.yuv file\n");
						#endif
						if (basic_enc == NULL)
						{
							basic_enc = create_basic_video_enc(screen_width, screen_height,
							 0, 
							VIDEO_YUV420 | RLE_TWO_PASS, 0);

							int _sz = basic_encode_next_frame(basic_enc, yuv_buffer);
							#ifdef _DEBUG
							printf("Size of first frame: %d\n", _sz);
							#endif
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						renderer_handle_click_released(renderer, event.button.x, event.button.y);
					}
					break;
			}
		}

		//clear the screen
		SDL_RenderClear(renderer -> renderer);
		
		//render the images
		renderer_draw_images(renderer);

		//SDL_RenderCopy(renderer -> renderer, renderer -> images[0][0] -> texture, NULL, &renderer -> images[0][0] -> rect);

		// update the screen
		SDL_RenderPresent(renderer -> renderer);

		if (basic_enc != NULL)
		{
		capture_screen(&screen_bits, &buffer_size, &screen_width, &screen_height);

		if (yuv_buffer == NULL)
			yuv_buffer = (unsigned char *)__aligned_malloc((screen_width * screen_height * 3) / 2, 16);
						
		//As it stands, using SIMD version of conversions yields to wrong results, maybe I will look into it
		ARGBToYUV420(&screen_bits, screen_width, screen_height, &yuv_buffer);

#ifdef _DEBUG
		clock_t start = clock();

		int sz = basic_encode_next_frame(basic_enc, yuv_buffer);

		clock_t end = clock();

		printf("Time for encode %d, size of image: %d\n", (end - start) * 1000 / CLOCKS_PER_SEC, sz);
#else
		basic_encode_next_frame(basic_enc, yuv_buffer);
#endif

		}

		// calculate to 24fps
		SDL_Delay(renderer -> ms);
	}

	return 0;
}
