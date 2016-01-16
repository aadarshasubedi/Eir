#include "eir_gfx_types.h"
#include "eir_rect.h"

typedef struct
{
   eir_gfx_api_vao_handle_t vao;
   eir_gfx_api_vbo_handle_t vbo;
   eir_gfx_api_program_handle_t program;
   eir_gfx_rect_array_t rects;
   bool built;
   bool modified;
} eir_gfx_rect_batch_t;
