#pragma once

typedef struct
{
   bool pressed;
} eir_gme_button_state_t;

#define EIR_GME_MOVE_RIGHT_BUTTON_INDEX 0
#define EIR_GME_MOVE_LEFT_BUTTON_INDEX 1
#define EIR_GME_MOVE_DOWN_BUTTON_INDEX 2
#define EIR_GME_MOVE_UP_BUTTON_INDEX 3
#define EIR_GME_ACTION_1_BUTTON_INDEX 4
#define EIR_GME_TOTAL_INPUT_BUTTON_COUNT 5

typedef struct
{
   bool is_connected;
   bool is_analog;
   float left_stick_value_x;
   float left_stick_value_y;
   eir_gme_button_state_t buttons[EIR_GME_TOTAL_INPUT_BUTTON_COUNT];
} eir_gme_input_controller_t;

#define EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT 2

typedef struct
{
   eir_gme_input_controller_t controllers[EIR_GME_TOTAL_INPUT_CONTROLLER_BUFFER_COUNT];
} eir_gme_input_controller_buffer_t;
