#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bgtiles.h"
#include "lsearch.h"

unsigned char cam_prevx = 0;
unsigned char cam_prevy = 0;
unsigned char cam_x = 0;
unsigned char cam_y = 0;

void scroll(char dx, char dy)
{
    cam_prevx = cam_x;
    cam_prevy = cam_y;
    cam_x += dx;
    cam_y += dy;

    if ((cam_prevx>>3) < (cam_x>>3)) {
        unsigned ofs = (21+(cam_x>>3))%32;
        for (unsigned i = 0; i < 32; i++) {
            T[i*32+ofs] = rand()%5;
        }
        set_bkg_submap(ofs,0,1,32,T,32);
    } else if ((cam_prevx>>3) > (cam_x>>3)) {
        unsigned ofs = (31+(cam_x>>3))%32;
        for (unsigned i = 0; i < 32; i++) {
            T[i*32+ofs] = rand()%5;
        }
        set_bkg_submap(ofs,0,1,32,T,32);
    }

    if ((cam_prevy>>3) < (cam_y>>3)) {
        unsigned ofs = (19+(cam_y>>3))%32;
        for (unsigned i = 0; i < 32; i++) {
            T[ofs*32+i] = rand()%5;
        }
        set_bkg_submap(0,ofs,32,1,T,32);
    } else if ((cam_prevy>>3) > (cam_y>>3)) {
        unsigned ofs = (31+(cam_y>>3))%32;
        for (unsigned i = 0; i < 32; i++) {
            T[ofs*32+i] = rand()%5;
        }
        set_bkg_submap(0,ofs,32,1,T,32);
    }
    move_bkg(cam_x,cam_y);
}

void main(void)
{
    unsigned i;
    for (i = 0; i < T_width; i++)
        T_at(i,-1) = TILE_NONE;

    lsearch();

    DISPLAY_ON;
    SHOW_BKG;

    set_bkg_data(0,bgtiles_tile_count,bgtiles_tile_data);
    set_bkg_tiles(0,0,32,32,T);
    for (;;) {
        scroll(1,1);
        wait_vbl_done();
    }
}
