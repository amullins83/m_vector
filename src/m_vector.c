#include "m_vector.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void _m_vector_internal_null_clear_func(M_VECTOR_DATA *element_ptr)
{
  // do nothing
}

MVector *m_vector_init(
  M_VECTOR_DATA *data,
  size_t object_size,
  size_t max_count)
{
  MVector *v = (MVector *)malloc(sizeof(MVector));
  if (v == NULL)
  {
	  return NULL;
  }

  v->object_size = object_size;
  v->size = object_size * max_count;
  v->data = data;
  if (v->data == NULL)
  {
	  free(v);
	  return NULL;
  }

  v->count = 0;
  v->clear_func = _m_vector_internal_null_clear_func;
  return v;
}

void m_vector_delete(MVector *v)
{
  size_t i;
  for(i = 0; i < v->count; i++)
  {
    v->clear_func(v->data + v->object_size * i);
  }

  free(v->data);
  free(v);
}

void m_vector_set_clear_func(MVector *v, MVectorClearFunction clear_func)
{
  v->clear_func = clear_func;
}

void _m_vector_internal_expand(MVector *v)
{
  int old_size = v->size, new_size = old_size * 2;
  char *new_data = (M_VECTOR_DATA *)calloc(sizeof(M_VECTOR_DATA), new_size);
  
  memcpy(new_data, v->data, old_size);

  free(v->data);

  v->data = new_data;
  v->size = new_size;
}

void _m_vector_internal_grow_if_needed(MVector *v)
{
  if(v->object_size * (v->count + 1) >= v->size)
  {
    _m_vector_internal_expand(v);
  }
}

void _m_vector_internal_append(MVector *v, const M_VECTOR_DATA *element_ptr)
{
  _m_vector_internal_insert(v, v->count, element_ptr);
}

void _m_vector_internal_prepend(MVector *v, const M_VECTOR_DATA *element_ptr)
{
  _m_vector_internal_insert(v, 0, element_ptr);
}

void _m_vector_internal_shift_right(MVector *, size_t);

void _m_vector_internal_insert(MVector *v, size_t index, const M_VECTOR_DATA *element_ptr)
{
  if(index > v->count)
  {
    index = v->count;
  }

  _m_vector_internal_shift_right(v, index);

  memcpy(v->data + v->object_size * index,
    element_ptr, v->object_size);
  
  v->count++;
}

void _m_vector_internal_shift_left(MVector *, size_t);

void m_vector_remove_index(MVector *v, size_t index)
{
  if(index >= v->count)
  {
    return; // this might be bad, in that it fails silently instead of complaining
  }

  v->clear_func(v->data + v->object_size * index);
  _m_vector_internal_shift_left(v, index + 1);
  v->count--;
}

void _m_vector_internal_shift_direction(MVector *, size_t, int);

void _m_vector_internal_shift_right(MVector *v, size_t index)
{
  _m_vector_internal_shift_direction(v, index, 1);
}

void _m_vector_internal_shift_left(MVector *v, size_t index)
{
  _m_vector_internal_shift_direction(v, index, -1);
}

void _m_vector_internal_shift_direction(MVector *v, size_t index, int direction)
{
  size_t copy_size = (v->count - index) * v->object_size;

  M_VECTOR_DATA *data_copy = (M_VECTOR_DATA *)malloc(copy_size);

  if(direction > 0)
  {
    _m_vector_internal_grow_if_needed(v);
  }
  
  memcpy(data_copy, v->data + v->object_size * index, copy_size);
  memcpy(v->data + v->object_size * (index + direction),
    v->data + v->object_size * index, copy_size);
}

#ifdef __cplusplus
};
#endif
