#ifndef BOMBS_H_
#define BOMBS_H_

#include <constant.h>
#include <game.h>
#include <map.h>
#include <misc.h>
#include <monster.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

struct bomb;

struct bomb *bomb_init(int x, int y, int timer, int range, struct bomb *old_bomb);

void bomb_display(struct game *game);

void bomb_explosion(struct game *game);

#endif
