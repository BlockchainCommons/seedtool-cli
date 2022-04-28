#!/bin/bash

#
# This is an example script that converts a set of SSKR shares to BIP-39 seed in one step. It takes the shares from STDIN and
# write the BIP-39 seed to STDOUT.
#
# Example invocation:
#
# ./sskr-to-bip39.sh
# tuna acid epic hard data tied visa acid acid able hope glow iced meow flew ruby omit fizz need task gear claw door hope meow waxy dark keep barn math beta tent half wasp idea fern peck void limp peck fact half cook game down fish
# tuna acid epic hard data tied visa acid acid acid keno kept oboe data rock skew plus vast free kick horn horn fund foxy tomb gray diet soap roof runs time pose yell able inch fern crux lung into twin pool back user purr fish guru
# ^D
#
# slender horse coil sketch public wink nest point royal believe inform lyrics critic harbor scheme enrich burden glance song chicken grief first panic actor

seedtool --in sskr | seedtool --in hex --out bip39
