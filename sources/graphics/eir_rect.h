#pragma once

#include "eir_vertex.h"
#include "../kernel/eir_array_macro.h"

#define EIR_GFX_TOTAL_RECT_VERTEX_COUNT 8

typedef struct
{
   eir_gfx_vertex_t vertices[EIR_GFX_TOTAL_RECT_VERTEX_COUNT];
} eir_gfx_rect_t;
