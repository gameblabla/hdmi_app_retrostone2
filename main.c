/*
Copyright (c) 2020 Gameblabla

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include "font_drawing.h"

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

SDL_Surface* sdl_screen, *backbuffer;
uint32_t key_pressed = 0;
uint8_t final_long_string[512];

int select_cursor = 0;

int main (int argc, char *argv[]) 
{
	SDL_Event event;
	int quit = 0;
	int i;
	
	setenv("SDL_VIDEO_REFRESHRATE", "50", 0);
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(0);
	sdl_screen = SDL_SetVideoMode(320, 240, 32, SDL_HWSURFACE | SDL_TRIPLEBUF);
	if (!sdl_screen)
	{
		printf("Can't set video mode\n");
		return 0;
	}
	backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, sdl_screen->w, sdl_screen->h, 16, 0, 0,0,0); 
	SDL_EnableKeyRepeat(500, 50);
	
	while(!quit)
	{
		while (SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_KEYUP:
					switch(event.key.keysym.sym) 
					{
						/* SDLK_HOME for OD needs the Power button to be detected upon release. */
						case SDLK_HOME:
						case SDLK_ESCAPE:
							quit = 1;
						break;
						default:
						break;
					}
				break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) 
					{
						case SDLK_UP:
						select_cursor--;
						if (select_cursor < 0) select_cursor = 0;
						break;
						case SDLK_DOWN:
						select_cursor++;
						if (select_cursor > 1) select_cursor = 1;
						break;
						case SDLK_RETURN:
						case SDLK_LCTRL:
							quit = 1;
						break;
						default:
						break;
					}
				break;
				case SDL_QUIT:
					quit = 1;
				break;
			}
		}

		SDL_FillRect(backbuffer, NULL, 0);
		
		print_string("Select your video output", SDL_MapRGB(sdl_screen->format,255,255,255), 0, 20, 5, backbuffer->pixels);
		
		print_string(">", SDL_MapRGB(sdl_screen->format,255,255,255), 0, 20, 50 + (select_cursor * 40), backbuffer->pixels);
		
		print_string("LCD output", SDL_MapRGB(sdl_screen->format,255,255,255), 0, 120, 50, backbuffer->pixels);
		print_string("HDMI output", SDL_MapRGB(sdl_screen->format,255,255,255), 0, 120, 90, backbuffer->pixels);
		
		print_string("Start/A : Apply changes", SDL_MapRGB(sdl_screen->format,255,255,255), 0, 20, 180, backbuffer->pixels);
		
		/* Print back buffer to the final screen */
		SDL_BlitSurface(backbuffer, NULL, sdl_screen, NULL);
		
		/* Flip the screen so it gets displayed*/
		SDL_Flip(sdl_screen);
	}
	
	/* Make sure to clean up the allocated surfaces before exiting.
	 * Most of the time when an SDL app leaks, it's for that reason.
	 * */
	if (sdl_screen)
	{
		SDL_FreeSurface(sdl_screen);
		sdl_screen = NULL;
	}
	if (backbuffer)
	{
		SDL_FreeSurface(backbuffer);
		backbuffer = NULL;
	}
	
	SDL_Quit();
	
	setenv("SDL_VIDEO_REFRESHRATE", "50", 0);
	
	if (select_cursor == 1)
	{
		snprintf(final_long_string, sizeof(final_long_string), "echo off > /sys/class/drm/card1-Unknown-1/status;echo on > /sys/class/drm/card1-HDMI-A-1/status");
	}
	else
	{
		snprintf(final_long_string, sizeof(final_long_string), "echo on > /sys/class/drm/card1-Unknown-1/status;echo off > /sys/class/drm/card1-HDMI-A-1/status");
	}
	
	execlp("/bin/sh","/bin/sh", "-c", final_long_string, (char *)NULL);
	
	return 0;
}
