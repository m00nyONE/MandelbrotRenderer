#include <SDL.h>
#include <string>

int WINDOW_WIDTH = 1024;
int WINDOW_HIGHT = 768;
int WIDTH = 800;
int HIGHT = 800;
long double MIN = -2.0;
long double MAX = 2.0;
int MAX_ITERATIONS = 200;
long double factor = 1;

bool zoom = true;

//mapping function -- see Arduino.cc/en/Reference/Map -- i am using it to center the coordinates
long double map(long double value, long double in_min, long double in_max, long double out_min, long double out_max)
{
	return ( value - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min ;
}

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_EVENTS);

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HIGHT, 0, &window, &renderer);
	SDL_RenderSetLogicalSize(renderer, WIDTH, HIGHT);

	int n = 0;

	if (zoom == true)
	{
		MIN = -2.84;
		MAX = 1.0;
	}

	while (true)
	{
		SDL_RenderPresent(renderer);

		if(zoom == true)	
		{
			MIN += 0.15 * factor;
			MAX -= 0.1 * factor;

			factor *= 0.9350;
			MAX_ITERATIONS += 5;
						
			if (n > 30)
				MAX_ITERATIONS *= 1.02;

		}

		std::string s = "Iterations: " + std::to_string(MAX_ITERATIONS) + " -- MIN: " + std::to_string(MIN) + " -- MAX: " + std::to_string(MAX);
		const char* title = s.c_str();
		SDL_SetWindowTitle(window, title);


		for (int x = 0; x < WIDTH; x++)
		{
			if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			{	
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				return 0; 
			}
				
			for (int y = 0; y < HIGHT; y++)
			{
				long double a = map(x, 0, WIDTH, MIN, MAX);
				long double b = map(y, 0, HIGHT, MIN, MAX);

				long double a0 = a;
				long double b0 = b;

				int ITERATIONS = 0;

				for (int i = 0; i < MAX_ITERATIONS; i++)
				{
					long double a1 = a * a - b * b ;
					long double b1 = 2 * a * b;

					a = a1 + a0;
					b = b1 + b0;

					if ((a + b) > 2)
						break;
					
					ITERATIONS++;
				}
				int brightness = map(ITERATIONS, 0, MAX_ITERATIONS, 0, 255);

				if ((ITERATIONS == MAX_ITERATIONS) || brightness < 0 )
					brightness = 0;

				long double t = map(ITERATIONS, 0, MAX_ITERATIONS, 0, 1);


				int red = static_cast<int>(9 * (1 - t) * t * t * t * 255);
				int green = static_cast<int>(15 * (1 - t) * (1 - t) * t * t * 255);
				int blue = static_cast<int>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

				SDL_SetRenderDrawColor(renderer, red, green, blue, brightness);
				SDL_RenderDrawPoint(renderer, x, y);
			}		
		}
		n += 1;
	}
	return 0;
}
