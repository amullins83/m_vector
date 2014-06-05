#include "m_vector.h"
#include <string.h>
#include <stdlib.h>

void _m_vector_internal_null_clear_func(M_VECTOR_DATA *element_ptr)
{
  // do nothing
}

MVector *m_vector_init(
  M_VECTOR_DATA *data,
  size_t object_size,
  size_t max_count)
{
  MVector *this = (MVector *)malloc(sizeof(MVector));
  this->object_size = object_size;
  this->size = object_size * max_count;
  this->data = data;
  this->count = 0;
  this->clear_func = _m_vector_internal_null_clear_func;
  return this;
}

void m_vector_delete(MVector *this)
{
  size_t i;
  for(i = 0; i < this->count; i++)
  {
    this->clear_func(this->data + this->object_size * i);
  }

  free(this->data);
  free(this);
}

void m_vector_set_clear_func(MVector *this, MVectorClearFunction clear_func)
{
  this->clear_func = clear_func;
}

void _m_vector_internal_expand(MVector *this)
{
  int old_size = this->size, new_size = old_size * 2;
  char *new_data = (M_VECTOR_DATA *)calloc(sizeof(M_VECTOR_DATA), new_size);
  
  memcpy(new_data, this->data, old_size);

  free(this->data);

  this->data = new_data;
  this->size = new_size;
}

void _m_vector_internal_grow_if_needed(MVector *this)
{
  if(this->object_size * (this->count + 1) >= this->size)
  {
    _m_vector_internal_expand(this);
  }
}

MVector *_m_vector_internal_append(MVector *this, const M_VECTOR_DATA *element_ptr)
{
  _m_vector_internal_insert(this, this->count, element_ptr);
}

MVector *_m_vector_internal_prepend(MVector *this, const M_VECTOR_DATA *element_ptr)
{
  _m_vector_internal_insert(this, 0, element_ptr);
}

void _m_vector_internal_shift_right(MVector *, size_t);

MVector *_m_vector_internal_insert(MVector *this, size_t index, const M_VECTOR_DATA *element_ptr)
{
  if(index > this->count)
  {
    index = this->count;
  }

  _m_vector_internal_shift_right(this, index);

  memcpy(this->data + this->object_size * index,
    element_ptr, this->object_size);
  
  this->count++;
}

void _m_vector_internal_shift_left(MVector *, size_t);

MVector *m_vector_remove_index(MVector *this, size_t index)
{
  if(index >= this->count)
  {
    return; // This might be bad, in that it fails silently instead of complaining
  }

  this->clear_func(this->data + this->object_size * index);
  _m_vector_internal_shift_left(this, index + 1);
  this->count--;
}

void _m_vector_internal_shift_direction(MVector *, size_t, int);

void _m_vector_internal_shift_right(MVector *this, size_t index)
{
  _m_vector_internal_shift_direction(this, index, 1);
}

void _m_vector_internal_shift_left(MVector *this, size_t index)
{
  _m_vector_internal_shift_direction(this, index, -1);
}

void _m_vector_internal_shift_direction(MVector *this, size_t index, int direction)
{
  size_t copy_size = (this->count - index) * this->object_size;

  M_VECTOR_DATA *data_copy = (M_VECTOR_DATA *)malloc(copy_size);

  if(direction > 0)
  {
    _m_vector_internal_grow_if_needed(this);
  }
  
  memcpy(data_copy, this->data + this->object_size * index, copy_size);
  memcpy(this->data + this->object_size * (index + direction),
    this->data + this->object_size * index, copy_size);
}
