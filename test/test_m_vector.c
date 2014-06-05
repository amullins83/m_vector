#include "unity.h"
#include "m_vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_VALUE 0x7777

MVector *test_vector;
int test_clear_value;
int test_clear_index;
int test_clear_call_count;

void hexdump(MVector *);

void test_clear_func(M_VECTOR_DATA *element_ptr)
{
  test_clear_call_count++;
  test_clear_value = *(int *)element_ptr;
  test_clear_index = (element_ptr - test_vector->data)/sizeof(int);
}

void setUp(void)
{
  test_vector = m_vector_new(int);
  test_clear_value = DEFAULT_VALUE;
  test_clear_call_count = 0;
}

void setUp_with_values(void)
{
  int i;
  int values[] = {0x5555, 0x8080, 0xAAAA, 0xFFFF, 0};
  for(i = 0; values[i] != 0; i++)
  {
    m_vector_append_val(test_vector, values[i]);
  }
}

void tearDown(void)
{
  if(test_vector != NULL)
  {
    m_vector_delete(test_vector);
    test_vector = NULL;
  }
}

void check_index(int expected, size_t index)
{
  TEST_ASSERT_EQUAL(expected, m_vector_index(test_vector, int, index));
}

void check_first(int expected)
{
  check_index(expected, 0);
}

void check_last(int expected)
{
  check_index(expected, test_vector->count - 1);
}

void test_new_vector(void)
{
	TEST_ASSERT_NOT_NULL(test_vector);
}

void test_append_empty(void)
{
  int test_val = 0x5555;

  m_vector_append_val(test_vector, test_val);
  //hexdump(test_vector);

  check_first(test_val);
}

void test_prepend_empty(void)
{
  int test_val = 0xAAAA;
  m_vector_prepend_val(test_vector, test_val);
  check_first(test_val);
}

void test_insert_empty_zero(void)
{
  int test_val = 0xFFFF;
  m_vector_insert_val(test_vector, 0, test_val);
  check_first(test_val);
}

void test_insert_empty_80(void)
{
  int test_val = 0x8080;
  m_vector_insert_val(test_vector, 80, test_val);
  check_first(test_val);
}

void test_append_with_values(void)
{
  int test_val = 0x0101;
  setUp_with_values();
  m_vector_append_val(test_vector, test_val);
  check_last(test_val);
}

void test_prepend_with_values(void)
{
  int test_val = 0x0202;
  setUp_with_values();
  m_vector_prepend_val(test_vector, test_val);
  check_first(test_val);
}

void test_insert_with_values_three(void)
{
  int old_last, test_val;
  
  setUp_with_values();
  
  old_last = m_vector_index(test_vector, int, test_vector->count - 1);
  test_val = 0x0303;

  m_vector_insert_val(test_vector, 3, test_val);

  check_index(test_val, 3);
  TEST_ASSERT_EQUAL(5, test_vector->count);
  check_last(old_last);
}

void test_insert_with_values_80(void)
{
  int test_val = 0x0404;

  setUp_with_values();
  m_vector_insert_val(test_vector, 80, test_val);
  check_last(test_val);
  TEST_ASSERT_EQUAL(5, test_vector->count);
}

void test_remove_next_to_last(void)
{
  int old_last, remove_index, old_count;

  setUp_with_values();

  old_last = m_vector_index(test_vector, int, test_vector->count - 1);
  old_count = test_vector->count;
  remove_index = old_count - 2;

  m_vector_remove_index(test_vector, remove_index);
  TEST_ASSERT_EQUAL(old_count - 1, test_vector->count);
  check_last(old_last);
}

void test_remove_80(void)
{
  int old_last, old_count;

  setUp_with_values();

  old_last = m_vector_index(test_vector, int, test_vector->count - 1);
  old_count = test_vector->count;

  m_vector_remove_index(test_vector, 80);
  TEST_ASSERT_EQUAL(old_count, test_vector->count);
  check_last(old_last);
}

void test_the_test_clear_func(void)
{
  int old_value, index = 2;
  m_vector_set_clear_func(test_vector, test_clear_func);
  setUp_with_values();

  old_value = m_vector_index(test_vector, int, index);
  test_vector->clear_func(test_vector->data + sizeof(int) * index);
  TEST_ASSERT_EQUAL(old_value, test_clear_value);
  TEST_ASSERT_EQUAL(index, test_clear_index);
}

void test_clear_func_called_by_remove_index(void)
{
  int old_value, remove_index = 2;
  m_vector_set_clear_func(test_vector, test_clear_func);

  setUp_with_values();

  old_value = m_vector_index(test_vector, int, remove_index);
  m_vector_remove_index(test_vector, remove_index);

  TEST_ASSERT_EQUAL(remove_index, test_clear_index);
  TEST_ASSERT_EQUAL(old_value, test_clear_value);
  TEST_ASSERT_EQUAL(1, test_clear_call_count);
}

void test_clear_func_called_by_delete(void)
{
  int old_count;
  setUp_with_values();
  old_count = test_vector->count;

  m_vector_set_clear_func(test_vector, test_clear_func);
  m_vector_delete(test_vector);
  test_vector = NULL;
  
  TEST_ASSERT_EQUAL(old_count, test_clear_call_count);
}

void hexdump(MVector *vector)
{
  size_t i;
  for(i = 0; i < vector->size; i++)
  {
    printf("%x ", test_vector->data[i]);
  }
  printf("\n");
}
