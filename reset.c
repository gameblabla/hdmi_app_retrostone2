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
	sdl_screen = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE | SDL_TRIPLEBUF);
	if (!sdl_screen)
	{
		printf("Can't set video mode\n");
		return 0;
	}
	backbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, sdl_screen->w, sdl_screen->h, 16, 0, 0,0,0); 
	SDL_EnableKeyRepeat(500, 50);
	
	SDL_FillRect(backbuffer, NULL, 0);
	SDL_Flip(sdl_screen);
	SDL_FillRect(backbuffer, NULL, 0);
	SDL_Flip(sdl_screen);
	SDL_FillRect(backbuffer, NULL, 0);
	SDL_Flip(sdl_screen);
	
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
	
	setenv("SDL_VIDEO_REFRESHRATE", "60", 0);
	
	return 0;
}
