#!/bin/sh
#                     The ONNC Project
#

call()
{
  local PROGRAM=`which $1`
  if [ ! -z "${PROGRAM}" ]; then
    $*
  else
    echo "error: $1: command not found"
    exit 1
  fi
}

LIBTOOLIZE=libtoolize
my_uname=`uname`

if [ ${my_uname} = "Darwin" ]; then
  LIBTOOLIZE=glibtoolize
fi
root=`dirname $0`
cd $root

call aclocal -I ./m4
call autoheader
call ${LIBTOOLIZE} --force
call automake --add-missing
call autoconf
