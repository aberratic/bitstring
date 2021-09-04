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

#ifdef __cplusplus
extern "C" {
#endif

static inline unsigned int *
bstr_get_int_for_bit_index(const bstr_bitstr_t *const bstr, unsigned int bit) {
  return (bstr->_bits + (bit / (sizeof(unsigned int) * 8)));
}

static inline bool bstr_is_ptr_out_of_bounds(const bstr_bitstr_t *const bstr,
                                             const unsigned int *const ptr) {
  if (bstr->_bits + bstr->_capacity <= ptr || bstr->_bits > ptr)
    return true;
  return false;
}

bstr_bitstr_t *bstr_create_bitstr(unsigned int capacity) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(capacity > 0);
#endif
  bstr_bitstr_t *result = (bstr_bitstr_t *)malloc(sizeof(bstr_bitstr_t));
  if (result == NULL)
    return NULL;
  result->_bits = (unsigned int *)malloc(capacity * sizeof(unsigned int));
  result->_capacity = capacity;
  memset(result->_bits, 0, result->_capacity * sizeof(unsigned int));
  return result;
}

void bstr_delete_bitstr(bstr_bitstr_t *bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
  assert(bstr->_bits != NULL);
#endif
  free(bstr->_bits);
  free(bstr);
}

bstr_err_t bstr_resize(bstr_bitstr_t *const bstr, unsigned int capacity) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
  assert(capacity > 0);
#endif
  unsigned int *oldbits = bstr->_bits;
  size_t new_capacity = capacity;
  size_t old_capacity = bstr->_capacity;
  unsigned int *newbits =
      (unsigned int *)malloc(new_capacity * sizeof(unsigned int));
  if (newbits == NULL)
    return MALLOC_FAILED;

  if (new_capacity >= old_capacity) {
    memset(newbits, 0, new_capacity * sizeof(unsigned int));
    memcpy(newbits, oldbits, old_capacity * sizeof(unsigned int));
  } else {
    memcpy(newbits, oldbits, new_capacity * sizeof(unsigned int));
  }
  bstr->_capacity = new_capacity;
  bstr->_bits = newbits;
  return NO_ERROR;
}

unsigned int bstr_get_capacity(const bstr_bitstr_t *const bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  return bstr->_capacity;
}

unsigned int bstr_get_bit_capacity(const bstr_bitstr_t *const bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  return bstr->_capacity * sizeof(unsigned int) * 8;
}

size_t bstr_to_string_size(const bstr_bitstr_t *const bstr) {
  //                                   bits per byte    trailing \0
  return (bstr->_capacity * sizeof(unsigned int) * 8) + 1;
}

void bstr_to_string(const bstr_bitstr_t *const bstr, char *const str) {
  const char one = '1';
  const char zero = '0';
  unsigned int len = bstr_get_bit_capacity(bstr);
  for (unsigned int i = 0; i != len; i++) {
    if (bstr_get(bstr, i)) {
      strncat(str, &one, 1);
    } else {
      strncat(str, &zero, 1);
    }
  }
}

void bstr_bindump(const bstr_bitstr_t *const bstr, char *const str,
                  const unsigned int line) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
  assert(str != NULL);
  assert(line < bstr->_capacity);
#endif
  unsigned int *target = bstr->_bits + line;
  snprintf(str, BSTR_BINDUMP_SIZE, "%p:", target);
  const unsigned int num_bits = sizeof(unsigned int) * 8;
  for (unsigned int bit = num_bits; bit > 0; bit--) {
    unsigned int bitval = ((*target) >> (bit - 1)) & 1U;
    if (bit % 8 == 0)
      strncat(str, " ", 2);
    if (bitval > 0) {
      strncat(str, "1", 2);
    } else {
      strncat(str, "0", 2);
    }
  }
}

void bstr_set(bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *target = bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(!bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_set = bit % (sizeof(unsigned int) * 8);
  *target |= 1 << bit_to_set;
  return;
}

void bstr_set_all(bstr_bitstr_t *const bstr, bool on) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned char value = 0;
  if (on)
    value = UCHAR_MAX;
  memset(bstr->_bits, value, bstr->_capacity * sizeof(unsigned int));
}

void bstr_clr(bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *target = bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(!bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_clear = bit % (sizeof(unsigned int) * 8);
  *target &= ~(1 << bit_to_clear);
}

bool bstr_get(const bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *target = bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(!bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_get = bit % (sizeof(unsigned int) * 8);
  unsigned int result = ((*target) >> bit_to_get) & 1U;
  if (result > 0) {
    return true;
  } else {
    return false;
  }
}

int bstr_ffs(const bstr_bitstr_t *const bstr) { // ffs();
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  unsigned int offset = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
    int result = ffs(*i);
    if (result > 0)
      return (offset * sizeof(unsigned int) * 8) + result - 1;
    offset++;
  }
  return -1;
}

int bstr_ctz(const bstr_bitstr_t *const bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  int result = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
    if (*i == 0) {
      result += sizeof(unsigned int) * 8;
      continue;
    }
    result += __builtin_ctz(*i);
    return result;
  }
  return result;
}

int bstr_clz(const bstr_bitstr_t *const bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *baseptr = bstr->_bits + bstr->_capacity - 1;
  int result = 0;
  for (unsigned int *i = baseptr; i >= bstr->_bits; i--) {
    if (*i == 0) {
      result += sizeof(unsigned int) * 8;
      continue;
    } else {
      return result + __builtin_clz(*i);
    }
  }
  return result;
}

int bstr_popcnt(const bstr_bitstr_t *const bstr) {
#ifdef CONFIG_BITSTREAM_ENABLE_BOUND_CHECKS
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  int popcnt = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
    popcnt += __builtin_popcount(*i);
  }
  return popcnt;
}

#ifdef __cplusplus
}
#endif
