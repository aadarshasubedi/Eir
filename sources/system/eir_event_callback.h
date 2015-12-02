#pragma once

#include <stdbool.h>
#include "../eir.h"

typedef bool (*eir_sys_event_callback_t)(const eir_event_t * event, eir_env_t * env);
bool eir_sys_default_event_callback(const eir_event_t * event, eir_env_t * env);
