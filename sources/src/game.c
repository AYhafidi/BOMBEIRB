/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <bombs.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

#define TAILLE 50
struct monster *game_get_monster(struct game *game);

struct game
{
	struct map **maps; // the game's map
	short levels;	   // nb maps of the game
	short level;
	short world;
	struct player* player;
	struct monster **monster;
	short monster_number;
	struct bomb* bomb;
};

struct game *game_new(void)
{
	sprite_load(); // load sprites into process memory
	struct game* game = malloc(sizeof(*game));
	game->world=1;
	game->monster_number=0;
	game->maps = malloc(sizeof(struct game));
	game->monster= malloc(sizeof(struct game));
	//game->bomb = NULL;
	upload_world(game);
	monster_on_map(game);
	/*game->maps[0] = map_get_static();
	game->levels = 1;
	game->level = 0;

	game->player = player_init(3);
	// Set default location of the player
	player_set_position(game->player, 1, 0);*/
	return game;
}

void game_free(struct game *game)
{
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map *game_get_current_map(struct game *game)
{
	assert(game);
	return game->maps[game->level];
}

struct monster ** game_get_monster_array(struct game* game)
{
	assert(game);
	return game->monster;
}

short game_get_monster_number(struct game* game){
	assert(game);
	return game->monster_number;
}
void game_inc_monster_number(struct game* game){
	assert(game);
	game->monster_number++;
}

struct bomb *game_get_bombs(struct game *game)
{
	assert(game);
	return game->bomb;
}

struct player *game_get_player(struct game *game)
{
	assert(game);
	return game->player;
}

void game_banner_display(struct game *game)
{
	assert(game);
	struct map *map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc / 3;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_number(game->level), x, y);

	x = 4 * white_bloc / 3;
	window_display_image(sprite_get_banner_life(), x, y);

	x = 4 * white_bloc / 3 + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 5 * white_bloc / 3 + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 5 * white_bloc / 3 + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 6 * white_bloc / 3 + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 6 * white_bloc / 3 + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_range(game_get_player(game))), x, y);
	x = 7 * white_bloc / 3 + 6 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);
	x = 7 * white_bloc / 3 + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_key(game_get_player(game))), x, y);
}

void game_display(struct game *game)
{
	assert(game);

	window_clear();
	map_display(game->maps[game->level]);
	player_display(game->player);
	monster_display(game);
	//bomb_display(game);
	game_banner_display(game);

	window_refresh();
}

static short input_keyboard(struct game *game)
{
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	Change_map(game);
	monsters_mouvement(game);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				if (player_get_nb_bomb(player) > 0)
				{
					game->bomb = bomb_init(player_get_x(player), player_get_y(player), SDL_GetTicks(), player_get_nb_range(player), game->bomb);
				}
				break;
			default:
				break;
			}
			break;
		}
	}
	return 0;
}

int game_update(struct game *game)
{
	if (input_keyboard(game))
		return 1; // exit game

	return 0;
}
//*------------------------charger les cartes a la memoire-------------------------*/

void upload_maps(struct game *game, char *prefixe)
{
	/********************************Avoir les noms des fichiers map**********************************************/
	char map[TAILLE] = "map/";
	strcat(map, prefixe);
	strcat(map, "_N");
	for (int i = 0; i < game->levels; i++)
	{
		FILE *file = NULL;
		map[strlen(map) - 1] = i + '0';
		file = fopen(map, "r");
		if (file != NULL)
		{
			game->maps[i] = get_map_dynamic(file);
			fclose(file);
		}
		else
		{
			printf("%s upload error\n", map);
		}
	}
}
/****************************************Chargement des Niveau**********************************/

void upload_world(struct game *game)
{
	short levels;
	short level;
	int x0, y0;
	char *world = "data/monde_";
	char res[64] = {0};
	sprintf(res, "%s%i", world, game->world);
	FILE *file = NULL;
	file = fopen(res, "r");
	if (file != NULL)
	{
		char res[64] = {0};
		sprintf(res,"%s%i",world,game->world);
		FILE* file=NULL;
		file=fopen(res,"r");
		if (file!=NULL){
			char res[64] = {0};
			fscanf(file,"%hi\n%hi:%d,%d\n%s",&levels,&level,&x0,&y0,res);
			game->levels=levels;
			upload_maps(game,res);
			game->level=level;
			game->player=player_init(3);
			//game->monster[0]=monster_init();
			player_set_position(game->player,x0,y0);
		}
		else{
			printf("World Error\n");
		}

}
}

/*Changement de Niveau*/
void Change_map(struct game *game)
{
	enum cell_type type = map_get_cell_type_AUX(game_get_current_map(game), player_get_x(game_get_player(game)), player_get_y(game_get_player(game)));

	//si le niveu de la porte ne depasse pas les niveaux du jeu on change la carte sinon on change le monde

	if ((type & 0xf0) == CELL_DOOR && ((type & 0x0e) >> 1) <= game->levels)
	{
		//si la porte est ouverte le joueur entre au niveau superieure

		if ((type & 0x01) == 1)
		{
					     //si la porte est ouverte le joueur entre au niveau superieure
							if( (type & 0x01)==1)
							{
											game->level=(type & 0x0e)>>1;
											window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),
											SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
							}

							//si la porte est ferme le joueur utilise une cles pour l'ouvrir

							else if ((type & 0x01)==0 && player_get_nb_key(game_get_player(game))>0){
											player_dec_nb_key(game_get_player(game));
											map_set_cell_type(game_get_current_map(game),player_get_x(game_get_player(game)),player_get_y(game_get_player(game)),type + 0x01);
											game->level=(type & 0x0e)>>1;
											window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),
											SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
						}
								monster_on_map(game);

		}

		//si la porte est ferme le joueur utilise une cles pour l'ouvrir

		else if ((type & 0x01) == 0 && player_get_nb_key(game_get_player(game)) > 0)
		{
			player_dec_nb_key(game_get_player(game));
			map_set_cell_type(game_get_current_map(game), player_get_x(game_get_player(game)), player_get_y(game_get_player(game)), type + 0x01);
			game->level = (type & 0x0e) >> 1;
			window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),
						  SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
		}
	}
}
/*Changement de monde*/
void Change_world(struct game *game)
{
	game->world++;
	upload_world(game);
	game->level = 0;
	window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),
				  SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
}
