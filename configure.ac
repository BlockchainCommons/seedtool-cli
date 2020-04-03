#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([bc-seedtool-cli], [1.0])
AC_CONFIG_SRCDIR([src/seedtool.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB(bc-crypto-base, sha256_Raw, [], [
  echo "### Error! libbc-crypto-base must be installed first."
  exit -1
  ])
AC_CHECK_LIB(bc-shamir, split_secret, [], [
  echo "### Error! libbc-shamir must be installed first."
  exit -1
  ])
AC_CHECK_LIB(bc-slip39, slip39_word_for_string, [], [
  echo "### Error! libbc-slip39 must be installed first."
  exit -1
  ])
AC_CHECK_LIB(bc-bip39, bip39_mnemonic_from_word, [], [
  echo "### Error! libbc-bip39 must be installed first."
  exit -1
  ])
AC_CHECK_LIB(argp, argp_parse, [], [
  echo "### Error! argp must be installed first. Try 'brew install argp-standalone'."
  exit -1
  ])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
