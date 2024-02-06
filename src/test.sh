#!/bin/bash

# Echo any provided arguments.
# [ $# -gt 0 ] && echo "#:$# 1:$1 2:$2 3:$3"

SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"
SEEDTOOL="${1:-${SCRIPT_DIR}/seedtool} --deterministic TEST"
SHUNIT2="${SCRIPT_DIR}/../deps/shunit2/shunit2"

testDefault()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d824' \
    "$(${SEEDTOOL})"
}

testRedundantDefault()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d824' \
    "$(${SEEDTOOL} --in random --out hex)"
}

testOutBTW()
{
  assertEquals $'next edge lamb liar city girl draw visa roof logo jolt city waxy jury trip dark loud duty obey monk' \
    "$(${SEEDTOOL} --out btw)"
}

testInBTW()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d824' \
    "$(${SEEDTOOL} --in btw 'next edge lamb liar city girl draw visa roof logo jolt city waxy jury trip dark loud duty obey monk')"
}

testOutBTWU()
{
  assertEquals $'deli-need-cats-taxi-dice-door-webs-vows-free-zero-legs-wall-half-waxy-trip-oval-memo-sets-rock-hill' \
    "$(${SEEDTOOL} --in hex --out btwu 279b18d0282aefe845fb83e956eed8a6)"
}

testOutBTWM()
{
  assertEquals $'dindcstidedrwsvsfezolswlhfwytpolmossrkhl' \
    "$(${SEEDTOOL} --in hex --out btwm 279b18d0282aefe845fb83e956eed8a6)"
}

testOutBits()
{
  assertEquals $'1001000111001010' \
    "$(${SEEDTOOL} --out bits)"
}

testOutCards()
{
  assertEquals $'6hjckdah6c4dtc8skh2htd6ctsjd5s8c' \
    "$(${SEEDTOOL} --out cards)"
}

testOutDice()
{
  assertEquals $'4234232654326352' \
    "$(${SEEDTOOL} --out dice)"
}

testOutBase6()
{
  assertEquals $'3123121543215241' \
    "$(${SEEDTOOL} --out base6)"
}

testOutBase10()
{
  assertEquals $'6245132875418481' \
    "$(${SEEDTOOL} --out base10)"
}

testOut5Cards()
{
  assertEquals $'6hjckdah6c' \
    "$(${SEEDTOOL} --out cards --count 5)"
}

testOutInts()
{
  assertEquals $'6 2 4 5 1 3 2 8 7 5 4 1 8 4 8 1' \
    "$(${SEEDTOOL} --out ints)"
}

testOutIntsRange()
{
  assertEquals $'62 21 50 52 11 31 18 90' \
    "$(${SEEDTOOL} --out ints --count 8 --low 1 --high 100)"
}

testOutBIP39()
{
  assertEquals $'outdoor physical three cruel tissue infant vendor assist brain inhale current emotion' \
    "$(${SEEDTOOL} --out bip39)"
}

testOutBIP39Count32()
{
  assertEquals $'outdoor physical three cruel tissue infant vendor assist brain inhale current embrace clinic knife mystery metal horror item twice assault wedding marble song buyer' \
    "$(${SEEDTOOL} --out bip39 --count 32)"
}

testOutSSKR()
{
  assertEquals $'tuna acid epic gyro many meow able able able next edge lamb liar city girl draw visa roof logo jolt city waxy jury trip dark safe arch flew what' \
    "$(${SEEDTOOL} --out sskr)"
}

testOutSSKRCount32()
{
  assertEquals $'tuna acid epic hard data many meow able able able next edge lamb liar city girl draw visa roof logo jolt city waxy jury trip dark flew purr yell idle news horn join pool ugly poem atom task many bias junk soap yoga idea yell work' \
    "$(${SEEDTOOL} --out sskr --count 32)"
}

