#-==========================================================================-#
# COND_CONFIG_FILES
#-==========================================================================-#
#
# SYNOPSIS
# --------
#  COND_CONFIG_FILES(
#    [dir],
#    [file ...],
#    [cmds],
#    [init-cmds]
#  )
#
# DESCRIPTION
# -----------
#   Calls `AC_CONFIG_FILES` if "[dir]" exists.
#
#-==========================================================================-#
# INIT_SUBMOD
#-==========================================================================-#
#
# SYNOPSIS
# --------
#  INIT_SUBMOD(
#    [name],
#    [version],
#  )
#
# DESCRIPTION
# -----------
#   Initialize output variables for a submodule.
#     [name]_VERSION=[version]
#
#-==========================================================================-#
# COND_CONFIG_SUBMOD
#-==========================================================================-#
#
# SYNOPSIS
# --------
#  COND_CONFIG_SUBMOD(
#    [name],
#    [version],
#    [file ...],
#    [cmds],
#    [init-cmds]
#  )
#
# DESCRIPTION
# -----------
#   Calls `INIT_SUBMOD` and `AC_CONFIG_FILES` if "tools/[name]" exists.
#
#-==========================================================================-#
# Yi Huang <yi@skymizer.com>
#-==========================================================================-#

### begin of COND_CONFIG_FILES
AC_DEFUN([COND_CONFIG_FILES], [dnl

AC_SUBST([CCF_ABSDIR], [${srcdir}/$1])

AC_MSG_CHECKING([if \${srcdir}/$1 exists.])
AS_IF([test ! -d "${CCF_ABSDIR}"], [dnl cond-if-not-found
  AC_MSG_RESULT([no])
], [dnl cond-found
  AC_MSG_RESULT([yes])
  AC_CONFIG_FILES($2, $3, $4)
])

])
### end of COND_CONFIG_FILES

### begin of INIT_SUBMOD
AC_DEFUN([INIT_SUBMOD], [dnl

AC_SUBST([$1_VERSION], [$2])
AC_MSG_RESULT([	$1_VERSION=$2])

])
### end of INIT_SUBMOD

### begin of COND_CONFIG_SUBMOD
AC_DEFUN([COND_CONFIG_SUBMOD], [dnl

COND_CONFIG_FILES([tools/$1], [$3], [$4], [$5])
INIT_SUBMOD([$1], [$2])

])
### end of COND_CONFIG_SUBMOD
