# 🌱 Seedtool

**Version 0.9.0**<br/>**October 4, 2020**

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
taxi work belt cash echo acid soap lung pool very nail list dark gear dark zero each twin lava lamb

#
# Convert the Bytewords-encoded seed to hex.
#

$ seedtool --in btw "taxi work belt cash echo acid soap lung pool very nail list dark gear dark zero each twin lava lamb"
d0f40d173301c88daee19987244b24fb

#
# Convert the same seed to Bytewords URI-compatible format.
#

$ seedtool --in hex --out btwu d0f40d173301c88daee19987244b24fb
taxi-work-belt-cash-echo-acid-soap-lung-pool-very-nail-list-dark-gear-dark-zero-each-twin-lava-lamb

#
# Convert the same seed to Bytewords minimal format.
#

$ seedtool --in hex --out btwm d0f40d173301c88daee19987244b24fb
tiwkbtcheoadsplgplvynlltdkgrdkzoehtnlalb
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
tuna acid epic gyro king cola able able able bias ugly aqua leaf mild numb cusp judo undo ruby jowl vast zero fund taxi gush stub iced hawk acid

#
# Generate a random 32-byte seed and display it as SSKR.
#

$ seedtool --out sskr --count 32
tuna acid epic hard data lava crux able able able love cost part wall monk gyro many play exit inch cats away eyes stub plus body wolf skew limp join wand twin deli idle days oval bald puma soap good beta city jazz knob tied down
```

n-of-m SSKR shares can be generated by specifing one or more groups.

```
#
# Generate a 16 byte seed and display it as SSKR, one group,
# 2-of-3 shares required for seed recovery.
#

$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro waxy blue able acid able jolt pose task days taxi wasp beta wasp barn aqua tuna hawk jade rock figs obey horn horn figs race
tuna acid epic gyro waxy blue able acid acid fund dice surf item math axis into ugly aqua lazy half apex horn slot miss item item curl twin cats
tuna acid epic gyro waxy blue able acid also cusp road solo ruin good figs undo leaf code buzz undo wave kiwi flux jump draw runs task what acid
```

Multiple groups and a group threshold may be specified. If omitted, the default `--group-threshold` is 1.

```
#
# Generate a 16 byte seed and display it as SSKR, first group
# requires 2-of-3, second and third groups require 3-of-5, at least
# two groups must meet their threshold.
#

$ seedtool --out sskr --group-threshold 2 --group 2-of-3 --group 3-of-5 --group 3-of-5
tuna acid epic gyro days rich brag acid able days brew rich cyan yawn belt ugly cost love roof kept flew wall inch safe saga very gush vibe acid
tuna acid epic gyro days rich brag acid acid hang paid barn work bias swan code jade apex high whiz nail luck flux silk trip door oval oval kept
tuna acid epic gyro days rich brag acid also sets knob soap mint cost mint girl yell legs into inky webs cyan diet solo yawn grim toil saga vast
tuna acid epic gyro days rich brag brag able able game jazz luau cats sets jazz down guru undo rich inky belt slot purr figs quad yurt zinc quiz
tuna acid epic gyro days rich brag brag acid lava cyan kiwi toil free real fair yoga item calm liar task menu keys pose need legs body sets yank
tuna acid epic gyro days rich brag brag also miss onyx noon wall kiln numb logo deli hawk silk horn nail soap bald quiz curl undo atom math bald
tuna acid epic gyro days rich brag brag apex cash swan many real deli time user work horn bias hill duty half quiz part list wasp waxy part game
tuna acid epic gyro days rich brag brag aqua echo kiwi zone city maze easy luck safe roof lava peck join eyes tomb girl easy lava figs tomb down
tuna acid epic gyro days rich brag cusp able keep noon flap wall hawk lung hope also navy nail next pose rich cash foxy gray leaf roof zest dark
tuna acid epic gyro days rich brag cusp acid loud film maze also work vibe able roof grim brew gala inky gems tuna jump holy tiny buzz deli view
tuna acid epic gyro days rich brag cusp also ruin quiz very arch luau veto taxi love yank grim loud cyan iris down able rust tuna back keys open
tuna acid epic gyro days rich brag cusp apex flew bulb each waxy days luau many edge figs trip hill wand next view even task many omit omit idle
tuna acid epic gyro days rich brag cusp aqua hard buzz keno pool news maze noon monk yurt diet cost redo lava race jazz iced note lazy knob cook
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
vypysaashhfhkkfhonylgunndadtdwrlldsrjzga
^D
e1abc2095c3f793fa5f7539e25292cb7
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
tuna acid epic hard data glow quad able able able zinc eyes barn zone cost drum navy cusp film play iron draw horn part soap jump cusp vial pose gala part hard work open barn luck rust paid fund buzz silk rich fish lamb zoom need
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
leemmtdkbsimmtamonktspltwzvwspftjsoyiejs

