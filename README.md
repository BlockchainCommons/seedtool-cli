# 🌱 Blockchain Commons bc-seedtool-cli

## Introduction

`seedtool` is a command-line tool for creating and transforming cryptographic seeds of the sort commonly used by blockchain applications. Here are a few examples of use:

### Generate a cryptographically-strong 16-byte seed

```
$ seedtool
8935a8068526d84da555cdb741a3b8a8
```

### Encode the above seed as BIP-39

```
$ seedtool --in hex --out bip39 8935a8068526d84da555cdb741a3b8a8
matrix pull accuse apart horn chat next rifle resemble artist until eye
```

### Decode the above BIP-39 back to hex

```
$ seedtool --in bip39 "matrix pull accuse apart horn chat next rifle resemble artist until eye"
8935a8068526d84da555cdb741a3b8a8
```

### Decode a seed encoded by Bytewords back to hex

```
$ seedtool --in btw "deli need cats taxi dice door webs vows free zest legs wall half waxy trip oval memo sets rock hill"
279b18d0282aefe845fb83e956eed8a6
```

### Generate a seed using entropy provided as coin flips

```
$ seedtool --in bits 1110110001110111
8d933e43b1bc8f2e3fc27adc98ad4534
```

### Generate a 32-byte seed using entropy provided as cards drawn from a deck of playing cards

```
$ seedtool --count 32 --in cards 6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs
7df301924511326d7350be14c9e7176d98e945f9ad0ed034726ad4ee0de59c25
```

### Generate a 16-byte seed and encode it using SSKR as 3 shares, 2 of which are required for recovery

```
$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro edge twin able acid able yoga nail wand keno paid ruin jazz keys acid time film pool skew flew luck ruby song owls soap obey
tuna acid epic gyro edge twin able acid acid yell omit mild webs warm epic flew liar view fuel deli fund glow skew dull knob claw gray surf wand
tuna acid epic gyro edge twin able acid also visa wave bulb hope drum quad duty need tied vast barn kick task gray tent crux owls easy jolt toil
```

### Recover the above seed using 2 of the 3 shares

```
$ seedtool --in sskr
tuna acid epic gyro edge twin able acid able yoga nail wand keno paid ruin jazz keys acid time film pool skew flew luck ruby song owls soap obey
tuna acid epic gyro edge twin able acid also visa wave bulb hope drum quad duty need tied vast barn kick task gray tent crux owls easy jolt toil
^D
8a7e9c3c0d783371d80e1192e5f6217d
```

### Generate a seed, encode it as UR, transform it to upper case, display it on the console, and encode it to a QR Code in the file "seedqrcode.png"

```
$ seedtool --ur | tr [:lower:] [:upper:] | tee /dev/tty | qrencode -o seedqrcode.png -l L
UR:CRYPTO-SEED/OEADGDHHGTPKFXLGHHLBPYMHLFRYHLPLSSRKPKAOTPIECFFDENTNJNWNPF
```

![](manual-images/seedqrcode.png)

### Generate a 64-byte seed using a deterministic random number generator and encode it as a multi-part UR with a maximum fragment size of 20 bytes

```
$ seedtool --deterministic=TEST --count 64 --ur=20
ur:crypto-seed/1-4/ltadaacsgecyuywdrnesguoeadhdfznteelblrcygldwvarflojtcywyjytpfsmdidpk
ur:crypto-seed/2-4/ltaoaacsgecyuywdrnesgudkfwprylienshnjnpluypmamtkmybsjkspvseeehwnpdbb
ur:crypto-seed/3-4/ltaxaacsgecyuywdrnesgusawmrltdlplgkplfbkqzztglfeoyaegsnedtronnfgpsas
ur:crypto-seed/4-4/ltaaaacsgecyuywdrnesguwsdpgtimmwzspfqdjkhshyaotpiecffdemaeaedmnsoxhf
```

### Same as above, but generate 5 additional parts using fountain codes

