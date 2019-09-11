#!/usr/bin/env bash
#                       The ONNC Project
set -e

TEMPLATE_BACKEND="Vanilla"
BACKEND_FOLDER_PATH="lib/Target"

# Set the user to the owner of the project
OWNER_UID=$(stat -c '%u' ./)
OWNER_GID=$(stat -c '%g' ./)
SUDO="sudo -u #${OWNER_UID} -g #${OWNER_GID}"

function help_info() {
  echo "Usage: create-new-backend.sh BACKEND"
  echo "    BACKEND		Name of new backend"
}

function replace_backend_name() {
  FILE_NAME=$1
  # Origin backend name
  $SUDO sed -i "s/${TEMPLATE_BACKEND}/${NEW_BACKEND}/g" ${FILE_NAME}
  # All upper-case
  $SUDO sed -i "s/${TEMPLATE_BACKEND^^}/${NEW_BACKEND^^}/g" ${FILE_NAME}
  # All lower-case
  $SUDO sed -i "s/${TEMPLATE_BACKEND,,}/${NEW_BACKEND,,}/g" ${FILE_NAME}
}


#----------------------------------------------------------
#   Arguments parsing
#----------------------------------------------------------
if [[ $# -ne 1 ]]; then
  help_info
  exit 1
fi
NEW_BACKEND=$1

#----------------------------------------------------------
#   Create new backend releated folder & files
#----------------------------------------------------------
if [ ! -d "${BACKEND_FOLDER_PATH}/${TEMPLATE_BACKEND}" ]; then
  echo "Please execute this script at the root folder of ONNC source"
  exit 1
fi

if [ -d "${BACKEND_FOLDER_PATH}/${NEW_BACKEND}" ]; then
  echo "The target folder has already existed. You might want to use another target name or delete the folder manually."
  exit 1
fi

pushd "${BACKEND_FOLDER_PATH}" > /dev/null
$SUDO cp -r ${TEMPLATE_BACKEND} ${NEW_BACKEND}
# Rename releated source code files
find ${NEW_BACKEND} -name "${TEMPLATE_BACKEND}*" | xargs -i $SUDO rename -e "s/${TEMPLATE_BACKEND}/${NEW_BACKEND}/g" {}
# Replace content of releated source code files
BACKEND_SOURCE_CODE_LIST=$(find ${NEW_BACKEND} -name "*" -type f)
for source_file in ${BACKEND_SOURCE_CODE_LIST}; do
  replace_backend_name "${source_file}"
done
popd > /dev/null

## Replace content of Quadruple files
$SUDO sed -i "s/\/\/ ${TEMPLATE_BACKEND} DLA/\/\/ ${TEMPLATE_BACKEND} DLA\n    ${NEW_BACKEND,,},/" include/onnc/IR/Quadruple.h

$SUDO sed -i "s/.Case(\"${TEMPLATE_BACKEND,,}\", Quadruple::${TEMPLATE_BACKEND,,})/.Case(\"${TEMPLATE_BACKEND,,}\", Quadruple::${TEMPLATE_BACKEND,,})\n    .Case(\"${NEW_BACKEND,,}\", Quadruple::${NEW_BACKEND,,})/" lib/IR/Quadruple.cpp
$SUDO sed -i "s/return \"${TEMPLATE_BACKEND,,}\";$/return \"${TEMPLATE_BACKEND,,}\";\n    case Quadruple::${NEW_BACKEND,,}: return \"${NEW_BACKEND,,}\";/" lib/IR/Quadruple.cpp
$SUDO sed -i "s/case Quadruple::${TEMPLATE_BACKEND,,}:$/case Quadruple::${NEW_BACKEND,,}:\n    return 8;\n\n  case Quadruple::${TEMPLATE_BACKEND,,}:/" lib/IR/Quadruple.cpp

# Replace content of make releated files
$SUDO sed -i "s/ENABLE_${TEMPLATE_BACKEND^^}_TARGET/ENABLE_${TEMPLATE_BACKEND^^}_TARGET\n\n\/* define ${NEW_BACKEND} target *\/\n#undef ENABLE_${NEW_BACKEND^^}_TARGET/" include/onnc/Config/Config.h.in

$SUDO sed -i "s/ENABLE_${TEMPLATE_BACKEND^^}_TARGET 1/ENABLE_${TEMPLATE_BACKEND^^}_TARGET 1\n\n\/* define ${NEW_BACKEND} target *\/\n#cmakedefine ENABLE_${NEW_BACKEND^^}_TARGET 1/" include/onnc/Config/Config.h.cmake.in

$SUDO sed -i "s/include \$(srcdir)\/Target\/${TEMPLATE_BACKEND}\/Makefile.am/include \$(srcdir)\/Target\/${TEMPLATE_BACKEND}\/Makefile.am\ninclude \$(srcdir)\/Target\/${NEW_BACKEND}\/Makefile.am/" lib/Makefile.am

$SUDO sed -i "s/${TEMPLATE_BACKEND,,}, x86/${TEMPLATE_BACKEND,,}, ${NEW_BACKEND,,}, x86/" m4/onnc-target.m4
$SUDO sed -i "s/${TEMPLATE_BACKEND} X86/${TEMPLATE_BACKEND} ${NEW_BACKEND} X86/" m4/onnc-target.m4
$SUDO sed -i "s/${TEMPLATE_BACKEND,,})/${NEW_BACKEND,,})\n        TARGETS_TO_BUILD=\"${NEW_BACKEND} \$TARGETS_TO_BUILD\"\n        AC_DEFINE(ENABLE_${NEW_BACKEND^^}_TARGET, 1, [define ${NEW_BACKEND} target])\n        ;;\n    ${TEMPLATE_BACKEND,,})/" m4/onnc-target.m4
$SUDO sed -i "s/AM_CONDITIONAL(\[ENABLE_${TEMPLATE_BACKEND^^}/AM_CONDITIONAL(\[ENABLE_${NEW_BACKEND^^}_TARGET\], \[ test \"\${TARGETS_TO_BUILD\/${NEW_BACKEND}}\" != \"\${TARGETS_TO_BUILD}\" \])\n  AM_CONDITIONAL(\[ENABLE_${TEMPLATE_BACKEND^^}/" m4/onnc-target.m4
