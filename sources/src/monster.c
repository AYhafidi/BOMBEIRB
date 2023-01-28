/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <bombs.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

struct monster
{
  int x, y;
  enum direction direction;
  int timer;
  int speed;
};

struct monster *monster_init(int x, int y)
{
    struct monster* monster=malloc(sizeof(struct monster));
  if (!monster){
    error("Memory error");
  }
  monster->direction=NORTH;
  monster->timer=SDL_GetTicks();
  monster->x=x;
  monster->y=y;
  monster->speed=500; //Pour controler la vitesse de mouvement du monsters, 500 Just pour essayer
  return monster;
}

void monster_set_position(struct monster *monster, int x, int y)
{
  assert(monster);
  monster->x = x;
  monster->y = y;
}


void monster_set_current_way(struct monster *monster, enum direction way)
{
  assert(monster);
  monster->direction = way;
}

int monster_get_x(struct monster *monster)
{
  assert(monster != NULL);
  return monster->x;
}

int monster_get_y(struct monster *monster)
{
  assert(monster != NULL);
  return monster->y;
}

static int monster_move_aux(struct monster *monster, struct map *map, int x, int y)
{

  if (!map_is_inside(map, x, y))
    return 0;

  switch (map_get_cell_type(map, x, y))
  {
  case CELL_EMPTY:
  case CELL_BONUS:
  case CELL_KEY:
  case CELL_BOMB:
  case CELL_MONSTER:
		return 1;
		break;
	default:
		return 0;
		break;
	}

	return 1;
}

int monster_move(struct monster *monster, struct map *map)
{
	int x = monster->x;
	int y = monster->y;
	int move = 0;


	switch (monster->direction)
	{

	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1) && y - 1 != -1)
		{
			monster->y--;
			move = 1;
		}
		break;

/*----------------limiter le mouvement en calculant la largeur et la hauteur de la map------------------------*/
	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1) && y + 1 != map_get_height(map))
		{
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y) && x - 1 != -1)
		{
			monster->x--;
			move = 1;
		}
		break;
void monster_free(struct monster *monster)
{
	assert(monster);
	free(monster);
}

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y) && x + 1 != map_get_width(map))
		{
			monster->x++;
			move = 1;

		}
		break;
	}

	if (move && map_get_cell_type(map,x,y)!=CELL_BONUS && map_get_cell_type(map,x,y)!=CELL_MONSTER ) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

/*affichage de monster*/

void monster_display(struct game *game)
{
        assert(game);
        struct monster **monster=game_get_monster_array(game);
        for (int i=0;i<game_get_monster_number(game);i++){
              window_display_image(sprite_get_monster(monster[i]->direction),monster[i]->x * SIZE_BLOC, monster[i]->y * SIZE_BLOC);
      }
}

 void monsters_mouvement (struct game* game){
  assert(game);
  struct monster ** monster=game_get_monster_array(game);
  struct map * map=game_get_current_map(game);
        for (int i=0;i<game_get_monster_number(game);i++) {
                  monster_random_mouvement(monster[i],map);
  }
}

/*Mouvement aleatoire des monsters*/
void monster_random_mouvement(struct monster *monster, struct map *map)
{
  if ((SDL_GetTicks() - monster->timer) > monster->speed)
  {
    int x = monster_get_x(monster);
    int y = monster_get_y(monster);
    char done = Continue_same_path(monster, map);
    while (done)
    {
      srand(SDL_GetTicks());
      enum direction direction = rand() % 4;
      switch (direction)
      {
      case NORTH:
        if ((map_get_cell_type(map, x, y - 1) == CELL_EMPTY || map_get_cell_type(map, x, y - 1) == CELL_BONUS) && y - 1 != -1)
        {
          monster->direction = direction;
          done = 0;
        }
      case EAST:
        if ((map_get_cell_type(map, x + 1, y) == CELL_EMPTY || map_get_cell_type(map, x + 1, y) == CELL_BONUS) && x + 1 != map_get_width(map))
        {
          monster->direction = direction;
          done = 0;
        }
        break;
      case SOUTH:
        if ((map_get_cell_type(map, x, y + 1) == CELL_EMPTY || map_get_cell_type(map, x, y + 1) == CELL_BONUS) && y + 1 != map_get_height(map))
        {
          monster->direction = direction;
          done = 0;
        }
        break;
      case WEST:
        if ((map_get_cell_type(map, x - 1, y) == CELL_EMPTY || map_get_cell_type(map, x - 1, y) == CELL_BONUS) && x - 1 != -1)
        {
          monster->direction = direction;
          done = 0;
        }
        break;
      }
    }
    monster_move(monster, map);
    monster->timer = SDL_GetTicks();
  }
}

/*Continuer dans le meme chemin*/
char Continue_same_path(struct monster *monster, struct map *map)
{
  int x = monster_get_x(monster);
  int y = monster_get_y(monster);
  switch (monster->direction)
  {
  case NORTH:
    if ((map_get_cell_type(map, x, y - 1) == CELL_EMPTY || map_get_cell_type(map, x, y - 1) == CELL_BONUS) && y - 1 != -1)
    {
      return 0;
    }
    break;
  case EAST:
    if ((map_get_cell_type(map, x + 1, y) == CELL_EMPTY || map_get_cell_type(map, x + 1, y) == CELL_BONUS) && x + 1 != map_get_width(map))
    {
      return 0;
    }
    break;
  case SOUTH:
    if ((map_get_cell_type(map, x, y + 1) == CELL_EMPTY || map_get_cell_type(map, x, y + 1) == CELL_BONUS) && y + 1 != map_get_height(map))
    {
      return 0;
    }
    break;
  case WEST:
    if ((map_get_cell_type(map, x - 1, y) == CELL_EMPTY || map_get_cell_type(map, x - 1, y) == CELL_BONUS) && x - 1 != -1)
    {
      return 0;
    }
    break;
  }
  return 1;
}
/*mettre les monsters a la map*/

void monster_on_map(struct game* game){
    struct map* map=game_get_current_map(game);
    struct monster **monster=game_get_monster_array(game);
          for ( int i = 0 ;i<map_get_width(map);i++){
              for( int j = 0 ;j<map_get_height(map);j++){
                  if (map_get_cell_type(map,i,j)==CELL_MONSTER){
                      monster[game_get_monster_number(game)]=monster_init(i,j);
                      game_inc_monster_number(game);
                    }
                  }
                }
}
