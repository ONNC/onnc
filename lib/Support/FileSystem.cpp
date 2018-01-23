//===- FileSystem.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileSystem.h>
#include <onnc/Support/Directory.h>
#include <onnc/Support/IFStream.h>
#include <onnc/Support/Path.h>
#include <onnc/Config/Config.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/Rope.h>
#include <cstring>
#include <queue>
#include <stack>
#include <errno.h>
#include <cstdio>

// Include the truly platform-specific parts.
#if defined(ONNC_ON_UNIX)
#include "Unix/FileSystem.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/FileSystem.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/FileSystem.inc"
#endif

bool onnc::diff(const Path& pPath1, const Path& pPath2, bool pIgnoreSpaces)
{
  IFStream fin1(pPath1);
  IFStream fin2(pPath2);
  int c1, c2;
  if (pIgnoreSpaces) {
    do {
      while ((!fin1.eof()) && (0 != isspace(c1 = fin1.get())));
      while ((!fin2.eof()) && (0 != isspace(c2 = fin2.get())));
    } while ((!fin1.eof()) && (!fin2.eof()) && (c1 == c2));
  }
  else {
    do {
      c1 = fin1.get();
      c2 = fin2.get();
    } while ((!fin1.eof() && (!fin2.eof())) && (c1 == c2));
  }
  return (!fin1.eof()) || (!fin2.eof());
}

onnc::SystemError onnc::remove(const Path& pPath)
{
  if (0 != ::remove(pPath.c_str()))
    return errno;
  return SystemError::kSuccess;
}

