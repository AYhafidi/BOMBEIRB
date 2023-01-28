/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <bombs.h>
#include <constant.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

// Abstract data type
struct game;

// Create a new game
struct game *game_new();

// Free a game
void game_free(struct game *game);

// Return the player of the current game
struct player *game_get_player(struct game *game);

// Return the monster of the current game
struct monster *game_get_monster(struct game *game);

struct bomb *game_get_bombs(struct game *game);
//get all the monsters in the map

void monster_on_map(struct game* game);

//move all monsters in the map
void monsters_mouvement(struct game *game);

// Return the current map
struct map *game_get_current_map(struct game *game);

// Display the game on the screen
void game_display(struct game *game);

// update
int game_update(struct game *game);

// upload the world
void upload_world(struct game *game);

// upload the cartes of the world
void upload_maps(struct game *game, char *prefixe);
// change maps
void Change_map(struct game *game);
//change world
void Change_world(struct game* game);

// monsters array
struct monster ** game_get_monster_array(struct game* game);
//number of monsters in the game
short game_get_monster_number(struct game* game);
//increase number of monsters
void game_inc_monster_number(struct game* game);

#endif /* GAME_H_ */
