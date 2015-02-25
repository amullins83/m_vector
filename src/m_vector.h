#ifndef m_vector_H
#define m_vector_H
#include <stdlib.h>

/*
 Generic-ish vector class for embedded C programs lacking GLib support
 
 This is intended for each vector to contain objects of one data type.
 Undefined behavior will occur if attempting to add objects of varying types to
 a single m_vector.

 This is heavily based on the GLib GArray type documented at:
 
 <a href="https://developer.gnome.org/glib/stable/glib-Arrays.html">
    GArray Home Page</a>

*/

#ifdef __cplusplus
extern "C" {
#endif

#define M_VECTOR_DEFAULT_SIZE 255
#define M_VECTOR_DATA unsigned char

	typedef void(*MVectorClearFunction)(M_VECTOR_DATA *element_ptr);

	typedef struct MVECTOR {
		size_t count;
		size_t size;
		size_t object_size;
		MVectorClearFunction clear_func;
		M_VECTOR_DATA *data;
	} MVector;

	MVector *m_vector_init(M_VECTOR_DATA *data, size_t object_size, size_t max_count);

#define m_vector_new(type) (m_vector_init((M_VECTOR_DATA *)calloc(sizeof(type), \
  M_VECTOR_DEFAULT_SIZE), sizeof(type), M_VECTOR_DEFAULT_SIZE))

#define m_vector_sized_new(type, max_count) (m_vector_init((M_VECTOR_DATA *)calloc(sizeof(type), \
  max_count), sizeof(type), (max_count)))

	void m_vector_delete(MVector *);
	void m_vector_set_clear_func(MVector *, MVectorClearFunction);

	void m_vector_remove_index(MVector *, size_t index);

	void _m_vector_internal_append(MVector *, const M_VECTOR_DATA *);
	void _m_vector_internal_prepend(MVector *, const M_VECTOR_DATA *);
	void _m_vector_internal_insert(MVector *, size_t, const M_VECTOR_DATA *);

#define m_vector_append_val(vector, value) ( \
  _m_vector_internal_append((vector), (const M_VECTOR_DATA *)&(value)))

#define m_vector_prepend_val(vector, value) ( \
  _m_vector_internal_prepend((vector), (const M_VECTOR_DATA *)&(value)))

#define m_vector_insert_val(vector, index, value) ( \
  _m_vector_internal_insert((vector), (size_t)(index), (const M_VECTOR_DATA *)&(value)))

#define m_vector_index(vector, type, index) (*(type *)((vector)->data + \
  (vector)->object_size*(index)))

#ifdef __cplusplus
};
#endif

#endif
