#!/bin/bash

set -e

ONNC_DIR="$( realpath $( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )/.. )"
CLANG_FORMAT=clang-format-7

DIRS=(
  lib/Target/NvDla
)

EXCLUDE_DIRS=(
  lib/Target/NvDla/include
  lib/Target/Sophon
)


for dir in $DIRS
do
  cd $ONNC_DIR/$dir

  FILES=$(find . -iname "*.h" -o -iname "*.hpp" -o -iname "*.c" -o -iname "*.cpp" -o -iname "*.cxx" -type f | xargs realpath)

  for exclude_dir in "${EXCLUDE_DIRS[@]}"
  do
    FILES=$(printf '%s\n' "${FILES[@]}" | grep -v "^$ONNC_DIR/$exclude_dir")
  done

  printf '%s\n' "${FILES[@]}" | xargs $CLANG_FORMAT -style=file -i
done
