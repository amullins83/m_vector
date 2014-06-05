#MVector Generic Array with Dynamic Resizing

##Summary

Created with [Ceedling](http://throwtheswitch.org)!

I decided to implement my own vector type for a Microchip PIC32 application, because:

1. There does not appear to be a Microchip library with this functionality
2. It is useful
3. It can be as simple as I want it to be

Basic concepts are derived from the [GLib library](https://developer.gnome.org/glib/stable/glib-Arrays.html).

##Use

In your source files:

    #include "m_vector.h"
    
Don't forget to link against it!

    $gcc -o m_vector.o m_vector.c
    $gcc -o my_app my_app.o m_vector.o
    
##Data Types

```C
#define M_VECTOR_DATA unsigned char

typedef void (*MVectorClearFunction)(M_VECTOR_DATA *element_ptr);

typedef struct _M_VECTOR
{
  ...
} MVector;
```

##Functions

###New

    #define m_vector_new(type)
    
Pass in the data type of the values you intend to store. This vector implementation only works if all stored objects are the same size in memory. It is assumed by the library that all objects are the same type.

###Sized New

    #define m_vector_sized_new(type, max_count)
    
Same as `m_vector_new`, except that the vector will be initialized with `max_count` slots. This is more efficient if you know ahead of time the expected maximum size of the data set you will work with.

###Delete

    void m_vector_delete(MVector *)

Deallocates the vector.

###Set Clear Function

    void m_vector_set_clear_func(MVector *, MVectorClearFunction);
    
You may set a function that will be called whenever an item is removed from the vector. This could be useful for deallocating memory, closing files, etc.

###Remove Index

    void m_vector_remove_index(MVector *, size_t index);
    
Removes item at the given index after calling the given MVectorClearFunction on it.

###Append Value

    #define m_vector_append_val(MVector *, type value)
    
Insert an item at the end of the vector. The vector expands when necessary.

###Prepend Value

    #define m_vector_prepend_val(MVector *, type value)
    
Insert an item at the beginning of the vector. The vector expands when necessary.

###Index

    #define m_vector_insert_val(MVector *, size_t index, type value)
    
Insert an item at the given index. The vector expands when necessary.