```
$ seedtool --deterministic=TEST --count 64 --ur=20 --parts 5
ur:crypto-seed/1-4/ltadaacsgecyuywdrnesguoeadhdfznteelblrcygldwvarflojtcywyjytpfsmdidpk
ur:crypto-seed/2-4/ltaoaacsgecyuywdrnesgudkfwprylienshnjnpluypmamtkmybsjkspvseeehwnpdbb
ur:crypto-seed/3-4/ltaxaacsgecyuywdrnesgusawmrltdlplgkplfbkqzztglfeoyaegsnedtronnfgpsas
ur:crypto-seed/4-4/ltaaaacsgecyuywdrnesguwsdpgtimmwzspfqdjkhshyaotpiecffdemaeaedmnsoxhf
ur:crypto-seed/5-4/ltahaacsgecyuywdrnesguwsdpgtimmwzspfqdjkhshyaotpiecffdemaeaegtndkijp
ur:crypto-seed/6-4/ltamaacsgecyuywdrnesguoeadhdfznteelblrcygldwvarflojtcywyjytptkwfjech
ur:crypto-seed/7-4/ltataacsgecyuywdrnesgusbjlzontwtiytiueutrdwfaachwmcmfrzovseerfsefmdp
ur:crypto-seed/8-4/ltayaacsgecyuywdrnesguhnwdwsmocwrhbkambezstspdytdtjthfjshlhnbdpygmao
ur:crypto-seed/9-4/ltasaacsgecyuywdrnesgusawmrltdlplgkplfbkqzztglfeoyaegsnedtroynmduyvl
```

### Recover the seed using a subset of the generated parts

```
$ seedtool --in ur
ur:crypto-seed/2-4/ltaoaacsgecyuywdrnesgudkfwprylienshnjnpluypmamtkmybsjkspvseeehwnpdbb
ur:crypto-seed/4-4/ltaaaacsgecyuywdrnesguwsdpgtimmwzspfqdjkhshyaotpiecffdemaeaedmnsoxhf
ur:crypto-seed/6-4/ltamaacsgecyuywdrnesguoeadhdfznteelblrcygldwvarflojtcywyjytptkwfjech
ur:crypto-seed/8-4/ltayaacsgecyuywdrnesguhnwdwsmocwrhbkambezstspdytdtjthfjshlhnbdpygmao
^D
9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e
```

### Display usage and help information

```
$ seedtool --help
```

## Full Documentation

See [`MANUAL.md`](MANUAL.md) for details, many more examples, and version history.

## Dependencies

