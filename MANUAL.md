# 🌱 Seedtool

**Version 0.7.0**<br/>**August 12, 2020**

*Copyright © 2020 by Blockchain Commons, LLC*<br/>*Licensed under the "BSD-2-Clause Plus Patent License"*

---

## Contents

* [Introduction](#introduction)
* [General Functionality](#general-functionality)
* [Generate Random Seeds](#generate-random-seeds)
* [Generating Seeds from Provided Entropy](#generating-seeds-from-provided-entropy)
* [Deterministic Randomness](#deterministic-randomness)
* [Compatibility](#compatibility)
* [Uniform Resources (URs)](#uniform-resources-urs)
* [Version History](#version-history)

## Introduction

`seedtool` is a command-line tool for creating and transforming cryptographic seeds of the sort commonly used by blockchain applications.

**✅ NOTE:** There is no mention below of password functionality for BIP39 and SLIP39 production. The current design allows for later inclusion of this feature via a `--password` option.


See [`README.md`](README.md) for installation and credits.

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

An output format `--out` and count `--count` may be specified. Count may be in [1-64] and the default `count` is 16. For the `hex` and Bytewords (`btw`, `btwu`, `btwm`) output formats, the count is the number of bytes generated. For other output formats, `count` is the number of "output units" (e.g., bits, cards, die rolls, etc.)

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

SLIP39 can be used as an output format. By default `--count` is 16 bytes (128 bits, 20 words), but may be any multiple of 2 in [16-32]. By default a single 1-of-1 share is generated. This is the same as running `seedtool --out slip39 --group-threshold 1 --group 1-of-1`.

```
#
# Generate a random 16-byte seed and display it as SLIP39.
#

$ seedtool --out slip39
activity away academic academic debut usher impact evidence worthy check vampire famous gums screw upstairs reunion practice welcome surface pink

#
# Generate a random 32-byte seed and display it as SLIP39.
#

$ seedtool --out slip39 --count 32
maiden corner academic academic always salon pancake software numb mixed spider artist smith total activity snake grumpy discuss pancake step apart elephant moisture strike review racism orange emission society lecture mansion forbid estate
```

n-of-m SLIP39 shares can be generated by specifing one or more groups.

```
#
# Generate a 16 byte seed and display it as SLIP39, one group,
# 2-of-3 shares required for seed recovery.
#

$ seedtool --out slip39 --group 2-of-3
health guest academic acid browser process marvel acid unkind usher entrance hobo punish quarter lawsuit aquatic advance rebuild satisfy remind
health guest academic agency capital salt leader video remember health nervous negative scroll spew survive patent expect capture should favorite
health guest academic always bolt inmate window treat elevator swing trend loyalty brother knife drift vampire forward counter emphasis preach
```

Multiple groups and a group threshold may be specified. If omitted, the default `--group-threshold` is 1.

**✅ NOTE**: The first two words of a set of SLIP39 shares are the same across all the shares. Each group within a set of SLIP39 shares has the same third word.

```
#
# Generate a 16 byte seed and display it as SLIP39, first group
# requires 2-of-3, second and third groups require 3-of-5, at least
# two groups must meet their threshold.
#

$ seedtool --out slip39 --group-threshold 2 --group 2-of-3 --group 3-of-5 --group 3-of-5
crucial enlarge acrobat leaf actress scatter brother cricket declare trial glance adult dress excuse cleanup coding behavior ting submit teacher
crucial enlarge acrobat lily aunt smirk strike plan oral terminal crunch mandate spark idle execute multiple exotic club learn rhythm
crucial enlarge acrobat lungs careful space necklace curious crunch zero mustang quiet gray detailed video vintage preach lunar hesitate upstairs
crucial enlarge beard learn desire pants display employer prayer square space shrimp warn expand buyer detailed likely safari therapy paces
crucial enlarge beard lips cultural symbolic scholar divorce flea drift exact join satisfy cowboy install decision mule ancient ruin network
crucial enlarge beard luxury blind mayor echo breathe froth chew payment payroll liberty excuse junction genius vitamins ordinary lift general
crucial enlarge beard march aquatic venture response group prize shadow dress away remember crowd axle gravity that finger phrase gross
crucial enlarge beard method dynamic legal shelter mineral switch sharp item scatter thumb frozen already slice family again pitch warmth
crucial enlarge ceramic learn custody ranked become kind petition burden axis royal freshman gasoline acrobat identify desire olympic depict marvel
crucial enlarge ceramic lips diminish necklace reject thunder increase aide senior wrist race papa depict national havoc grasp inform extra
crucial enlarge ceramic luxury anxiety platform marvel expect browser unfold shadow withdraw software trend river license guitar true beard identify
crucial enlarge ceramic march beaver lilac cleanup stadium smirk valid away salon dismiss corner material grief dictate bike firm pregnant
crucial enlarge ceramic method custody maximum campus earth ordinary twice adequate twin prayer cowboy relate window ruin raspy dominant equation
```

## Generating Seeds from Provided Entropy

When the `--in` option is used, seedtool takes one or more arguments and uses them to construct the seed. If no arguments are given on the command line, it reads input from stdin and uses what it reads to construct the seed. In the examples below, the end of input to stdin is marked by `^D` on its own line.

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

`bip39` and `slip39` output formats can be combined with the `random` (default) input format. If the `--count N` option is used with the `hex` or Bytewords (`btw`, `btwu`, `btwm`) input formats, it results in a seed of `N` bytes being generated and used.

```
#
# Create a random 16-byte seed and display it as BIP39.
#

$ seedtool --out bip39
monitor place true skirt uncover scissors tower alley fame grunt sun outer

#
# Create a random 32-byte seed and display it as a single
# SLIP39 share.
#

$ seedtool --in random --out slip39 --count 32
pumps guest academic academic analysis election admit harvest very webcam acquire answer primary viral venture declare have short bucket pickup pistol squeeze script racism western alarm depend depart lilac zero capacity capture warn
```

`bip39` and `slip39` output formats can be combined with the `hex` or Bytewords (`btw`, `btwu`, `btwm`) input formats. The `--count` option is not allowed and the whole hex seed is used. For `bip39` the seed must be 12-32 bytes and even. For `slip39` the seed must be 16-32 bytes and even.

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

`slip39` can be used as an input format, in which case the original seed is recovered. The SLIP39 shares may be passed on the command line or entered via stdin. If passed on the command line, the shares must each be a single argument (i.e., quoted). If passed via stdin, each share must appear by itself on one line.

```
#
# Recover a seed from a SLIP39 share, providing the entire share
# as a single (quoted) argument on the command line.
#

$ seedtool --in slip39 "activity away academic academic debut usher impact evidence worthy check vampire famous gums screw upstairs reunion practice welcome surface pink"
6c641757596dfefb95863b13f5f8a247

#
# Recover from the same SLIP39 share, providing the share via stdin.
#

$ seedtool --in slip39
activity away academic academic debut usher impact evidence worthy check vampire famous gums screw upstairs reunion practice welcome surface pink
^D
6c641757596dfefb95863b13f5f8a247

#
# The same SLIP39 share is used as above, but since it is not
# quoted, seedtool tries to use each word as a single share and
# fails to recover the seed.
#

$ seedtool --in slip39 activity away academic academic debut usher impact evidence worthy check vampire famous gums screw upstairs reunion practice welcome surface pink
seedtool: Invalid SLIP39 shares.

```

In this example, 2 shares of a 2-of-3 split are entered on the command line (each separately quoted) and via stdin (each on its own separate line.)

```
#
# Generate a random 16-byte seed and display it as SLIP39 in a
# single group requiring 2-of-3 shares.
#

$ seedtool --out slip39 --group 2-of-3
cowboy leader academic acid critical employer aspect stick result much camera favorite smirk domestic staff phantom fake result slush loud
cowboy leader academic agency biology imply grasp buyer strategy founder alive hybrid cultural forget maiden playoff analysis home moment snapshot
cowboy leader academic always benefit crisis quick tendency decision being curious priority evoke welfare hour burning champion tracks maiden salary

#
# Recover from the first and third shares above, providing each
# separately quoted on the command line.
#

$ seedtool --in slip39 "cowboy leader academic acid critical employer aspect stick result much camera favorite smirk domestic staff phantom fake result slush loud" "cowboy leader academic always benefit crisis quick tendency decision being curious priority evoke welfare hour burning champion tracks maiden salary"
747008a3b462468c5c3e62bf692349d0

#
# Recover from the first and third shares above, providing each
# on its own separate line via stdin.
#

$ seedtool --in slip39
cowboy leader academic acid critical employer aspect stick result much camera favorite smirk domestic staff phantom fake result slush loud
cowboy leader academic always benefit crisis quick tendency decision being curious priority evoke welfare hour burning champion tracks maiden salary
^D
747008a3b462468c5c3e62bf692349d0
```

## Deterministic Randomness

When using the `--in random` (default input format) option, seedtool uses a cryptographically-strong random number generator provided by the operating system. The same random number generator is also used when constructing SLIP39 shares, so the same entropy input will yield different shares each time.

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
# Construct SLIP39 share from seed.
#

$ seedtool --in hex --out slip39 5d1c30bbc6f3cfd070067b63c851ffe7
edge enlarge academic academic body necklace surprise resident burden taxi painting slim research teammate peasant ivory weapon gesture voice flexible

#
# Recover the seed from the SLIP39 share.
#

$ seedtool --in slip39 "edge enlarge academic academic body necklace surprise resident burden taxi painting slim research teammate peasant ivory weapon gesture voice flexible"
5d1c30bbc6f3cfd070067b63c851ffe7

#
# Again construct SLIP39 share from the same seed. Notice the
# share is different than last time.
#

$ seedtool --in hex --out slip39 5d1c30bbc6f3cfd070067b63c851ffe7
result leader academic academic apart length alcohol adult patrol military counter enjoy animal standard club facility belong rumor problem answer

#
# Recover the second share and notice that the seed is the same.
#

$ seedtool --in slip39 "result leader academic academic apart length alcohol adult patrol military counter enjoy animal standard club facility belong rumor problem answer"
5d1c30bbc6f3cfd070067b63c851ffe7
```


Seedtool also provides the `--deterministic S` option, which takes a string `S`, produces the SHA256 hash of that string, and then uses that to seed it's own cryptography-quality random number generator it uses for the rest of its run. This means that seeds generated by seedtool with the same `--deterministic` input will yield the same results.

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

In this example, the same entropy is used twice in producing a SLIP39 share, which would normally result in two different shares being produced. But by providing `--deterministic FOOBAR`, the same results are produced.

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
# Display our seed as a SLIP39 2-of-3 shares, using
# deterministic randomness.
#

$ seedtool --deterministic FOOBAR --in hex --out slip39 --group 2-of-3 5cd271b50b98a869da1c26a526e1d3a8
response lunch academic acid breathe ocean mixture traffic object sheriff rapids victim froth testify retreat patent prize human usual hobo
response lunch academic agency activity fiber escape research earth pipeline prevent prepare activity erode grumpy problem justice vampire tension custody
response lunch academic always crisis cards chemical expect muscle anxiety training thunder climate plan geology bulb scatter teacher gravity guilt

#
# Do all of the above on a single line, and note that because
# of deterministic randomness, the same final output is produced.
#

$ seedtool --deterministic FOOBAR --out dice | seedtool --in dice | seedtool --deterministic FOOBAR --in hex --out slip39 --group 2-of-3
response lunch academic acid breathe ocean mixture traffic object sheriff rapids victim froth testify retreat patent prize human usual hobo
response lunch academic agency activity fiber escape research earth pipeline prevent prepare activity erode grumpy problem justice vampire tension custody
response lunch academic always crisis cards chemical expect muscle anxiety training thunder climate plan geology bulb scatter teacher gravity guilt
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

Seedtool can encode and decode binary (hex) seeds, BIP39-encoded seeds, or SLIP39-encoded shares in the Uniform Resource (UR) format. This format is defined in [BCR-0005: Uniform Resources (UR): Encoding Structured Binary Data for Transport in URIs and QR Codes](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0005-ur.md). The UR types supported for encoding and decoding are `crypto-seed`, `crypto-bip39` and `crypto-slip39`. These types are defined in [BCR-0006: Registry of Uniform Resource (UR) Types](https://github.com/BlockchainCommons/Research/blob/master/papers/bcr-0006-urtypes.md).

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
ur:crypto-bip39/5gqccemvda38xar9wfjxxmr4vf5xvetnw35hvctvv3jkzumev3kk2ctwvashgarjv93hgetnd9nksar9v4shyargv4cxj7n6v9jxymmy09nkket5vd582urxwfjkvatnv5pdseqeglssz4y9pq
^D
4dbec34b9f72a0f315af983222b2ca22

#
# Re-encode the previous result to BIP39 words
#

$ seedtool --out bip39 --in hex 4dbec34b9f72a0f315af983222b2ca22
ethics voyage spray discover claw kangaroo find vessel cram better raw dune

#
# Re-encode the previous result to Bytewords
#

$ seedtool --out btw --in hex 4dbec34b9f72a0f315af983222b2ca22
gift ruin scar gear note jump numb wolf buzz pose monk easy cusp purr song cusp kick wave keno brew
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

![](manual-images/seedqrcode.png)

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
$ seedtool --ur=100 --out slip39 --group 2-of-3
ur:crypto-slip39/1-5/ltadahcfadsgcyrtrhhfprhdhhoeadlsmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiehsiainieisiehskpioisjyihjpisjpihhsiajyinjljtisiehsjpjejtihjkjkihjyjpkpjkjyiohsjnhskninjtioiyiyjzhskojljpihktjljnhsjtihihhsjppmlfenwd
ur:crypto-slip39/2-5/ltaoahcfadsgcyrtrhhfprhdhhjyisiyktihidiahsjniojoishsjtjyjljniyiahsjpidjljtiyjljphsjtioihisjkjyjphsjyihiokkiojzhsktjkkpinjyiyiohsjzhskskkisimkpieiniainhsjzmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsamolzojk
ur:crypto-slip39/3-5/ltaxahcfadsgcyrtrhhfprhdhhioihjtiakkiyiajljzkpjnjtiyimhsiajeihjyiokohsjnjoinjpihiyhsiyjphsinieiyhskpiokpjkjyisjljpieinjthsjpkkiejphsiaihiskpjojkjyhsinjpjkihjyjpihhsjyiyjkinjkjyihjpioihjtiejljpjkihisiyinjtieinjtjsfhrnfs
ur:crypto-slip39/4-5/ltaaahcfadsgcyrtrhhfprhdhhiojkiyiohsjyisihjpiyjphsinjkinjtisidjpinihiyinjtioiyjpihkoinihktmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsjzkthskkjkihihhsjpjyisioieihiajzhsjpihiyjnkpjkiajzihihjliyjyihjtemfwmecn
ur:crypto-slip39/5-5/ltahahcfadsgcyrtrhhfprhdhhiojkiahsjyjyihjpiyjkjyihhsiekkihiskpjnhsjtiyjokpjtinjkisisidjpinihiyinjtioiyinjkjzhsjtieiejkhsjzjyiyiahsjpjoihjyioiakpjkjyjliekkihjskpinihjyiokoinjtjyhsioihihjnkpjkiniaaotpiecffdenaeaechoyrnzm
```

The original seed can be recovered from the above:

```
$ seedtool --in ur
ur:crypto-slip39/1-5/ltadahcfadsgcyrtrhhfprhdhhoeadlsmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiehsiainieisiehskpioisjyihjpisjpihhsiajyinjljtisiehsjpjejtihjkjkihjyjpkpjkjyiohsjnhskninjtioiyiyjzhskojljpihktjljnhsjtihihhsjppmlfenwd
ur:crypto-slip39/2-5/ltaoahcfadsgcyrtrhhfprhdhhjyisiyktihidiahsjniojoishsjtjyjljniyiahsjpidjljtiyjljphsjtioihisjkjyjphsjyihiokkiojzhsktjkkpinjyiyiohsjzhskskkisimkpieiniainhsjzmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsamolzojk
ur:crypto-slip39/3-5/ltaxahcfadsgcyrtrhhfprhdhhioihjtiakkiyiajljzkpjnjtiyimhsiajeihjyiokohsjnjoinjpihiyhsiyjphsinieiyhskpiokpjkjyisjljpieinjthsjpkkiejphsiaihiskpjojkjyhsinjpjkihjyjpihhsjyiyjkinjkjyihjpioihjtiejljpjkihisiyinjtieinjtjsfhrnfs
ur:crypto-slip39/4-5/ltaaahcfadsgcyrtrhhfprhdhhiojkiyiohsjyisihjpiyjphsinjkinjtisidjpinihiyinjtioiyjpihkoinihktmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsjzkthskkjkihihhsjpjyisioieihiajzhsjpihiyjnkpjkiajzihihjliyjyihjtemfwmecn
ur:crypto-slip39/5-5/ltahahcfadsgcyrtrhhfprhdhhiojkiahsjyjyihjpiyjkjyihhsiekkihiskpjnhsjtiyjokpjtinjkisisidjpinihiyinjtioiyinjkjzhsjtieiejkhsjzjyiyiahsjpjoihjyioiakpjkjyjliekkihjskpinihjyiokoinjtjyhsioihihjnkpjkiniaaotpiecffdenaeaechoyrnzm
^D
3b9e4e41fe053ae2b2f0aea8de8c5fad
```

The encoded SLIP-39 shares can also be recovered:

```
$ seedtool --in ur --out slip39
ur:crypto-slip39/1-5/ltadahcfadsgcyrtrhhfprhdhhoeadlsmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiehsiainieisiehskpioisjyihjpisjpihhsiajyinjljtisiehsjpjejtihjkjkihjyjpkpjkjyiohsjnhskninjtioiyiyjzhskojljpihktjljnhsjtihihhsjppmlfenwd
ur:crypto-slip39/2-5/ltaoahcfadsgcyrtrhhfprhdhhjyisiyktihidiahsjniojoishsjtjyjljniyiahsjpidjljtiyjljphsjtioihisjkjyjphsjyihiokkiojzhsktjkkpinjyiyiohsjzhskskkisimkpieiniainhsjzmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsamolzojk
ur:crypto-slip39/3-5/ltaxahcfadsgcyrtrhhfprhdhhioihjtiakkiyiajljzkpjnjtiyimhsiajeihjyiokohsjnjoinjpihiyhsiyjphsinieiyhskpiokpjkjyisjljpieinjthsjpkkiejphsiaihiskpjojkjyhsinjpjkihjyjpihhsjyiyjkinjkjyihjpioihjtiejljpjkihisiyinjtieinjtjsfhrnfs
ur:crypto-slip39/4-5/ltaaahcfadsgcyrtrhhfprhdhhiojkiyiohsjyisihjpiyjphsinjkinjtisidjpinihiyinjtioiyjpihkoinihktmwisjkihiakpjpinjykkiyiohsjpieihjtishsiahsieihjniniaiyhsjzkthskkjkihihhsjpjyisioieihiajzhsjpihiyjnkpjkiajzihihjliyjyihjtemfwmecn
ur:crypto-slip39/5-5/ltahahcfadsgcyrtrhhfprhdhhiojkiahsjyjyihjpiyjkjyihhsiekkihiskpjnhsjtiyjokpjtinjkisisidjpinihiyinjtioiyinjkjzhsjtieiejkhsjzjyiyiahsjpjoihjyioiakpjkjyjliekkihjskpinihjyiokoinjtjyhsioihihjnkpjkiniaaotpiecffdenaeaechoyrnzm
^D
security garden academic acid daughter reaction darkness trust amazing flavor woman earth webcam phantom carbon orange strategy lawsuit galaxy judicial
security garden academic agency column jacket vampire afraid august ordinary race upstairs treat sister endorse findings gather raisin briefing review
security garden academic always earth declare muscle often scatter steady human punish briefing island salt carpet custody quiet vintage music
```

```
$ seedtool --in slip39
genuine taught academic acid calcium often grocery item blanket quarter froth tadpole argue deliver stick slim bumpy evidence campus scholar
genuine taught academic agency describe roster isolate domestic ting eraser scramble story lamp omit founder true render similar spray alcohol
genuine taught academic always antenna custody deploy exceed raisin forget holy species presence insect style desert carve keyboard bumpy domestic
^D
3b9e4e41fe053ae2b2f0aea8de8c5fad
```

## Multipart Encoding of URs with Fountain Codes

When encoding a multi-part UR, seedtool outputs the minimum number of parts needed to reconstruct the message. You can, however, ask seedtool to produce any number of additional parts beyond the minimum. These are generated using [fountain codes](https://en.wikipedia.org/wiki/Fountain_code), such that each additional part carries some mix of the other parts. The primary purpose of this is to produce a continuous stream of QR codes that can be read starting at any point, and even with a certain number of QR codes missed, still reproduce the original message without ending up having to wait for a single specific part to be displayed.

```
#
# Generate a multi-part UR
#

$ seedtool --deterministic=TEST --ur=50 --out slip39 --group 2-of-3
ur:crypto-slip39/1-9/ltadascfadrkcyrkfmesjlhdeyoeadlsmwioiahsjpihiykpjzisjohsinjtjyinjtioishsiahsieihjniniaiehsiainieihhsiekpjzjyihjniniejkjyiejzihnldafplf
ur:crypto-slip39/2-9/ltaoascfadrkcyrkfmesjlhdeyhsiyiejtkpjnidiyjzihhskoihjkiyinjtiyhsjtjyiejyjpinjoiojkiahsjtiehsjzisktinjpihjzihjkjkisiejljninjthstyvwrsao
ur:crypto-slip39/3-9/ltaxascfadrkcyrkfmesjlhdeyjtjyihjkjnihhsjpisjkkpjtjzinioisjyihjyjpihjtieisiyinjtieinjtiojkihjkjyhsiyiyieiajzkpidmwioiahsjpihiytkltzoda
ur:crypto-slip39/4-9/ltaaascfadrkcyrkfmesjlhdeykpjzisjohsinjtjyinjtioishsiahsieihjniniaiyhsioihjtiakkioidihjzinihkoihiohsjziajlisjljziyieihiaihjtjykepyttkk
ur:crypto-slip39/5-9/ltahascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkntpskbso
ur:crypto-slip39/6-9/ltamascfadrkcyrkfmesjlhdeyiyiajlkpjpjkihihiainkoinjzieiyhsiajyiojpihkpjtinjljtioiajzjljyisihjkmwioiahsjpihiykpjzisjohsinjtjyinlyrfaofm
ur:crypto-slip39/7-9/ltatascfadrkcyrkfmesjlhdeyjtioishsiahsieihjniniaiyhsjzkthskkjkisiahsjyihiojljpkkiojoihjthsjzjykkiyjnhsjpkoihjziekthsjpjtihiakpasrpjymw
ur:crypto-slip39/8-9/ltayascfadrkcyrkfmesjlhdeyidiniaiojkjzhskoihjpkkisjyihhsjkjojljljtiykthsjzjtkpjyihiejlkpioisioiyinjkisinjtioisieihhsiejzinjtihjswptaps
ur:crypto-slip39/9-9/ltasascfadrkcyrkfmesjlhdeyisiejphsjnhsjyiniaihjzhsieihjtihjljpidinjyiyiohsjyisihjpisiejliakpjnihjtjyaotpiecffdenaeaeaeaeaeaeaetdwtheke

#
# Generate the same multi-part UR, but add 10 additional parts
#

$ seedtool --deterministic=TEST --ur=50 --out slip39 --group 2-of-3 --parts 10
ur:crypto-slip39/1-9/ltadascfadrkcyrkfmesjlhdeyoeadlsmwioiahsjpihiykpjzisjohsinjtjyinjtioishsiahsieihjniniaiehsiainieihhsiekpjzjyihjniniejkjyiejzihnldafplf
ur:crypto-slip39/2-9/ltaoascfadrkcyrkfmesjlhdeyhsiyiejtkpjnidiyjzihhskoihjkiyinjtiyhsjtjyiejyjpinjoiojkiahsjtiehsjzisktinjpihjzihjkjkisiejljninjthstyvwrsao
ur:crypto-slip39/3-9/ltaxascfadrkcyrkfmesjlhdeyjtjyihjkjnihhsjpisjkkpjtjzinioisjyihjyjpihjtieisiyinjtieinjtiojkihjkjyhsiyiyieiajzkpidmwioiahsjpihiytkltzoda
ur:crypto-slip39/4-9/ltaaascfadrkcyrkfmesjlhdeykpjzisjohsinjtjyinjtioishsiahsieihjniniaiyhsioihjtiakkioidihjzinihkoihiohsjziajlisjljziyieihiaihjtjykepyttkk
ur:crypto-slip39/5-9/ltahascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkntpskbso
ur:crypto-slip39/6-9/ltamascfadrkcyrkfmesjlhdeyiyiajlkpjpjkihihiainkoinjzieiyhsiajyiojpihkpjtinjljtioiajzjljyisihjkmwioiahsjpihiykpjzisjohsinjtjyinlyrfaofm
ur:crypto-slip39/7-9/ltatascfadrkcyrkfmesjlhdeyjtioishsiahsieihjniniaiyhsjzkthskkjkisiahsjyihiojljpkkiojoihjthsjzjykkiyjnhsjpkoihjziekthsjpjtihiakpasrpjymw
ur:crypto-slip39/8-9/ltayascfadrkcyrkfmesjlhdeyidiniaiojkjzhskoihjpkkisjyihhsjkjojljljtiykthsjzjtkpjyihiejlkpioisioiyinjkisinjtioisieihhsiejzinjtihjswptaps
ur:crypto-slip39/9-9/ltasascfadrkcyrkfmesjlhdeyisiejphsjnhsjyiniaihjzhsieihjtihjljpidinjyiyiohsjyisihjpisiejliakpjnihjtjyaotpiecffdenaeaeaeaeaeaeaetdwtheke
ur:crypto-slip39/10-9/ltbkascfadrkcyrkfmesjlhdeyidiniaiojkjzhskoihjpkkisjyihhsjkjojljljtiykthsjzjtkpjyihiejlkpioisioiyinjkisinjtioisieihhsiejzinjtihswpkctbt
ur:crypto-slip39/11-9/ltbdascfadrkcyrkfmesjlhdeyuyiavtvocmatbkbzaabwaybkcecycmctaoahataeambkaobtbaaebyayctbnbwbaaobnckbtckadbtcwckbaadcaaeaebzbtbsadlopalbrs
ur:crypto-slip39/12-9/ltbnascfadrkcyrkfmesjlhdeyiahnkpkojejpktlbknjnjejtjoiekbjykbhniekbknlbiyjtjykoihjkkphnimjektkbkslbjobttkkobefpeobyaachbschbsbnwzgryawf
ur:crypto-slip39/13-9/ltbtascfadrkcyrkfmesjlhdeyayaaatbbbybgadaebaaebzbsbtaybyaecyatbsbyaaadbdbaaececkaacebkcyasasatweadbaaeaebwaxcfayctbybwatbdchcedmaegwvo
ur:crypto-slip39/14-9/ltbaascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkdwkkfdhe
ur:crypto-slip39/15-9/ltbsascfadrkcyrkfmesjlhdeyjtjyihjkjnihhsjpisjkkpjtjzinioisjyihjyjpihjtieisiyinjtieinjtiojkihjkjyhsiyiyieiajzkpidmwioiahsjpihiysgwniehs
ur:crypto-slip39/16-9/ltbeascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkdkdreokp
ur:crypto-slip39/17-9/ltbyascfadrkcyrkfmesjlhdeyaaahbdcsbectahbzcsaaahctbeckadcfamatbscycwasbecyamceckbzbaaeahbtbsbdbnaeaeceaeayascmbgbaadbkbncwaocsgmwpaypk
ur:crypto-slip39/18-9/ltbgascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkmujzykty
ur:crypto-slip39/19-9/ltbwascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkdaktbzox

#
# Reconstruct the message from a subset of the generated parts.
#

$ seedtool --in ur
ur:crypto-slip39/1-9/ltadascfadrkcyrkfmesjlhdeyoeadlsmwioiahsjpihiykpjzisjohsinjtjyinjtioishsiahsieihjniniaiehsiainieihhsiekpjzjyihjniniejkjyiejzihnldafplf
ur:crypto-slip39/2-9/ltaoascfadrkcyrkfmesjlhdeyhsiyiejtkpjnidiyjzihhskoihjkiyinjtiyhsjtjyiejyjpinjoiojkiahsjtiehsjzisktinjpihjzihjkjkisiejljninjthstyvwrsao
ur:crypto-slip39/3-9/ltaxascfadrkcyrkfmesjlhdeyjtjyihjkjnihhsjpisjkkpjtjzinioisjyihjyjpihjtieisiyinjtieinjtiojkihjkjyhsiyiyieiajzkpidmwioiahsjpihiytkltzoda
ur:crypto-slip39/4-9/ltaaascfadrkcyrkfmesjlhdeykpjzisjohsinjtjyinjtioishsiahsieihjniniaiyhsioihjtiakkioidihjzinihkoihiohsjziajlisjljziyieihiaihjtjykepyttkk
ur:crypto-slip39/5-9/ltahascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkntpskbso
ur:crypto-slip39/10-9/ltbkascfadrkcyrkfmesjlhdeyidiniaiojkjzhskoihjpkkisjyihhsjkjojljljtiykthsjzjtkpjyihiejlkpioisioiyinjkisinjtioisieihhsiejzinjtihswpkctbt
ur:crypto-slip39/11-9/ltbdascfadrkcyrkfmesjlhdeyuyiavtvocmatbkbzaabwaybkcecycmctaoahataeambkaobtbaaebyayctbnbwbaaobnckbtckadbtcwckbaadcaaeaebzbtbsadlopalbrs
ur:crypto-slip39/12-9/ltbnascfadrkcyrkfmesjlhdeyiahnkpkojejpktlbknjnjejtjoiekbjykbhniekbknlbiyjtjykoihjkkphnimjektkbkslbjobttkkobefpeobyaachbschbsbnwzgryawf
ur:crypto-slip39/13-9/ltbtascfadrkcyrkfmesjlhdeyayaaatbbbybgadaebaaebzbsbtaybyaecyatbsbyaaadbdbaaececkaacebkcyasasatweadbaaeaebwaxcfayctbybwatbdchcedmaegwvo
ur:crypto-slip39/18-9/ltbgascfadrkcyrkfmesjlhdeyihiajlkoihjpiojkjyhsieinkpjniojoishsjtjyjljnieisjljzkkiyjnhsjeinjtioiektinjtihiejzihhsiyihihhsjpjzkkmujzykty
^D
9d347f841a4e2ce6bc886e1aee74d824
```

## Version History

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
