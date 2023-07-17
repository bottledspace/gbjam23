#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

unsigned seed = 1;
unsigned rand()
{
    return seed = (75*seed+74) % (1UL<<16 + 1);
}

#define T_width 32
#define T_height 32

#define numTiles 15
#define TILE_NONE 14
#define T_at(x,y) (T[((uint32_t)T_width)*(y)+(x)])
#define T_set(x,y,val) (T[((uint32_t)T_width)*(y)+(x)]=val)

uint8_t T[T_width*T_height];
uint16_t D[numTiles] = {141,3138,8720,4384,141,141,141,3138,4384,8720,4384,8720,3138,3138,(1<<TILE_NONE)-1};
uint16_t R[numTiles] = {533,5378,8328,533,2144,533,2144,8328,532,5378,2144,5378,8328,5378,(1<<TILE_NONE)-1};
uint16_t U[numTiles] = {113,12418,113,113,2564,1288,12418,113,1288,2564,12418,12418,1288,2564,(1<<TILE_NONE)-1};
uint16_t L[numTiles] = {41,10754,297,4228,297,1104,1104,4228,10754,297,10754,1104,10754,4228,(1<<TILE_NONE)-1};

uint8_t rolls[numTiles-1][100] = {
    {0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 2, 2, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 2, 1, 0, 2, 2, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 1, 1, 0, 0, 2, 0, 1, 0, 2, 2, 2, 1, 0, 1, 0, 2, 2, 1, 0, 0, 0, 2, 0, 0, 2, 2, 0, 1, 0, 0, 2, 0, 2, 1, 1, 2, 1, 2, 2, 0, 2, 2, 2, 1, 0, 1, 0, 2, 1, 1, 2, 0, 0, 2},
    {2, 3, 2, 0, 1, 2, 3, 3, 0, 2, 0, 2, 2, 0, 3, 3, 0, 1, 1, 0, 3, 0, 0, 2, 3, 1, 1, 3, 1, 0, 3, 3, 3, 1, 3, 0, 1, 0, 1, 0, 2, 3, 3, 2, 0, 0, 2, 1, 3, 2, 3, 0, 0, 0, 0, 0, 2, 2, 1, 0, 3, 3, 2, 3, 1, 0, 1, 1, 0, 2, 2, 2, 3, 1, 0, 3, 2, 3, 3, 1, 3, 2, 1, 3, 1, 3, 3, 0, 2, 2, 0, 0, 3, 1, 1, 0, 1, 2, 2, 1},
    {3, 2, 2, 0, 0, 2, 1, 4, 1, 3, 1, 2, 4, 2, 3, 4, 2, 0, 0, 0, 1, 3, 0, 1, 2, 0, 1, 3, 2, 2, 3, 1, 3, 4, 1, 0, 0, 3, 2, 2, 1, 1, 1, 3, 0, 4, 1, 2, 1, 1, 0, 4, 1, 2, 4, 1, 0, 0, 3, 3, 0, 4, 3, 2, 2, 3, 3, 4, 1, 1, 4, 2, 4, 4, 1, 0, 0, 0, 4, 2, 4, 4, 4, 2, 0, 2, 3, 4, 3, 1, 2, 2, 4, 1, 0, 3, 2, 3, 1, 0},
    {1, 5, 3, 5, 5, 0, 0, 2, 3, 2, 4, 4, 3, 2, 2, 1, 3, 3, 3, 2, 5, 1, 1, 0, 5, 2, 5, 2, 5, 5, 4, 5, 0, 0, 5, 1, 4, 4, 3, 1, 5, 3, 0, 0, 2, 5, 1, 0, 5, 2, 4, 1, 2, 3, 1, 5, 2, 0, 3, 0, 0, 2, 0, 1, 3, 5, 5, 0, 1, 1, 0, 1, 4, 5, 5, 5, 1, 0, 2, 0, 3, 5, 3, 2, 3, 1, 4, 0, 3, 3, 5, 1, 3, 3, 5, 0, 5, 4, 4, 1},
    {3, 0, 6, 0, 0, 4, 2, 4, 6, 3, 5, 2, 4, 5, 1, 0, 5, 5, 1, 6, 5, 6, 4, 0, 6, 2, 1, 4, 6, 3, 0, 3, 4, 5, 0, 0, 2, 2, 4, 3, 0, 5, 0, 5, 0, 1, 2, 2, 1, 5, 6, 2, 0, 3, 2, 1, 4, 1, 3, 4, 4, 2, 5, 4, 1, 6, 4, 2, 5, 0, 4, 1, 1, 6, 3, 3, 4, 1, 5, 1, 4, 2, 6, 1, 3, 6, 4, 3, 1, 5, 3, 2, 2, 3, 2, 0, 4, 5, 0, 3},
    {1, 6, 2, 2, 3, 5, 4, 5, 2, 2, 6, 2, 2, 6, 5, 7, 6, 5, 6, 4, 7, 2, 5, 4, 4, 1, 3, 6, 7, 4, 1, 0, 4, 6, 7, 6, 4, 5, 5, 3, 4, 5, 6, 6, 3, 0, 6, 1, 1, 1, 3, 6, 1, 1, 1, 7, 1, 1, 2, 4, 3, 7, 4, 3, 7, 1, 1, 7, 7, 5, 6, 0, 5, 4, 1, 2, 0, 7, 4, 5, 1, 6, 4, 3, 6, 0, 3, 2, 3, 3, 2, 0, 5, 4, 1, 2, 1, 2, 3, 6},
    {6, 5, 3, 6, 4, 3, 3, 0, 5, 3, 3, 5, 0, 6, 3, 7, 7, 3, 0, 0, 5, 4, 5, 6, 4, 6, 3, 0, 1, 6, 7, 3, 1, 7, 6, 2, 4, 7, 5, 2, 3, 8, 2, 7, 0, 5, 6, 8, 6, 4, 6, 3, 5, 2, 4, 6, 0, 8, 0, 3, 3, 0, 5, 2, 0, 8, 4, 1, 4, 5, 0, 6, 5, 1, 8, 3, 2, 3, 5, 6, 5, 0, 3, 4, 7, 4, 4, 7, 3, 1, 8, 2, 1, 6, 5, 7, 7, 0, 0, 7},
    {3, 3, 7, 8, 2, 5, 8, 4, 8, 2, 5, 4, 9, 9, 5, 0, 7, 6, 0, 0, 5, 4, 8, 7, 4, 6, 6, 3, 1, 0, 8, 9, 8, 3, 3, 4, 0, 4, 5, 4, 1, 6, 3, 7, 6, 6, 9, 9, 7, 7, 8, 8, 9, 4, 6, 0, 7, 0, 2, 5, 4, 3, 8, 0, 3, 9, 3, 5, 4, 8, 0, 3, 0, 9, 9, 2, 6, 6, 7, 9, 5, 5, 7, 6, 9, 2, 3, 8, 8, 1, 9, 3, 9, 8, 8, 1, 0, 7, 7, 5},
    {2, 5, 3, 6, 5, 6, 9, 8, 10, 8, 0, 2, 4, 1, 9, 4, 0, 5, 8, 8, 3, 6, 10, 1, 7, 0, 1, 6, 1, 2, 2, 10, 0, 8, 10, 3, 5, 5, 5, 9, 3, 7, 5, 2, 5, 3, 7, 8, 1, 3, 5, 0, 6, 5, 1, 0, 1, 3, 0, 7, 1, 2, 6, 9, 8, 3, 0, 8, 1, 1, 4, 5, 5, 6, 0, 9, 7, 10, 3, 9, 3, 10, 1, 3, 2, 2, 0, 5, 1, 4, 8, 6, 9, 7, 5, 7, 7, 4, 3, 0},
    {2, 2, 4, 7, 6, 1, 1, 5, 4, 10, 6, 9, 4, 11, 1, 2, 11, 5, 9, 11, 5, 6, 11, 7, 9, 8, 10, 5, 0, 7, 4, 3, 10, 7, 11, 7, 4, 5, 9, 5, 9, 7, 2, 8, 2, 9, 1, 10, 6, 11, 10, 9, 11, 9, 8, 5, 1, 3, 7, 11, 0, 4, 4, 1, 1, 7, 8, 0, 4, 9, 9, 8, 1, 4, 6, 6, 8, 3, 6, 3, 3, 7, 8, 2, 10, 9, 9, 5, 11, 0, 8, 9, 10, 6, 7, 0, 1, 2, 7, 8},
    {1, 5, 6, 6, 4, 3, 4, 0, 8, 9, 7, 11, 5, 4, 8, 11, 12, 6, 6, 11, 5, 8, 11, 11, 10, 2, 6, 5, 6, 6, 11, 6, 10, 10, 4, 6, 4, 4, 7, 4, 0, 5, 10, 7, 9, 5, 12, 11, 8, 0, 2, 11, 7, 11, 11, 0, 10, 9, 4, 2, 0, 11, 6, 10, 1, 2, 8, 11, 2, 6, 5, 6, 1, 5, 9, 0, 0, 5, 8, 8, 4, 10, 10, 1, 8, 0, 10, 0, 4, 12, 8, 8, 0, 3, 11, 0, 8, 0, 10, 5},
    {9, 2, 2, 13, 5, 2, 13, 5, 11, 1, 11, 0, 6, 4, 8, 12, 7, 8, 3, 13, 2, 9, 2, 12, 2, 6, 4, 10, 2, 12, 2, 2, 8, 10, 6, 13, 8, 0, 4, 11, 3, 7, 7, 5, 1, 6, 10, 6, 0, 1, 6, 1, 7, 4, 3, 2, 7, 11, 1, 7, 11, 2, 2, 8, 8, 7, 3, 7, 4, 8, 9, 1, 2, 2, 13, 1, 10, 13, 8, 12, 6, 13, 3, 13, 5, 5, 12, 10, 1, 8, 0, 13, 13, 2, 2, 9, 9, 2, 7, 6},
    {9, 5, 4, 6, 14, 3, 14, 11, 7, 10, 4, 12, 1, 6, 1, 0, 12, 2, 13, 12, 12, 4, 8, 5, 14, 9, 7, 5, 10, 8, 4, 8, 9, 13, 11, 9, 3, 11, 9, 1, 0, 7, 1, 12, 0, 5, 4, 10, 8, 10, 7, 0, 10, 8, 7, 12, 9, 1, 11, 8, 8, 10, 1, 6, 10, 4, 6, 1, 7, 5, 3, 11, 7, 4, 0, 7, 2, 8, 0, 14, 3, 6, 10, 3, 2, 5, 10, 9, 1, 2, 13, 6, 6, 14, 2, 9, 2, 9, 1, 4}
};