testOutSSKRGroup2Of3()
{
  assertEquals $'tuna acid epic gyro many meow able acid able mild fern pool door purr calm trip cyan flew zest cats tuna omit figs bias acid aunt keys play frog
tuna acid epic gyro many meow able acid acid keep undo peck poem kiwi jazz cola luck hope rock into film jolt lava flux rust gala sets ruin toil
tuna acid epic gyro many meow able acid also girl void oval fish exam veto gala inky keys jump visa barn cusp high miss monk jazz numb dice foxy' \
    "$(${SEEDTOOL} --out sskr --group 2-of-3)"
}

testOutSSKR3Groups()
{
  assertEquals $'tuna acid epic gyro many meow brag acid able noon diet race hope glow junk kept oval jump hope heat keys high barn hang into next miss logo oval
tuna acid epic gyro many meow brag acid acid heat noon peck aqua keno wolf wasp fund aqua gush cyan gear hill inky drop scar buzz city toys mild
tuna acid epic gyro many meow brag acid also belt high luau wall figs iris heat huts noon gala paid cook holy trip omit edge miss cats keep runs
tuna acid epic gyro many meow brag brag able kiwi pool yell user math girl figs gift free part when liar kiln view iced buzz keep eyes taxi wolf
tuna acid epic gyro many meow brag brag acid surf miss barn kick surf miss lamb calm miss dark song kite fuel real hill mint apex dull runs unit
tuna acid epic gyro many meow brag brag also skew love quad dull iced join each curl numb data aqua wand vows owls idea cash keep echo keys math
tuna acid epic gyro many meow brag brag apex kept quad fund loud film quiz junk kiln jump paid fish brag toil yank high meow apex data cash ruin
tuna acid epic gyro many meow brag brag aqua epic open wave main high calm echo taxi void paid fact kiln soap tiny list join kiln rust blue flap
tuna acid epic gyro many meow brag cusp able legs acid keep menu film yell very lava undo flew curl wolf soap chef idle fuel king runs trip urge
tuna acid epic gyro many meow brag cusp acid puff kiwi crux soap rock toys bulb hill jade beta jade zinc loud void junk kiln soap draw edge pose
tuna acid epic gyro many meow brag cusp also heat apex plus mild glow saga lava yoga user game diet mild gyro limp puma down echo quiz user oboe
tuna acid epic gyro many meow brag cusp apex iron lamb yurt taco song veto keep data iris atom idea need bulb king oval calm lava deli echo time
tuna acid epic gyro many meow brag cusp aqua vast rust bald iron fuel grim gyro eyes chef dark kept surf mild swan bulb slot draw kick zero onyx' \
    "$(${SEEDTOOL} --out sskr --group-threshold 2 --group 2-of-3 --group 3-of-5 --group 3-of-5)"
}

testInBits()
{
  assertEquals $'8d933e43b1bc8f2e3fc27adc98ad4534' \
    "$(${SEEDTOOL} --in bits 1110110001110111)"
}

testInCards()
{
  assertEquals $'7df301924511326d7350be14c9e7176d' \
    "$(${SEEDTOOL} --in cards 6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs)"
}

testInCardsCount32()
{
  assertEquals $'7df301924511326d7350be14c9e7176d98e945f9ad0ed034726ad4ee0de59c25' \
    "$(${SEEDTOOL} --count 32 --in cards 6c9s8c7c9c4cah6c2sjs7d5c2s4c4dqs)"
}

testInDice()
{
  assertEquals $'77ae0de807d60367311eb040c70690d2' \
    "$(${SEEDTOOL} --in dice 3343462611234633)"
}

testInBase6()
{
  assertEquals $'51621269b3a91fe6482ceb7779f0d1d1' \
    "$(${SEEDTOOL} --in base6 3242235101442242)"
}

testInBase10()
{
  assertEquals $'a0ca21e20db54b4df7479737c145f6db' \
    "$(${SEEDTOOL} --in base10 3190125)"
}

testInInts()
{
  assertEquals $'a38385ba7a67b7f5882b37f75b43c2df' \
    "$(${SEEDTOOL} --in ints 71 22 95 6 23 65 27 10 67 16)"
}

testDicePipe()
{
  assertEquals $'eefa19b88c5846e71fcb52d007066ae4a2bd0be166e12c8de95c8e71a712c581' \
    "$(${SEEDTOOL} --out dice | ${SEEDTOOL} --in dice --count 32)"
}

