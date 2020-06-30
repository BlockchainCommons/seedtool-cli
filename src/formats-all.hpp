//
//  formats-all.hpp
//
//  Copyright © 2020 by Blockchain Commons, LLC
//  Licensed under the "BSD-2-Clause Plus Patent License"
//

#pragma once

#include "format-base6.hpp"
#include "format-base10.hpp"
#include "format-bip39.hpp"
#include "format-bits.hpp"
#include "format-cards.hpp"
#include "format-dice.hpp"
#include "format-hex.hpp"
#include "format-ints.hpp"
#include "format-random.hpp"
#include "format-slip39.hpp"
#include "format-bytewords.hpp"
#include "format-bytewords-uri.hpp"
#include "format-bytewords-minimal.hpp"

bool is_random(const Format* f) { return dynamic_cast<const FormatRandom*>(f) != NULL; }
bool is_hex(const Format* f) { return dynamic_cast<const FormatHex*>(f) != NULL; }
bool is_bits(const Format* f) { return dynamic_cast<const FormatBits*>(f) != NULL; }
bool is_cards(const Format* f) { return dynamic_cast<const FormatCards*>(f) != NULL; }
bool is_dice(const Format* f) { return dynamic_cast<const FormatDice*>(f) != NULL; }
bool is_base6(const Format* f) { return dynamic_cast<const FormatBase6*>(f) != NULL; }
bool is_base10(const Format* f) { return dynamic_cast<const FormatBase10*>(f) != NULL; }
bool is_ints(const Format* f) { return dynamic_cast<const FormatInts*>(f) != NULL; }
bool is_bip39(const Format* f) { return dynamic_cast<const FormatBIP39*>(f) != NULL; }
bool is_slip39(const Format* f) { return dynamic_cast<const FormatSLIP39*>(f) != NULL; }
bool is_bytewords(const Format* f) { return dynamic_cast<const FormatBytewords*>(f) != NULL; }
bool is_bytewords_uri(const Format* f) { return dynamic_cast<const FormatBytewordsURI*>(f) != NULL; }
bool is_bytewords_minimal(const Format* f) { return dynamic_cast<const FormatBytewordsMinimal*>(f) != NULL; }
bool is_bytewords_any(const Format* f) { return is_bytewords(f) || is_bytewords_uri(f) || is_bytewords_minimal(f); }