#
# Display the Bytewords-encoded seed as BIP39.
#

$ seedtool --in btwm --out bip39 leemmtdkbsimmtamonktspltwzvwspftjsoyiejs
mechanic royal math burst practice addict noise weekend margin now improve invest

#
# The --count option is not available when providing the
# seed yourself.
#

$ seedtool --count 12 --in hex --out bip39 8a3796240f6a9606a577c887f2e5c83a
seedtool: The --count option is not available for hex input.

$ seedtool --count 5 --in btwm leemmtdkbsimmtamonktspltwzvwspftjsoyiejs
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
love exam mint dark bias item mint atom open kept soap list whiz view soap fact jugs obey idle jugs
```

`sskr` can be used as an input format, in which case the original seed is recovered. The SSKR shares may be passed on the command line or entered via stdin. If passed on the command line, the shares must each be a single argument (i.e., quoted). If passed via stdin, each share must appear by itself on one line.

```
#
# Recover a seed from a SSKR share, providing the entire share
# as a single (quoted) argument on the command line.
#

$ seedtool --in sskr "tuna acid epic gyro calm mild able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob gift aqua warm taxi"
82f188b091c38fced97bb45732bddf7e

#
# Recover from the same SSKR share, providing the share via stdin.
#

$ seedtool --in sskr
tuna acid epic gyro calm mild able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob gift aqua warm taxi
^D
82f188b091c38fced97bb45732bddf7e

#
# The same SSKR share is used as above, but since it is not
# quoted, seedtool fails to decode it.
#

$ seedtool --in sskr tuna acid epic gyro calm mild able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob gift aqua warm taxi
seedtool: Invalid Bytewords.

```

In this example, 2 shares of a 2-of-3 split are entered on the command line (each separately quoted) and via stdin (each on its own separate line.)

```
#
# Generate a random 16-byte seed and display it as SSKR in a
# single group requiring 2-of-3 shares.
#

$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro deli each able acid able cats loud kiwi kiln drop loud tent rock fizz keno jump eyes numb holy edge solo dull trip hawk yawn
tuna acid epic gyro deli each able acid acid bald ruin toys zaps down film gush liar kite lung webs film toys drop roof barn king news frog taco
tuna acid epic gyro deli each able acid also film void easy huts curl zest rust silk exit need guru exam girl redo fish hard runs owls hard very

#
# Recover from the first and third shares above, providing each
# separately quoted on the command line.
#

$ seedtool --in sskr "tuna acid epic gyro deli each able acid able cats loud kiwi kiln drop loud tent rock fizz keno jump eyes numb holy edge solo dull trip hawk yawn" "tuna acid epic gyro deli each able acid also film void easy huts curl zest rust silk exit need guru exam girl redo fish hard runs owls hard very"
6bf9961f199c9a7293b4e9f26f40e83d

#
# Recover from the first and third shares above, providing each
# on its own separate line via stdin.
#

$ seedtool --in sskr
tuna acid epic gyro deli each able acid able cats loud kiwi kiln drop loud tent rock fizz keno jump eyes numb holy edge solo dull trip hawk yawn
tuna acid epic gyro deli each able acid also film void easy huts curl zest rust silk exit need guru exam girl redo fish hard runs owls hard very
^D
6bf9961f199c9a7293b4e9f26f40e83d
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
tuna acid epic gyro race ruin able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zoom void waxy sets undo work

#
# Recover the seed from the SSKR share.
#