* [`bc-crypto-base`](https://github.com/blockchaincommons/bc-crypto-base)
* [`bc-shamir`](https://github.com/blockchaincommons/bc-shamir)
* [`bc-sskr`](https://github.com/blockchaincommons/bc-sskr)
* [`bc-bip39`](https://github.com/blockchaincommons/bc-bip39)
* [`bc-ur`](https://github.com/blockchaincommons/bc-ur)
* [`GNU argp`](https://www.gnu.org/software/libc/manual/html_node/Argp.html)
* [`CBOR Lite`](https://bitbucket.org/isode/cbor-lite)

## Installation

These dependencies are automatically installed as submodules when you run the build script. This is the recommended way to install.

### MacOS

```bash
$ ./build.sh
$ sudo make install
```

### Linux

Make sure you have llvm/clang, libc++ and libc++abi installed. All of them with
a minimum recommended version 10.

#### Ubuntu and Debian

```bash
$ sudo apt-get install make

$ wget https://apt.llvm.org/llvm.sh
$ chmod +x llvm.sh
$ sudo ./llvm.sh 10  # version 10

$ sudo apt-get install libc++-10-dev libc++abi-10-dev
```

```bash
$ export CC="clang-10" && export CXX="clang++-10" && ./build.sh
$ sudo make install
```

### Windows

See [instructions here](Windows.md).

## Alternative Installation

This sequence does *not* install the dependencies from submodules; instead they must be installed in the usual places on the build system, otherwise the `./configure` step below will fail.

```bash
$ ./configure
$ make
$ sudo make install
```
*Note:* On Linux the first step is `./configure CC=clang-10 CXX=clang++-10`

## Incremental Builds

If you wish to make changes to the source code and rebuild:

```bash
# Make source changes
$ source set_build_paths.sh # sets shell variables used by make
$ make clean # If you want a clean build
$ make
```

## Notes for Maintainers

Before accepting a PR that can affect build or unit tests, make sure the following sequence of commands succeeds:

```bash
$ ./build.sh
$ make check
$ make distclean
```

## Origin, Authors, Copyright & Licenses

Unless otherwise noted (either in this [/README.md](./README.md) or in the file's header comments) the contents of this repository are Copyright © 2020 by Blockchain Commons, LLC, and are [licensed](./LICENSE) under the [spdx:BSD-2-Clause Plus Patent License](https://spdx.org/licenses/BSD-2-Clause-Patent.html).

In most cases, the authors, copyright, and license for each file reside in header comments in the source code. When it does not we have attempted to attribute it accurately in the table below.

This table below also establishes provenance (repository of origin, permalink, and commit id) for files included from repositories that are outside of this repository. Contributors to these files are listed in the commit history for each repository, first with changes found in the commit history of this repo, then in changes in the commit history of their repo of their origin.

| File      | From                                                         | Commit                                                       | Authors & Copyright (c)                                | License                                                     |
| --------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------ | ----------------------------------------------------------- |
| hkdf.c | [rustyrussell/ccan](https://github.com/rustyrussell/ccan/blob/master/ccan/crypto/hkdf_sha256/hkdf_sha256.c) | [d07f742](https://github.com/rustyrussell/ccan/commit/d07f742c5925b97ed558eb07aae285616f5df823) | 2016 [Rusty Russell](https://github.com/rustyrussell)<br/>2020 Wolf McNally | [MIT](https://github.com/rustyrussell/ccan/blob/master/ccan/crypto/hkdf_sha256/LICENSE)
| hkdf.h | [rustyrussell/ccan](https://github.com/rustyrussell/ccan/blob/master/ccan/crypto/hkdf_sha256/hkdf_sha256.h) | [d07f742](https://github.com/rustyrussell/ccan/commit/d07f742c5925b97ed558eb07aae285616f5df823) | 2016 [Rusty Russell](https://github.com/rustyrussell) | [MIT](https://github.com/rustyrussell/ccan/blob/master/ccan/crypto/hkdf_sha256/LICENSE)
| randombytes.c | [dsprenkels/randombytes](https://github.com/dsprenkels/randombytes/blob/master/randombytes.c) | [6db39aa](https://github.com/dsprenkels/randombytes/commit/6db39aaae6bb9ab97beca00d81bcfe935c56c88d) | 2017-2019 [Daan Sprenkels](https://github.com/dsprenkels/) | [MIT](https://github.com/dsprenkels/randombytes/commit/73ae9b4fce2e62babdd6a480b53ad449dd745ed9) |
| randombytes.h | [dsprenkels/randombytes](https://github.com/dsprenkels/randombytes/blob/master/randombytes.h) | [19fd002](https://github.com/dsprenkels/randombytes/commit/19fd002d9b7b001b333a671186a91231b60d821b) | 2017-2019 [Daan Sprenkels](https://github.com/dsprenkels/) | [MIT](https://github.com/dsprenkels/randombytes/commit/73ae9b4fce2e62babdd6a480b53ad449dd745ed9) |

### Tool Dependencies

To build `seedtool` you'll need to use the following tools:

- autotools - Gnu Build System from Free Software Foundation ([intro](https://www.gnu.org/software/automake/manual/html_node/Autotools-Introduction.html)).

## Financial Support

Seedtool is a project of [Blockchain Commons](https://www.blockchaincommons.com/). We are proudly a "not-for-profit" social benefit corporation committed to open source & open development. Our work is funded entirely by donations and collaborative partnerships with people like you. Every contribution will be spent on building open tools, technologies, and techniques that sustain and advance blockchain and internet security infrastructure and promote an open web.

To financially support further development of Seedtool and other projects, please consider becoming a Patron of Blockchain Commons through ongoing monthly patronage as a [GitHub Sponsor](https://github.com/sponsors/BlockchainCommons). You can also support Blockchain Commons with bitcoins at our [BTCPay Server](https://btcpay.blockchaincommons.com/).

## Contributing

We encourage public contributions through issues and pull-requests! Please review [CONTRIBUTING.md](./CONTRIBUTING.md) for details on our development process. All contributions to this repository require a GPG signed [Contributor License Agreement](./CLA.md).

### Discussions

The best place to talk about Blockchain Commons and its projects is in our GitHub Discussions areas.

[**Wallet Standard Discussions**](https://github.com/BlockchainCommons/AirgappedSigning/discussions). For standards and open-source developers who want to talk about wallet standards, please use the Discussions area of the [Airgapped Signing repo](https://github.com/BlockchainCommons/AirgappedSigning). This is where you can talk about projects like our [LetheKit](https://github.com/BlockchainCommons/bc-lethekit) and command line tools such as [seedtool](https://github.com/BlockchainCommons/bc-seedtool-cli), both of which are intended to testbed wallet technologies, plus the libraries that we've built to support your own deployment of wallet technology such as [bc-bip39](https://github.com/BlockchainCommons/bc-bip39), [bc-slip39](https://github.com/BlockchainCommons/bc-slip39), [bc-shamir](https://github.com/BlockchainCommons/bc-shamir), [Shamir Secret Key Recovery](https://github.com/BlockchainCommons/bc-sskr), [bc-ur](https://github.com/BlockchainCommons/bc-ur), and the [bc-crypto-base](https://github.com/BlockchainCommons/bc-crypto-base). If it's a wallet-focused technology or a more general discussion of wallet standards,discuss it here.

[**Blockchain Commons Discussions**](https://github.com/BlockchainCommons/Community/discussions). For developers, interns, and patrons of Blockchain Commons, please use the discussions area of the [Community repo](https://github.com/BlockchainCommons/Community) to talk about general Blockchain Commons issues, the intern program, or topics other than the [Gordian System](https://github.com/BlockchainCommons/Gordian/discussions) or the [wallet standards](https://github.com/BlockchainCommons/AirgappedSigning/discussions), each of which have their own discussion areas.

### Other Questions & Problems

As an open-source, open-development community, Blockchain Commons does not have the resources to provide direct support of our projects. Please consider the discussions area as a locale where you might get answers to questions. Alternatively, please use this repository's [issues](./issues) feature. Unfortunately, we can not make any promises on response time.

If your company requires support to use our projects, please feel free to contact us directly about options. We may be able to offer you a contract for support from one of our contributors, or we might be able to point you to another entity who can offer the contractual support that you need.

### Credits

The following people directly contributed to this repository. You can add your name here by getting involved — the first step is to learn how to contribute from our [CONTRIBUTING.md](./CONTRIBUTING.md) documentation.

| Name              | Role                | Github                                            | Email                                 | GPG Fingerprint                                    |
| ----------------- | ------------------- | ------------------------------------------------- | ------------------------------------- | -------------------------------------------------- |
| Christopher Allen | Principal Architect | [@ChristopherA](https://github.com/ChristopherA) | \<ChristopherA@LifeWithAlacrity.com\> | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |
| Wolf McNally      | Project Lead        | [@WolfMcNally](https://github.com/wolfmcnally)    | \<Wolf@WolfMcNally.com\>              | 9436 52EE 3844 1760 C3DC  3536 4B6C 2FCF 8947 80AE |

## Responsible Disclosure

We want to keep all our software safe for everyone. If you have discovered a security vulnerability, we appreciate your help in disclosing it to us in a responsible manner. We are unfortunately not able to offer bug bounties at this time.

We do ask that you offer us good faith and use best efforts not to leak information or harm any user, their data, or our developer community. Please give us a reasonable amount of time to fix the issue before you publish it. Do not defraud our users or us in the process of discovery. We promise not to bring legal action against researchers who point out a problem provided they do their best to follow the these guidelines.

## Reporting a Vulnerability

Please report suspected security vulnerabilities in private via email to ChristopherA@BlockchainCommons.com (do not use this email for support). Please do NOT create publicly viewable issues for suspected security vulnerabilities.

The following keys may be used to communicate sensitive information to developers:

| Name              | Fingerprint                                        |
| ----------------- | -------------------------------------------------- |
| Christopher Allen | FDFE 14A5 4ECB 30FC 5D22  74EF F8D3 6C91 3574 05ED |

You can import a key by running the following command with that individual’s fingerprint: `gpg --recv-keys "<fingerprint>"` Ensure that you put quotes around fingerprints that contain spaces.
