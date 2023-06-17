#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bgtiles.h"

unsigned char plane0[20*18];

void main(void)
{
    DISPLAY_ON;
    SHOW_BKG;

    memset(plane0, 2, 20*18);
    unsigned r = 1;
    for (int i = 0; i < 20*18; i++) {
        r = (75*r+74) % (1UL<<16 + 1);
        plane0[i] = r%5;
    }

    /*set_bkg_data(0,178,LaroldsJubilantJunkyard_data);
    set_bkg_tiles(0,0,20,18,LaroldsJubilantJunkyard_map_plane0);
    set_bkg_palette(0,1,LaroldsJubilantJunkyard_pallette);
    VBK_REG=1;
    set_bkg_tiles(0,0,20,18,LaroldsJubilantJunkyard_map_plane1);
    VBK_REG=0;*/
    
    set_bkg_data(0,bgtiles_tile_count,bgtiles_tile_data);
    set_bkg_tiles(0,0,20,18,plane0);


}
