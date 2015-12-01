#include "eir.h"

int main()
{
   eir_env_t * env = 0;

   env = eir_create_env();
   eir_run(env);
   eir_destroy_env(env);
   return 0;
}