$ seedtool --in sskr "tuna acid epic gyro race ruin able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zoom void waxy sets undo work"
5d1c30bbc6f3cfd070067b63c851ffe7

#
# Again construct SSKR share from the same seed. Notice the
# share is different than last time.
#

$ seedtool --in hex --out sskr 5d1c30bbc6f3cfd070067b63c851ffe7
tuna acid epic gyro knob fair able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zoom void even acid cash able

#
# Recover the second share and notice that the seed is the same.
#

$ seedtool --in sskr "tuna acid epic gyro knob fair able able able hill code duty rock skew wolf task taxi judo atom king idea soap gray zoom void even acid cash able"
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
tuna acid epic gyro urge body able acid able exam fern lung visa barn yawn flap open inky drum chef grim paid lion owls vows veto yank curl soap
tuna acid epic gyro urge body able acid acid cost zaps paid hard hard purr time yawn also horn ugly quad leaf film vast part toys lion bulb game
tuna acid epic gyro urge body able acid also into play slot lazy onyx knob knob apex pool purr lion luau zest wave diet item kiwi flap taco many

#
# Do all of the above on a single line, and note that because
# of deterministic randomness, the same final output is produced.
#

$ seedtool --deterministic FOOBAR --out dice | seedtool --in dice | seedtool --deterministic FOOBAR --in hex --out sskr --group 2-of-3
tuna acid epic gyro urge body able acid able exam fern lung visa barn yawn flap open inky drum chef grim paid lion owls vows veto yank curl soap
tuna acid epic gyro urge body able acid acid cost zaps paid hard hard purr time yawn also horn ugly quad leaf film vast part toys lion bulb game
tuna acid epic gyro urge body able acid also into play slot lazy onyx knob knob apex pool purr lion luau zest wave diet item kiwi flap taco many
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
ur:crypto-seed/oyadgdiywlamaejszswdwytltifeenftlnmnwkbdhnssro

# OK, UR is still small enough to be encoded in one part

$ seedtool --ur=500
ur:crypto-seed/oyadgdheasgwhlglcelagovsfsrspywklansfdvadshlhl

# Illegal: Optional values must be defined using equal sign.

$ seedtool --ur 500
seedtool: Do not provide arguments when using the random (default) input format.

# Invalid: MAX_FRAGMENT_LENGTH too small:

$ seedtool --ur=9
seedtool: MAX_FRAGMENT_LENGTH must be >= 10.
```

To decode a UR in one of the supported formats, use the UR input method `--in ur`.

```
$ seedtool --in ur ur:crypto-seed/oyadgdiywlamaejszswdwytltifeenftlnmnwkbdhnssro
66e9060071faeaeed5d045363a868ef4
```

As in other cases, input may be supplied on separate lines and terminated by `^D`.

```
$ seedtool --in ur
ur:crypto-seed/oyadgdiywlamaejszswdwytltifeenftlnmnwkbdhnssro
^D
66e9060071faeaeed5d045363a868ef4
```

As with other input and output methods, seedtool operates in either encoding mode or decoding mode. Thus it is illegal to combine the `--ur` option with the `--in ur` input method.

```
$ seedtool --ur --in ur ur:crypto-seed/oyadgdiywlamaejszswdwytltifeenftlnmnwkbdhnssro
seedtool: The --ur option may not be combined with the --in ur input method.
```

The following example pipes the output of one invocation of seedtool to another, first decoding a binary seed UR to hex, and then re-encoding that hex seed as BIP39.

```
$ seedtool --in ur ur:crypto-seed/oyadgdiywlamaejszswdwytltifeenftlnmnwkbdhnssro | seedtool --out bip39 --ur
ur:crypto-bip39/oyadlkiyiajpjlkpiaisiejzjljtioiojyjlidhsiaiajlieiajljpjtiejzjlhsjtiykohsjzjzihkkiejzinjnidiyjnjljnihjtjyiyktinjtjyihjpiektisinjoioiskpjtiejpihieiyjkinjzkoihjpteaycyem

#
# Decode the previous result
#

