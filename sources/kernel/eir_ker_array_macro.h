#pragma once

#include "../system/eir_sys_memory.h"

/**
 * =====================================================================
 * This is a macro serie to help user for array manipulation.
 * Requirements :
 * your struct must contains the following attributes :
 * - data: the item array which is a pointer of item
 * - capacity: will be used for allocation, this is the max item count
 * - used: the actual number of item set in your array
 * Ex of struct:
 * typedef struct
 * {
 *    my_struct_t * data;
 *    size_t capacity;
 *    size_t used;
 * } array_my_struct_t;
 * =====================================================================
 */

/**
 * Create array struct
 */
#define EIR_KER_DEFINE_ARRAY_STRUCT(data_type, array_name)	\
   typedef struct						\
   {								\
      data_type * data;						\
      size_t capacity;						\
      size_t used;						\
   } array_name;

#define EIR_KER_INIT_ARRAY(array)		\
   array.data = 0;				\
   array.used = 0;				\
   array.capacity = 0;

/**
 * Init an array.
 */
#define EIR_KER_ALLOCATE_ARRAY(item_type, array, max_capacity)		\
   if (max_capacity > 0)                                          \
   {                                                              \
   EIR_SYS_ALLOC(array.data, sizeof(item_type), max_capacity);		\
   array.capacity = max_capacity;					                  \
   array.used = 0;                                                \
   }

/**
 * Init an array and all its elements
 */
#define EIR_KER_ALLOCATE_ARRAY_BIS(item_type, array, max_capacity, data_init_func) \
   EIR_KER_ALLOCATE_ARRAY(item_type, array, max_capacity)		\
   for (int data_index = 0; data_index < array.capacity; ++data_index)	\
   {									\
      data_init_func(&array.data[data_index]);				\
   }

/**
 * Reserve the next array empty slot.
 */
#define EIR_KER_RESERVE_ARRAY_NEXT_EMPTY_SLOT(array, out_index)		\
   if (array.used < array.capacity)					\
   {									\
      out_index = array.used;						\
      ++array.used;							\
   }									\
   else									\
   {									\
      out_index = -1;							\
      EIR_KER_LOG_ERROR("max capacity already reached: %d/%d", array.used, array.capacity); \
   }

/**
 * Reserve the next array empty slot.
 */
#define EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT(array, item_ptr)		\
   if (array.used < array.capacity)					\
   {									\
      item_ptr = &array.data[array.used];				\
      ++array.used;							\
   }									\
   else									\
   {									\
      item_ptr = 0;							\
      EIR_KER_LOG_WARNING("max capacity already reached: %d/%d", array.used, array.capacity); \
   }

/**
 * Reserve the next array empty slot.
 */
#define EIR_KER_GET_ARRAY_NEXT_EMPTY_SLOT_BIS(array, item_ptr, out_index) \
   if (array.used < array.capacity)					\
   {									\
      item_ptr = &array.data[array.used];				\
      out_index = array.used;						\
      ++array.used;							\
   }									\
   else									\
   {									\
      item_ptr = 0;							\
      out_index = -1;							\
      EIR_KER_LOG_WARNING("max capacity already reached: %d/%d", array.used, array.capacity); \
   }

/**
 * Free last reserved slot (memory is not free, we just allow user to overwrite item)
 */
#define EIR_KER_FREE_ARRAY_LAST_RESERVED_SLOT(array)			\
   if (0 < array.used)							\
   {									\
      --array.used;							\
   }

/**
 * Get array item safe
 */
#define EIR_KER_GET_ARRAY_ITEM(array, index, item_ptr)			\
   item_ptr = (index >= 0 && index < array.used) ? &array.data[index] : 0;

/**
 * Reinit structure. Does not change array size and does not free allocated memory
 */
#define EIR_KER_RELEASE_ARRAY(array)		\
   array.used = 0;				\
   array.capacity = 0;

/**
 * Release array data and reinit structure. Does not change array size and doest not free allocated
 * memory
 */
#define EIR_KER_RELEASE_ARRAY_BIS(array, data_release_func)		\
   for (int data_index = 0; data_index < array.capacity; ++data_index)	\
   {									\
      data_release_func(&array.data[data_index]);			\
   }									\
   EIR_KER_RELEASE_ARRAY(array);

/**
 * Free array data allocated memory and reinit the struct.
 */
#define EIR_KER_FREE_ARRAY(array)		\
   EIR_SYS_FREE(array.data);			\
   array.capacity = 0;				\
   array.used = 0;				\
   array.data = 0;

/**
 * Free array allocated memory, release data and reinit the struct.
 */
#define EIR_KER_FREE_ARRAY_BIS(array, data_release_func)		\
   for (int data_index = 0; data_index < array.capacity; ++data_index)	\
   {									\
      data_release_func(&array.data[data_index]);			\
   }									\
   EIR_KER_FREE_ARRAY(array)
