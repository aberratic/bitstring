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
_bstr_get_int_for_bit_index(const bstr_bitstr_t *const bstr, unsigned int bit) {
  return (bstr->_bits + (bit / (sizeof(unsigned int) * CHAR_BIT)));
}

#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
static inline bool _bstr_is_ptr_out_of_bounds(const bstr_bitstr_t *const bstr,
                                              const unsigned int *const ptr) {
  if (bstr->_bits + bstr->_capacity <= ptr || bstr->_bits > ptr)
    return true;
  return false;
}
#endif

bstr_bitstr_t *bstr_create_bitstr(unsigned int capacity) {
#ifdef DEBUG
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
#ifdef DEBUG
  assert(bstr != NULL);
  assert(bstr->_bits != NULL);
#endif
  free(bstr->_bits);
  free(bstr);
}

bstr_err_t bstr_resize(bstr_bitstr_t *const bstr, unsigned int capacity) {
#ifdef DEBUG
  assert(bstr != NULL);
  assert(capacity > 0);
#endif

  if (capacity == bstr->_capacity)
    return BSTR_NO_ERROR;

  unsigned int *newMem =
      (unsigned int *)realloc(bstr->_bits, capacity * sizeof(unsigned int));
  if (newMem == NULL)
    return BSTR_MALLOC_FAILED;

  if (capacity < bstr->_capacity) {
    bstr->_bits = newMem;
    bstr->_capacity = capacity;
    return BSTR_NO_ERROR;
  } else {
    bstr->_bits = newMem;
    for (int i = bstr->_capacity; i != capacity; i++) {
      unsigned int *target = bstr->_bits + i;
      *target = 0;
    }
    bstr->_capacity = capacity;
    return BSTR_NO_ERROR;
  }
  return BSTR_NO_ERROR;
}

unsigned int bstr_get_capacity(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  return bstr->_capacity;
}

unsigned int bstr_get_bit_capacity(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  return bstr->_capacity * sizeof(unsigned int) * CHAR_BIT;
}

size_t bstr_to_string_size(const bstr_bitstr_t *const bstr) {
  //                                   bits per byte    trailing \0
  return (bstr->_capacity * sizeof(unsigned int) * CHAR_BIT) + 1;
}

void bstr_to_string(const bstr_bitstr_t *const bstr, char *const str) {
  const unsigned int len = bstr_get_bit_capacity(bstr);
  for (unsigned int i = 0; i != len; i++) {
    if (bstr_get(bstr, i)) {
      strncat(str, "1", 2);
    } else {
      strncat(str, "0", 2);
    }
  }
}

void bstr_bindump(const bstr_bitstr_t *const bstr, char *const str,
                  const unsigned int line) {
#ifdef DEBUG
  assert(bstr != NULL);
  assert(str != NULL);
#endif
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
  assert(line < bstr->_capacity);
#endif
  unsigned int *target = bstr->_bits + line;
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
  assert(!_bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  snprintf(str, BSTR_BINDUMP_SIZE, "%p:", target);
  const unsigned int num_bits = sizeof(unsigned int) * CHAR_BIT;
  for (unsigned int bit = num_bits; bit > 0; bit--) {
    unsigned int bitval = ((*target) >> (bit - 1)) & 1U;
    if (bit % CHAR_BIT == 0)
      strncat(str, " ", 2);
    if (bitval > 0) {
      strncat(str, "1", 2);
    } else {
      strncat(str, "0", 2);
    }
  }
}

void bstr_set(bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *target = _bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
  assert(!_bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_set = bit % (sizeof(unsigned int) * CHAR_BIT);
  *target |= 1 << bit_to_set;
  return;
}

void bstr_set_all(bstr_bitstr_t *const bstr, bool on) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned char value = 0;
  if (on)
    value = UCHAR_MAX;
  memset(bstr->_bits, value, bstr->_capacity * sizeof(unsigned int));
}

void bstr_clr(bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *target = _bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
  assert(!_bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_clear = bit % (sizeof(unsigned int) * CHAR_BIT);
  *target &= ~(1 << bit_to_clear);
}

bool bstr_get(const bstr_bitstr_t *const bstr, unsigned int bit) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *target = _bstr_get_int_for_bit_index(bstr, bit);
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
  assert(!_bstr_is_ptr_out_of_bounds(bstr, target));
#endif
  unsigned int bit_to_get = bit % (sizeof(unsigned int) * CHAR_BIT);
  unsigned int result = ((*target) >> bit_to_get) & 1U;
  if (result > 0) {
    return true;
  } else {
    return false;
  }
}

int bstr_ffs(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  unsigned int offset = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
    assert(!_bstr_is_ptr_out_of_bounds(bstr, i));
#endif
    int result = ffs(*i);
    if (result > 0)
      return (offset * sizeof(unsigned int) * CHAR_BIT) + result - 1;
    offset++;
  }
  return -1;
}

int bstr_ffus(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  int offset = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
    assert(!_bstr_is_ptr_out_of_bounds(bstr, i));
#endif
    if (*i == UINT_MAX) {
      offset += sizeof(unsigned int) * CHAR_BIT;
      continue;
    }
    for (int bit = offset; bit != bstr_get_bit_capacity(bstr); bit++) {
      if (!bstr_get(bstr, bit))
        return bit;
    }
  }
  return -1;
}

int bstr_ctz(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  int result = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
    assert(!_bstr_is_ptr_out_of_bounds(bstr, i));
#endif
    if (*i == 0) {
      result += sizeof(unsigned int) * CHAR_BIT;
      continue;
    }
    result += __builtin_ctz(*i);
    return result;
  }
  return result;
}

int bstr_clz(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *baseptr = bstr->_bits + bstr->_capacity - 1;
  int result = 0;
  for (unsigned int *i = baseptr; i >= bstr->_bits; i--) {
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
    assert(!_bstr_is_ptr_out_of_bounds(bstr, i));
#endif
    if (*i == 0) {
      result += sizeof(unsigned int) * CHAR_BIT;
      continue;
    } else {
      return result + __builtin_clz(*i);
    }
  }
  return result;
}

int bstr_popcnt(const bstr_bitstr_t *const bstr) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  unsigned int *targetptr = bstr->_bits + bstr->_capacity;
  int popcnt = 0;
  for (unsigned int *i = bstr->_bits; i < targetptr; i++) {
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
    assert(!_bstr_is_ptr_out_of_bounds(bstr, i));
#endif
    popcnt += __builtin_popcount(*i);
  }
  return popcnt;
}

int bstr_next_set_bit(const bstr_bitstr_t *const bstr, unsigned int offset) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  const unsigned int cap = bstr_get_bit_capacity(bstr);
  for (unsigned int i = offset; i < cap; i++) {
    if (bstr_get(bstr, i))
      return i;
  }
  return -1;
}

int bstr_next_unset_bit(const bstr_bitstr_t *const bstr, unsigned int offset) {
#ifdef DEBUG
  assert(bstr != NULL);
#endif
  const unsigned int cap = bstr_get_bit_capacity(bstr);
  for (unsigned int i = offset; i < cap; i++) {
    if (!bstr_get(bstr, i))
      return i;
  }
  return -1;
}

#ifdef __cplusplus
}
#endif
