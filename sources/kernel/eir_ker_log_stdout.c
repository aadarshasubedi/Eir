#include <stdio.h>
#include <stdarg.h>
#include "eir_ker_log.h"

void eir_ker_log(const char * format, ...)
{
   va_list args;

   va_start(args, format);
   vfprintf(stdout, format, args);
   va_end(args);
}
