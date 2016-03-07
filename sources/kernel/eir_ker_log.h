#pragma once

void eir_ker_log(const char * format, ...);

#ifdef EIR_DEBUG
#define EIR_KER_LOG_MESSAGE(...)				\
   eir_ker_log("> (%s:%d) ", __func__, __LINE__);		\
   eir_ker_log(__VA_ARGS__);					\
   eir_ker_log("\n");
#define EIR_KER_LOG_WARNING(...)				\
   eir_ker_log("> (%s:%d) ## WARNING! ", __func__, __LINE__);	\
   eir_ker_log(__VA_ARGS__);					\
   eir_ker_log("\n");
#define EIR_KER_LOG_ERROR(...)					\
   eir_ker_log("> (%s:%d) #### ERROR! ", __func__, __LINE__);	\
   eir_ker_log(__VA_ARGS__);					\
   eir_ker_log("\n");
#else
#define EIR_KER_LOG_MESSAGE(...)
#define EIR_KER_LOG_WARNING(...)
#define EIR_KER_LOG_ERROR(...)
#endif
