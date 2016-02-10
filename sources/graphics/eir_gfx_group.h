#pragma once

#include "eir_sprite_batch.h"
#include "eir_rect_batch.h"
#include "../kernel/eir_array_macro.h"

typedef struct
{
	eir_sprite_batch_array_t text_batches;
	eir_sprite_batch_array_t sprite_batches;
	eir_rect_batch_t rect_batch;
	bool visible;
} eir_gfx_group_t;

EIR_KER_DEFINE_ARRAY_STRUCT(eir_gfx_group_t, eir_gfx_group_array_t)