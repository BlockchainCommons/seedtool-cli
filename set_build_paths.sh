#!/bin/bash

SYSROOT=${PWD}/sysroot
LIB=${SYSROOT}/lib
INCLUDE=${SYSROOT}/include

export CFLAGS="${CFLAGS} -I${INCLUDE}"
export CXXFLAGS="${CXXFLAGS} -I${INCLUDE}"
export LDFLAGS="${LDFLAGS} -L${LIB}"
