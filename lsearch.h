#include <stdlib.h>
#include <stdio.h>

unsigned seed = 1;
unsigned rand()
{
    return seed = (75*seed+74) % (1UL<<16 + 1);
}

#define numTiles 15
#define T_width 32
#define T_height 32

#define TILE_NONE 14

unsigned char T[T_width*(T_height+1)];
unsigned long D[numTiles] = {141,3138,8720,4384,141,141,141,3138,4384,8720,4384,8720,3138,3138,(1<<(numTiles))-1};
unsigned long R[numTiles] = {533,5378,8328,533,2144,532,2144,8328,532,5378,2144,5378,8328,5378,(1<<(numTiles))-1};
    

#define T_at(x,y) T[((y)+1)*T_width+(x)]
#define D_at(a,b) ((D[a]&(1<<(b))) != 0)
#define R_at(a,b) ((R[a]&(1<<(b))) != 0)

void lsearch()
{
    unsigned left, above, count, x,y,k;
    for (y = 0; y < T_height; y++) {
        left = TILE_NONE;
        for (x = 0; x < T_width; x++) {
            above = T_at(x,y-1);

            // First we count the number of possible tiles.
            count = 0;
            for (k = 0; k < numTiles; k++)
                count += D_at(above,k) && R_at(left,k);

            // Now we pick randomly from these tiles and count down until we
            // reach zero, at which point we have found the tile we chose.
            count = rand() % count;
            for (k = 0; k < numTiles; k++)
                if (D_at(above,k) && R_at(left,k))
                    if (count-- == 0)
                        break;
            
            left = T_at(x,y) = k;
        }
    }
}
