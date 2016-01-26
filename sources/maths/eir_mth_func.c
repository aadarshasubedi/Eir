#include "eir_mth_func.h"

float eir_mth_square_f(float value)
{
   float result = value * value;
   return result;
}

double eir_mth_square_d(double value)
{
   double result = value * value;
   return result;
}

float eir_mth_abs(float value)
{
   float result = value < 0.0f ? -value : value;
   return result;
}

float eir_mth_min(float a, float b)
{
   float result = a < b ? a : b;
   return result;
}

float eir_mth_max(float a, float b)
{
   float result = a > b ? a : b;
   return result;
}

void eir_mth_set_vec2(eir_mth_vec2_t * v, float x, float y)
{
   if (v)
   {
      v->x = x;
      v->y = y;
   }
}

void eir_mth_set_vec3(eir_mth_vec3_t * v, float x, float y, float z)
{
   if (v)
   {
      v->x = x;
      v->y = y;
      v->z = z;
   }
}

void eir_mth_set_vec4(eir_mth_vec4_t * v, float x, float y, float z, float w)
{
   if (v)
   {
      v->x = x;
      v->y = y;
      v->z = z;
      v->w = w;
   }
}

void eir_mth_scale_vec4(eir_mth_vec4_t * v, float s)
{
   if (v)
   {
      v->x *= s;
      v->y *= s;
      v->z *= s;
      v->w *= s;
   }
}

void eir_mth_add_vec4(eir_mth_vec4_t * a, const eir_mth_vec4_t * b)
{
   if (a && b)
   {
      a->x += b->x;
      a->y += b->y;
      a->z += b->z;
      a->w += b->w;
   }
}

void eir_mth_set_identity_mat4(eir_mth_mat4_t * m)
{
   if (m)
   {
      m->values[0][0] = 1.0f;
      m->values[0][1] = 0.0f;
      m->values[0][2] = 0.0f;
      m->values[0][3] = 0.0f;
      m->values[1][0] = 0.0f;
      m->values[1][1] = 1.0f;
      m->values[1][2] = 0.0f;
      m->values[1][3] = 0.0f;
      m->values[2][0] = 0.0f;
      m->values[2][1] = 0.0f;
      m->values[2][2] = 1.0f;
      m->values[2][3] = 0.0f;
      m->values[3][0] = 0.0f;
      m->values[3][1] = 0.0f;
      m->values[3][2] = 0.0f;
      m->values[3][3] = 1.0f;
   }
}

void eir_mth_set_ortho_mat4(eir_mth_mat4_t * m, float left, float right, float bottom, float top)
{
   if (m)
   {
      m->values[0][0] = 2.0f / (right - left);
      m->values[0][1] = 0.0f;
      m->values[0][2] = 0.0f;
      m->values[0][3] = 0.0f;

      m->values[1][0] = 0.0f;
      m->values[1][1] = 2.0f / (top - bottom);
      m->values[1][2] = 0.0f;
      m->values[1][3] = 0.0f;

      m->values[2][0] = 0.0f;
      m->values[2][1] = 0.0f;
      m->values[2][2] = -1.0f;
      m->values[2][3] = 0.0f;

      m->values[3][0] = - (right + left) / (right - left);
      m->values[3][1] = - (top + bottom) / (top - bottom);
      m->values[3][2] = 0.0f;
      m->values[3][3] = 1.0f;
   }
}

void eir_mth_set_translation_mat4(eir_mth_mat4_t * m, const eir_mth_vec3_t * v)
{
   if (m && v)
   {
      eir_mth_vec4_t a;
      eir_mth_vec4_t b;
      eir_mth_vec4_t c;
      eir_mth_vec4_t d;

      eir_mth_set_vec4(&a, 1.0f, 0.0f, 0.0f, 0.0f);
      eir_mth_set_vec4(&b, 0.0f, 1.0f, 0.0f, 0.0f);
      eir_mth_set_vec4(&c, 0.0f, 0.0f, 1.0f, 0.0f);
      eir_mth_set_vec4(&d, 0.0f, 0.0f, 0.0f, 1.0f);
      eir_mth_scale_vec4(&a, v->x);
      eir_mth_scale_vec4(&b, v->y);
      eir_mth_scale_vec4(&c, v->z);
      eir_mth_add_vec4(&a, &b);
      eir_mth_add_vec4(&a, &c);
      eir_mth_add_vec4(&a, &d);

      eir_mth_set_identity_mat4(m);
      m->values[3][0] = a.x;
      m->values[3][1] = a.y;
      m->values[3][2] = a.z;
      m->values[3][3] = a.w;
   }
}