onnc::SystemError onnc::clean(const Path& pPath)
{
  if (!exists(pPath))
    return SystemError::kSuccess;

  std::stack<Path> to_visit;
  to_visit.push(pPath);
  while (!to_visit.empty()) {
    SystemError err = remove(to_visit.top());
    if (SystemError::kSuccess == err.code()) {
      // Pop the path out of stack after it is removed.
      to_visit.pop();
    }
    else { //< Fail to remove.
      if (SystemError::kDirectoryNotEmpty != err.code()) {
        // An internal error occurs. Must report.
        return err;
      }

      // Add all children to stack when error says that directory isn't empty.
      Path current = to_visit.top();
      Directory dir(current);
      Directory::const_iterator it, pEnd = dir.end();
      for (it = dir.begin(); it != pEnd; it.next()) {
        Path path(current);
        path.append(it.fileInfo().path());
        to_visit.push(path);
      }
    }
  } // end of while

  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// Copy
//===----------------------------------------------------------------------===//
onnc::SystemError
onnc::copy(const Path& pFrom, const Path& pTo, CopyOptions pOptions)
{
  // The source is not a directory, use copy_file.
  if (!is_directory(pFrom)) {
    if (!is_directory(pTo))
      return copy_file(pFrom, pTo, pOptions);
    else {
      Path target(pTo);
      target.append(pFrom.filename());
      return copy_file(pFrom, target, pOptions);
    }
  }

  Path target(pTo);
  if (exists(target)) {
    // The target is not a directory.
    if (!is_directory(target))
      return SystemError::kFileExists;

    // Copy "into" the target directory.
    target.append(pFrom.basename());

    if (exists(target)) {
      // The target is not a directory.
      if (!is_directory(target))
        return SystemError::kFileExists;
    }
    else {
      FileStatus st;
      status(pFrom, st);
      SystemError err = mkdir(target, st.permission());
      if (SystemError::kSuccess != err.code())
        return err;
    }
  }
  else {
    FileStatus st;
    status(pFrom, st);
    SystemError err = mkdir(target, st.permission());
    if (SystemError::kSuccess != err.code())
      return err;
  }

  // The source is a directory, add all children to queue.
  std::queue<Path> to_visit;
  Directory dir(pFrom);
  Directory::const_iterator it, pEnd = dir.end();
  for (it = dir.begin(); it != pEnd; it.next()) {
    to_visit.push(it.fileInfo().path());
  }

  while (!to_visit.empty()) {
    Path current = to_visit.front();
    to_visit.pop();

    Path src(pFrom), dst(target);
    src.append(current);
    dst.append(current);

    SystemError err;
    if (!is_directory(src))
      err = copy_file(src, dst);
    else {
      dir.open(src);
      for (it = dir.begin(), pEnd = dir.end(); it != pEnd; it.next()) {
        Path child(current);
        child.append(it.fileInfo().path());
        to_visit.push(child);
      }

      FileStatus st;
      status(src, st);
      err = mkdir(dst, st.permission());
    }
    if (SystemError::kSuccess != err.code())
      return err;
  }

  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// which
//===----------------------------------------------------------------------===//
static inline bool is_there_exec(const onnc::Path& pFile)
{
  // a executable file on
  // 1. regular file
  // 2. symlink to a regular file
  if (is_executable(pFile)) {
    if (is_regular(pFile))
      return true;

    onnc::Path target(pFile);
    while (is_symlink(target)) {
      onnc::Path canon;
      if (!onnc::realpath(canon, target))
        return false;

      target = canon;
      if (is_regular(target))
        return true;
    } // end of while
  }
  return false;
}

// Locate a program file in the user's path.
//
// @param[out] pPath The found path
// @param[in] pFilename The name of the program file.
//
// @retval true If we found the path of the file.
bool onnc::which(Path& pPath, const Path& pFile)
{
  // get the environment variable "PATH"
  char* PATH = nullptr;
  if (nullptr == (PATH = getenv("PATH"))) {
    // can not get environment
    return false;
  }

  return onnc::which(pPath, pFile, StringRef(PATH));
}

// Locate a program file in the user's path.
//
// @param[out] pPath The found path
// @param[in] pFilename The name of the program file.
//
// @retval true If we found the path of the file.
bool onnc::which(Path& pPath, const Path& pFile, StringRef pEnviron)
{
  // contains separator
  if (NULL != strchr(pFile.native().c_str(), Path::separator)) {
    if (is_there_exec(pFile)) {
      pPath = pFile;
      return true;
    }
    return false;
  }

  bool found = false;
  StringRef::size_type pos = 0, length = pEnviron.find_first_of(':');
  while (!found && pos != StringRef::npos) {
    Path result;
    if (0 == length)
      result.assign(".");
    else
      result.assign(pEnviron.substr(pos, length).str());
    result.append(pFile);

    if (is_there_exec(result)) {
      pPath = result;
      found = true;
    }

    pos = pEnviron.find_first_not_of(':', pos + length);
    length = pEnviron.find_first_of(':', pos) - pos;
  }
  return found;
}

//===----------------------------------------------------------------------===//
// realexe
//===----------------------------------------------------------------------===//
static inline bool
is_there_exec(onnc::Path& pTarget, const onnc::Path& pFile)
{
  // a executable file on
  // 1. regular file
  // 2. symlink to a regular file
  if (is_executable(pFile)) {
    if (is_regular(pFile)) {
      pTarget = pFile;
      return true;
    }

    // find the final regular program.
    onnc::Path target(pFile);
    while (is_symlink(target)) {
      onnc::Path canon;
      if (!onnc::realpath(canon, target))
        return false;

      target = canon;
      if (is_regular(target)) {
        pTarget = target;
        return true;
      }
    } // end of while
  }
  return false;
}

bool onnc::realexe(Path& pCanon, const Path& pFile)
{
  // get the environment variable "PATH"
  char* PATH = nullptr;
  if (nullptr == (PATH = getenv("PATH"))) {
    // can not get environment
    return false; 
  }

  return realexe(pCanon, pFile, PATH);
}

bool onnc::realexe(Path& pCanon, const Path& pFile, StringRef pEnviron)
{
  // contains separator
  if (NULL != strchr(pFile.native().c_str(), Path::separator)) {
    Path result;
    if (!is_there_exec(result, pFile))
      return false;

    if (!canonicalize(result))
      return false;

    pCanon = result;
    return true;
  }

  // search in pEnviron
  bool found = false;
  StringRef::size_type pos = 0, length = pEnviron.find_first_of(':');
  while (!found && pos != StringRef::npos) {
    Path target;
    if (0 == length)
      target.assign(".");
    else
      target.assign(pEnviron.substr(pos, length).str());
    target.append(pFile);

    Path result;
    if (is_there_exec(result, target)) {
      if (!canonicalize(result))
        return false;
      pCanon = result;
      found = true;
    }

    pos = pEnviron.find_first_not_of(':', pos + length);
    length = pEnviron.find_first_of(':', pos) - pos;
  }

  return found;
}

//===----------------------------------------------------------------------===//
// realpath
//===----------------------------------------------------------------------===//
bool onnc::realpath(Path& pCanon, const Path& pFilepath)
{
  char* path = ::realpath(pFilepath.c_str(), nullptr);
  if (nullptr == path)
    return false;

  pCanon.assign(path);
  ::free(path);
  return true;
}

//===----------------------------------------------------------------------===//
// absolute
//===----------------------------------------------------------------------===//
bool onnc::absolute(Path& pPath)
{
  Path result(pPath);
  if (!result.isFromRoot()) {
    getwd(result);
    result.append(pPath);
  }

  if (is_directory(result) && Path::separator == result.native().back())
    result.native().pop_back();

  pPath = result;
  return true;
}

bool onnc::absolute(Path& pAbs, const Path& pPath)
{
  Path result(pPath);
  if (!result.isFromRoot()) {
    char* cwd = getcwd(NULL, 0);
    result.assign(cwd);
    free(cwd);
    result.append(pPath);
  }

  if (is_directory(result) && Path::separator == result.native().back())
    result.native().pop_back();

  pAbs = result;
  return true;
}

onnc::Path onnc::absolute(const Path& pPath)
{
  Path result;
  if (absolute(result, pPath))
    return result;
  return pPath;
}

//===----------------------------------------------------------------------===//
// canonicalize
//===----------------------------------------------------------------------===//
bool onnc::canonicalize(Path& pPath)
{
  Path result(pPath);
  if (!result.isFromRoot() && !absolute(result)) {
    return false;
  }

  if (!sys::canonicalize(result)) {
    return false;
  }

  pPath = result;
  return true;
}

bool onnc::canonicalize(Path& pCanon, const Path& pPath)
{
  Path result(pPath);
  if (!result.isFromRoot() && !absolute(result)) {
    return false;
  }

  if (!sys::canonicalize(result)) {
    return false;
  }

  pCanon = result;
  return true;
}

onnc::Path onnc::canonicalize(const Path& pPath)
{
  Path result(pPath);
  if (!result.isFromRoot() && !absolute(result)) {
    return Path();
  }

  if (!sys::canonicalize(result)) {
    return Path();
  }

  return result;
}

//===----------------------------------------------------------------------===//
// chdir
//===----------------------------------------------------------------------===//
onnc::SystemError onnc::chdir(const Path& pPath)
{
  if (-1 == ::chdir(pPath.c_str()))
    return SystemError(errno);
  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// rename
//===----------------------------------------------------------------------===//
onnc::SystemError onnc::rename(const Path& pFrom, const Path& pTo)
{
  if (-1 == ::rename(pFrom.c_str(), pTo.c_str()))
    return SystemError(errno);
  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// symlink
//===----------------------------------------------------------------------===//
onnc::SystemError onnc::symlink(const Path& pSrc, const Path& pLinkage)
{
  if (-1 == ::symlink(pSrc.c_str(), pLinkage.c_str()))
    return SystemError(errno);
  return SystemError::kSuccess;
}
