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
// Letzte Änderung am 27.11.2022                //
//                                              //
//                                              //
//////////////////////////////////////////////////

#define SMOOTHING_ON           1

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "./plasma.h"
#include "./cometrain.h"
#include "./pictureshow.h"
#include "./fire.h"
#include "./pixeltest.h"
#include "./game_of_life.h"

using namespace std;

static SDL_Renderer *ren;

static bool quit = false;

#define WIDTH 25
#define HEIGHT 30
#define LED_COUNT WIDTH * HEIGHT
#define EFFECT_TIME	5		// for 5sek
#define FPS	50				// 50 Frame per second

#define RASTER_SIZE_XW 50
#define RASTER_SIZE_YW 40
#define WINDOW_WIDTH WIDTH * RASTER_SIZE_XW
#define WINDOW_HEIGHT HEIGHT * RASTER_SIZE_YW

uint32_t buffer[LED_COUNT];
uint32_t matrix[LED_COUNT];

enum effects{FIRE, PLASMA, COMETRAIN, PICTURESHOW, GAMEOFLIFE, PIXELTEST, EFFECT_COUNT};

int current_effect = GAMEOFLIFE;
int effect_time_counter = EFFECT_TIME * FPS;

void buffer_clear();
void matrix_to_leds();
void buffer_to_matrix();

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

	// Init Effects

	Fire fire(WIDTH, HEIGHT, buffer);
	fire.Init();

	Plasma plasma(WIDTH, HEIGHT, buffer);
	plasma.Init();

	Cometrain cometrain(WIDTH, HEIGHT, buffer);
	cometrain.Init();

	PictureShow pictureshow(WIDTH, HEIGHT, buffer);

	PixelTest pixel_test(WIDTH, HEIGHT, buffer);

	GameOfLife game_of_life(WIDTH, HEIGHT, buffer);

	if(!pictureshow.LoadPNG("/home/thorsten/baum3.png"))
		cout << "Picture not open." << endl;

	quit = false;

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

		buffer_clear();

		// render effects

		switch(current_effect)
		{
		case FIRE:
			fire.Render();
			break;
		case PLASMA:
			plasma.Render();
			break;

		case COMETRAIN:
			cometrain.Render();
			break;

		case PICTURESHOW:
			pictureshow.Render();
			break;

		case GAMEOFLIFE:
			game_of_life.Render();
			break;

		case PIXELTEST:
			pixel_test.Render();
			break;
		}

		effect_time_counter--;
		if(effect_time_counter == 0)
		{
			effect_time_counter = EFFECT_TIME * FPS;
			current_effect++;
			if(current_effect == EFFECT_COUNT)
				current_effect = FIRE;
		}

		buffer_to_matrix();
		matrix_to_leds();

		//Update the screen
		SDL_RenderPresent(ren);
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

void buffer_clear()
{
	for(int i=0; i<LED_COUNT; i++)
		buffer[i] = 0x00000000;
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
				x_center = ((led_x - 1) * RASTER_SIZE_XW) + RASTER_SIZE_XW / 2;
				y_center = ((led_y - 1) * RASTER_SIZE_YW) + RASTER_SIZE_YW / 2;
			}
			else
			{
				x_center = ((led_x - 1) * RASTER_SIZE_XW) + RASTER_SIZE_XW / 2;
				y_center = (((HEIGHT - led_y)) * RASTER_SIZE_YW) + RASTER_SIZE_YW / 2;
			}

			float br = 0.0f;
			for(int i=20; i>=0; i--)
			{
				filledCircleColor(ren,x_center, y_center, i, 0xff000000 | (uint8_t)((float)b * br ) << 16 | (uint8_t)((float)g * br ) << 8 | (uint8_t)((float)r * br ));
				br += 0.05;
			}

			m_index++;
		}
		flip++;
	}
}

void buffer_to_matrix()
{
	int x = WIDTH-1;
	int y = HEIGHT-1;

	bool y_direction = false;

	for(int i=0; i<LED_COUNT; i++)
	{
		matrix[i] = buffer[y*WIDTH+x];

		if(y_direction)
		{
			y++;
			if(y>=HEIGHT)
			{
				y=HEIGHT-1;
				x--;
				y_direction = false;
			}
		}
		else
		{
			y--;
			if(y<0)
			{
				y=0;
				x--;
				y_direction = true;
			}
		}
	}
}