$ seedtool --in ur
ur:crypto-bip39/oyadlkiyiajpjlkpiaisiejzjljtioiojyjlidhsiaiajlieiajljpjtiejzjlhsjtiykohsjzjzihkkiejzinjnidiyjnjljnihjtjyiyktinjtjyihjpiektisinjoioiskpjtiejpihieiyjkinjzkoihjpteaycyem
^D
34307b8c983831e1e07477fc1f4dbce4

#
# Re-encode the previous result to BIP39 words
#

$ seedtool --out bip39 --in hex 34307b8c983831e1e07477fc1f4dbce4
crouch long tobacco corn loan valley limb moment winter whip hundred silver

#
# Re-encode the previous result to Bytewords
#

$ seedtool --out btw --in hex 34307b8c983831e1e07477fc1f4dbce4
edge duty king luck monk exit each very vast jury kept zest cost gift roof vibe hang many city flew
```

The UR format is designed to be efficiently transmitted in QR codes, because it uses only characters in the QR Code "alphanumeric" character set. This set uses only upper case letters. The Unix `tr` tool can be used to transform the QR to upper case:

```
$ seedtool --ur | tr [:lower:] [:upper:]
UR:CRYPTO-SEED/OYADGDFMOTZEWPCAHHFYUTREKPEYGHGSGAKESKJYHPWYEY
```

```
#
# Generate a seed, encode it as UR, transform it to upper case,
# display it on the console, and encode it to a QR Code in
# the file "seedqrcode.png".
#

