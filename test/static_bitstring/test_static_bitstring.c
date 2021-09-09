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

#include "bitstring_static.h"
#include "unity.h"

#ifdef __cplusplus
extern "C" {
#endif
BSTR_STATIC_DECLARE_SIZED_BITSTRING_STRUCT(64);
BSTR_STATIC_DECLARE_GET_CAPACITY(64);
BSTR_STATIC_DECLARE_BOUND_CHECK(64);
BSTR_STATIC_DECLARE_GET_BIT_CAPACITY(64);
BSTR_STATIC_DECLARE_TO_STRING_SIZE(64);
BSTR_STATIC_DECLARE__GET_INT_FOR_BIT_INDEX(64);
BSTR_STATIC_DECLARE_GET(64);
BSTR_STATIC_DECLARE_TO_STRING(64);
BSTR_STATIC_DECLARE_BINDUMP(64);
BSTR_STATIC_DECLARE_SET(64);
BSTR_STATIC_DECLARE_SET_ALL(64);
BSTR_STATIC_DECLARE_CLR(64);
BSTR_STATIC_DECLARE_FFS(64);
BSTR_STATIC_DECLARE_FFUS(64);
BSTR_STATIC_DECLARE_CTZ(64);
BSTR_STATIC_DECLARE_CLZ(64);
BSTR_STATIC_DECLARE_POPCNT(64);

void bitdump(const bstr_bitstr64_t *const bstr) {
  char bdump[BSTR_BINDUMP_SIZE] = {0};
  for (unsigned int j = 0; j < bstrs_get_capacity(64); j++) {
    bstrs_bindump(64, bstr, (char *const)&bdump, j);
    printf("%s\n", bdump);
  }
}

void test_bstrs_create(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(0, bstrs_popcnt(64, &test));
}

void test_bstrs_get_capacity(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(64, bstrs_get_capacity(64));
}

void test_bstrs_get_bit_capacity(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(64 * sizeof(unsigned int) * 8,
                        bstrs_get_bit_capacity(64));
}

void test_bstrs_to_string_size(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT((64 * sizeof(unsigned int) * 8) + 1,
                        bstrs_to_string_size(64));
}

void test_bstrs_set(void) {
  bstr_static_t(64) test = bstrs_initialize;
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    // printf("%d\n", i);
    bstrs_set(64, &test, i);
    // bitdump(&test);
    TEST_ASSERT_TRUE(bstrs_get(64, &test, i));
    TEST_ASSERT_EQUAL_INT(i + 1, bstrs_popcnt(64, &test));
  }
}

void test_bstrs_set_all(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(0, bstrs_popcnt(64, &test));
  bstrs_set_all(64, &test, true);
  TEST_ASSERT_EQUAL_INT(64 * sizeof(unsigned int) * 8, bstrs_popcnt(64, &test));
}

void test_bstrs_clr(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(0, bstrs_popcnt(64, &test));
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    bstrs_set(64, &test, i);
    TEST_ASSERT_TRUE(bstrs_get(64, &test, i));
    bstrs_clr(64, &test, i);
    TEST_ASSERT_FALSE(bstrs_get(64, &test, i));
  }
}

void test_bstrs_ffs(void) {
  bstr_static_t(64) test = bstrs_initialize;
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    bstrs_set(64, &test, i);
    TEST_ASSERT_EQUAL_INT(i, bstrs_ffs(64, &test));
    bstrs_clr(64, &test, i);
  }
}

void test_bstrs_ctz(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(bstrs_get_bit_capacity(64), bstrs_ctz(64, &test));
  bstrs_set_all(64, &test, true);
  TEST_ASSERT_EQUAL_INT(0, bstrs_ctz(64, &test));
  bstrs_set_all(64, &test, false);
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    bstrs_set(64, &test, i);
    TEST_ASSERT_EQUAL_INT(i, bstrs_ctz(64, &test));
    bstrs_clr(64, &test, i);
  }
}

void test_bstrs_clz(void) {
  bstr_static_t(64) test = bstrs_initialize;
  unsigned int capacity = bstrs_get_bit_capacity(64);
  TEST_ASSERT_EQUAL_INT(capacity, bstrs_clz(64, &test));

  bstrs_set_all(64, &test, true);
  TEST_ASSERT_EQUAL_INT(0, bstrs_clz(64, &test));

  bstrs_set_all(64, &test, false);
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    bstrs_set(64, &test, i);
    TEST_ASSERT_EQUAL_INT(bstrs_get_bit_capacity(64) - i - 1,
                          bstrs_clz(64, &test));
    bstrs_clr(64, &test, i);
  }
}

void test_bstrs_popcnt(void) {
  bstr_static_t(64) test = bstrs_initialize;
  TEST_ASSERT_EQUAL_INT(0, bstrs_popcnt(64, &test));
  bstrs_set_all(64, &test, true);
  TEST_ASSERT_EQUAL_INT(bstrs_get_bit_capacity(64), bstrs_popcnt(64, &test));
  bstrs_set_all(64, &test, false);
  TEST_ASSERT_EQUAL_INT(0, bstrs_popcnt(64, &test));
  for (int i = 0; i != bstrs_get_bit_capacity(64); i++) {
    bstrs_set(64, &test, i);
    TEST_ASSERT_TRUE(bstrs_get(64, &test, i));
    TEST_ASSERT_EQUAL_INT(i + 1, bstrs_popcnt(64, &test));
  }
}

void test_bstrs_ffus(void) {
  bstr_static_t(64) test = bstrs_initialize;
  bstrs_set_all(64, &test, true);
  for (int i = bstrs_get_bit_capacity(64) - 1; i >= 0; i--) {
    bstrs_clr(64, &test, i);
    TEST_ASSERT_EQUAL_INT(i, bstrs_ffus(64, &test));
    bstrs_set(64, &test, i);
  }
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_bstrs_create);
  RUN_TEST(test_bstrs_get_capacity);
  RUN_TEST(test_bstrs_get_bit_capacity);
  RUN_TEST(test_bstrs_to_string_size);
  RUN_TEST(test_bstrs_set);
  RUN_TEST(test_bstrs_set_all);
  RUN_TEST(test_bstrs_clr);
  RUN_TEST(test_bstrs_ffs);
  RUN_TEST(test_bstrs_ctz);
  RUN_TEST(test_bstrs_popcnt);
  RUN_TEST(test_bstrs_ffus);
  RUN_TEST(test_bstrs_clz);
  UNITY_END();
}

#ifdef __cplusplus
}
#endif
