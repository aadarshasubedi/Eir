#pragma once

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

/**
 * Init an array.
 */
#define EIR_KER_INIT_ARRAY(item_type, array, max_capacity)		\
   EIR_SYS_ALLOC(array.data, sizeof(item_type), max_capacity);		\
   array.capacity = max_capacity;					\
   array.used = 0;

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
 * Release array data allocated memory and reinit the struct.
 */
#define EIR_KER_RELEASE_ARRAY(array)		\
   EIR_SYS_FREE(array.data);			\
   array.capacity = 0;				\
   array.used = 0;				\
   array.data = 0;
