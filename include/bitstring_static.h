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

#ifndef BSTR_BITSTRING_STATIC_H
#define BSTR_BITSTRING_STATIC_H

#include "bitstring_common.h"
#include "limits.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
#include "assert.h"
#define BSTR_STATIC_BOUND_CHECK(size, bst, ptr)                                \
  assert(!_bstr##size##_is_ptr_out_of_bounds(bst, ptr));

#define BSTR_STATIC_DECLARE_BOUND_CHECK(size)                                  \
  __attribute__((nonnull(1, 2))) bool _bstr##size##_is_ptr_out_of_bounds(      \
      const bstr_bitstr##size##_t *const bstr,                                 \
      const unsigned int *const ptr) {                                         \
    if (bstr->_bits + bstr##size##_get_capacity <= ptr || bstr->_bits > ptr)   \
      return true;                                                             \
    return false;                                                              \
  }
#else
#define BSTR_STATIC_BOUND_CHECK(size, bst, ptr)
#define BSTR_STATIC_DECLARE_BOUND_CHECK(size)
#endif

/**
 * @brief Macro to get the type name for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define bstr_static_t(size) bstr_bitstr##size##_t

/**
 * @brief Macro to declare the sized bitstring struct.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_SIZED_BITSTRING_STRUCT(size)                       \
  typedef struct bstr_bitstr##size##_t {                                       \
    unsigned int _bits[size];                                                  \
  } bstr_bitstr##size##_t;

/**
 * @brief Macro to declare a to_string function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_TO_STRING(size)                                    \
  __attribute__((nonnull(1, 2))) void bstr##size##_to_string(                  \
      const bstr_bitstr##size##_t *const bstr, char *const str) {              \
    const unsigned int len = bstrs_get_bit_capacity(size);                     \
    for (unsigned int i = 0; i != len; i++) {                                  \
      if (bstr##size##_get(bstr, i)) {                                         \
        strncat(str, "1", 2);                                                  \
      } else {                                                                 \
        strncat(str, "0", 2);                                                  \
      }                                                                        \
    }                                                                          \
  }

/**
 * @brief Macro to declare a bindump function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_BINDUMP(size)                                      \
  __attribute__((nonnull(1, 2))) void bstr##size##_bindump(                    \
      const bstr_bitstr##size##_t *const bstr, char *const str,                \
      const unsigned int line) {                                               \
    snprintf(str, BSTR_BINDUMP_SIZE, "%p:", &(bstr->_bits[line]));             \
    const unsigned int num_bits = sizeof(unsigned int) * CHAR_BIT;             \
    for (unsigned int bit = num_bits; bit > 0; bit--) {                        \
      unsigned int bitval = (bstr->_bits[line] >> (bit - 1)) & 1U;             \
      if (bit % CHAR_BIT == 0)                                                 \
        strncat(str, " ", 2);                                                  \
      if (bitval > 0) {                                                        \
        strncat(str, "1", 2);                                                  \
      } else {                                                                 \
        strncat(str, "0", 2);                                                  \
      }                                                                        \
    }                                                                          \
  }

/**
 * @brief Macro to declare a _get_int_for_bit_index function for a sized
 * bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE__GET_INT_FOR_BIT_INDEX(size)                       \
  static inline unsigned int *_bstr##size##_get_int_for_bit_index(             \
      const bstr_bitstr##size##_t *const bstr, unsigned int bit) {             \
    return (unsigned int *)(bstr->_bits +                                      \
                            (bit / (sizeof(unsigned int) * CHAR_BIT)));        \
  }

/**
 * @brief Macro to declare a _set function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_SET(size)                                          \
  __attribute__((nonnull(1))) void bstr##size##_set(                           \
      bstr_bitstr##size##_t *const bstr, unsigned int bit) {                   \
    unsigned int *target = _bstr##size##_get_int_for_bit_index(bstr, bit);     \
    BSTR_STATIC_BOUND_CHECK(size, bstr, target)                                \
    unsigned int bit_to_set = bit % ((sizeof(unsigned int)) * CHAR_BIT);       \
    *target |= 1 << bit_to_set;                                                \
  }

/**
 * @brief Macro to declare a _set_all function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_SET_ALL(size)                                      \
  __attribute__((nonnull(1))) void bstr##size##_set_all(                       \
      bstr_bitstr##size##_t *const bstr, bool on) {                            \
    unsigned char value = 0;                                                   \
    if (on)                                                                    \
      value = UCHAR_MAX;                                                       \
    memset(bstr->_bits, value,                                                 \
           bstrs_get_capacity(size) * sizeof(unsigned int));                   \
  }

/**
 * @brief Macro to declare a _clr function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_CLR(size)                                          \
  __attribute__((nonnull(1))) void bstr##size##_clr(                           \
      bstr_bitstr##size##_t *const bstr, unsigned int bit) {                   \
    unsigned int bit_to_clear = bit % (sizeof(unsigned int) * CHAR_BIT);       \
    unsigned int *target = _bstr##size##_get_int_for_bit_index(bstr, bit);     \
    BSTR_STATIC_BOUND_CHECK(size, bstr, target)                                \
    *target &= ~(1 << bit_to_clear);                                           \
  }

/**
 * @brief Macro to declare a _get function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_GET(size)                                          \
  __attribute__((nonnull(1))) bool bstr##size##_get(                           \
      const bstr_bitstr##size##_t *const bstr, unsigned int bit) {             \
    unsigned int *target = _bstr##size##_get_int_for_bit_index(bstr, bit);     \
    unsigned int bit_to_get = bit % (sizeof(unsigned int) * CHAR_BIT);         \
    unsigned int result = ((*target) >> bit_to_get) & 1U;                      \
    BSTR_STATIC_BOUND_CHECK(size, bstr, target)                                \
    if (result > 0) {                                                          \
      return true;                                                             \
    } else {                                                                   \
      return false;                                                            \
    }                                                                          \
  }

/**
 * @brief Macro to declare a _ffs function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_FFS(size)                                          \
  __attribute__((nonnull(1))) int bstr##size##_ffs(                            \
      const bstr_bitstr##size##_t *const bstr) {                               \
    unsigned int offset = 0;                                                   \
    for (unsigned int i = 0; i < bstrs_get_capacity(size); i++) {              \
      int result = ffs(bstr->_bits[i]);                                        \
      if (result > 0)                                                          \
        return (offset * sizeof(unsigned int) * CHAR_BIT) + result - 1;        \
      offset++;                                                                \
    }                                                                          \
    return -1;                                                                 \
  }

/**
 * @brief Macro to declare a _ffus function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_FFUS(size)                                         \
  __attribute__((nonnull(1))) int bstr##size##_ffus(                           \
      const bstr_bitstr##size##_t *const bstr) {                               \
    int offset = 0;                                                            \
    for (unsigned int i = 0; i < bstrs_get_capacity(size); i++) {              \
      if (bstr->_bits[i] == UINT_MAX) {                                        \
        offset += sizeof(unsigned int) * CHAR_BIT;                             \
        continue;                                                              \
      }                                                                        \
      for (int bit = offset; bit != bstrs_get_bit_capacity(size); bit++) {     \
        if (!bstr##size##_get(bstr, bit))                                      \
          return bit;                                                          \
      }                                                                        \
    }                                                                          \
    return -1;                                                                 \
  }

/**
 * @brief Macro to declare a _ctz function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_CTZ(size)                                          \
  __attribute__((nonnull(1))) int bstr##size##_ctz(                            \
      const bstr_bitstr##size##_t *const bstr) {                               \
    int result = 0;                                                            \
    for (unsigned int i = 0; i < bstrs_get_capacity(size); i++) {              \
      if (bstr->_bits[i] == 0) {                                               \
        result += sizeof(unsigned int) * CHAR_BIT;                             \
        continue;                                                              \
      }                                                                        \
      result += __builtin_ctz(bstr->_bits[i]);                                 \
      return result;                                                           \
    }                                                                          \
    return result;                                                             \
  }

/**
 * @brief Macro to declare a _clz function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_CLZ(size)                                          \
  __attribute__((nonnull(1))) int bstr##size##_clz(                            \
      const bstr_bitstr##size##_t *const bstr) {                               \
    unsigned int *baseptr =                                                    \
        (unsigned int *)bstr->_bits + bstrs_get_capacity(size) - 1;            \
    int result = 0;                                                            \
    for (unsigned int *i = baseptr; i >= bstr->_bits; i--) {                   \
      if (*i == 0) {                                                           \
        result += sizeof(unsigned int) * CHAR_BIT;                             \
        continue;                                                              \
      } else {                                                                 \
        return result + __builtin_clz(*i);                                     \
      }                                                                        \
    }                                                                          \
    return result;                                                             \
  }

/**
 * @brief Macro to declare a _popcnt function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_POPCNT(size)                                       \
  __attribute__((nonnull(1))) int bstr##size##_popcnt(                         \
      const bstr_bitstr##size##_t *const bstr) {                               \
    int popcnt = 0;                                                            \
    for (unsigned int i = 0; i < bstrs_get_capacity(size); i++) {              \
      popcnt += __builtin_popcount(bstr->_bits[i]);                            \
    }                                                                          \
    return popcnt;                                                             \
  }

/**
 * @brief Macro to declare a _next_set_bit function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_NEXT_SET_BIT(size)                                 \
  __attribute__((nonnull(1))) int bstr##size##_next_set_bit(                   \
      const bstr_bitstr##size##_t *const bstr, unsigned int offset) {          \
    for (unsigned int i = offset; i < bstrs_get_bit_capacity(size); i++) {     \
      if (bstr##size##_get(bstr, i))                                           \
        return i;                                                              \
    }                                                                          \
    return -1;                                                                 \
  }

/**
 * @brief Macro to declare a _next_unset_bit function for a sized bitstring.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_NEXT_UNSET_BIT(size)                               \
  __attribute__((nonnull(1))) int bstr##size##_next_unset_bit(                 \
      const bstr_bitstr##size##_t *const bstr, unsigned int offset) {          \
    for (unsigned int i = offset; i < bstrs_get_bit_capacity(size); i++) {     \
      if (!bstr##size##_get(bstr, i))                                          \
        return i;                                                              \
    }                                                                          \
    return -1;                                                                 \
  }

/**
 * @brief A convinience macro to declare a complete sized bitstring
 * implemenation. This is the recommended way of creating a static sized
 * bitstring. If you want to use it at several different places you may
 * call this macro in a dedicated header file.
 *
 * @param size How many unsigned ints this bitstring contains.
 */
#define BSTR_STATIC_DECLARE_ALL(size)                                          \
  BSTR_STATIC_DECLARE_SIZED_BITSTRING_STRUCT(size);                            \
  BSTR_STATIC_DECLARE_BOUND_CHECK(size);                                       \
  BSTR_STATIC_DECLARE__GET_INT_FOR_BIT_INDEX(size);                            \
  BSTR_STATIC_DECLARE_GET(size);                                               \
  BSTR_STATIC_DECLARE_TO_STRING(size);                                         \
  BSTR_STATIC_DECLARE_BINDUMP(size);                                           \
  BSTR_STATIC_DECLARE_SET(size);                                               \
  BSTR_STATIC_DECLARE_SET_ALL(size);                                           \
  BSTR_STATIC_DECLARE_CLR(size);                                               \
  BSTR_STATIC_DECLARE_FFS(size);                                               \
  BSTR_STATIC_DECLARE_FFUS(size);                                              \
  BSTR_STATIC_DECLARE_CTZ(size);                                               \
  BSTR_STATIC_DECLARE_CLZ(size);                                               \
  BSTR_STATIC_DECLARE_POPCNT(size);                                            \
  BSTR_STATIC_DECLARE_NEXT_SET_BIT(size);                                      \
  BSTR_STATIC_DECLARE_NEXT_UNSET_BIT(size);

/**
 * @brief Macro that creates the rvalue for a sized bitstream initialization
 *
 */
#define bstrs_initialize {._bits = {0}};

/**
 * @brief Macro that creates a typesafe function call to get_capacity
 *
 * @param size How many unsigned ints this bitstring contains.
 *
 * @return unsigned int How many unsigned ints are in this bitstring.
 */
#define bstrs_get_capacity(size) (size)

/**
 * @brief Macro that creates a typesafe function call to get_bit_capacity
 *
 * @param size How many unsigned ints this bitstring contains.
 *
 * @return unsigned int How many bits can be stored in this bitstring.
 */
#define bstrs_get_bit_capacity(size) (size * sizeof(unsigned int) * CHAR_BIT)

/**
 * @brief Macro that creates a typesafe function call to to_stream_size
 *
 * @param size How many unsigned ints this bitstring contains.
 *
 * @return size_t Returns how many bytes are needed to store a human readable
 * string representing this bitstring. It includes the trailing \0.
 */
#define bstrs_to_string_size(size)                                             \
  ((size * sizeof(unsigned int) * CHAR_BIT) + 1)

/**
 * @brief Macro that creates a typesafe function call to to_string
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst_t *const Pointer to the bitstring object.
 * @param str char *const Pointer to a string with at least to_string_size size.
 */
#define bstrs_to_string(size, bst, str) bstr##size##_to_string(bst, str)

/**
 * @brief Macro that creates a typesafe function call to _bindump
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst_t *const Pointer to the bitstring object.
 * @param str char *const Pointer to a astring with at least BSTR_BINDUMP_SIZE
 * size.
 * @param line const unsigned int Which line should be printed. line has to be
 * smaller than _get_capacity
 */
#define bstrs_bindump(size, bst, str, line) bstr##size##_bindump(bst, str, line)

/**
 * @brief Macro that creates a typesafe function call to _set
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst *const Pointer to the bitstring object.
 * @param bit unsigned int Index of the bit to set.
 */
#define bstrs_set(size, bst, bit) bstr##size##_set(bst, bit)

/**
 * @brief Macro that creates a typesafe function call to _set_all
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst  *const Pointer to the bitstring object.
 * @param on bool Whether all bits should be set or unset.
 */
#define bstrs_set_all(size, bst, on) bstr##size##_set_all(bst, on)

/**
 * @brief Macro that creates a typesafe function call to _clr
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst *const Pointer to the bitstring object.
 * @param bit unsigned int Index of the bit to clear.
 */
#define bstrs_clr(size, bst, bit) bstr##size##_clr(bst, bit)

/**
 * @brief Macro that creates a typesafe function call to _get
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 * @param bit unsigned int Index of the bit to get.
 *
 * @return bool True when the bit was set. False when unset.
 */
#define bstrs_get(size, bst, bit) bstr##size##_get(bst, bit)

/**
 * @brief Macro that creates a typesafe function call to _ffs
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 *
 * @return int Index of the first set bit.
 */
#define bstrs_ffs(size, bst) bstr##size##_ffs(bst)

/**
 * @brief Macro that creates a typesafe function call to _ffus
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 *
 * @return int Index of the first unset bit.
 */
#define bstrs_ffus(size, bst) bstr##size##_ffus(bst)

/**
 * @brief Macro that creates a typesafe function call to _ctz. Starts at the
 * least significant bit positon.
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 *
 * @return int Count of trailing zeros.
 */
#define bstrs_ctz(size, bst) bstr##size##_ctz(bst)

/**
 * @brief Macro that creates a typesafe function call to _clz. Starts at the
 * most significant bit position.
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 *
 * @return int Count of leading zeros.
 */
#define bstrs_clz(size, bst) bstr##size##_clz(bst)

/**
 * @brief Macro that creates a typesafe function call to _popcnt
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 *
 * @return int Number of set bits.
 */
#define bstrs_popcnt(size, bst) bstr##size##_popcnt(bst)

/**
 * @brief Macro that creates a typesafe functio call to _next_set_bit . Get the
 * index of the next set bit based upon the offset.
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 * @param offset Where to begin the search.
 */
#define bstrs_next_set_bit(size, bst, offset)                                  \
  bstr##size##_next_set_bit(bst, offset)

/**
 * @brief Macro that creates a typesafe functio call to _next_unset_bit . Get
 * the index of the next unset bit based upon the offset.
 *
 * @param size How many unsigned ints this bitstring contains.
 * @param bst const bst *const Pointer to the bitstring object.
 * @param offset Where to begin the search.
 */
#define bstrs_next_unset_bit(size, bst, offset)                                \
  bstr##size##_next_unset_bit(bst, offset)
#endif