testInBIP39OutBTW()
{
  assertEquals $'love exam mint dark bias item mint atom open kept soap list whiz view soap fact jugs obey idle jugs' \
    "$(${SEEDTOOL} --in bip39 --out btw mechanic royal math burst practice addict noise weekend margin now improve invest)"
}

testInSSKR()
{
  assertEquals $'82f188b091c38fced97bb45732bddf7e' \
    "$(${SEEDTOOL} --in sskr 'tuna acid epic gyro edge next able able able leaf when logo puff maze scar many taco tuna king quiz hang easy ruby user knob omit time chef slot')"
}

testInSSKR2Of3()
{
  assertEquals $'2c4b163bfe9b791dbc0e755b911d01f6' \
    "$(${SEEDTOOL} --in sskr \
        'tuna acid epic gyro edge next able acid able flux down eyes pool task cyan gyro kept exit paid wave high news bias zone zaps calm note poem aqua' \
        'tuna acid epic gyro edge next able acid also wolf vows bald sets claw bald dull cost slot inky liar work note tied zest safe crux kiln work exit')"
}

testOutUR()
{
  # seedtool --deterministic TEST --ur
  assertEquals $'ur:seed/oyadgdnteelblrcygldwvarflojtcywyjytpdkjspafltb' \
    "$(${SEEDTOOL} --ur)"
}

testInUR()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d824' \
    "$(${SEEDTOOL} --in ur ur:seed/oyadgdnteelblrcygldwvarflojtcywyjytpdkjspafltb)"
}

testOutMultipartUR()
{
  assertEquals $'ur:seed/1-5/lpadahcfadmdcyknoxqdgohdgyoyadhkadmhnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlnlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszoyknbvavs
ur:seed/2-5/lpaoahcfadmdcyknoxqdgohdgygmdstpoennbtflwykbtpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlbmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnsdeesecrd
ur:seed/3-5/lpaxahcfadmdcyknoxqdgohdgypscsutkkiyfzeyvtyajlvlsewnhkjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspriymotofgcemhguskgeflfyfspliorntefgdkykpswzwyrydwlamdft
ur:seed/4-5/lpaaahcfadmdcyknoxqdgohdgywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpefppfemlkntknghveglrypslaahtblyvdspolptcpaxrpgs
ur:seed/5-5/lpahahcfadmdcyknoxqdgohdgypkflsfmdfxjydpioihltbnwppfhtielguthtwdpdihaykgoyclentldykkmnvedsbwuyeojejennbkrtkevotacppyjsvdgohtvewenyneylfekihgcplpgljoisgwverkmokgldyavseojehncfvakgehcmrojlwezmdnkkwe' \
    "$(${SEEDTOOL} --count 400 --ur=100)"
}

testInMultipartUR()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e3bf736a89e9ceb105f2109fb5226d8a29e0d47ee7ed8774f20245ac5f47b95958b1483daa4aaabc9dad616a30bf338b4ef1e971d2cc449bdcc339258f93f7f91a3d067522b085ca6b2f7c72732ce5aed7ae0ef273f13c8d92ffa89b69cac18dd79664032e0f86fe3c1f1596fd2dc582c690f17407d0852932d23798056a424cacae3bfd4e30fe8030f943645fcdf4d86de1b45570778b26692ce461c9053c54a47443dae67bed34624f5acf2eebdf0b0e5283505d25ce6849aa0d0f10b1fdec20d8e35e6b2072c7fe3d84c4e950c989f0a781a92417732e2076fb302e4cc3ff7506a061a011f4cd4952ff6af41b0378c9d7a54e44ebdac8005d681e7c8a6a9aa47cc9543742d6765870cecb05a648ddd5aeaa865087ba12136d530798ee42613db336b6b9e0ac07ce2d922ab71e7555ae4ed9a9ff7457d5722854e70684fe4bb927b89f8e8336b6019e67b3116b86fed' \
    "$(${SEEDTOOL} --in ur 'ur:seed/1-5/lpadahcfadmdcyknoxqdgohdgyoyadhkadmhnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlnlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszoyknbvavs' \
'ur:seed/2-5/lpaoahcfadmdcyknoxqdgohdgygmdstpoennbtflwykbtpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlbmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnsdeesecrd' \
'ur:seed/3-5/lpaxahcfadmdcyknoxqdgohdgypscsutkkiyfzeyvtyajlvlsewnhkjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspriymotofgcemhguskgeflfyfspliorntefgdkykpswzwyrydwlamdft' \
'ur:seed/4-5/lpaaahcfadmdcyknoxqdgohdgywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmnecvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpefppfemlkntknghveglrypslaahtblyvdspolptcpaxrpgs' \
'ur:seed/5-5/lpahahcfadmdcyknoxqdgohdgypkflsfmdfxjydpioihltbnwppfhtielguthtwdpdihaykgoyclentldykkmnvedsbwuyeojejennbkrtkevotacppyjsvdgohtvewenyneylfekihgcplpgljoisgwverkmokgldyavseojehncfvakgehcmrojlwezmdnkkwe')"
}

