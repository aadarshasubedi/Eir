#pragma once

#include "eir_gme_map_component.h"

/* ============================================================================
** struct to manage a colliding tile
** ============================================================================
*/
typedef struct
{
   eir_gme_map_tile_t * map_tile_ptr;
   float intersection_area;
} colliding_map_tile_t;

/* ============================================================================
** struct to manage all colliding tiles for a given entity
** ============================================================================
*/
typedef struct
{
   colliding_map_tile_t * data;
   size_t max_capacity;
   size_t used;
} colliding_map_tile_array_t;

/* ============================================================================
** array of colliding tiles used by the game world and systems
** ============================================================================
*/
typedef struct
{
   colliding_map_tile_array_t * data;
   size_t max_capacity;
   size_t used;
} colliding_map_tile_array_array_t;
