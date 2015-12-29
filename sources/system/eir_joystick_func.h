#pragma once

int eir_sys_get_pad_count();
eir_sys_pad_handle_t eir_sys_init_pad(int pad_index);
void eir_sys_release_pad(eir_sys_pad_handle_t pad_handle);