testOutFountainUR()
{
  assertEquals $'ur:seed/1-7/lpadatcfadehcyetoyioluhddwoyadhkaddwnteelblrcygldwvarflojtcywyjytpdkfwprylienshnjnpluypmamtkmybsjkspvseesawmrltdlnbkaatllr
ur:seed/2-7/lpaoatcfadehcyetoyioluhddwlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszogmdstpoennbtfljpyaihsn
ur:seed/3-7/lpaxatcfadehcyetoyioluhddwwykbtpktgwcxdkhtskwkkgmdmdlubblstnoxpkpysotntbcmotbdwfetqzwsckmscadwssgarysfeomohdytfhlblerhdest
ur:seed/4-7/lpaaatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhksscaimzt
ur:seed/5-7/lpahatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprpytiinki
ur:seed/6-7/lpamatcfadehcyetoyioluhddwiymotofgcemhguskgeflfyfspliorntefgdkykpswzwyrywtpfvwdeecahtdhhvalrnynbtiwnbdctuesabtmneceegwrkkn
ur:seed/7-7/lpatatcfadehcyetoyioluhddwvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaeaeaerosssfjl
ur:seed/8-7/lpayatcfadehcyetoyioluhddwtotelphkaewkfrisssiofgkbkpmepychnyrotiztwylogyfdpflbjllppmtyesdylejktifmglendlltrfpfpkeeglchdakp
ur:seed/9-7/lpasatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkfxjypdtk
ur:seed/10-7/lpbkatcfadehcyetoyioluhddwdwjyuybdmkiyknftdacyaoqdtkaaiofxhglrvtcpmwwnsemtftosmshpamfeehweurtidmkneordbgcygadturseztzekpgl
ur:seed/11-7/lpbdatcfadehcyetoyioluhddwehdmfycnkblknsvdotwflfldynferlatkkaxcsdsbelrfgtlkshtlffmhldaryhetyrysnpyvelrtygmimlpwddivdytgsti
ur:seed/12-7/lpbnatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprhtwevebg
ur:seed/13-7/lpbtatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprflksylsk
ur:seed/14-7/lpbaatcfadehcyetoyioluhddwylehckclglrkhpnlwpykqdzsldgojoldpyhybzfxtlttpletinsgtdrfqdglwtzehkvlswmhsrwmvdpaclsflbjzctmefdbk
ur:seed/15-7/lpbsatcfadehcyetoyioluhddwknfynsdnzsfzbzuogtykzmihnbweneoxvaeelyrhihiduthkemwndalecwtijzbzgarfvdnletrhseinlsgmjpdnfmwlgytb
ur:seed/16-7/lpbeatcfadehcyetoyioluhddwntknhffwlpnljnghlffrspneaydwcmlgzsvoknnymwnsgoaewtztwnjlptvordgmmdynhhgtwznbynhngloyrktevtmufstd
ur:seed/17-7/lpbyatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspruroxcabe' \
    "$(${SEEDTOOL} --count 300 --ur=50 --parts 10)"
}

