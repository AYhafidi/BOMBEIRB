#include <SDL/SDL_image.h>
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

struct bomb
{
    int x, y;
    int timer;
    int range;
    struct bomb *prec;
    struct bomb *next;
};

/*-------ajoute une bombe à la liste chainée-----------------------------------*/
struct bomb *bomb_init(int x, int y, int timer, int range, struct bomb *old_bomb)
{
    struct bomb *bomb = malloc(sizeof(*bomb));
    if (!bomb)
        error("Memory error");

    bomb->x = x;
    bomb->y = y;
    bomb->timer = timer;
    bomb->range = range;
    bomb->next = old_bomb;

    return bomb;
}

/*------lance le processus d'explosion---------------------------------------*/
void bomb_display(struct game *game)
{
    struct bomb *bomb = game_get_bombs(game);
    if (bomb != NULL)
    {
        int x = bomb->x ;
        int y = bomb->y ;
        while (bomb != NULL)
        {
            if ((SDL_GetTicks() - bomb->timer) > 0 && (SDL_GetTicks() - bomb->timer) < 1000)
            {
                window_display_image(sprite_get_bomb4(), x, y);
            }
            else if ((SDL_GetTicks() - bomb->timer) > 1000 && (SDL_GetTicks() - bomb->timer) < 2000)
            {
                window_display_image(sprite_get_bomb3(), x, y);
            }
            else if ((SDL_GetTicks() - bomb->timer) > 2000 && (SDL_GetTicks() - bomb->timer) < 3000)
            {
                window_display_image(sprite_get_bomb2(), x, y);
            }
            else if ((SDL_GetTicks() - bomb->timer) > 3000 && (SDL_GetTicks() - bomb->timer) < 4000)
            {
                window_display_image(sprite_get_bomb1(), x, y);
            }
            else if ((SDL_GetTicks() - bomb->timer) > 4000 && (SDL_GetTicks() - bomb->timer) < 5000)
            {
                bomb_explosion(game);
            }
            else
            {
                map_set_cell_type(game_get_current_map(game), x, y, CELL_EMPTY);
                free(bomb);
            }
            bomb = bomb->next;
        }
    }
}

/*--------traite la situation pour adapter l'explosion-------------*/
void bomb_explosion(struct game *game)
{
    struct bomb *bomb = game_get_bombs(game);
    struct map *map = game_get_current_map(game);
    struct player *player = game_get_player(game);

    int x = bomb->x * SIZE_BLOC;
    int y = bomb->y * SIZE_BLOC;
    int range = player_get_nb_range(player) * SIZE_BLOC;

    window_display_image(sprite_get_explosion(), x, y);


/*------traitement des cas non fonctionnel :(-------------------------------
    for (int i = 1; i <= range; i++)
    {
        i = i - 1 + SIZE_BLOC;

        unsigned char type = map_get_cell_type(map, x + i, y);
        switch (type & 0xf0)
        {
        case CELL_BOX:
            map_set_cell_type(game_get_current_map(game), x + i, y, CELL_EMPTY);
            window_display_image(sprite_get_explosion(), x + i, y);
            break;
        case CELL_EMPTY:
            window_display_image(sprite_get_explosion(), x + i, y);
        default:
            break;
        }

        type = map_get_cell_type(game_get_current_map(game), x - i, y);
        switch (type & 0xf0)
        {
        case CELL_BOX:
            map_set_cell_type(game_get_current_map(game), x - i, y, CELL_EMPTY);
            window_display_image(sprite_get_explosion(), x - i, y);
            break;
        case CELL_EMPTY:
            window_display_image(sprite_get_explosion(), x - i, y);
        default:
            break;
        }

        type = map_get_cell_type(game_get_current_map(game), x, y + i);
        switch (type & 0xf0)
        {
        case CELL_BOX:
            map_set_cell_type(game_get_current_map(game), x, y + i, CELL_EMPTY);
            window_display_image(sprite_get_explosion(), x, y + i);
            break;
        case CELL_EMPTY:
            window_display_image(sprite_get_explosion(), x, y + i);
        default:
            break;
        }

        type = map_get_cell_type(game_get_current_map(game), x + i, y);
        switch (type & 0xf0)
        {
        case CELL_BOX:
            map_set_cell_type(game_get_current_map(game), x, y - i, CELL_EMPTY);
            window_display_image(sprite_get_explosion(), x, y - i);
            break;
        case CELL_EMPTY:
            window_display_image(sprite_get_explosion(), x, y - i);
        default:
            break;
        }
    }*/

    for (int i = 1; i <= range; i++)
    {
        i=i-1+SIZE_BLOC;
        window_display_image(sprite_get_explosion(), x + i, y);
        window_display_image(sprite_get_explosion(), x - i, y);
        window_display_image(sprite_get_explosion(), x, y + i);
        window_display_image(sprite_get_explosion(), x, y - i);
    }
}
