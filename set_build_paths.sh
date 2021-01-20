#!/bin/bash

SYSROOT=${PWD}/sysroot
LIB=${SYSROOT}/lib
INCLUDE=${SYSROOT}/include

export CPPFLAGS="${CPPFLAGS} -I${INCLUDE}"
export LDFLAGS="${LDFLAGS} -L${LIB}"
