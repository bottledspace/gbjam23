#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bgtiles.h"
#include "lsearch.h"

const uint8_t tileset[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x07, 0x00, 0x0F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x00, 0xE0, 0x00, 0xF0, 0x00,
    0x0F, 0x00, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xF0, 0x00, 0xE0, 0x00, 0xC0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00,
    0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFE, 0x00, 0xFC, 0x00, 0xF8, 0x00, 0xF0, 0x00,
    0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x7F, 0x00, 0x3F, 0x00, 0x1F, 0x00, 0x0F, 0x00,
    0xF0, 0x00, 0xF8, 0x00, 0xFC, 0x00, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0x0F, 0x00, 0x1F, 0x00, 0x3F, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
};


int8_t cam_x = 0;
int8_t cam_y = 0;

void scroll(int8_t dx, int8_t dy)
{
    cam_x = (32+dx+cam_x)%32;
    cam_y = (32+dy+cam_y)%32;
    move_bkg(8ul*cam_x,8ul*cam_y);

    if (dx > 0) {
        int8_t x = (19+cam_x)%32;
        for (int8_t y = 0; y < 18; y++) {
            lsearch(x,(cam_y+y)%32);
        }
        wait_vbl_done();
        set_bkg_submap(x,0,1,32,T,32);
    } else if (dx < 0) {
        T_set(cam_x,(cam_y+18)%32,TILE_NONE);
        int8_t x = cam_x;
        for (int8_t y = 17; y >= 0; y--) {
            lsearch2(x,(cam_y+y)%32);
        }
        wait_vbl_done();
        set_bkg_submap(x,0,1,32,T,32);
    }
 
    if (dy > 0) {
        int8_t y = (17+cam_y)%32;
        for (int8_t x = 0; x < 20; x++) {
            lsearch((cam_x+x)%32,y);
        }
        wait_vbl_done();
        set_bkg_submap(0,y,32,1,T,32);
    } else if (dy < 0) {
        T_set((cam_x+20)%32,cam_y,TILE_NONE);
        int8_t y = cam_y;
        for (int8_t x = 19; x >= 0; x--) {
            lsearch2((cam_x+x)%32,y);
        }
        wait_vbl_done();
        set_bkg_submap(0,y,32,1,T,32);
    }
}

void main(void)
{
    memset(T, TILE_NONE, T_width*T_height);

    DISPLAY_ON;
    SHOW_BKG;

    set_bkg_data(0,numTiles-1,tileset);
    set_bkg_tiles(0,0,T_width,T_height,&T_at(0,0));
    for (;;) {
        scroll(-1,-1);
        for(int i = 0; i < 100; i++)
            wait_vbl_done();
    }
}
