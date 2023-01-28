/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <bombs.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

struct player
{
	int x, y;
	enum direction direction;
	int bombs;
	int key;
	int range;
	int life;
};

struct player *player_init()
{
	struct player *player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = 3;
	player->key = 3;
	player->range = 1;
	player->life = 7;

	return player;
}

void player_set_position(struct player *player, int x, int y)
{
	assert(player);
	player->x = x;
	player->y = y;
}

void player_free(struct player *player)
{
	assert(player);
	free(player);
}

int player_get_x(struct player *player)
{
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player *player)
{
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player *player, enum direction way)
{
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player *player)
{
	assert(player);
	return player->bombs;
}

void player_inc_nb_bomb(struct player *player)
{
	printf("+\n");
	assert(player);
	player->bombs++;
}

void player_dec_nb_bomb(struct player *player)
{
	printf("-\n");
	assert(player);
	player->bombs--;
}
/* retourne le nombre de vie du joueur*/
int player_get_nb_life(struct player *player)
{
	assert(player);
	return player->life;
}

void player_inc_life(struct player *player)
{
	assert(player);
	player->life++;
}

void player_dec_life(struct player *player)
{
	assert(player);
	player->life--;
}
/* retourne la portee des bombes*/

int player_get_nb_range(struct player *player)
{
	assert(player);
	return player->range;
}

void player_inc_range(struct player *player)
{
	assert(player);
	player->range++;
}

void player_dec_range(struct player *player)
{
	assert(player);
	if (player->range >= 2)
	{
		player->range--;
	}
}
/*retourne le nombre de cle que possède le joueur*/
int player_get_nb_key(struct player *player)
{
	assert(player);
	return player->key;
}

void player_inc_nb_key(struct player *player)
{
	assert(player);
	player->key++;
}

void player_dec_nb_key(struct player *player)
{
	assert(player);
	player->key--;
}

/*----------------------------Autoriser le mouvement des caisses ou le blocker-----------------------------*/

static int move_box_aux(struct map *map, struct player *player)
{
	int index = 0;
	int x = player->x;
	int y = player->y;
	switch (player->direction)
	{
	case NORTH:
		if (map_get_cell_type(map, x, y - 1) == CELL_BOX && map_get_cell_type(map, x, y - 2) == CELL_EMPTY && y - 2 != -1)
		{
			map_set_cell_type(map, x, y - 1, CELL_EMPTY);
			map_set_cell_type(map, x, y - 2, CELL_BOX);
			index = 1;
		}
		break;
	case SOUTH:
		if (map_get_cell_type(map, x, y + 1) == CELL_BOX && map_get_cell_type(map, x, y + 2) == CELL_EMPTY && y + 2 != map_get_height(map))
		{
			map_set_cell_type(map, x, y + 1, CELL_EMPTY);
			map_set_cell_type(map, x, y + 2, CELL_BOX);
			index = 1;
		}
		break;
	case WEST:
		if (map_get_cell_type(map, x - 1, y) == CELL_BOX && map_get_cell_type(map, x - 2, y) == CELL_EMPTY && x - 2 != -1)
		{
			map_set_cell_type(map, x - 1, y, CELL_EMPTY);
			map_set_cell_type(map, x - 2, y, CELL_BOX);
			index = 1;
		}
		break;
	case EAST:
		if (map_get_cell_type(map, x + 1, y) == CELL_BOX && map_get_cell_type(map, x + 2, y) == CELL_EMPTY && x + 2 != map_get_width(map))
		{
			map_set_cell_type(map, x + 1, y, CELL_EMPTY);
			map_set_cell_type(map, x + 2, y, CELL_BOX);
			index = 1;
		}
		break;
	}
	return index;
}
/*----------------------------------------------------------------------------------------------*/

/*--------gestion des bonus---------------------------------------------------------------------*/

static int bonus_action(struct map *map, struct player *player, int x, int y)
{

	switch (map_get_bonus_type(map, x, y))
	{
	case BONUS_BOMB_RANGE_DEC:
		player_dec_range(player);
		break;
	case BONUS_BOMB_RANGE_INC:
		player_inc_range(player);
		break;
	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;
	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;
	case BONUS_MONSTER:
		break;
	case BONUS_LIFE:
		player_inc_life(player);
		break;
	}
	return 1;
}

/*----------------------------------------------------------------------------------------------*/

static int player_move_aux(struct player *player, struct map *map, int x, int y)
{

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y))
	{
	case CELL_EMPTY:
		return 1;
		break;
	case CELL_SCENERY:
		return 0;
		break;
	case CELL_BOX:
		return move_box_aux(map, player);
		break;
	case CELL_BONUS:
		return bonus_action(map, player, x,y);
		break;
	case CELL_MONSTER:
		break;
	case CELL_DOOR:
		break;
	case CELL_KEY:
		player_inc_nb_key(player);
		break;
	default:
		return 0;
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player *player, struct map *map)
{
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction)
	{

	case NORTH:
		if (player_move_aux(player, map, x, y - 1) && y - 1 != -1)
		{
			player->y--;
			move = 1;
		}
		break;

		/*----------------limiter le mouvement en calculant la largeur et la hauteur de la map------------------------*/
	case SOUTH:
		if (player_move_aux(player, map, x, y + 1) && y + 1 != map_get_height(map))
		{
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y) && x - 1 != -1)
		{
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y) && x + 1 != map_get_width(map))
		{
			player->x++;
			move = 1;
		}
		break;
	}

	if (move && map_get_cell_type(map, x, y) != CELL_DOOR && map_get_cell_type(map, x, y) != CELL_BOMB)
	{
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void player_display(struct player *player)
{
	assert(player);
	window_display_image(sprite_get_player(player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
