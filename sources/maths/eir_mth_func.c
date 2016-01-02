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
