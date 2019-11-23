#include "configs.h"
#include "cli.h"

/* pid maps: k_p, k_i, k_d */
int16_t pid_maps[NUMBER_OF_MAPPINGS * 3] = {
    250, 0, 300, // 400, 0, 500
    250, 0, 300, 250, 0, 300};

int16_t vel_maps[NUMBER_OF_MAPPINGS] = {
    // 650, 675, 725
    // 600, 650, 700 // mapping 1st test
    // 700, 750, 780 // mapping 2nd test
    // 650, 725, 800 // mapping vel normal
    //775, 800, 825 // for testing purposes
    //500, 600, 700 
  //300, 400, 425
  //300, 400, 450 // Record circuito (aguanta pero le cuesta)
  //300, 400, 425
  300, 350, 400 
};

int16_t vel_recovery_maps[NUMBER_OF_MAPPINGS] = {
  200, 200, 200
};
  

// Best mapping 600/525; 650/525/35 (1 vuelta -morro corto), 650/515/25 (morro
// corto stripped)
int16_t pid_nool_maps[NUMBER_OF_MAPPINGS * 3] = {
    400, 0, 600, 400, 0, 600, 400, 0, 600};

int16_t vel_nool_maps[NUMBER_OF_MAPPINGS] = {
    // 350, 350, 350 // mapping vel nool
    150, 150, 150 // testing
};

int16_t pid_turbo_maps[NUMBER_OF_MAPPINGS * 3] = {
    200, 0, 300, // 350, 0, 600
    200, 0, 300, 200, 0, 300};

int16_t vel_turbo_maps[NUMBER_OF_MAPPINGS] = {
    // 515, 515, 515
    // 575, 600, 625 //mapping 1st test
    // 625, 650, 675 //mapping 2nd test
    // 600, 650, 700 // mapping vel turbo
    // 675, 725, 775 // testing turbo
    //800, 900, 950
    //300, 375, 400
  //300, 400, 425
  //300, 375, 450 // Record circuito (aguanta pero le cuesta)
  275, 300, 325
};

int16_t vel_hyper_turbo_maps[NUMBER_OF_MAPPINGS] = {
  //650, 650, 650
  //550, 675, 700
  375, 400, 500
};

int16_t vel_hyper_turbo_corner_maps[NUMBER_OF_MAPPINGS] = {
  //650, 650, 650
  //400, 525, 550
  300, 425, 550
};


int16_t vel_brake_maps[NUMBER_OF_MAPPINGS] = {
    // 515, 515, 515
    // 575, 600, 625 //mapping 1st test
    // 625, 650, 675 //mapping 2nd test
    //550, 600, 650 // mapping vel brake
    600, 650, 700
    //300, 400, 500 // testing turbo
};

const uint8_t map_songs[NUMBER_OF_MAPPINGS] = {
    SONG_ONE_BEAT_ORDER, SONG_TWO_BEAT_ORDER, SONG_THREE_BEAT_ORDER
};


void config_vars(void)
{
    /* pid maps: k_p, k_i, k_d */
    add_config_var(pid_maps, 1, INT16_T, NUMBER_OF_MAPPINGS * 3);
    add_config_var(vel_maps, 2, INT16_T, NUMBER_OF_MAPPINGS);

    add_config_var(pid_nool_maps, 3, INT16_T, NUMBER_OF_MAPPINGS * 3);
    add_config_var(vel_nool_maps, 4, INT16_T, NUMBER_OF_MAPPINGS);

    add_config_var(pid_turbo_maps, 5, INT16_T, NUMBER_OF_MAPPINGS * 3);
    add_config_var(vel_turbo_maps, 6, INT16_T, NUMBER_OF_MAPPINGS);

    add_config_var(vel_brake_maps, 7, INT16_T, NUMBER_OF_MAPPINGS * 3);
}