$ seedtool --ur | tr [:lower:] [:upper:] | tee /dev/tty | qrencode -o seedqrcode.png -l L
UR:CRYPTO-SEED/OYADGDCPCNKOCSNNQDCKUEGABKMUZMYNSGGUBDRYTYVTSN
```

`seedqrcode.png`:

![](../manual-images/seedqrcode.png)

The payload of a UR is [CBOR](https://tools.ietf.org/html/rfc7049) encoded as [Bytewords](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-2020-012-bytewords.md). If you wish to examine the CBOR encoding, you can use seedtool to decode the Bytewords payload of a UR. In this example we use the seed above, but only decode the part after the slash as Bytewords.

```
$ seedtool --in btwm OEADGDJOCNNEESSPDECECMVLFMLUOSBTDWQZFEAOTPIECFFDJZPYTAIEKK
a2015070239f39c8281c16e33e8ba70d2cb44502d86419486c
```

Inputting this byte sequence into the [CBOR Playground](http://cbor.me/), we see the CBOR diagnostic notation output:

```
{1: h'70239F39C8281C16E33E8BA70D2CB445', 2: 100(18540)}
```

This is a map with two fields, labeled `1` (the seed itself) and `2` (the date the seed was encoded as number of days since the Unix epoch, tagged with `100` as per the [IETF specification for such dates](https://datatracker.ietf.org/doc/draft-ietf-cbor-date-tag/?include_text=1). The map labels and their meanings are defined in the [Registry of Uniform Resource (UR) Types](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0006-urtypes.md).

**✳️ NOTE:** Currently seedtool doe not encode the a seed birthdate when encoding output in UR format, and currently seedtool ignores this field when decoding UR format.

When a UR encoding must be broken up into parts, seedtool prints each part on a separate line.

```
$ seedtool --count 400 --ur=100
ur:crypto-seed/1-5/lpadahcfadmdcyindaseahhdgyoyadhkadmhynhliyctvsmdyawdwmjoyljllgmofnnygdfmpddnimkgmkhgrsvozctbdaaactchdnoegscseehfndsepllfcslnathlktiydpdebzrffsmyidmywftstlzefttsdipagugtqdktleiynbamvovohersaeryvewd
ur:crypto-seed/2-5/lpaoahcfadmdcyindaseahhdgyssdtbbksjkjkjzmopsssmyhkhguroepylpnbcxonztlbhgehcmhgjeyloxechdskrovdyntlyafgzopfahsffmkogsgmnnjtzsiehhttswjyktfgaennhedpkocejeolpmlbiohfeotypmyajllsfdcxwkvlrndegmmweeaxih
ur:crypto-seed/3-5/lpaxahcfadmdcyindaseahhdgyiolgktieghksfsmwwppmutghgwolmyoxsoylqzpypylehdfphfecbgtlnyzmgdcpsnmejputdnaeguaodnwyfensbacymwtdgewnbtbnlozerobnzcvawndizegwftwnrlehregenbrlhkahadnbskjznshhkkbasrhltoptdm
ur:crypto-seed/4-5/lpaaahcfadmdcyindaseahhdgyylbgheprcpecmhghfnrsghlfbwidjepkiytaptpdoesrfwfylstpfrgwrffxwydizeoniybkmkflemledemnahnymerpjkktwnrtpsgltydwdyrlsobbldkklghlbncwzsryflmekpvednesbektlgbeyntyostngdwzhdcnbs
ur:crypto-seed/5-5/lpahahcfadmdcyindaseahhdgytncfguweptroasfdwdtbsofwdyaejzutjshlhkfhutynkbglwnwtgsvanyclurveehosfptbrpfertwpetdkwegmsamtbebyjesrlbhgrystjnmwmniynywnjobawsstcevahnecnsaadylfwdlyaerddaatdajnbwjeidldcm
```

The original seed can be recovered from the above:

```
$ seedtool --in ur
ur:crypto-seed/1-5/lpadahcfadmdcyindaseahhdgyoyadhkadmhynhliyctvsmdyawdwmjoyljllgmofnnygdfmpddnimkgmkhgrsvozctbdaaactchdnoegscseehfndsepllfcslnathlktiydpdebzrffsmyidmywftstlzefttsdipagugtqdktleiynbamvovohersaeryvewd
ur:crypto-seed/2-5/lpaoahcfadmdcyindaseahhdgyssdtbbksjkjkjzmopsssmyhkhguroepylpnbcxonztlbhgehcmhgjeyloxechdskrovdyntlyafgzopfahsffmkogsgmnnjtzsiehhttswjyktfgaennhedpkocejeolpmlbiohfeotypmyajllsfdcxwkvlrndegmmweeaxih
ur:crypto-seed/3-5/lpaxahcfadmdcyindaseahhdgyiolgktieghksfsmwwppmutghgwolmyoxsoylqzpypylehdfphfecbgtlnyzmgdcpsnmejputdnaeguaodnwyfensbacymwtdgewnbtbnlozerobnzcvawndizegwftwnrlehregenbrlhkahadnbskjznshhkkbasrhltoptdm
ur:crypto-seed/4-5/lpaaahcfadmdcyindaseahhdgyylbgheprcpecmhghfnrsghlfbwidjepkiytaptpdoesrfwfylstpfrgwrffxwydizeoniybkmkflemledemnahnymerpjkktwnrtpsgltydwdyrlsobbldkklghlbncwzsryflmekpvednesbektlgbeyntyostngdwzhdcnbs
ur:crypto-seed/5-5/lpahahcfadmdcyindaseahhdgytncfguweptroasfdwdtbsofwdyaejzutjshlhkfhutynkbglwnwtgsvanyclurveehosfptbrpfertwpetdkwegmsamtbebyjesrlbhgrystjnmwmniynywnjobawsstcevahnecnsaadylfwdlyaerddaatdajnbwjeidldcm
^D
f65d661fe895f8eaeb70f76f8d923c9a503ea82b6a7b9857bfe2fdd625041f172ba24c1834569bc1ae821886075d77662d2815bc3d8f628ff3d7d5fe3ad727b1534db3778a66a006e2e25fbfc429147873736c92acc48f5957dfa2ab85a020a5fc7f573116576bf7a43558c5b8e7f6d5f846fbb005cc3e764c529e6efa645cd1c6747746009e5f2d761c6ba6ad7f675633d4adf86f834820f4e3be2852678d776454783d94ecaddd544fa68fa4c9f7b4abab8a5841563512d59aff5022cd9172dd2b0053022bee459c0e1a94d24af10d0c88feb80cfde6f127fe4f3af1b731b54aa0b7590501a0c56c9c5c790ec3f7125fb2223590543cbf548213626baa66d9a9a8a2c3424483d83b4fbc43ee27fea5660a9847378a288e059a91b67377f1c0ac4ed42c30b7c91489798d5d0c1bfabd479175e42b3910778d10f6d4a7da50da1953eda9b80948ead6c94230006cdd715d593fddf67e4ef1f04ce69a21dfe431a741d6b645c0ec3824ed52c29610116bc37f57bdc76d948e669af1700eefc71ce660359c043082ea8100ba2507256d13
```

## Multipart Encoding of URs with Fountain Codes

When encoding a multi-part UR, seedtool outputs the minimum number of parts needed to reconstruct the message. You can, however, ask seedtool to produce any number of additional parts beyond the minimum. These are generated using [fountain codes](https://en.wikipedia.org/wiki/Fountain_code), such that each additional part carries some mix of the other parts. The primary purpose of this is to produce a continuous stream of QR codes that can be read starting at any point, and even with a certain number of QR codes missed, still reproduce the original message without ending up having to wait for a single specific part to be displayed.

```
#
# Generate a multi-part UR
#

