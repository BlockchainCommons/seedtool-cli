# 🌱 Seedtool Usage Examples

## Basic Information

### Display usage and help information

```
$ seedtool --help
```

## Seed Generation

### Generate a cryptographically-strong 16-byte seed

```
$ seedtool
8935a8068526d84da555cdb741a3b8a8
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

## BIP-39 Mnemonics

### Encode a 16-byte seed as BIP-39

```
$ seedtool --in hex --out bip39 8935a8068526d84da555cdb741a3b8a8
matrix pull accuse apart horn chat next rifle resemble artist until eye
```

### Decode BIP-39 mnemonics to hex

```
$ seedtool --in bip39 "matrix pull accuse apart horn chat next rifle resemble artist until eye"
8935a8068526d84da555cdb741a3b8a8
```

## Bytewords

### Decode Bytewords to hex

```
$ seedtool --in btw "deli need cats taxi dice door webs vows free zest legs wall half waxy trip oval memo sets rock hill"
279b18d0282aefe845fb83e956eed8a6
```

## SSKR

### Generate a 16-byte seed and encode it using SSKR as 3 shares, 2 of which are required for recovery

```
$ seedtool --out sskr --group 2-of-3
tuna acid epic gyro edge twin able acid able yoga nail wand keno paid ruin jazz keys acid time film pool skew flew luck ruby song owls soap obey
tuna acid epic gyro edge twin able acid acid yell omit mild webs warm epic flew liar view fuel deli fund glow skew dull knob claw gray surf wand
tuna acid epic gyro edge twin able acid also visa wave bulb hope drum quad duty need tied vast barn kick task gray tent crux owls easy jolt toil
```

### Recover a SSKR-encoded seed using 2 of the 3 shares

```
$ seedtool --in sskr
tuna acid epic gyro edge twin able acid able yoga nail wand keno paid ruin jazz keys acid time film pool skew flew luck ruby song owls soap obey
tuna acid epic gyro edge twin able acid also visa wave bulb hope drum quad duty need tied vast barn kick task gray tent crux owls easy jolt toil
^D
8a7e9c3c0d783371d80e1192e5f6217d
```

## UR

### Generate a seed, encode it as UR, transform it to upper case, display it on the console, and encode it to a QR Code in the file "seedqrcode.png"

```
$ seedtool --ur | tr [:lower:] [:upper:] | tee /dev/tty | qrencode -o seedqrcode.png -l L
UR:CRYPTO-SEED/OEADGDHHGTPKFXLGHHLBPYMHLFRYHLPLSSRKPKAOTPIECFFDENTNJNWNPF
```

![](../manual-images/seedqrcode.png)

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

### Recover the seed from UR using a subset of the generated parts

```
$ seedtool --in ur
ur:crypto-seed/2-4/ltaoaacsgecyuywdrnesgudkfwprylienshnjnpluypmamtkmybsjkspvseeehwnpdbb
ur:crypto-seed/4-4/ltaaaacsgecyuywdrnesguwsdpgtimmwzspfqdjkhshyaotpiecffdemaeaedmnsoxhf
ur:crypto-seed/6-4/ltamaacsgecyuywdrnesguoeadhdfznteelblrcygldwvarflojtcywyjytptkwfjech
ur:crypto-seed/8-4/ltayaacsgecyuywdrnesguhnwdwsmocwrhbkambezstspdytdtjthfjshlhnbdpygmao
^D
9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e
```

