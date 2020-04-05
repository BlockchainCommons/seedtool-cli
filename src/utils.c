//
//  utils.c
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#include "utils.h"

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <bc-crypto-base/bc-crypto-base.h>

char* data_to_hex(uint8_t* in, size_t insz)
{
  char* out = malloc(insz * 2 + 1);
  uint8_t* pin = in;
  const char * hex = "0123456789abcdef";
  char* pout = out;
  for(; pin < in + insz; pout += 2, pin++){
    pout[0] = hex[(*pin>>4) & 0xF];
    pout[1] = hex[ *pin     & 0xF];
  }
  pout[0] = 0;
  return out;
}

bool hex_digit_to_bin(const char hex, char *out) {
	if (out == NULL)
		return false;

	if (hex >= '0' && hex <= '9') {
		*out = hex - '0';
	} else if (hex >= 'A' && hex <= 'F') {
		*out = hex - 'A' + 10;
	} else if (hex >= 'a' && hex <= 'f') {
		*out = hex - 'a' + 10;
	} else {
		return false;
	}

	return true;
}

size_t hex_to_data(const char *hex, uint8_t **out) {
	if (hex == NULL || *hex == '\0') {
        *out = NULL;
		return 0;
    }
    if (out == NULL) {
        return 0;
    }

	size_t len = strlen(hex);
	if (len % 2 != 0)
		return 0;
	len /= 2;

	*out = malloc(len);
	for (size_t i = 0; i < len; i++) {
  	char b1;
  	char b2;
		if (!hex_digit_to_bin(hex[i * 2], &b1) || !hex_digit_to_bin(hex[i * 2 + 1], &b2)) {
			return 0;
		}
		(*out)[i] = (b1 << 4) | b2;
	}
	return len;
}

bool equal_strings(const char* a, const char* b) {
  return strcmp(a, b) == 0;
}

uint8_t* alloc_uint8_buffer(size_t len, uint8_t value) {
  uint8_t* buf = malloc(len);
  for(int i = 0; i < len; i++) {
    buf[i] = value;
  }
  return buf;
}

uint16_t* alloc_uint16_buffer(size_t len, uint16_t value) {
  uint16_t* buf = malloc(len * sizeof(uint16_t));
  for(int i = 0; i < len; i++) {
    buf[i] = value;
  }
  return buf;
}

bool equal_uint8_buffers(const uint8_t* buf1, size_t len1, const uint8_t* buf2, size_t len2) {
  if (len1 != len2) {
    return false;
  }

  for(int i = 0; i < len1; i++) {
    if (buf1[i] != buf2[i]) {
      return false;
    }
  }

  return true;
}

bool equal_uint16_buffers(const uint16_t* buf1, size_t len1, const uint16_t* buf2, size_t len2) {
  if (len1 != len2) {
    return false;
  }

  for(int i = 0; i < len1; i++) {
    if (buf1[i] != buf2[i]) {
      return false;
    }
  }

  return true;
}