$ seedtool --count 300 --deterministic TEST --ur=50
ur:crypto-seed/1-7/lpadatcfadehcyetoyioluhddwoyadhkaddwnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlnbkaatllr
ur:crypto-seed/2-7/lpaoatcfadehcyetoyioluhddwlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszogmdstpoennbtfljpyaihsn
ur:crypto-seed/3-7/lpaxatcfadehcyetoyioluhddwwykbtpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlblerhdest
ur:crypto-seed/4-7/lpaaatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhksscaimzt
ur:crypto-seed/5-7/lpahatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprpytiinki
ur:crypto-seed/6-7/lpamatcfadehcyetoyioluhddwiymotofgcemhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmneceegwrkkn
ur:crypto-seed/7-7/lpatatcfadehcyetoyioluhddwvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaeaeaerosssfjl
#
# Generate the same multi-part UR, but add 10 additional parts
#

$ seedtool --deterministic TEST --count 300 --ur=50 --parts 10
ur:crypto-seed/1-7/lpadatcfadehcyetoyioluhddwoyadhkaddwnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlnbkaatllr
ur:crypto-seed/2-7/lpaoatcfadehcyetoyioluhddwlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszogmdstpoennbtfljpyaihsn
ur:crypto-seed/3-7/lpaxatcfadehcyetoyioluhddwwykbtpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlblerhdest
ur:crypto-seed/4-7/lpaaatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhksscaimzt
ur:crypto-seed/5-7/lpahatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprpytiinki
ur:crypto-seed/6-7/lpamatcfadehcyetoyioluhddwiymotofgcemhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmneceegwrkkn
ur:crypto-seed/7-7/lpatatcfadehcyetoyioluhddwvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaeaeaerosssfjl
ur:crypto-seed/8-7/lpayatcfadehcyetoyioluhddwtotelphkaewkfrisssiofgkbkpmepychnyrotiztwylogyfdpflbjllppmtyesdylejktifmglendlltrfpfpkeeglchdakp
ur:crypto-seed/9-7/lpasatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkfxjypdtk
ur:crypto-seed/10-7/lpbkatcfadehcyetoyioluhddwdwjyuybdmkiyknftdacyaoqdtkaaiofxhglrvtcpmwwnsemtftosmshpamfeehweurtidmkneordbgcygadturseztzekpgl
ur:crypto-seed/11-7/lpbdatcfadehcyetoyioluhddwehdmfycnkblknsvdotwflfldynferlatkkaxcsdsbelrfgtlkshtlffmhldaryhetyrysnpyvelrtygmimlpwddivdytgsti
ur:crypto-seed/12-7/lpbnatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprhtwevebg
ur:crypto-seed/13-7/lpbtatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprflksylsk
ur:crypto-seed/14-7/lpbaatcfadehcyetoyioluhddwylehckclglrkhpnlwpykqdzsldgojoldpyhybzfxtlttpletinsgtdrfqdglwtzehkvlswmhsrwmvdpaclsflbjzctmefdbk
ur:crypto-seed/15-7/lpbsatcfadehcyetoyioluhddwknfynsdnzsfzbzuogtykzmihnbweneoxvaeelyrhihiduthkemwndalecwtijzbzgarfvdnletrhseinlsgmjpdnfmwlgytb
ur:crypto-seed/16-7/lpbeatcfadehcyetoyioluhddwntknhffwlpnljnghlffrspneaydwcmlgzsvoknnymwnsgoaewtztwnjlptvordgmmdynhhgtwznbynhngloyrktevtmufstd
ur:crypto-seed/17-7/lpbyatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspruroxcabe

