//
//  utils.h
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#ifndef BC_SEEDTOOL_UTILS_H
#define BC_SEEDTOOL_UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

char* data_to_hex(uint8_t* in, size_t insz);
bool hex_digit_to_bin(const char hex, char *out);
size_t hex_to_data(const char *hex, uint8_t **out);
bool equal_strings(const char* a, const char* b);

uint8_t* alloc_uint8_buffer(size_t len, uint8_t value);
uint16_t* alloc_uint16_buffer(size_t len, uint16_t value);

bool equal_uint8_buffers(const uint8_t* buf1, size_t len1, const uint8_t* buf2, size_t len2);
bool equal_uint16_buffers(const uint16_t* buf1, size_t len1, const uint16_t* buf2, size_t len2);

uint8_t* data_to_base(size_t base, const uint8_t* buf, size_t count);
char* data_to_alphabet(const uint8_t* data, size_t count, size_t base, char* (to_alphabet)(size_t));
char* data_to_ints(const uint8_t* in, size_t count, size_t low, size_t high, const char* separator);

#endif /* BC_SEEDTOOL_UTILS_H */
