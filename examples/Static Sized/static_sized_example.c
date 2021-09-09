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

#include "assert.h"
#include "bitstring_static.h"

// Step one: Use either the convinient DECLARE_ALL macro to declare a complete
// implementation of a static sized bitstring with just one call or declare the
// functions one by one. You may need to check for dependencies between
// functions. After all this is not recommended.
BSTR_STATIC_DECLARE_ALL(16);

int main(void) {

  // Step two: declaration and initialization
  bstr_static_t(16) example = bstrs_initialize;

  // If you ever can't remember how big your datastructure is there is this
  // macro: (Your compiler will most likely optimize this away as it is known at
  // compile time).
  assert(16 == bstrs_get_capacity(16));

  // You may want to find out how many bits are available for your bit storing
  // needs. Here you go:
  assert((16 * sizeof(unsigned int) * CHAR_BIT) == bstrs_get_bit_capacity(16));

  // There are two ways of printing the contents of this bitstring. Here is
  // option one:
  // First you need to find out how long your string will be
  char str[bstrs_to_string_size(16)] = {0};

  bstrs_to_string(16, &example, str);
  printf("Here is an example to_string() output:\n%s\n", str);

  // The more fancier way is to use bindump:
  // Here you also need to allocate enough space for a string. Luckily the
  // string size is known at compile time and is defined as BSTR_BINDUMP_SIZE
  char bdump[BSTR_BINDUMP_SIZE] = {0};

  // The bindump interface is line based. So we need to iterate through our
  // object. The number of lines is equal to the capacity.
  printf("An example bindump printout:\n");
  for (unsigned int i = 0; i < bstrs_get_capacity(16); i++) {

    // Then we can fill our buffer
    bstrs_bindump(16, &example, (char *const)&bdump, i);
    printf("%s\n", bdump);
  }

  // Here are some examples to manipulate the data
  // Set a bit
  bstrs_set(16, &example, 0);

  // Query a bit
  assert(bstrs_get(16, &example, 0));

  // And clear a bit
  bstrs_clr(16, &example, 0);

  // Proof:
  assert(!bstrs_get(16, &example, 0));

  // You may want to set/unset all:
  bstrs_set_all(16, &example, true);

  // Then there are convinient functions to get several specific metrics about
  // this bitstring:
  // popcnt: Popcount - count how many bits are set
  assert(bstrs_get_bit_capacity(16) == bstrs_popcnt(16, &example));

  bstrs_clr(16, &example, bstrs_get_bit_capacity(16) - 1);
  // Which is the first unset bit?
  assert(bstrs_ffus(16, &example) == bstrs_get_bit_capacity(16) - 1);

  bstrs_clr(16, &example, 0);
  // and which one the first set?
  assert(bstrs_ffs(16, &example) == 1);

  bstrs_clr(16, &example, bstrs_get_bit_capacity(16) - 2);
  bstrs_clr(16, &example, bstrs_get_bit_capacity(16) - 3);
  // or how many 0 are leading:
  assert(bstrs_clz(16, &example) == 3);

  // or trailing
  assert(bstrs_ctz(16, &example) == 1);

  // Here comes NO destroy/delete/free call as it is all statically
  // allocated on the stack. Yipee :D
  printf("Successfully ran the static_sized_example\n");
  return EXIT_SUCCESS;
}
