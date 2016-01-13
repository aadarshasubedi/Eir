#pragma once

#include "eir_vector.h"
#include "eir_matrix.h"

float eir_mth_square_f(float value);
double eir_mth_square_d(double value);
float eir_mth_abs(float value);
void eir_mth_set_vec2(eir_mth_vec2_t * v, float x, float y);
void eir_mth_set_vec4(eir_mth_vec4_t * v, float x, float y, float z, float w);
void eir_mth_scale_vec4(eir_mth_vec4_t * v, float s);
void eir_mth_add_vec4(eir_mth_vec4_t * a, const eir_mth_vec4_t * b);
void eir_mth_set_identity_mat4(eir_mth_mat4_t * m);
void eir_mth_set_ortho_mat4(eir_mth_mat4_t * m, float left, float right, float bottom, float top);
void eir_mth_set_translation_mat4(eir_mth_mat4_t * m, const eir_mth_vec3_t * v);
