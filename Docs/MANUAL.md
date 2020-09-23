# 🌱 Seedtool

**Version 0.8.1**<br/>**September 22, 2020**

*Copyright © 2020 by Blockchain Commons, LLC*<br/>*Licensed under the "BSD-2-Clause Plus Patent License"*

---

## Contents

* [Introduction](#introduction)
* [General Functionality](#general-functionality)
* [Generating Random Seeds](#generating-random-seeds)
* [Generating Seeds from Provided Entropy](#generating-seeds-from-provided-entropy)
* [Deterministic Randomness](#deterministic-randomness)
* [Compatibility](#compatibility)
* [Uniform Resources (URs)](#uniform-resources-urs)
* [Version History](#version-history)

## Introduction

`seedtool` is a command-line tool for creating and transforming cryptographic seeds of the sort commonly used by blockchain applications.

See [`README.md`](../README.md) for installation and credits.

## General Functionality

Supports `--help`, `--usage`, and `--version`.

```
$ seedtool --help
<help summary>
```

## Generating Random Seeds

When run by itself, seedtool generates 16 random bytes (128 bits) and displays them as hex. This is the same as running `seedtool --in random --out hex`.

```
#
# Generate a 16-byte random seed.
#

$ seedtool
5b13fe9ac2778e728df31c7b1afd874f

#
# Same as above.
#

$ seedtool --in random --out hex
06799f71d16fad08ec5407d32d670147
```

### Bytewords

The Bytewords format can be used in place of `hex`. Unlike hex, where every possible input is a seed, Bytewords includes error detection.

```
#
# Generate a 16-byte random seed and display it as Bytewords.
#

$ seedtool --out btw
deli need cats taxi dice door webs vows free zest legs wall half waxy trip oval memo sets rock hill

#
# Convert the Bytewords-encoded seed to hex.
#

$ seedtool --in btw "deli need cats taxi dice door webs vows free zest legs wall half waxy trip oval memo sets rock hill"
279b18d0282aefe845fb83e956eed8a6

#
# Convert the same seed to Bytewords URI-compatible format.
#

$ seedtool --in hex --out btwu 279b18d0282aefe845fb83e956eed8a6
deli-need-cats-taxi-dice-door-webs-vows-free-zest-legs-wall-half-waxy-trip-oval-memo-sets-rock-hill

#
# Convert the same seed to Bytewords minimal format.
#

$ seedtool --in hex --out btwm 279b18d0282aefe845fb83e956eed8a6
dindcstidedrwsvsfeztlswlhfwytpolmossrkhl
```
### Outputs & Counts
An output format `--out` and count `--count` may be specified. Count may be in [1-1024] and the default `count` is 16. For the `hex` and Bytewords (`btw`, `btwu`, `btwm`) output formats, the count is the number of bytes generated. For other output formats, `count` is the number of "output units" (e.g., bits, cards, die rolls, etc.)

```
#
# Generate 16 random bytes.
#

$ seedtool --out hex
66a12c8a8c719dd3b2f951053910f849

#
# Generate 16 random bits.
#

$ seedtool --out bits
0110101010011010

#
# Generate 16 random playing cards.
#

$ seedtool --out cards
3d9sqcthkcad8c9hah5dqdtd5c8sjs7s

#
# Roll 16 random 6-sided dice.
#

$ seedtool --out dice
6565245451152635

#
# 6-sided dice, counting from zero.
#

$ seedtool --out base6
0534233032352210

#
# Generate 16 random digits in [0-9].
#

$ seedtool --out base10
9808565945186765

#
# Generate 5 random playing cards.
#

$ seedtool --out cards --count 5
9d6s2s9d8s
```

**✅ NOTE:** Playing cards may be duplicated; it is as if each card is drawn from a freshly shuffled deck.

### Low & High
Sequences of random integers may be generated. By default, `--count` is 16, `--low` is 0, and `--high` is 9.

`low` < `high` < `256`

```
#
# Generate 16 random integers in [0-9].
#

$ seedtool --out ints
9 8 2 2 8 3 4 7 2 1 7 5 9 5 2 1

#
# Generate 8 random integers in [1-100].
#

$ seedtool --out ints --count 8 --low 1 --high 100
76 15 25 57 99 41 4 95
```

### BIP39

BIP39 can be used as an output format. The default `--count` is 16 bytes (128 bits, 12 words), but may be any multiple of 2 in [12,32].

```
#
# Generate a random 16-byte seed and display it as BIP39.
#

$ seedtool --out bip39
sorry pupil battle tortoise ceiling hurdle family market device primary language grit

#
# Generate a random 32-byte seed and display it as BIP39.
#

$ seedtool --out bip39 --count 32
mind knock evoke recycle payment snack pear party mean rubber open work rug trophy federal connect indicate security release three buzz buddy motion game
```

### SSKRs

SSKR can be used as an output format. By default `--count` is 16 bytes (128 bits, 20 words), but may be any multiple of 2 in [16-32]. By default a single 1-of-1 share is generated. This is the same as running `seedtool --out sskr --group-threshold 1 --group 1-of-1`.

```
#
# Generate a random 16-byte seed and display it as SSKR.
#

$ seedtool --out sskr
tuna acid epic gyro memo real able able able lazy hill webs crux judo omit oval grim calm join cash saga solo apex jump body many mint peck gala

#
# Generate a random 32-byte seed and display it as SSKR.
#

$ seedtool --out sskr --count 32
tuna acid epic hard data oval twin able able able wolf free glow high gala barn dark flew zinc flux away visa belt zero bulb door gyro kiwi vibe toil lava jump beta gems huts join inch cyan dark webs fact gush toil inky flap tent
```

n-of-m SSKR shares can be generated by specifing one or more groups.

```
#
# Generate a 16 byte seed and display it as SSKR, one group,
# 2-of-3 shares required for seed recovery.
#

$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro tiny able able acid able real taco wand time obey epic pool pose open when oval inky half idea pose surf glow liar atom kiln
tuna acid epic gyro tiny able able acid acid leaf work wolf vial free jump grim legs mint numb hawk gyro undo bias redo data jolt twin warm arch
tuna acid epic gyro tiny able able acid also unit road trip quad jump rock gift yell scar guru flux able hawk rock lazy arch curl skew luck frog
```

Multiple groups and a group threshold may be specified. If omitted, the default `--group-threshold` is 1.

**✅ NOTE**: The first two words of a set of SSKR shares are the same across all the shares. Each group within a set of SSKR shares has the same third word.

```
#
# Generate a 16 byte seed and display it as SSKR, first group
# requires 2-of-3, second and third groups require 3-of-5, at least
# two groups must meet their threshold.
#

$ seedtool --out sskr --group-threshold 2 --group 2-of-3 --group 3-of-5 --group 3-of-5
tuna acid epic gyro into knob brag acid able list twin skew vast gala zoom yank roof scar omit jowl jury free solo jazz gray vast skew ruby quiz
tuna acid epic gyro into knob brag acid acid tiny bulb hope visa blue zone real inky miss jade drop each wand gems door kiwi fact item gray tied
tuna acid epic gyro into knob brag acid also deli hill webs wasp zest yoga jugs brew jade dice warm zoom able trip vast axis ruin idea inch crux
tuna acid epic gyro into knob brag brag able wave visa lazy able lava kick knob zaps idle echo draw time fact part rock part glow play fish high
tuna acid epic gyro into knob brag brag acid slot brag ruby item love scar open when back tiny inky vial many fact toys grim bulb foxy fair kite
tuna acid epic gyro into knob brag brag also help days lava chef yoga free numb join high belt swan onyx onyx exit duty keno yell time ruin news
tuna acid epic gyro into knob brag brag apex jugs tied roof junk whiz zero king inky easy wand lion meow body play high lung plus fern road roof
tuna acid epic gyro into knob brag brag aqua fish puma kite down ugly half beta zone part taco fact huts luck calm very sets onyx arch yank zone
tuna acid epic gyro into knob brag cusp able iron logo code time sets junk gift eyes play need slot exit aunt cook noon note note skew judo ugly
tuna acid epic gyro into knob brag cusp acid cash jazz what zest guru vibe hawk jazz yawn buzz girl brag dice pose wolf good blue belt axis axis
tuna acid epic gyro into knob brag cusp also arch cyan cook zero ruin waxy many zest onyx oboe aqua unit help gray drop bald iced aqua skew visa
tuna acid epic gyro into knob brag cusp apex king slot whiz toys diet kick need pool yurt draw lung yell jury vast fizz sets wave task runs edge
tuna acid epic gyro into knob brag cusp aqua slot film each horn cash sets hang cola hang body hope claw real hard high vows hard drum stub puma
```

## Generating Seeds from Provided Entropy

When the `--in` option is used, seedtool takes one or more arguments and uses them to construct the seed. If no arguments are given on the command line, it reads input from stdin and uses what it reads to construct the seed. In the examples below, the end of input to stdin is marked by `^D` on its own line.

### Hex & Bytewords

When the input format is `hex` or Bytewords (`btw`, `btwu`, `btwm`), the construction is the identity function (passthrough.)

```
#
# Input a hex seed via stdin, receive the same seed back.
#

$ seedtool --in hex
3d1d142cd016cf8a393a1b477891c5e594fb7c9479b175a0db653067d6de0b17
^D
3d1d142cd016cf8a393a1b477891c5e594fb7c9479b175a0db653067d6de0b17

#
# Input a bytewords seed via stdin, receive the same seed back in hex.
#

$ seedtool --in btwm
dindcstidedrwsvsfeztlswlhfwytpolmossrkhl
^D
279b18d0282aefe845fb83e956eed8a6
```

### Other Inputs

For the other input formats, each "unit" of the input (bit, digit, card, etc.) is converted to a byte and placed in an array. The SHA256 is then taken of the resulting array, yielding a deterministic seed. This seed is then used to generate a cryptographic seed of `count` bytes.

```
#
# Start by generating 16 random bits. You could do this by flipping
# a coin 16 times.
#

$ seedtool --out bits
1110110001110111

#
# Construct a 16-byte seed from those bits, providing them on
# the command line.
#

$ seedtool --in bits 1110110001110111
8d933e43b1bc8f2e3fc27adc98ad4534

#
# Construct the same seed from those bits, providing them via stdin.
#

$ seedtool --in bits
1110110001110111
^D
8d933e43b1bc8f2e3fc27adc98ad4534
```

```
#
# Start by generating 16 random playing cards. You could do this
# by drawing cards from an actual deck, shuffling between each draw.
#

$ seedtool --out cards
6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs

#
# Construct a 16-byte seed from those playing cards, providing
# them on the command line.
#

$ seedtool --in cards 6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs
7df301924511326d7350be14c9e7176d

#
# Construct a longer 32-byte seed from those same playing cards.
#

$ seedtool --count 32 --in cards 6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs
7df301924511326d7350be14c9e7176d98e945f9ad0ed034726ad4ee0de59c25
```

```
#
# Construct a 16-byte seed from any number of die rolls. You could
# do this by rolling actual dice.
#

$ seedtool --in dice 3343462611234633
77ae0de807d60367311eb040c70690d2

#
# Construct a 16-byte seed from any number of digits in [0-5].
#

$ seedtool --in base6 3242235101442242
51621269b3a91fe6482ceb7779f0d1d1

#
# Construct a 16-byte seed from any number of digits in [0-9].
#

$ seedtool --in base10 3190125
a0ca21e20db54b4df7479737c145f6db

#
# Construct a 16-byte seed from any number of integers in [0-255].
#
$ seedtool --in ints 71 22 95 6 23 65 27 10 67 16
a38385ba7a67b7f5882b37f75b43c2df
```

### Piping Output & Input

Output of one call to seedtool can be piped into another.

```
#
# Roll 16 dice and create a 32-byte seed from them.
#

$ seedtool --out dice | seedtool --in dice --count 32
7bdf68608e30da4b9ec48af0cb48f2601b41d1bcc8859b4f625d6c0470f3a6dd

#
# Roll 16 dice saving them to a file, and create a 16-byte seed
# from them.
#

$ seedtool --out dice | tee dice.asc | seedtool --in dice
c13be193c8e3451a20b75e8dc0f69284
$ cat dice.asc
4435442555226432
```

### Count Options

If a smaller or larger seed is desired, the `--count` option specifies how many bytes it contains.

```
#
# Create an 8-byte seed from any number of bits.
#

$ seedtool --in bits --count 8 0111100011000011
1a4783f9e4e8eb68

#
# Create a 32-byte seed from any number of bits.
#

$ seedtool --in bits --count 32 0111100011000011
1a4783f9e4e8eb6862cbd34acfe4f79ee8ee4e0e6f5726e589a36c7e8bf8a547

#
# Create a 20-byte seed from any number of playing cards.
#

$ seedtool --in cards --count 20 6c2c3hthacts6d4hkhtd2d7c6c3sqs6h
731e0a4c76189b2b55f4c705ccbb0105d3ee72c0
```

### Inputs for BIP39 & SSKR

`bip39` and `sskr` output formats can be combined with the `random` (default) input format. If the `--count N` option is used with the `hex` or Bytewords (`btw`, `btwu`, `btwm`) input formats, it results in a seed of `N` bytes being generated and used.

```
#
# Create a random 16-byte seed and display it as BIP39.
#

$ seedtool --out bip39
monitor place true skirt uncover scissors tower alley fame grunt sun outer

#
# Create a random 32-byte seed and display it as a single
# SSKR share.
#

$ seedtool --in random --out sskr --count 32
tuna acid epic hard data body easy able able able holy lazy zero calm buzz exit iron rock aqua jugs pose fair vows navy heat gyro silk puma keno zero ramp easy barn omit drop user exit rust vibe inch real junk trip barn liar lamb
```

`bip39` and `sskr` output formats can be combined with the `hex` or Bytewords (`btw`, `btwu`, `btwm`) input formats. The `--count` option is not allowed and the whole hex seed is used. For `bip39` the seed must be 12-32 bytes and even. For `sskr` the seed must be 16-32 bytes and even.

```
#
# Start by generating a random 16-byte seed.
#

$ seedtool --out hex
8a3796240f6a9606a577c887f2e5c83a

#
# Input the seed above and display it as BIP39.
#

$ seedtool --in hex --out bip39 8a3796240f6a9606a577c887f2e5c83a
mechanic royal math burst practice addict noise weekend margin now improve invest

#
# Convert the seed above to Bytewords minimal encoding.
#

$ seedtool --in hex --out btwm 8a3796240f6a9606a577c887f2e5c83a
leemmtdkbsimmtamonktsplnwzvwspftjsoyiejs

#
# Display the Bytewords-encoded seed as BIP39.
#

$ seedtool --in btwm --out bip39 leemmtdkbsimmtamonktsplnwzvwspftjsoyiejs
mechanic royal math burst practice addict noise weekend margin now improve invest

#
# The --count option is not available when providing the
# seed yourself.
#

$ seedtool --count 12 --in hex --out bip39 8a3796240f6a9606a577c887f2e5c83a
seedtool: The --count option is not available for hex input.

$ seedtool --count 5 --in btwm leemmtdkbsimmtamonktsplnwzvwspftjsoyiejs
seedtool: The --count option is not available for Bytewords input.

#
# The seed you provide must conform to the output format constraints.
#

$ seedtool --in hex --out bip39 8a3796240f6a
seedtool: Invalid seed length for BIP39. Must be in [12-32] and even.
```

`bip39` can be used as an input format, in which case the original seed is recovered. The BIP39 mnemonic sequence may be passed as one or more arguments on the command line, or entered via stdin.

```
#
# Recover a seed from a BIP39 mnemonic sequence, providing each
# word as a separate argument on the command line.
#

$ seedtool --in bip39 mechanic royal math burst practice addict noise weekend margin now improve invest
8a3796240f6a9606a577c887f2e5c83a

#
# Recover from the same BIP39 mnemonic sequence, providing all
# words as a single (quoted) argument on the command line.
#

$ seedtool --in bip39 "mechanic royal math burst practice addict noise weekend margin now improve invest"
8a3796240f6a9606a577c887f2e5c83a

#
# Recover from the same BIP39 mnemonic sequence, providing all
# the words via stdin.
#

$ seedtool --in bip39
mechanic royal math burst practice addict noise weekend margin now improve invest
^D
8a3796240f6a9606a577c887f2e5c83a

#
# Recover from the same BIP39 mnemonic sequence, providing all
# the words via stdin, and displaying the result as Bytewords.
#

$ seedtool --in bip39 --out btw
mechanic royal math burst practice addict noise weekend margin now improve invest
^D
love exam mint dark bias item mint atom open kept soap lion whiz view soap fact jugs obey idle jugs
```

`sskr` can be used as an input format, in which case the original seed is recovered. The SSKR shares may be passed on the command line or entered via stdin. If passed on the command line, the shares must each be a single argument (i.e., quoted). If passed via stdin, each share must appear by itself on one line.

```
#
# Recover a seed from a SSKR share, providing the entire share
# as a single (quoted) argument on the command line.
#

$ seedtool --in sskr "tuna acid epic gyro junk gala able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob taco yell jump dice"
82f188b091c38fced97bb45732bddf7e

#
# Recover from the same SSKR share, providing the share via stdin.
#

$ seedtool --in sskr
tuna acid epic gyro junk gala able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob taco yell jump dice
^D
82f188b091c38fced97bb45732bddf7e

#
# The same SSKR share is used as above, but since it is not
# quoted, seedtool fails to decode it.
#

$ seedtool --in sskr tuna acid epic gyro junk gala able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob taco yell jump dice
seedtool: Invalid Bytewords.

```

In this example, 2 shares of a 2-of-3 split are entered on the command line (each separately quoted) and via stdin (each on its own separate line.)

```
#
# Generate a random 16-byte seed and display it as SSKR in a
# single group requiring 2-of-3 shares.
#

$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro next time able acid able solo luau girl also tuna gems also rust zero iron frog safe yoga jolt duty part able atom twin good
tuna acid epic gyro next time able acid acid stub fund edge logo crux able when gush navy tent bulb fizz vows kiwi heat lion buzz work user real
tuna acid epic gyro next time able acid also swan calm road belt duty tiny zero wolf epic also veto trip trip fund vibe yank junk cash oboe jury

#
# Recover from the first and third shares above, providing each
# separately quoted on the command line.
#

$ seedtool --in sskr "tuna acid epic gyro next time able acid able solo luau girl also tuna gems also rust zero iron frog safe yoga jolt duty part able atom twin good" "tuna acid epic gyro next time able acid also swan calm road belt duty tiny zero wolf epic also veto trip trip fund vibe yank junk cash oboe jury"
2c4b163bfe9b791dbc0e755b911d01f6

#
# Recover from the first and third shares above, providing each
# on its own separate line via stdin.
#

$ seedtool --in sskr
tuna acid epic gyro next time able acid able solo luau girl also tuna gems also rust zero iron frog safe yoga jolt duty part able atom twin good
tuna acid epic gyro next time able acid also swan calm road belt duty tiny zero wolf epic also veto trip trip fund vibe yank junk cash oboe jury
^D
2c4b163bfe9b791dbc0e755b911d01f6
```

## Deterministic Randomness

When using the `--in random` (default input format) option, seedtool uses a cryptographically-strong random number generator provided by the operating system. The same random number generator is also used when constructing SSKR shares, so the same entropy input will yield different shares each time.

```
#
# Generate entropy. You could do this by rolling real dice.
#

$ seedtool --out dice
5343553122555345

#
# Construct seed from entropy.
#

$ seedtool --in dice 5343553122555345
5d1c30bbc6f3cfd070067b63c851ffe7

#
# Construct SSKR share from seed.
#

$ seedtool --in hex --out sskr 5d1c30bbc6f3cfd070067b63c851ffe7
tuna acid epic gyro ugly roof able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zero void meow wasp taco very

#
# Recover the seed from the SSKR share.
#

$ seedtool --in sskr "tuna acid epic gyro ugly roof able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zero void meow wasp taco very"
5d1c30bbc6f3cfd070067b63c851ffe7

#
# Again construct SSKR share from the same seed. Notice the
# share is different than last time.
#

$ seedtool --in hex --out sskr 5d1c30bbc6f3cfd070067b63c851ffe7
tuna acid epic gyro brag aqua able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zero void vibe limp yurt warm

#
# Recover the second share and notice that the seed is the same.
#

$ seedtool --in sskr "tuna acid epic gyro brag aqua able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zero void vibe limp yurt warm"
5d1c30bbc6f3cfd070067b63c851ffe7
```

Seedtool also provides the `--deterministic S` option, which takes a string `S`, produces the SHA256 hash of that string, and then uses that to seed its own cryptography-quality random number generator, which it uses for the rest of its run. This means that seeds generated by seedtool with the same `--deterministic` input will yield the same results.

```
#
# Generate a 16-byte seed using deterministic randomness.
#

$ seedtool --deterministic test
d551108c3e7831532beded6b29438683

#
# Notice that using the same value for --deterministic results
# in the same output.
#

$ seedtool --deterministic test
d551108c3e7831532beded6b29438683

#
# Using a different value for --deterministic results in
# different output.
#

$ seedtool --deterministic test2
a97770028023ece0f9307bf867b4c740
```

In this example, the same entropy is used twice in producing a SSKR share, which would normally result in two different shares being produced. But by providing `--deterministic FOOBAR`, the same results are produced.

```
#
# Generate 16 die rolls using deterministic randomness.
#

$ seedtool --deterministic FOOBAR --out dice
1533324122434244

#
# Use our die rolls to generate a 16-byte seed.
#

$ seedtool --in dice 1533324122434244
5cd271b50b98a869da1c26a526e1d3a8

#
# Display our seed as a SSKR 2-of-3 shares, using
# deterministic randomness.
#

$ seedtool --deterministic FOOBAR --in hex --out sskr --group 2-of-3 5cd271b50b98a869da1c26a526e1d3a8
tuna acid epic gyro urge body able acid able exam fern lung visa barn yawn flap open inky drum chef grim paid limp owls vows veto yank curl soap
tuna acid epic gyro urge body able acid acid cost zaps paid hard hard purr time yawn also horn ugly quad leaf film vast part toys limp bulb game
tuna acid epic gyro urge body able acid also into play slot lazy onyx knob knob apex pool purr limp luau zinc wave diet item kiwi flap taco many

#
# Do all of the above on a single line, and note that because
# of deterministic randomness, the same final output is produced.
#

$ seedtool --deterministic FOOBAR --out dice | seedtool --in dice | seedtool --deterministic FOOBAR --in hex --out sskr --group 2-of-3
tuna acid epic gyro urge body able acid able exam fern lung visa barn yawn flap open inky drum chef grim paid limp owls vows veto yank curl soap
tuna acid epic gyro urge body able acid acid cost zaps paid hard hard purr time yawn also horn ugly quad leaf film vast part toys limp bulb game
tuna acid epic gyro urge body able acid also into play slot lazy onyx knob knob apex pool purr limp luau zinc wave diet item kiwi flap taco many
```

## Compatibility

* seedtool has been tested to give the same results as [Ian Coleman's BIP39 tool](https://iancoleman.io/bip39/) for the following input methods: `base6`, `base10`, `bits`, and `dice`.
* `hex` as an input method is already compatible if used with his tool in "raw entropy" mode.
* `cards` is not currently compatible.

The example below can be replicated using Coleman's tool and selecting the following options:

* Show entropy
* Base: 10
* Entropy: 123456
* Mnemonic length: 12 words

```
$ seedtool --in base10 123456 | seedtool --in hex --out bip39
mirror reject rookie talk pudding throw happy era myth already payment owner
```

## Uniform Resources (URs)

Seedtool can encode and decode binary (hex) seeds, BIP39-encoded seeds, or SSKR-encoded shares in the Uniform Resource (UR) format. This format is defined in [BCR-0005: Uniform Resources (UR): Encoding Structured Binary Data for Transport in URIs and QR Codes](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0005-ur.md). The UR types supported for encoding and decoding are `crypto-seed`, `crypto-bip39` and `crypto-sskr`. These types are defined in [BCR-0006: Registry of Uniform Resource (UR) Types](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0006-urtypes.md).

To encode the result of `seedtool` as a UR, supply the `--ur[=MAX_PART_LENGTH]` command line option. `MAX_PART_LENGTH` is a positive integer that sets the maximum number of characters allowed in a UR part. If a UR cannot be completely encoded in `MAX_PART_LENGTH` or fewer characters, it will be split into a number of roughly equal parts small enough to fall below the `MAX_PART_LENGTH` limit. The default for `MAX_PART_LENGTH` is 2500, and can be set higher or lower. Since `MAX_PART_LENGTH` is optional, it *must* be supplied after an equal sign:

```
# OK:

$ seedtool --ur
ur:crypto-seed/oeadgdcnwnvtkebnbncpaevdjkkeluveglnyldaotpiecffdendigyguki

# OK, UR is still small enough to be encoded in one part

$ seedtool --ur=500
ur:crypto-seed/oeadgdestskgnlknnlpsesbwdsrfkolosbpmynaotpiecffdengmaypssn

# Illegal: Optional values must be defined using equal sign.

$ seedtool --ur 500
seedtool: Do not provide arguments when using the random (default) input format.

# Invalid: MAX_FRAGMENT_LENGTH too small:

$ seedtool --ur=9
seedtool: MAX_FRAGMENT_LENGTH must be >= 10.
```

To decode a UR in one of the supported formats, use the UR input method `--in ur`.

```
$ seedtool --in ur ur:crypto-seed/oeadgdcnwnvtkebnbncpaevdjkkeluveglnyldaotpiecffdendigyguki
23f1e07c0c0c2200e7737c8be44e9a89
```

As in other cases, input may be supplied on separate lines and terminated by `^D`.

```
$ seedtool --in ur
ur:crypto-seed/oeadgdcnwnvtkebnbncpaevdjkkeluveglnyldaotpiecffdendigyguki
^D
23f1e07c0c0c2200e7737c8be44e9a89
```

As with other input and output methods, seedtool operates in either encoding mode or decoding mode. Thus it is illegal to combine the `--ur` option with the `--in ur` input method.

```
$ seedtool --ur --in ur ur:crypto-seed/oeadgdcnwnvtkebnbncpaevdjkkeluveglnyldaotpiecffdendigyguki
seedtool: The --ur option may not be combined with the --in ur input method.
```

The following example pipes the output of one invocation of seedtool to another, first decoding a binary seed UR to hex, and then re-encoding that hex seed as BIP39.

```
$ seedtool --in ur ur:crypto-seed/oeadgdcnwnvtkebnbncpaevdjkkeluveglnyldaotpiecffdendigyguki | seedtool --out bip39 --ur
ur:crypto-ur:crypto-bip39/oeadlkiyihjyisiniajkiykojlkkhsioihihjkjojphskkisieinjkiajlkoihjpieiajzhsktisjehsjtiohsjpjljlieiyinjtieiykoihjkjkihjzieiajphsjniyidihjyjyihjpiajphsktieiekpjtihaotpiecffdenjlprfsvw

#
# Decode the previous result
#

$ seedtool --in ur
ur:crypto-bip39/oeadlkiejpiniaisiojkkpiaiaihjkjkieisjljlieiyjpiskkjyisjniyhsjzkthskkjkihidkpjpjkjyihjzkpjtiaisiohsjojojpjlkoihiokojljziahsjtjlihidjphsjtieiyieinihjkihjziyjeinjyjyihjtaotpiecffdhkdrrepetd
^D
b95b09b55c60783da14856f5a360f63d

#
# Re-encode the previous result to BIP39 words
#

$ seedtool --out bip39 --in hex b95b09b55c60783da14856f5a360f63d
rich success hood rhythm always burst lunch approve volcano brand diesel kitten

#
# Re-encode the previous result to Bytewords
#

$ seedtool --out btw --in hex b95b09b55c60783da14856f5a360f63d
rich help axis race high horn keys figs obey fund half yank omit horn yawn figs swan vast toys gift
```

The UR format is designed to be efficiently transmitted in QR codes, because it uses only characters in the QR Code "alphanumeric" character set. This set uses only upper case letters. The Unix `tr` tool can be used to transform the QR to upper case:

```
$ seedtool --ur | tr [:lower:] [:upper:]
UR:CRYPTO-SEED/OEADGDPLLGAEVTQZESVONNPRLPJLPATAYKWDZCAOTPIECFFDENIMDLDRIO
```

```
#
# Generate a seed, encode it as UR, transform it to upper case,
# display it on the console, and encode it to a QR Code in
# the file "seedqrcode.png".
#

$ seedtool --ur | tr [:lower:] [:upper:] | tee /dev/tty | qrencode -o seedqrcode.png -l L
UR:CRYPTO-SEED/OEADGDHHGTPKFXLGHHLBPYMHLFRYHLPLSSRKPKAOTPIECFFDENTNJNWNPF
```

`seedqrcode.png`:

![](../manual-images/seedqrcode.png)

The payload of a UR is [CBOR](https://tools.ietf.org/html/rfc7049) encoded as [Bytewords](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-012-bytewords.md). If you wish to examine the CBOR encoding, you can use seedtool to decode the Bytewords payload of a UR. In this example we use the seed above, but only decode the part after the slash as Bytewords.

```
$ seedtool --in btwm OEADGDHHGTPKFXLGHHLBPYMHLFRYHLPLSSRKPKAOTPIECFFDENTNJNWNPF
a201505c4daa438d5c7fab9082bd5daec4bbaa02d864194836
```

Inputting this byte sequence into the [CBOR Playground](http://cbor.me/), we see the CBOR diagnostic notation output:

```
{1: h'5C4DAA438D5C7FAB9082BD5DAEC4BBAA', 2: 100(18486)}
```

This is a map with two fields, labeled `1` (the seed itself) and `2` (the date the seed was encoded as number of days since the Unix epoch, tagged with `100` as per the [IETF specification for such dates](https://datatracker.ietf.org/doc/draft-ietf-cbor-date-tag/?include_text=1). The map labels and their meanings are defined in the [Registry of Uniform Resource (UR) Types](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0006-urtypes.md).

**✳️ NOTE:** Currently seedtool encodes the current date as the seed birthdate when encoding output in UR format. Currently seedtool ignores this field when decoding UR format.

When a UR encoding must be broken up into parts, seedtool prints each part on a separate line.

```
$ seedtool --count 400 --ur=100
ur:crypto-seed/1-5/ltadahcfadndcyesbbaebahdguoeadhkadmhttsghgnevtlndmkpdlctrsgrcatedllfledwbaiyswlsdwsplplrckmtoyrsghgaptfgkstinnkgvscaisbblfbnrtksfefestrsbklsrspkpyghlentwtlomybyuydydaeoldtojydsgtwdeobzfstpgasnehflludm
ur:crypto-seed/2-5/ltaoahcfadndcyesbbaebahdgupmcproftwklosocybkhgoxcxhsjlurdpfwahlkfmgwwpbedwcerltdmtrlutcnnlwednatwdmkoesgmtcstljtoxhsfnplonhdjkongohglusrdpemcalkgwgukpmtdwurwnrocejtlgzezsgywllygudebzvlwedthsgoemspeogs
ur:crypto-seed/3-5/ltaxahcfadndcyesbbaebahdgukbeoahjelussjzmottamcxctsgeygasohgmsemrpztdwotglnlynvyayhkkbenhhbbsewntsayadbzdirhaasbcadeotfzgdtpqzyltosstelybtbwwsvwjlgelnjohkbbbbaanlpshgjlvoknsbasfhjzututsorybwzcdsisrnzs
ur:crypto-seed/4-5/ltaaahcfadndcyesbbaebahdgubsztwewpplehdrvoeowybafttooszmbwsttpdllytybkdlsepmdakbsgtyfwenteeosbmsrtkbtnbtgesrvtadbthtdsdkdsnslntkcsgedrwygsmwfldnbgflgrzsgslnswmwmnhygrnnioswkowdcwmniaclmutapyskwlcylfie
ur:crypto-seed/5-5/ltahahcfadndcyesbbaebahdguaezcrhbngybagsfdosytetnttpuyierkvslthpdecwpllgwtvanngyemfgynplvojnlprtfyhywsaarycynblraybnotiyttknzmlgskkbsrhpcwjocewspftbwzlsoytykovwlnchhseouoaaaotpiecffdhkaeaeaeaekkahnnhp
```

The original seed can be recovered from the above:

```
$ seedtool --in ur
ur:crypto-seed/1-5/ltadahcfadndcyesbbaebahdguoeadhkadmhttsghgnevtlndmkpdlctrsgrcatedllfledwbaiyswlsdwsplplrckmtoyrsghgaptfgkstinnkgvscaisbblfbnrtksfefestrsbklsrspkpyghlentwtlomybyuydydaeoldtojydsgtwdeobzfstpgasnehflludm
ur:crypto-seed/2-5/ltaoahcfadndcyesbbaebahdgupmcproftwklosocybkhgoxcxhsjlurdpfwahlkfmgwwpbedwcerltdmtrlutcnnlwednatwdmkoesgmtcstljtoxhsfnplonhdjkongohglusrdpemcalkgwgukpmtdwurwnrocejtlgzezsgywllygudebzvlwedthsgoemspeogs
ur:crypto-seed/3-5/ltaxahcfadndcyesbbaebahdgukbeoahjelussjzmottamcxctsgeygasohgmsemrpztdwotglnlynvyayhkkbenhhbbsewntsayadbzdirhaasbcadeotfzgdtpqzyltosstelybtbwwsvwjlgelnjohkbbbbaanlpshgjlvoknsbasfhjzututsorybwzcdsisrnzs
ur:crypto-seed/4-5/ltaaahcfadndcyesbbaebahdgubsztwewpplehdrvoeowybafttooszmbwsttpdllytybkdlsepmdakbsgtyfwenteeosbmsrtkbtnbtgesrvtadbthtdsdkdsnslntkcsgedrwygsmwfldnbgflgrzsgslnswmwmnhygrnnioswkowdcwmniaclmutapyskwlcylfie
ur:crypto-seed/5-5/ltahahcfadndcyesbbaebahdguaezcrhbngybagsfdosytetnttpuyierkvslthpdecwpllgwtvanngyemfgynplvojnlprtfyhywsaarycynblraybnotiyttknzmlgskkbsrhpcwjocewspftbwzlsoytykovwlnchhseouoaaaotpiecffdhkaeaeaeaekkahnnhp
^D
d1ca579fe0872e752f1fbf4b1dd32f828a2c0e66c6832cc886841e96a1bf5449a94678d09e7be81d6814820cc0784545c7bf0a83bfaaab548a9df0888f11db30253389ce74264dea33153dd849cdad22b83af488c91a0a57a420616fdf2d42058c3e4fec102c1cb7d296b7dd2399ed2b07ea98a2ca9618d56ea4613caea55873a555578bc32d371d8c4f5375962cdff1b81c6e8dfdfa51e981532815e3ed2961557e33056b8bc46c92d106201fca3249c9579737b6fb2ca34e99f6e108597e365c14c1f1d708011527b904cb1d28a34050d8b4f7cec4d3810d13efe56f4a87705914140499ac576fe27acb093f6cddddc9bd13fc0ffbedecae312ae233ee0e3acea7fe13c7d82f81d40a2fc1ad257ecad44236d333cb97c07eda0d4ac3e0010d5a2624269c87cf184a2aee4c94472b12474bfa4c87c6948e5e4b9e67c676ea1b8e632193d9abc500fcb90c510e4c48a7f9389dd8db64bbe8855b281bae8df0e69e513746f6aee26d86c0445eef04bd1aa084080ca366d17afe8dc57ec35b1b701cefb0d6f283a1d476e587176133dc04
```

## Multipart Encoding of URs with Fountain Codes

When encoding a multi-part UR, seedtool outputs the minimum number of parts needed to reconstruct the message. You can, however, ask seedtool to produce any number of additional parts beyond the minimum. These are generated using [fountain codes](https://en.wikipedia.org/wiki/Fountain_code), such that each additional part carries some mix of the other parts. The primary purpose of this is to produce a continuous stream of QR codes that can be read starting at any point, and even with a certain number of QR codes missed, still reproduce the original message without ending up having to wait for a single specific part to be displayed.

```
#
# Generate a multi-part UR
#

$ seedtool --count 300 --deterministic=TEST --ur=50
ur:crypto-seed/1-7/ltadatcfademcyimclztbehddpoeadhkaddwnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlplgcpwsadjk
ur:crypto-seed/2-7/ltaoatcfademcyimclztbehddpkplfbkqzztglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclasztgmdstpoennbtflwykbcxclayrn
ur:crypto-seed/3-7/ltaxatcfademcyimclztbehddptpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlbmeottijlfrvefm
ur:crypto-seed/4-7/ltaaatcfademcyimclztbehddpiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkjltduohddaahbkia
ur:crypto-seed/5-7/ltahatcfademcyimclztbehddpdwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezcurgtlpuecwfehgatkspriymotofgcettkpgrla
ur:crypto-seed/6-7/ltamatcfademcyimclztbehddpmhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvlisytlati
ur:crypto-seed/7-7/ltatatcfademcyimclztbehddptpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaotpiecffdhkaeaeaeaewfjyvlsg

#
# Generate the same multi-part UR, but add 10 additional parts
#

$ seedtool --count 300 --deterministic=TEST --ur=50 --parts 10
ur:crypto-seed/1-7/ltadatcfademcyimclztbehddpoeadhkaddwnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlplgcpwsadjk
ur:crypto-seed/2-7/ltaoatcfademcyimclztbehddpkplfbkqzztglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclasztgmdstpoennbtflwykbcxclayrn
ur:crypto-seed/3-7/ltaxatcfademcyimclztbehddptpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlbmeottijlfrvefm
ur:crypto-seed/4-7/ltaaatcfademcyimclztbehddpiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkjltduohddaahbkia
ur:crypto-seed/5-7/ltahatcfademcyimclztbehddpdwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezcurgtlpuecwfehgatkspriymotofgcettkpgrla
ur:crypto-seed/6-7/ltamatcfademcyimclztbehddpmhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvlisytlati
ur:crypto-seed/7-7/ltatatcfademcyimclztbehddptpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaotpiecffdhkaeaeaeaewfjyvlsg
ur:crypto-seed/8-7/ltayatcfademcyimclztbehddpmhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvlzmgyzmpd
ur:crypto-seed/9-7/ltasatcfademcyimclztbehddptslsguretstejsuelrhfttahhyguoncnjoknmnismsehtepthejehfskdyflfgcmmhplamlonytowphnkprdmdiswfttcndnmh
ur:crypto-seed/10-7/ltbkatcfademcyimclztbehddpcactfnnskeotcflffrdiwyotsbiabeldnykngadlcxoximhllndwvartptgeaeyknbgrfzldlbjlwpkgwmtpaehttlehspkgzs
ur:crypto-seed/11-7/ltbdatcfademcyimclztbehddpkkgmrhvyhertwlleqzclluchkgfxsaztbbolmuzsstkkwfrkonflcxntislybgihwlsretsacmjyyttbaeeyjkrtzstpfrmufe
ur:crypto-seed/12-7/ltbnatcfademcyimclztbehddpaefradredesahtzmaxmyatsgiapaetotskcssbfmcydtghhpnlfmplwyaduysorhwmrsbgtowmynfppaiylbmeottikbyllyhs
ur:crypto-seed/13-7/ltbtatcfademcyimclztbehddpbntiqdgooxmnpsdnqzjnbbfmsrpswsrpkbeyingejybkmovwnnpfcmecyncaytkprpvoehesfygmcljynnfheedmlrryhestue
ur:crypto-seed/14-7/ltbaatcfademcyimclztbehddpfsollauemkgdryztceiotavelonlhpcwdmlfsrtnlagtylgdcymuiesrcypydssnmhktimykjzftrerybwkpzseygtzsctdpbb
ur:crypto-seed/15-7/ltbsatcfademcyimclztbehddpwddatejegwlssfdamkehayvytbsgzmethyyagtprchkedkytfeyaeyamdrwphnuyaetajzkiynwkhkutiytkjlhtrnsolrlaby
ur:crypto-seed/16-7/ltbeatcfademcyimclztbehddpfltimtzomhmsgsjovlvsaofxknolasttnnstkbtpjpcfvapslgempffppkvdmtvdndpatpgemsfztalpstryrhchbefpwdeyvs
ur:crypto-seed/17-7/ltbyatcfademcyimclztbehddpfsollauemkgdryztceiotavelonlhpcwdmlfsrtnlagtylgdcymuiesrcypydssnmhktimykjzftrerybwkpzseygtdaseoyas

#
# Reconstruct the message from a subset of the generated parts.
#

$ seedtool --in ur
ur:crypto-seed/2-7/ltaoatcfademcyimclztbehddpkplfbkqzztglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclasztgmdstpoennbtflwykbcxclayrn
ur:crypto-seed/4-7/ltaaatcfademcyimclztbehddpiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkjltduohddaahbkia
ur:crypto-seed/5-7/ltahatcfademcyimclztbehddpdwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezcurgtlpuecwfehgatkspriymotofgcettkpgrla
ur:crypto-seed/7-7/ltatatcfademcyimclztbehddptpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaotpiecffdhkaeaeaeaewfjyvlsg
ur:crypto-seed/8-7/ltayatcfademcyimclztbehddpmhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvlzmgyzmpd
ur:crypto-seed/10-7/ltbkatcfademcyimclztbehddpcactfnnskeotcflffrdiwyotsbiabeldnykngadlcxoximhllndwvartptgeaeyknbgrfzldlbjlwpkgwmtpaehttlehspkgzs
ur:crypto-seed/14-7/ltbaatcfademcyimclztbehddpfsollauemkgdryztceiotavelonlhpcwdmlfsrtnlagtylgdcymuiesrcypydssnmhktimykjzftrerybwkpzseygtzsctdpbb
ur:crypto-seed/16-7/ltbeatcfademcyimclztbehddpfltimtzomhmsgsjovlvsaofxknolasttnnstkbtpjpcfvapslgempffppkvdmtvdndpatpgemsfztalpstryrhchbefpwdeyvs
^D
9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e3bf736a89e9ceb105f2109fb5226d8a29e0d47ee7ed8774f20245ac5f47b95958b1483daa4aaabc9dad616a30bf338b4ef1e971d2cc449bdcc339258f93f7f91a3d067522b085ca6b2f7c72732ce5aed7ae0ef273f13c8d92ffa89b69cac18dd79664032e0f86fe3c1f1596fd2dc582c690f17407d0852932d23798056a424cacae3bfd4e30fe8030f943645fcdf4d86de1b45570778b26692ce461c9053c54a47443dae67bed34624f5acf2eebdf0b0e5283505d25ce6849aa0d0f10b1fdec20d8e35e6b2072c7fe3d84c4e950c989f0a781a92417732e2076fb302e4cc3ff7506a061a011f4cd4952ff6af
```

The example above uses a multi-part UR to encode a seed. Each of the parts of a multi-part UR are *not* equivalent to a share of a multi-share recovery tool like SSKR. Seedtool can also decode a SSKR seed from one or more single-part URs that each represent one share of a SSKR.

```
#
# Recover a SSKR seed using 2 of 3 shares. Each UR is one share.
#

$ seedtool --in ur
ur:crypto-sskr/taadecgolywpaeadaelfesaxylbetokntnbsgessutmtecdtyttshnfncp
ur:crypto-sskr/taadecgolywpaeadaoskfmnlmhbtemmovyzohffniaeehlcendglsfmkem
^D
e3b0c44298fc1c149afbf4c8996fb924
```

## Version History

### 0.8.1, 9/19/2020

* Updated to latest versions of dependencies.

### 0.8.0, 9/15/2020

* Replaced SLIP-39 with SSKR.
* Increased maximum value of `--count` to 1024.

### 0.7.1, 8/21/2020

* Fixed bug where multi-share URs like SSKR were not decoded correctly due to support for multi-part URs being added. Seedtool now supports *either* decoding a multi-part UR or a multi-share UR where the shares are encoded as URs, but not both simultaneously.
* Added example to MANUAL of decoding URs containing SSKR shares.

### 0.7.0, 8/12/2020

* Now using bc-ur for UR encoding/decoding. Support multi-part fountain encoding/decoding.

### 0.6.0, 7/1/2020

* Replaced SHA256 with CRC32 for multipart URs.

### 0.5.0, 6/30/2020

* Replaced BC32 with Bytewords.

### 0.4.0, 5/19/2020

* Added support for encoding to and decoding from UR format.

### 0.3.0, 5/1/2020

* Modified "Bech32" format to be "BC32", including dropping "seed" HRP and '1' divider.

### 0.2.0, 4/22/2020

* Added "bech32" input and output formats.

### 0.1.1, 4/8/2020

* Added compatibility with Ian Coleman's BIP39 tool for several input formats:
    * `base6`, `base10`, `bits`, and `dice` are now compatible.
    * `hex` as an input method is already compatible if used with his tool in "raw entropy" mode.
    * `cards` is not compatible as his algorithm is complex and possibly not worth spending the time on.
* Updated MANUAL.md examples to remain accurate.
* Added a section on Compatibility with other tools to MANUAL.md.

### 0.1.0, 4/8/2020

* First test release.
