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
