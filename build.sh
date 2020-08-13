#!/bin/bash

git submodule init
git submodule update

source set_build_paths.sh

# Terminal colors
RED=`tput setaf 1`
GREEN=`tput setaf 2`
RESET=`tput sgr0`

pushd deps/bc-crypto-base
./configure --prefix ${SYSROOT}
make check
make install
popd

pushd deps/bc-shamir
./configure --prefix ${SYSROOT}
make check
make install
popd

pushd deps/bc-slip39
./configure --prefix ${SYSROOT}
make check
make install
popd

pushd deps/bc-bip39
./configure --prefix ${SYSROOT}
make check
make install
popd

pushd deps/bc-ur
./configure --prefix ${SYSROOT}
make check
make install
popd

pushd deps/argp-standalone/argp-standalone
patch -N <../patch-argp-fmtstream.h
./configure --prefix ${SYSROOT}
make install
cp libargp.a ${SYSROOT}/lib/
cp argp.h ${SYSROOT}/include/
popd

./configure
make check
echo "${GREEN}*** Seedtool built.${RESET}"
echo "${GREEN}Next step: sudo make install${RESET}"
