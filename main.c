#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bgtiles.h"

unsigned char plane0[20*18];
unsigned char shim[32];

unsigned seed = 1;
unsigned rand()
{
    return seed = (75*seed+74) % (1UL<<16 + 1);
}

void main(void)
{
    DISPLAY_ON;
    SHOW_BKG;

    memset(plane0, 2, 20*18);
    for (int i = 0; i < 20*18; i++) {
        plane0[i] = rand()%5;
    }
    memset(shim, 0, 32);
    
    set_bkg_data(0,bgtiles_tile_count,bgtiles_tile_data);
    set_bkg_tiles(0,0,20,18,plane0);

    unsigned old_cam_x = 0;
    unsigned cam_x = 0;
    for (;;) {
        move_bkg(cam_x,0);
        old_cam_x = cam_x;
        cam_x++;

        wait_vbl_done();
        if ((old_cam_x>>3) < (cam_x>>3)) {
            for (int i = 0; i < 32; i++) {
                shim[i] = rand()%5;
            }
            int ofs = 20+(cam_x>>3)%32;
            set_bkg_submap(ofs,0,1,20,shim-ofs,1);
        }
    }
}