const uint8_t BitsSetTable256[256] = 
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

static const int Mod37BitPosition[] = {
    32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4,
    7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5,
    20, 8, 19, 18
};

void lsearch(uint8_t x, uint8_t y)
{
    uint8_t above = T_at(x,(y+31)%32);
    uint8_t left = T_at((x+31)%32,y);

    uint16_t possible = D[above] & R[left];
    int8_t count = BitsSetTable256[(possible>>8) & 0xff]
                 + BitsSetTable256[possible & 0xff];
    if (count < 2)
        count = 0;
    else
        count = rolls[count-2][++seed%100];

    while (count) {
        possible &= possible-1;
        count--;
    }
    T_set(x,y, Mod37BitPosition[(-possible & possible) % 37]);
}

void lsearch2(int8_t x, int8_t y)
{
    uint8_t below = T_at(x,(y+1)%32);
    uint8_t right = T_at((x+1)%32,y);

    uint16_t possible = U[below] & L[right];
    uint8_t count = BitsSetTable256[(possible>>8)&0xff]
                  + BitsSetTable256[possible&0xff];

    if (count < 2)
        count = 0;
    else
        count = rolls[count-2][++seed%100];

    while (count) {
        possible &= possible-1;
        count--;
    }
    T_set(x,y, Mod37BitPosition[(-possible & possible) % 37]);
}
