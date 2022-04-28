#!/bin/bash

#
# This is an example script that uses Seedtool that converts a BIP39 seed into a set of SSKR shares in one step.
#
#
# Example invocation on single line:
#
# ./bip39-to-sskr.sh "slender horse coil sketch public wink nest point royal believe inform lyrics critic harbor scheme enrich burden glance song chicken grief first panic actor" --group-threshold 1 --group 2-of-3 --group 3-of-5
#
# tuna acid epic hard data tied visa acid acid able hope glow iced meow flew ruby omit fizz need task gear claw door hope meow waxy dark keep barn math beta tent half wasp idea fern peck void limp peck fact half cook game down fish
# tuna acid epic hard data tied visa acid acid acid keno kept oboe data rock skew plus vast free kick horn horn fund foxy tomb gray diet soap roof runs time pose yell able inch fern crux lung into twin pool back user purr fish guru
# tuna acid epic hard data tied visa acid acid also belt fish yurt wave play gear ruby claw fern redo cola wave waxy iron blue luau film bulb kept taco pose drop bias dull jowl fern open echo heat game axis waxy jury list king judo
# tuna acid epic hard data tied visa acid brag able drop mild paid what rich fair jolt urge user exit race saga lion user play cyan crux heat fizz wolf figs chef oval void half numb king gems puma exam away oval scar play vibe cook
# tuna acid epic hard data tied visa acid brag acid cyan axis mild trip fizz unit dice calm memo ramp flew sets toil trip view waxy mild days lung news gush diet yank yank back pool aunt scar roof draw even zest many rust axis memo
# tuna acid epic hard data tied visa acid brag also menu noon flux lamb quiz gift lazy slot twin very axis high hope ruby aqua surf acid very rich each knob fund purr guru oboe mint flux pool fizz gear sets apex deli kiwi tent bald
# tuna acid epic hard data tied visa acid brag apex next also knob hang gift play slot bias miss jowl zone heat barn road game acid quiz next jury holy cash keys very flap zone monk fish curl gift good zaps hawk jade calm fern list
# tuna acid epic hard data tied visa acid brag aqua work cyan cash onyx cook inch fern memo note sets wall need soap puff cusp free eyes crux grim code solo lamb judo veto main idle days cusp solo zest kiwi quad foxy claw play unit
#
#
# Example invocation using STDIN to supply the BIP39 words
#
# ./bip39-to-sskr.sh "" --group-threshold 1 --group 2-of-3 --group 3-of-5
# slender horse coil sketch public wink nest point royal believe inform lyrics critic harbor scheme enrich burden glance song chicken grief first panic actor
# ^D
#
# tuna acid epic hard data skew wand acid acid able jugs noon kiln aunt roof leaf race figs horn exit knob wave next fuel pose taco miss work ruby webs huts film mild wolf zoom free puff wave whiz sets beta wave tent time kiwi vibe
# tuna acid epic hard data skew wand acid acid acid hill oboe lazy stub hard fair diet days task jowl glow jade hope into hope undo game toys view flux vibe navy time judo duty main eyes hope junk what news lazy redo half yank gyro
# tuna acid epic hard data skew wand acid acid also diet visa miss liar jowl warm mint bald data mint code zaps also aunt gush wand even purr belt plus judo join chef waxy kiln soap rich memo warm plus each epic task numb unit kept
# tuna acid epic hard data skew wand acid brag able leaf wall toys trip undo exit ruby navy girl work legs bulb purr luau girl wasp poem quiz able void skew puff yawn gyro kite rich cook each peck belt lazy time void luck zaps maze
# tuna acid epic hard data skew wand acid brag acid yoga jolt gems song mild hard inky when zest keep surf work hard news calm gear poem plus zone cook numb lava leaf work judo ruin zinc oval task next acid gyro oboe road lion runs
# tuna acid epic hard data skew wand acid brag also high runs memo onyx note skew good jury oval peck pose menu toil lung news note iris hard slot kite arch navy heat note tent high item very barn limp zone quad taxi judo fuel eyes
# tuna acid epic hard data skew wand acid brag apex days exit axis ramp tomb oval luau cost bulb down vast junk fish navy sets exit iris fizz eyes limp idea peck drum film unit help loud keno iron buzz knob epic mild frog fair cash
# tuna acid epic hard data skew wand acid brag aqua lung grim visa help echo deli into ruby webs glow data item wand deli iced memo arch door buzz chef into toys ruin numb user wall idle echo half zest work tiny tied pose apex vibe
#

BIP39=$1
SSKR_OPTIONS=${@:2}
seedtool --in bip39 ${BIP39} | seedtool --in hex --out sskr ${SSKR_OPTIONS}
