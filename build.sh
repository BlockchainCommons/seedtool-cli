#!/bin/bash

git submodule init
git submodule update

source set_build_paths.sh

# Terminal colors
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BLUE=`tput setaf 4`
RESET=`tput sgr0`

echo "${BLUE}== bc-crypto-base ==${RESET}"

pushd deps/bc-crypto-base
./configure --prefix ${SYSROOT}
make check
make install
popd

echo "${BLUE}== bc-shamir ==${RESET}"

pushd deps/bc-shamir
./configure --prefix ${SYSROOT}
make check
make install
popd

echo "${BLUE}== bc-sskr ==${RESET}"

pushd deps/bc-sskr
./configure --prefix ${SYSROOT}
make check
make install
popd

echo "${BLUE}== bc-bip39 ==${RESET}"

pushd deps/bc-bip39
./configure --prefix ${SYSROOT}
make check
make install
popd

echo "${BLUE}== bc-ur ==${RESET}"

pushd deps/bc-ur
./configure --prefix ${SYSROOT}
make check
make install
popd

echo "${BLUE}== argp-standalone ==${RESET}"

pushd deps/argp-standalone/argp-standalone
set +e
patch -N <../patch-argp-fmtstream.h
set -e
./configure --prefix ${SYSROOT}
make install
cp libargp.a ${SYSROOT}/lib/
cp argp.h ${SYSROOT}/include/
popd

echo "${BLUE}== seedtool ==${RESET}"

./configure
make clean
make check
echo "${GREEN}*** Seedtool built.${RESET}"
echo "${GREEN}Next step: sudo make install${RESET}"
