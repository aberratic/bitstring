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

#ifndef BSTR_BITSTRING_H
#define BSTR_BITSTRING_H

#include "limits.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"
#ifdef CONFIG_BITSTRING_ENABLE_BOUND_CHECKS
#include "assert.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief How much space you have to allocate for one line of bindump.
 *
 */
#define BSTR_BINDUMP_SIZE                                                      \
  ((sizeof(char *) * 2) + 2) + 2 + (sizeof(unsigned int) * 8) +                \
      sizeof(unsigned int) + 2

/**
 * @brief A descriptive error type for all operation which could fail.
 *
 */
typedef enum bstr_err_t {
  /**
   * @brief No error occured
   *
   */
  BSTR_NO_ERROR = 0,
  /**
   * @brief Memory allocation failed
   *
   */
  BSTR_MALLOC_FAILED = -1,
} bstr_err_t;

/**
 * @brief This is the main bit string object. Create it with
 * <bstr_create_bitstr> to ensure correct initialization.
 * Delete it with bstr_delete_bitstr().
 *
 */
typedef struct bstr_bitstr_t {
  /**
   * @brief Number of unsigned int allocated at _bits.
   * Note: This field is private use bstr_get_capacity() and DO NOT EVEN THINK
   * about setting this field from outside!
   *
   */
  unsigned int _capacity;
  /**
   * @brief Pointer to our internal array
   * Note: This field is private. Use the functions below to access data stored
   * here.
   *
   */
  unsigned int *_bits;
} bstr_bitstr_t;

/**
 * @brief Method to create and initialize a bitstring object. Returns NULL when
 * there is no memory left.
 *
 * @param capacity Number of unsigned int that are going to be allocated.
 * capacity * sizeof(unsigned int) * 8 == capacity for bit storage.
 *
 * @return bstr_bitstr_t*
 */
bstr_bitstr_t *bstr_create_bitstr(unsigned int capacity);

/**
 * @brief Free all allocated datastructures. Do not use the bstr pointer
 * afterwards.
 *
 * @param bstr Pointer to bitstring object.
 */
void bstr_delete_bitstr(bstr_bitstr_t *bstr) __attribute__((nonnull(1)));

/**
 * @brief Resize the bitstring.
 *
 * @param bstr Pointer to bitstring object.
 * @param capacity Number of unsigned ints that store bits.
 * capacity * sizeof(unsigned int) * 8 == capacity for bit storage.
 */
bstr_err_t bstr_resize(bstr_bitstr_t *const bstr, unsigned int capacity)
    __attribute__((nonnull(1)));

/**
 * @brief Returns the number of unsigned ints that are allocated
 *
 * @param bstr Pointer to bitstring object.
 * @return unsigned int
 */
unsigned int bstr_get_capacity(const bstr_bitstr_t *const bstr)
    __attribute__((nonnull(1)));

/**
 * @brief Returns the number of bits that are stored here
 *
 * @param bstr Pointer to bitstring object
 * @return unsigned int
 */
unsigned int bstr_get_bit_capacity(const bstr_bitstr_t *const bstr)
    __attribute__((nonnull(1)));

/**
 * @brief Returns the size of a formatted string which would print the complete
 * bistring including the \0 on the end.
 *
 * @param bstr Pointer to bitstring object
 * @return size_t
 */
size_t bstr_to_string_size(const bstr_bitstr_t *const bstr)
    __attribute__((nonnull(1)));

/**
 * @brief Returns a long list of zeros and ones. Call bstr_to_string_size()
 * first to get the needed buffer size.
 *
 * @param bstr
 * @param str
 */
void bstr_to_string(const bstr_bitstr_t *const bstr, char *const str)
    __attribute__((nonnull(1, 2)));

/**
 * @brief Prints a fancy dump line by line. The linesize is statically known and
 * available at BSTR_BINDUMP_SIZE .
 *
 * Example:
 *     char bdump[BSTR_BINDUMP_SIZE] = {0};
 *     for (unsigned int j = 0; j < bstr_get_capacity(bstr); j++) {
 *         bstr_bindump(bstr, (char *const)&bdump, j);
 *         printf("%s\n", bdump);
 *     }
 *
 * @param bstr Pointer to bitstring object
 * @param str Pointer to a reasonable sized buffer
 * @param line Linenumber to print. Has to be < bstr_get_capacity()
 */
void bstr_bindump(const bstr_bitstr_t *const bstr, char *const str,
                  const unsigned int line) __attribute__((nonnull(1, 2)));

/**
 * @brief Set a bit in bitstring.
 *
 * @param bstr Pointer to bitstring object.
 * @param bit Number of the bit which will be set. Will panic when a out of
 * bounds access happens.
 */
void bstr_set(bstr_bitstr_t *const bstr, unsigned int bit)
    __attribute__((nonnull(1)));

/**
 * @brief Set all bits to the value of @param on.
 *
 * @param bstr Pointer to bitstring object.
 * @param on Wheter all bits should be set or not.
 */
void bstr_set_all(bstr_bitstr_t *const bstr, bool on)
    __attribute__((nonnull(1)));

/**
 * @brief Clears a bit in bitstring.
 *
 * @param bstr Pointer to bitstring object.
 * @param bit Number of the bit which will be set. Will panic when a out of
 * bounds access happens.
 *
 */
void bstr_clr(bstr_bitstr_t *const bstr, unsigned int bit)
    __attribute__((nonnull(1)));

/**
 * @brief Check if a bit is set.
 *
 * @param bstr Pointer to bitstring object.
 * @param bit Number of the bit which will be set. Will panic when a out of
 * bounds access happens.
 *
 * @return - true   when set
 *         - false  when not set
 */
bool bstr_get(const bstr_bitstr_t *const bstr, unsigned int bit)
    __attribute__((nonnull(1)));

/**
 * @brief Find the first set bit.
 *
 * @param bstr Pointer to bitstring object.
 * @return int Index of the first set bit or -1 when there was none.
 */
int bstr_ffs(const bstr_bitstr_t *const bstr) __attribute__((nonnull(1)));

/**
 * @brief Find the first unset bit.
 *
 * @param bstr Pointer to bitstring object.
 * @return int Index of the first unset bit or -1 when there was none.
 */
int bstr_ffus(const bstr_bitstr_t *const bstr) __attribute__((nonnull(1)));

/**
 * @brief Count trailing zeros.
 *
 * @param bstr Pointer to bitstring object.
 * @return int Count of trailing zeros.
 */
int bstr_ctz(const bstr_bitstr_t *const bstr) __attribute__((nonnull(1)));

/**
 * @brief Count leading zeros.
 *
 * @param bstr Pointer to bitstring object.
 * @return int Count of leading zeros.
 */
int bstr_clz(const bstr_bitstr_t *const bstr) __attribute__((nonnull(1)));

/**
 * @brief Count how many bits are set.
 *
 * @param bstr Pointer to bitstring object.
 * @return int How many bits are set.
 */
int bstr_popcnt(const bstr_bitstr_t *const bstr) __attribute__((nonnull(1)));

#ifdef __cplusplus
}
#endif
#endif
