#pragma once

#include "colliding_map_tile.h"

void init_colliding_map_tile(colliding_map_tile_t * colliding_map_tile);
void init_colliding_map_tile_array(colliding_map_tile_array_t * array);
void alloc_colliding_map_tile_array(colliding_map_tile_array_t * array, size_t max_capacity);
void swap_colliding_map_tile(colliding_map_tile_array_t * array, size_t tile_id_1, size_t tile_id_2);
void use_colliding_map_tile(colliding_map_tile_array_t * array, size_t tile_id);
void unuse_colliding_map_tile(colliding_map_tile_array_t * array, size_t tile_id);
int push_colliding_map_tile(colliding_map_tile_array_t * array, eir_gme_map_tile_t * map_tile, float intersection_area);
void sort_colliding_map_tile_array(colliding_map_tile_array_t * array);
void empty_colliding_map_tile_array(colliding_map_tile_array_t * array);
void free_colliding_map_tile_array(colliding_map_tile_array_t * arrays);
void init_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array);
void alloc_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array, size_t max_capacity);
colliding_map_tile_array_t * get_unused_colliding_map_tile_array(colliding_map_tile_array_array_t * array);
void free_colliding_map_tile_array_array(colliding_map_tile_array_array_t * array);
