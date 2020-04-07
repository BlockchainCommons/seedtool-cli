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
#include <math.h>
#include <stdio.h>

#include <bc-crypto-base/bc-crypto-base.h>

char* data_to_hex(uint8_t* in, size_t insz)
{
  auto out = (char*)malloc(insz * 2 + 1);
  auto pin = in;
  auto hex = "0123456789abcdef";
  auto pout = out;
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

	auto len = strlen(hex);
	if (len % 2 != 0)
		return 0;
	len /= 2;

	*out = (uint8_t*)malloc(len);
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
  auto buf = (uint8_t*)malloc(len);
  for(auto i = 0; i < len; i++) {
    buf[i] = value;
  }
  return buf;
}

uint16_t* alloc_uint16_buffer(size_t len, uint16_t value) {
  auto buf = (uint16_t*)malloc(len * sizeof(uint16_t));
  for(auto i = 0; i < len; i++) {
    buf[i] = value;
  }
  return buf;
}

bool equal_uint8_buffers(const uint8_t* buf1, size_t len1, const uint8_t* buf2, size_t len2) {
  if (len1 != len2) {
    return false;
  }

  for(auto i = 0; i < len1; i++) {
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

  for(auto i = 0; i < len1; i++) {
    if (buf1[i] != buf2[i]) {
      return false;
    }
  }

  return true;
}

uint8_t* data_to_base(size_t base, const uint8_t* buf, size_t count) {
    assert(2 <= base && base <= 256);

    auto out = (uint8_t*)malloc(count);
    if (base < 256) {
        for(auto i = 0; i < count; i++) {
            out[i] = roundf(buf[i] / 255.0 * (base - 1));
        }
    } else {
        memcpy(out, buf, count);
    }
    return out;
}

char* data_to_alphabet(const uint8_t* in, size_t count, size_t base, char* (to_alphabet)(size_t)) {
    auto data = data_to_base(base, in, count);

    size_t len = 0;
    for(int i = 0; i < count; i++) {
        size_t d = data[i];
        assert(d < base);
        auto a = to_alphabet(d);
        len += strlen(a);
        free(a);
    }

    auto string = (char*)malloc(len + 1);
    string[0] = '\0';
    for(auto i = 0; i < count; i++) {
        auto a = to_alphabet(data[i]);
        strcat(string, a);
        free(a);
    }

    free(data);

    return string;
}

static void int_to_string(int n, char* out) {
    sprintf(out, "%d", n);
}

char* data_to_ints(const uint8_t* in, size_t count, size_t low, size_t high, const char* separator) {
    if(!(0 <= low && low < high && high <= 255)) { return NULL; }

    size_t base = high - low + 1;
    auto data = data_to_base(base, in, count);

    auto separator_len = strlen(separator);

    size_t len = 0;
    char buf[10];
    for(auto i = 0; i < count; i++) {
        data[i] += low;

        if(i > 0) {
            len += separator_len;
        }
        size_t d = data[i];
        int_to_string(d, buf);
        len += strlen(buf) + separator_len;
    }

    auto string = (char*)malloc(len + 1);
    string[0] = '\0';
    for(auto i = 0; i < count; i++) {
        if(i > 0) {
            strcat(string, separator);
        }
        int_to_string(data[i], buf);
        strcat(string, buf);
    }

    free(data);

    return string;
}