testInFountainUR()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d82442b2f7649c606daedbad06cf8f0f73c8e834c2ebb7d2868d75820ab4fb4e45a1004c9f29b8ef2d4d6a94fab0b373615e3bf736a89e9ceb105f2109fb5226d8a29e0d47ee7ed8774f20245ac5f47b95958b1483daa4aaabc9dad616a30bf338b4ef1e971d2cc449bdcc339258f93f7f91a3d067522b085ca6b2f7c72732ce5aed7ae0ef273f13c8d92ffa89b69cac18dd79664032e0f86fe3c1f1596fd2dc582c690f17407d0852932d23798056a424cacae3bfd4e30fe8030f943645fcdf4d86de1b45570778b26692ce461c9053c54a47443dae67bed34624f5acf2eebdf0b0e5283505d25ce6849aa0d0f10b1fdec20d8e35e6b2072c7fe3d84c4e950c989f0a781a92417732e2076fb302e4cc3ff7506a061a011f4cd4952ff6af' \
    "$(${SEEDTOOL} --in ur \
    'ur:seed/2-7/lpaoatcfadehcyetoyioluhddwlgkplfbkqzzoglfeoyaegsnedtrowsdpgtimmwzspfqdjkhshyfrylenpdnnnswmbeheclaszogmdstpoennbtfljpyaihsn' \
    'ur:seed/4-7/lpaaatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhksscaimzt' \
    'ur:seed/5-7/lpahatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatksprpytiinki' \
    'ur:seed/7-7/lpatatcfadehcyetoyioluhddwvapratdwlbvltpgsglmdbnmknebkkscymofpkteyvoatjlqdaovesffhylgdimamcyadctgstymddlynpeaeaeaerosssfjl' \
    'ur:seed/8-7/lpayatcfadehcyetoyioluhddwtotelphkaewkfrisssiofgkbkpmepychnyrotiztwylogyfdpflbjllppmtyesdylejktifmglendlltrfpfpkeeglchdakp' \
    'ur:seed/9-7/lpasatcfadehcyetoyioluhddwmeottiiogmdnayhholprylstdieytohtweknvtwsdifhbwsptadlzsldrpnspscsutkkiyfzeyvtyajlvlsewnhkfxjypdtk' \
    'ur:seed/10-7/lpbkatcfadehcyetoyioluhddwdwjyuybdmkiyknftdacyaoqdtkaaiofxhglrvtcpmwwnsemtftosmshpamfeehweurtidmkneordbgcygadturseztzekpgl' \
    'ur:seed/14-7/lpbaatcfadehcyetoyioluhddwylehckclglrkhpnlwpykqdzsldgojoldpyhybzfxtlttpletinsgtdrfqdglwtzehkvlswmhsrwmvdpaclsflbjzctmefdbk' \
    'ur:seed/16-7/lpbeatcfadehcyetoyioluhddwntknhffwlpnljnghlffrspneaydwcmlgzsvoknnymwnsgoaewtztwnjlptvordgmmdynhhgtwznbynhngloyrktevtmufstd' \
    'ur:seed/17-7/lpbyatcfadehcyetoyioluhddwjltduohddwinbschfzkiaygmmudpcnkklahfoxdksgsgvlrstyvlbsvsaxbsmwenfezturgtlnuecwfehgatkspruroxcabe')"
}

testOutSSKRUR()
{
    assertEquals $'ur:crypto-sskr/taadecgomymwaeadaemdfnpldrprcmtpcnfwztcstaotfsbsadatkspyfg
ur:crypto-sskr/taadecgomymwaeadadkpuopkpmkijzcalkherkiofmjtlafxrtgassrntl
ur:crypto-sskr/taadecgomymwaeadaoglvdolfhemvogaiyksjpvabncphhmsmkjznbdefy' \
    "$(${SEEDTOOL} --out sskr --group 2-of-3 --ur)"
}

testInSSKRUR()
{
  assertEquals $'9d347f841a4e2ce6bc886e1aee74d824' \
    "$(${SEEDTOOL} --in ur ur:crypto-sskr/taadecgomymwaeadaemdfnpldrprcmtpcnfwztcstaotfsbsadatkspyfg ur:crypto-sskr/taadecgomymwaeadaoglvdolfhemvogaiyksjpvabncphhmsmkjznbdefy)"
}

