/*
MIT License

Copyright (c) 2021 Stefan Luecke <git@aberrational.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "bitstring.h"
#include "unity.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_BSTR_MAX_TEST_CAPACITY 64

void test_bstr_create_and_delete_bitstr(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_resize(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(i, bstr_get_capacity(test));
    bstr_set_all(test, true);
    int popcnt = bstr_popcnt(test);
    TEST_ASSERT_EQUAL_INT(BSTR_NO_ERROR, bstr_resize(test, i * 2));
    TEST_ASSERT_EQUAL_INT(popcnt, bstr_popcnt(test));
    TEST_ASSERT_EQUAL_INT(i * 2, bstr_get_capacity(test));
    bstr_set_all(test, false);
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_resize_with_same_size(void) {
  bstr_bitstr_t *sameSize = bstr_create_bitstr(32);
  TEST_ASSERT_NOT_NULL(sameSize);
  TEST_ASSERT_EQUAL_INT(32, bstr_get_capacity(sameSize));
  TEST_ASSERT_EQUAL_INT(BSTR_NO_ERROR, bstr_resize(sameSize, 32));
  TEST_ASSERT_EQUAL_INT(32, bstr_get_capacity(sameSize));
  bstr_delete_bitstr(sameSize);
}

void test_bstr_resize_with_smaller_size(void) {
  bstr_bitstr_t *shrink = bstr_create_bitstr(64);
  TEST_ASSERT_NOT_NULL(shrink);
  TEST_ASSERT_EQUAL_INT(64, bstr_get_capacity(shrink));
  bstr_set_all(shrink, true);
  int popcnt = bstr_popcnt(shrink);
  if (bstr_resize(shrink, 32) != BSTR_NO_ERROR) {
    TEST_FAIL_MESSAGE("bstr_resize failed");
  }
  TEST_ASSERT_EQUAL_INT(32, bstr_get_capacity(shrink));
  TEST_ASSERT_EQUAL_INT(popcnt / 2, bstr_popcnt(shrink));
  bstr_delete_bitstr(shrink);
}

void test_bstr_get_capacity(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(i, bstr_get_capacity(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_get_bit_capacity(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(i * sizeof(unsigned int) * 8,
                          bstr_get_bit_capacity(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_to_string_size(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_UINT((i * sizeof(unsigned int) * 8) + 1,
                           bstr_to_string_size(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_set_linear(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_TRUE(bstr_get(test, j));
      TEST_ASSERT_EQUAL_INT(j + 1, bstr_popcnt(test));
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_set_even(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    int expected_popcount = 0;
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      if (j % 2 == 0) {
        bstr_set(test, j);
        TEST_ASSERT_TRUE(bstr_get(test, j));
        expected_popcount++;
      } else {
        TEST_ASSERT_FALSE(bstr_get(test, j));
      }
      TEST_ASSERT_EQUAL_INT(expected_popcount, bstr_popcnt(test));
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_set_uneven(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    int expected_popcount = 0;
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      if (j % 2 == 1) {
        bstr_set(test, j);
        TEST_ASSERT_TRUE(bstr_get(test, j));
        expected_popcount++;
      } else {
        TEST_ASSERT_FALSE(bstr_get(test, j));
      }
      TEST_ASSERT_EQUAL_INT(expected_popcount, bstr_popcnt(test));
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_set_all(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    bstr_set_all(test, true);
    TEST_ASSERT_EQUAL_INT(i * sizeof(unsigned int) * 8, bstr_popcnt(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_clr(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_TRUE(bstr_get(test, j));
      bstr_clr(test, j);
      TEST_ASSERT_FALSE(bstr_get(test, j));
    }
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    bstr_delete_bitstr(test);
  }
}

void test_bstr_ffs(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_EQUAL_INT(j, bstr_ffs(test));
      bstr_clr(test, j);
    }
    bstr_delete_bitstr(test);
  }
}

void bitdump(const bstr_bitstr_t *const bstr) {
  char bdump[BSTR_BINDUMP_SIZE] = {0};
  for (unsigned int j = 0; j < bstr_get_capacity(bstr); j++) {
    bstr_bindump(bstr, (char *const)&bdump, j);
    printf("%s\n", bdump);
  }
}

void test_bstr_ctz(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(bstr_get_bit_capacity(test), bstr_ctz(test));
    bstr_set_all(test, true);
    TEST_ASSERT_EQUAL_INT(0, bstr_ctz(test));
    bstr_set_all(test, false);
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_EQUAL_INT(j, bstr_ctz(test));
      bstr_clr(test, j);
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_clz(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(bstr_get_bit_capacity(test), bstr_clz(test));
    bstr_set_all(test, true);
    TEST_ASSERT_EQUAL_INT(0, bstr_clz(test));
    bstr_set_all(test, false);
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_EQUAL_INT(bstr_get_bit_capacity(test) - j - 1,
                            bstr_clz(test));
      bstr_clr(test, j);
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_popcnt(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    bstr_set_all(test, true);
    TEST_ASSERT_EQUAL_INT(bstr_get_bit_capacity(test), bstr_popcnt(test));
    bstr_set_all(test, false);
    TEST_ASSERT_EQUAL_INT(0, bstr_popcnt(test));
    for (int j = 0; j != bstr_get_bit_capacity(test); j++) {
      bstr_set(test, j);
      TEST_ASSERT_TRUE(bstr_get(test, j));
      TEST_ASSERT_EQUAL_INT(j + 1, bstr_popcnt(test));
    }
    bstr_delete_bitstr(test);
  }
}

void test_bstr_ffus(void) {
  for (unsigned int i = 1; i < TEST_BSTR_MAX_TEST_CAPACITY; i++) {
    bstr_bitstr_t *test = bstr_create_bitstr(i);
    TEST_ASSERT_NOT_NULL(test);
    bstr_set_all(test, true);
    for (int j = bstr_get_bit_capacity(test) - 1; j >= 0; j--) {
      bstr_clr(test, j);
      TEST_ASSERT_EQUAL_INT(j, bstr_ffus(test));
      bstr_set(test, j);
    }
    bstr_delete_bitstr(test);
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_bstr_create_and_delete_bitstr);
  RUN_TEST(test_bstr_resize);
  RUN_TEST(test_bstr_resize_with_same_size);
  RUN_TEST(test_bstr_resize_with_smaller_size);
  RUN_TEST(test_bstr_get_capacity);
  RUN_TEST(test_bstr_get_bit_capacity);
  RUN_TEST(test_bstr_to_string_size);
  RUN_TEST(test_bstr_set_linear);
  RUN_TEST(test_bstr_set_even);
  RUN_TEST(test_bstr_set_uneven);
  RUN_TEST(test_bstr_set_all);
  RUN_TEST(test_bstr_clr);
  RUN_TEST(test_bstr_ffs);
  RUN_TEST(test_bstr_ctz);
  RUN_TEST(test_bstr_clz);
  RUN_TEST(test_bstr_popcnt);
  RUN_TEST(test_bstr_ffus);
  UNITY_END();
}

#ifdef __cplusplus
}
#endif
