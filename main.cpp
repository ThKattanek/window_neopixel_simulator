//////////////////////////////////////////////////
//                                              //
// SDL2 Render Threading                        //
// von Thorsten Kattanek                        //
//                                              //
// #file: main.cpp                              //
//                                              //
// Dieser Sourcecode ist Copyright geschützt!   //
// Geistiges Eigentum von Th.Kattanek           //
//                                              //
// Letzte Änderung am 13.11.2021                //
//                                              //
//                                              //
//////////////////////////////////////////////////

#define SMOOTHING_ON           1

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

using namespace std;

static SDL_Renderer *ren;

static bool quit = false;

#define WIDTH 25
#define HEIGHT 30
#define LED_COUNT WIDTH * HEIGHT

#define RASTER_SIZE 40
#define WINDOW_WIDTH WIDTH * RASTER_SIZE
#define WINDOW_HEIGHT HEIGHT * RASTER_SIZE

uint32_t matrix[LED_COUNT];

void matrix_init();
void matrix_render();
void matrix_to_leds();

#undef main
int main()
{
    // SLD initialize only video
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a SDL Window
	SDL_Window *win = SDL_CreateWindow("Window Neopixel Sinulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (win == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a SDL Renderer
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr){
        SDL_DestroyWindow(win);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

	matrix_init();

    // Create and running the Render Thread
	//thread1 = SDL_CreateThread(RenderThread, "RenderThread", nullptr);

    // main loop with event handling
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e)){
            //If user closes the window
            if (e.type == SDL_QUIT){
                quit = true;
            }
            //If user presses any key
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
            //If user clicks the mouse
			/*
            if (e.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
			*/
        }

		matrix_render();
		matrix_to_leds();

		//Update the screen
		SDL_RenderPresent(ren);

		//cout << "TEST" << endl;

		SDL_Delay(1000/50);
    }

    // Destroys all created objects
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    // SDL Quit
    SDL_Quit();

    // Bye
    return 0;
}

void matrix_init()
{
	int c = 0;
	int b = 0;
	for(int i=0; i<LED_COUNT; i++)
	{
		switch(b)
		{
		case 0:
			matrix[i] = 0x0000ff00;
			break;
		case 1:
			matrix[i] = 0x000000ff;
			break;
		case 2:
			matrix[i] = 0x00ff0000;
			break;
		default:
			break;
		}

		c++;
		if(c == 5)
		{
			b++;
			if(b == 3) b = 0;
			c = 0;
		}
	}
}

void matrix_render()
{
	uint32_t tmp = matrix[0];

	for(int i=0; i<LED_COUNT; i++)
	{
		matrix[i] = matrix[i+1];
	}
	matrix[LED_COUNT-1] = tmp;
}

void matrix_to_leds()
{
	//Clear the renderer
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderClear(ren);

	int m_index = 0;
	int flip = 0;
	for(int led_x = WIDTH; led_x > 0; led_x--)
	{
		for(int led_y = HEIGHT; led_y > 0; led_y--)
		{
			uint8_t r = matrix[m_index] & 0xff;
			uint8_t g = (matrix[m_index] >> 8) & 0xff;
			uint8_t b = (matrix[m_index] >> 16) & 0xff;

			int x_center;
			int y_center;

			if(!(flip & 1))
			{
				x_center = ((led_x - 1) * RASTER_SIZE) + RASTER_SIZE / 2;
				y_center = ((led_y - 1) * RASTER_SIZE) + RASTER_SIZE / 2;
			}
			else
			{
				x_center = ((led_x - 1) * RASTER_SIZE) + RASTER_SIZE / 2;
				y_center = (((HEIGHT - led_y)) * RASTER_SIZE) + RASTER_SIZE / 2;
			}

			float br = 0.0f;
			for(int i=14; i>=0; i--)
			{
				filledCircleColor(ren,x_center, y_center, i, 0xff000000 | (uint8_t)((float)b * br ) << 16 | (uint8_t)((float)g * br ) << 8 | (uint8_t)((float)r * br ));
				br += 0.070;
			}

			m_index++;
		}
		flip++;
	}
}