testBip39ToSskr()
{
  assertEquals $'tuna acid epic hard data edge next acid acid able rust claw zaps curl zero exit wolf solo beta whiz jowl keys beta help nail flap data peck beta task flap blue grim holy warm curl ruin main cook twin exit dark when brag zero blue
tuna acid epic hard data edge next acid acid acid aqua solo legs away back poem inky cash view ruin nail stub puma duty hang inch edge rock lion taco iron puma legs knob many keno note work huts next meow kept pose view buzz item
tuna acid epic hard data edge next acid acid also guru onyx away junk also axis saga jolt scar item monk arch jade lung cook axis aunt logo arch swan body gala warm cook cyan many zest kiln vast gush king leaf horn bulb vibe meow
tuna acid epic hard data edge next acid brag able toil edge wall gala obey task body zinc cyan solo meow view roof free wall what tomb paid brew note judo owls chef kept stub deli down wall owls wave fair kept kept duty vows city
tuna acid epic hard data edge next acid brag acid runs lion note blue glow flux legs slot purr claw task quad roof list grim cash ruin what iris fizz cash mild slot cost bias wolf runs quiz ruin wave iced navy gear monk huts blue
tuna acid epic hard data edge next acid brag also bald down vibe foxy iced part wasp heat kiwi mint cyan kite kick redo wall fair judo zest chef edge fuel game legs poem apex keys lava oboe cats hard time ramp gift drum zoom hang
tuna acid epic hard data edge next acid brag apex huts nail memo cola luck data knob horn wasp foxy keys door kick kiln grim undo cats onyx iced warm crux keys hill silk slot plus bulb zoom acid hard love help jugs lion keno hill
tuna acid epic hard data edge next acid brag aqua heat maze heat nail draw luck toil solo cats twin view cats bulb exam frog arch flew ruby twin wand toys jury saga what exam aqua duty mint rust knob legs waxy beta quiz knob taco' \
    "$(${SEEDTOOL} --in bip39 --out sskr --group-threshold 1 --group 2-of-3 --group 3-of-5 'slender horse coil sketch public wink nest point royal believe inform lyrics critic harbor scheme enrich burden glance song chicken grief first panic actor')"
}

testSskrToBip39()
{
  assertEquals $'clump flavor moment wood resource mom usual solid fortune material document under' \
    "$(${SEEDTOOL} --in sskr --out bip39 \
        'tuna acid epic gyro edge next able acid able flux down eyes pool task cyan gyro kept exit paid wave high news bias zone zaps calm note poem aqua' \
        'tuna acid epic gyro edge next able acid also wolf vows bald sets claw bald dull cost slot inky liar work note tied zest safe crux kiln work exit')"
}

testAllInputOutputCombinations()
{
  # Tests all combinations of input and output formats. We don't want to
  # hard-code these all, so we just test whether `hex -> A` is the same as `hex
  # -> A -> B -> A`.
  #
  # The formats below are all the formats that can represent a full seed. There
  # are others like "ints", but these don't represent a full seed with the
  # default --count setting.
  formats="hex bip39 sskr btw btwu btwm"
  for dst_format in $formats; do
    for src_format in $formats; do
      output_orig="$(${SEEDTOOL} | ${SEEDTOOL} --in hex --out $dst_format)"
      output_roundtrip="$(${SEEDTOOL} | ${SEEDTOOL} --in hex --out $dst_format | ${SEEDTOOL} --in $dst_format --out $src_format | ${SEEDTOOL} --in $src_format --out $dst_format)"
      assertEquals "dst=$dst_format and src=$src_format must be compatible." "$output_orig" "$output_roundtrip"
    done
  done
}

# Eat all command-line arguments before calling shunit2.
shift $#
# TODO no shunit2 on windows/msys2/mingw64 platform
if [ "$(uname)" == "Darwin" ] || [ "$(uname)" == "Linux" ]; then
  . "${SHUNIT2}"
fi
