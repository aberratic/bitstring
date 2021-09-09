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
#include "bitstring.h"

int main(void) {
  // Step 1: create a bitstring object
  // The argument determines how big the bitstring will be. It is measured in
  // unsigned int.
  bstr_bitstr_t *example = bstr_create_bitstr(4);

  // To check how big our bitstring is we can use the get_capacity function.
  // It returns the size measured in unsigned int.
  assert(bstr_get_capacity(example) == 4);

  // To find out how many bits we can store there is a get_bit_capacity
  // function.
  assert(bstr_get_bit_capacity(example) == sizeof(unsigned int) * CHAR_BIT * 4);

  // You may want to print a bitstring. There are two methods of doing so.
  // Method one: A plain string
  // First you need to determine how big your string will be.
  size_t strSize = bstr_to_string_size(example);
  char *str = (char *)alloca(strSize);

  // Then you can fill the buffer
  bstr_to_string(example, str);
  printf("An example to_string printout\n");
  // You may print it now:
  printf("%s\n", str);

  // Method two: The fancy bindump method
  // Here you also need to allocate enough space for a string. Luckily the
  // string size is known at compile time and is defined as BSTR_BINDUMP_SIZE
  char bdump[BSTR_BINDUMP_SIZE] = {0};

  // The bindump interface is line based. So we need to iterate through our
  // object. The number of lines is equal to the capacity.
  printf("An example bindump printout:\n");
  for (unsigned int i = 0; i < bstr_get_capacity(example); i++) {

    // Then we can fill our buffer
    bstr_bindump(example, (char *const)&bdump, i);
    printf("%s\n", bdump);
  }

  // To set a bit we can use the following function:
  // Please ensure that the bit index is smaller than bstr_get_bit_capacity()
  bstr_set(example, 23);

  // To get a bit value we use the corresponding function:
  assert(bstr_get(example, 23));

  // There may be situation where we want to set all bits to a specific value
  // the bstr_set_all() function will come in handy
  bstr_set_all(example, true);

  // To check this works we can use the bstr_popcnt() function. It will count
  // all set bits.
  assert(bstr_popcnt(example) == bstr_get_bit_capacity(example));

  // Clearing a bit is easy. Lets do it several times.
  bstr_clr(example, 0);
  bstr_clr(example, 1);
  bstr_clr(example, 2); // Flipping bits is fun
  bstr_clr(example, 3);

  // To find the first set bit, we use the ffs() function:
  assert(4 == bstr_ffs(example));

  // We can also find the first unset bit
  assert(0 == bstr_ffus(example));

  // There is also a function to count all trailing zeros
  assert(4 == bstr_ctz(example));

  // There is also a function to count all leading zeros. Before we can use
  // it we need to flip some bits.
  bstr_set_all(example, true);
  bstr_clr(example, bstr_get_bit_capacity(example) - 1);
  bstr_clr(example, bstr_get_bit_capacity(example) - 2);
  assert(2 == bstr_clz(example));

  // Resizing the bitstring is also easy
  bstr_err_t error_you_should_check = bstr_resize(example, 32);
  if (error_you_should_check != BSTR_NO_ERROR) {
    // the internal call to malloc failed. The existing structure will remain
    // untouched but you may want to free up some memory
  }

  // Last Step: delete the object
  bstr_delete_bitstr(example);
  printf("dynamic_sized_example ran successfully!");
  return EXIT_SUCCESS;
}
