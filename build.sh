#!/bin/bash

git submodule init
git submodule update

LIBS=${PWD}/libs
LIB=${LIBS}/lib
INCLUDE=${LIBS}/include

export CFLAGS="${CFLAGS} -I${INCLUDE}"
export CXXFLAGS="${CXXFLAGS} -I${INCLUDE}"
export LDFLAGS="${LDFLAGS} -L${LIB}"

# Terminal colors
RED=`tput setaf 1`
GREEN=`tput setaf 2`
RESET=`tput sgr0`

pushd deps/bc-crypto-base
./configure --prefix ${LIBS}
make check
make install
popd

pushd deps/bc-shamir
./configure --prefix ${LIBS}
make check
make install
popd

pushd deps/bc-slip39
./configure --prefix ${LIBS}
make check
make install
popd

pushd deps/bc-bip39
./configure --prefix ${LIBS}
make check
make install
popd

pushd deps/bc-bech32
./configure --prefix ${LIBS}
make check
make install
popd

pushd deps/argp-standalone/argp-standalone
patch -N <../patch-argp-fmtstream.h
./configure --prefix ${LIBS}
make install
cp libargp.a ${LIBS}/lib/
cp argp.h ${LIBS}/include/
popd

./configure
make check
echo "${GREEN}*** Seedtool built.${RESET}"
echo "${GREEN}Next step: sudo make install${RESET}"
