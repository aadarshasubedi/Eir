#pragma once

#include "eir_gfx_text.h"
#include "eir_gfx_text_info.h"
#include "../kernel/eir_ker_array_macro.h"

typedef struct
{
   eir_gfx_text_t * text;
   eir_gfx_text_info_t * text_info;
} eir_gfx_text_proxy_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_text_proxy_t, eir_gfx_text_proxy_array_t)
