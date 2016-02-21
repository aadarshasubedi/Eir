#pragma once

#include "eir_gfx_rect.h"
#include "eir_gfx_rect_info.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gfx_rect_t * rect;
   eir_gfx_rect_info_t * rect_info;
} eir_gfx_rect_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_rect_proxy_t, eir_gfx_rect_proxy_array_t)