#
# Reconstruct the message from a subset of the generated parts.
#

$ seedtool --in ur
ur:crypto-seed/2-7/lpaoatcfadehcyetoyioluhddwlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszogmdstpoennbtfljpyaihsn
ur:crypto-seed/4-7/lpaaatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhksscaimzt
ur:crypto-seed/5-7/lpahatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprpytiinki
ur:crypto-seed/7-7/lpatatcfadehcyetoyioluhddwvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaeaeaerosssfjl
ur:crypto-seed/8-7/lpayatcfadehcyetoyioluhddwtotelphkaewkfrisssiofgkbkpmepychnyrotiztwylogyfdpflbjllppmtyesdylejktifmglendlltrfpfpkeeglchdakp
ur:crypto-seed/9-7/lpasatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkfxjypdtk
ur:crypto-seed/10-7/lpbkatcfadehcyetoyioluhddwdwjyuybdmkiyknftdacyaoqdtkaaiofxhglrvtcpmwwnsemtftosmshpamfeehweurtidmkneordbgcygadturseztzekpgl
ur:crypto-seed/14-7/lpbaatcfadehcyetoyioluhddwylehckclglrkhpnlwpykqdzsldgojoldpyhybzfxtlttpletinsgtdrfqdglwtzehkvlswmhsrwmvdpaclsflbjzctmefdbk
ur:crypto-seed/16-7/lpbeatcfadehcyetoyioluhddwntknhffwlpnljnghlffrspneaydwcmlgzsvoknnymwnsgoaewtztwnjlptvordgmmdynhhgtwznbynhngloyrktevtmufstd
ur:crypto-seed/17-7/lpbyatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspruroxcabe
^D
9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e3bf736a89e9ceb105f2109fb5226d8a29e0d47ee7ed8774f20245ac5f47b95958b1483daa4aaabc9dad616a30bf338b4ef1e971d2cc449bdcc339258f93f7f91a3d067522b085ca6b2f7c72732ce5aed7ae0ef273f13c8d92ffa89b69cac18dd79664032e0f86fe3c1f1596fd2dc582c690f17407d0852932d23798056a424cacae3bfd4e30fe8030f943645fcdf4d86de1b45570778b26692ce461c9053c54a47443dae67bed34624f5acf2eebdf0b0e5283505d25ce6849aa0d0f10b1fdec20d8e35e6b2072c7fe3d84c4e950c989f0a781a92417732e2076fb302e4cc3ff7506a061a011f4cd4952ff6af
```

The example above uses a multi-part UR to encode a seed. Each of the parts of a multi-part UR are *not* equivalent to a share of a multi-share recovery tool like SSKR. Seedtool can also decode a SSKR seed from one or more single-part URs that each represent one share of a SSKR.

```
#
# Recover a SSKR seed using 2 of 3 shares. Each UR is one share.
#

$ seedtool --out sskr --group 2-of-3 --ur
ur:crypto-sskr/taadecgoretkaeadaesacachmnnsjkpklslujtvscezernhylngassdtin
ur:crypto-sskr/taadecgoretkaeadadrhfxlblgosbycerhiecfnesgnblfatmucedalboe
ur:crypto-sskr/taadecgoretkaeadaoeeoystlowdrlutylgllaampyfwswwppsaocfbtns

$ seedtool --in ur
ur:crypto-sskr/taadecgoretkaeadaesacachmnnsjkpklslujtvscezernhylngassdtin
ur:crypto-sskr/taadecgoretkaeadaoeeoystlowdrlutylgllaampyfwswwppsaocfbtns
^D
6546c39484fb4064f1a1279ba56dcec1
```

## Version History

### 0.9.1, 10/13/2020

* Removed `birthdate` field from generated `ur:crypto-seed` URs, which was causing unit tests to become invalid. A specific flag to set the `birthdate` field could be added at a later date.

### 0.9.0, 10/4/2020

* Updated to latest version of bc-ur, which fixes the [ByteWords wordlist alphabetization error](https://github.com/BlockchainCommons/Research/issues/45). This was a breaking change, so all documentation and examples were updated.

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
