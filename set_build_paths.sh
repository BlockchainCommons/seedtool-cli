#!/bin/bash

LIBS=${PWD}/libs
LIB=${LIBS}/lib
INCLUDE=${LIBS}/include

export CFLAGS="${CFLAGS} -I${INCLUDE}"
export CXXFLAGS="${CXXFLAGS} -I${INCLUDE}"
export LDFLAGS="${LDFLAGS} -L${LIB}"
