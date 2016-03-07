#pragma once

typedef enum
{
   eir_sys_keyboard_event_type_unknown,
   eir_sys_keyboard_event_type_key_down,
   eir_sys_keyboard_event_type_key_up
} eir_sys_keyboard_event_type_t;

typedef enum
{
   eir_sys_keyboard_key_unknown,
   eir_sys_keyboard_key_escapce,
   eir_sys_keyboard_key_left_arrow,
   eir_sys_keyboard_key_right_arrow
} eir_sys_keyboard_key_t;

typedef struct
{
   eir_sys_keyboard_event_type_t type;
   eir_sys_keyboard_key_t key;
} eir_sys_keyboard_event_t;
