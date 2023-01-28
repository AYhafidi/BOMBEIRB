/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <bombs.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

int main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	struct game *game = game_new();

	window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),
				  SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	while (!done)
	{
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	game_free(game);

	SDL_Quit();

	return EXIT_SUCCESS;
}
