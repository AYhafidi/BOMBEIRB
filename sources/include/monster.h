
#ifndef MONSTER_H_
#define MONSTER_H_

#include <bombs.h>
#include <constant.h>
//#include <game.h>
#include <map.h>
#include <misc.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

struct monster;
struct game;
// Creates a new monster
struct monster *monster_init();
void monster_free(struct monster *monster);

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y);

// Returns the current position of the monster
int monster_get_x(struct monster *monster);
int monster_get_y(struct monster *monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster *monster, enum direction direction);

 // Display the player on the screen
void monster_display(struct game* game);

// Move the player according to the current direction
int monster_move(struct monster *monster, struct map *map);

//move monster randomly
void monster_random_mouvement(struct monster *monster, struct map *map);

// Keep the monster in the same path
char Continue_same_path(struct monster *monster, struct map *map);
//get all the monsters in the map

void monster_on_map(struct game* game);

//move all monsters in the map
void monsters_mouvement(struct game *game);


#endif
