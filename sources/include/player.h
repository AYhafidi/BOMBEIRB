/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <bombs.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <sprite.h>
#include <window.h>

struct player;

// Creates a new player with a given number of available bombs
struct player *player_init();
void player_free(struct player *player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player *player);
int player_get_y(struct player *player);

// Set the direction of the next move of the player
void player_set_current_way(struct player *player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int player_get_nb_bomb(struct player *player);
void player_inc_nb_bomb(struct player *player);
void player_dec_nb_bomb(struct player *player);

// Set, Increase, Decrease the number of life
int player_get_nb_life(struct player *player);
void player_inc_life(struct player *player);
void player_dec_life(struct player *player);

// Set, Increase, Decrease the range
int player_get_nb_range(struct player *player);
void player_inc_range(struct player *player);
void player_dec_range(struct player *player);

// Set, Increase, Decrease the number of keys
int player_get_nb_key(struct player *player);
void player_inc_nb_key(struct player *player);
void player_dec_nb_key(struct player *player);

// Move the player according to the current direction
int player_move(struct player *player, struct map *map);

// Display the player on the screen
void player_display(struct player *player);

int Move_monster_random(struct map *map, int x, int y);

#endif /* PLAYER_H_ */
