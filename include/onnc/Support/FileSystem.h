//===- FileSystem.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_FILE_SYSTEM_H
#define ONNC_SUPPORT_FILE_SYSTEM_H
#include <onnc/Support/DataTypes.h>
#include <onnc/Support/ErrorCode.h>
#include <onnc/Support/FileStatus.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/CEnv.h>
#include <onnc/ADT/StringRef.h>

namespace onnc {

enum CopyOptions
{
  kNone,
  kFailIfExists = kNone,
  kOverwriteIfExists
};

/// Canonicalizes the path.
bool canonicalize(Path& pPath);

/// @return true if permissions of \ref pPath is successfully changed.
bool chmod(const Path& pPath, uint32_t pMode);

/// copy a file or symbolic link @ref pFrom to @ref pTo. When @ref pOptions
/// is kFailIfExists, this function fails to copy if @ref pTo exists. On the
/// other hand, when @ref pOption is kOverwriteIfExists, this function
/// overrides @ref pTo if @ref pTo exists and return successfully.
///
/// @param pFrom    [in]  the path of source file
/// @param pTo      [in]  the path of target file
/// @param pOptions [in]  the options to control @ref pError when @ref pTo exists
/// @return return kSuccess if no error occurs.
SystemError copy_file(const Path& pFrom, const Path& pTo,
                      CopyOptions pOptions = kFailIfExists);

/// copy a file, a symlink, or a directory @ref pFrom to @ref pTo.
/// When @ref pOptions is kFailIfExists, this function fails to copy
/// if @ref pTo exists. On the other hand, when @ref pOption is
/// kOverwriteIfExists, this function overrides @ref pTo if @ref pTo exists
/// and return successfully.
///
/// @param pFrom    [in]  the path of source file
/// @param pTo      [in]  the path of target file
/// @param pOptions [in]  the options to control @ref pError when @ref pTo exists
/// @return kSuccess if no error occurs.
SystemError
copy(const Path& pFrom, const Path& pTo, CopyOptions pOptions = kFailIfExists);

/// make a director file.
/// @param[in] pDir  The path of directory
/// @param[in] pMode The possible permission bits
/// @retval false Fails to create a directory file.
/// @see FileStatus
SystemError mkdir(const Path& pDir, uint32_t pMode);

/// Compare two files character by character.
/// @param[in] pIgnoreSpaces Ignore all whitespace characters in the files.
/// @retval true if the two files are different.
bool diff(const Path& pPath1, const Path& pPath2, bool pIgnoreSpaces = false);

/// Remove the file or directory specified by @ref pPath.
/// If path specifies a directory, remove(path) is the equivalent of rmdir(path).
/// Otherwise, it is the equivalent of unlink(path).
SystemError remove(const Path& pPath);

/// Remove the file or directory specified by @ref pPath.
/// It is the equivalent of `rm -rf [path]`.
SystemError clean(const Path& pPath);

/// Read value of a symbolic link.
/// @param[out] pTarget   Value of the symbolic link.
/// @param[in]  pPath     Path of the symbolic link.
SystemError readlink(Path& pTarget, const Path& pPath);

/// Locate a program's path in the user's PATH environment variable.
///
/// @param[out] pPath     The found path
/// @param[in]  pFilename The name of the program file.
/// @retval true if we found the path of the file.
///
/// @note man 1 which
/// @note http://www.opensource.apple.com/source/shell_cmds/shell_cmds-170/which/which.c
bool which(Path& pPath, const Path& pFilename);

/// Locate a program's path in the environment argument
///
/// @param[out] pPath     The found path
/// @param[in]  pFilename The name of the program file
/// @param[in]  pEnviron  The environment argument string.
///                       Must be separated by ':'.
/// @retval true if we found the path of the file.
///
/// @note man 1 which
/// @note http://www.opensource.apple.com/source/shell_cmds/shell_cmds-170/which/which.c
bool which(Path& pPath, const Path& pFilename, StringRef pEnviron);

/// Locate a program's absolute path in ${PATH}.
/// realexe is a kind of `which` function, which locates a program's path in the user's
/// PATH environment variable. The difference is
/// 1. realexe resolves the symbolic links, and
/// 2. realexe returns the canonical form
bool realexe(Path& pCanon, const Path& pFilename);

/// Locate a program's absolute path in the environment argument.
bool realexe(Path& pCanon, const Path& pFilename, StringRef pEnviron);

/// Resolve all symbolic links and returns the canonicalized absolute pathname
/// All components of @ref pFilename must exist when realpath() is called.
///
/// @param[out] pCanon The resulting absolute pathname
/// @param[in]  pFilename The resolved path
/// @return Success or not
/// @see man(3) realpath
bool realpath(Path& pCanon, const Path& pFilename);

/// Change @ref pPath from the native form to absolute form.
/// @param[in,out] pPath The path being changed.
/// example:
/// @code
/// In folder /home/user/project
/// ./a -> $PWD/a -> /home/user/project/./a
/// ../b -> $PWD/../b -> /home/user/project/../b
/// ../b/ -> $PWD/../b/ -> /home/user/project/../b/ -> /home/user/project/../b
/// @endcode
bool absolute(Path& pPath);

/// Change @ref pPath from the native form to absolute form.
/// @param[out] pAbsResult The change result.
/// @param[in]  pPath      The path being change.
/// @code
/// In folder /home/user/project
/// ./a  -> $PWD/a -> /home/user/project/./a
/// ../b -> $PWD/../b -> /home/user/project/../b
/// ../b/ -> $PWD/../b/ -> /home/user/project/../b/ -> /home/user/project/../b
/// @endcode
/// @retval false Error occurs.
bool absolute(Path& pAbsResult, const Path& pPath);

/// Change @ref pPath from the native from to absolute form.
/// @return The absolute path.
/// @retval empty If this function fails.
Path absolute(const Path& pPath);

/// Change @ref pPath from the native form to canonical form.
/// Canonical form means that a path P is a absolute form without dot and dot
/// dot in the path.
/// @param[in,out] pPath The path being changed.
/// example:
/// @code
/// If @ref pPath is a relative path, we will transform pPath to absolute form
/// and then change it to canonical form.
///
/// In folder /home/user/project
/// ./a -> $PWD/a -> /home/user/project/a
/// ../b -> $PWD/../b -> /home/user/b
/// ../b/ -> $PWD/../b/ -> /home/user/b/ -> /home/user/b
/// ../../../../.. -> $PWD/../../../../.. -> /
/// ../../../../../bin -> $PWD/../../../../../bin -> /bin
/// /home/user/project/./a -> /home/user/project/a
/// /home/user/project/.././b -> /home/user/b
/// /home/user/project/../../../../../bin -> /bin
/// @endcode
/// @retval false The path does not exist.
bool canonicalize(Path& pPath);

/// Change @ref pPath from the native form to canonical form.
/// @param[out] pAbsResult The change result.
/// @param[in]  pPath      The path being change.
/// example:
/// @code
/// If @ref pPath is a relative path, we will transform pPath to absolute form
/// and then change it to canonical form.
///
/// In folder /home/user/project
/// ./a -> $PWD/a -> /home/user/project/a
/// ../b -> $PWD/../b -> /home/user/b
/// ../b/ -> $PWD/../b/ -> /home/user/b/ -> /home/user/b
/// ../../../../.. -> $PWD/../../../../.. -> /
/// ../../../../../bin -> $PWD/../../../../../bin -> /bin
/// /home/user/project/./a -> /home/user/project/a
/// /home/user/project/.././b -> /home/user/b
/// /home/user/project/../../../../../bin -> /bin
/// @endcode
/// @retval false The path does not exist.
bool canonicalize(Path& pCanonResult, const Path& pPath);

/// Change @ref pPath from the native from to canonical form.
/// @return The canonical path.
/// @retval empty If this function fails.
Path canonicalize(const Path& pPath);

/// Change current working directory
/// @param[in] pPath The pathname of the directory.
SystemError chdir(const Path& pPath);

/// Get current working directory
/// @param[out] pPath The pathname of current working directory.
SystemError getwd(Path& pPath);

int open_file(const Path& pPath, int pOFlag);

int open_file(const Path& pPath, int pOFlag, int pPerm);

/// Rename the old path file or folder to new pathname
/// @param[in] pFrom The original pathname
/// @param[in] pTo   The new pathname
/// @return The error value
/// @retval kSuccess success to rename.
SystemError rename(const Path& pFrom, const Path& pTo);

/// Create a symbolic linkage @ref pLinkage to file @ref pSource
SystemError symlink(const Path& pSource, const Path& pLinkage);

} // namespace of onnc

#endif
