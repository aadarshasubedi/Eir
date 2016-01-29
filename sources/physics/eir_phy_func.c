#include "eir_phy_func.h"

bool eir_phy_check_aabb_intersection(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b)
{
   bool result = false;

   if (a && b)
   {
      if (
            a->position.x  > (b->position.x + b->size.x)
            || a->position.y > (b->position.y + b->size.y)
            || (a->position.x + a->size.x) < b->position.x
            || (a->position.y + a->size.y) < b->position.y
         )
      {
         result = false;
      }
      else
      {
         result = true;
      }
   }
   return result;
}

float eir_phy_get_x_aabb_intersection_depth(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b)
{
   float result = 0.0f;

   if (a->position.x < b->position.x)
   {
      result = b->position.x - (a->position.x + a->size.x);
   }
   else
   {
      result = (b->position.x + b->size.x) - a->position.x;
   }
   return result;
}

float eir_phy_get_y_aabb_intersection_depth(const eir_phy_aabb_t * a, const eir_phy_aabb_t * b)
{
   float result = 0.0f;

   if (a->position.y < b->position.y)
   {
      result = -((a->position.y + a->size.y) - b->position.y);
   }
   else
   {
      result = (b->position.y + b->size.y) - a->position.y;
   }
   return result;
}
