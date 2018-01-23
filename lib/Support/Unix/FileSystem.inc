//===- FileSystem.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileHandle.h>
#include <onnc/Support/Expansion.h>
#include <onnc/Support/OStrStream.h>

#include <string>
#include <stack>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#if defined(__APPLE__)
#include <copyfile.h>
#elif defined(__linux__)
#include <sys/sendfile.h>
#endif

namespace onnc {

const char Path::separator                   = '/';
const char Path::preferred_separator         = '/';
const Path::StringType Path::separator_str   = Path::StringType("/");
#if defined(__APPLE__)
const Path::StringType Path::dylib_extension = Path::StringType("dylib");
#else
const Path::StringType Path::dylib_extension = Path::StringType("so");
#endif

//===----------------------------------------------------------------------===//
// canonicalize
//===----------------------------------------------------------------------===//
namespace sys {

bool canonicalize(Path& path)
{
  // Variable Index //
  // SepTable - stack of result separators
  // LR(1) Algorithm //
  // traverse pPathName
  //   if we meet '//', '///', '////', ...
  //     -> ignore it
  //     -> push current into stack
  //     -> jump to the next not '/'
  //   if we meet '/./'
  //     -> ignore
  //     -> jump to the next not '/'
  //   if we meet '/../'
  //     if stack.size() <= 1
  //       -> do nothing, '/../' => '/'
  //     else
  //       -> pop previous position of '/' P
  //       -> erase P+1 to now
  //   if we meet other else
  //     -> go go go

  std::string pathname = path.c_str();

  /* canonical path must start with '/' */
  if (pathname.empty() || pathname.front() != Path::separator) {
    char* pwd = ::getcwd(NULL, 0);
    std::string pwds(pwd);
    ::free(pwd);
    pathname = pwds + Path::separator + pathname;
  }

  /* . and .. implies a hidden '/' */
  if ('.' == pathname.back())
    pathname += Path::separator;

  size_t handler = 0;
  std::stack<size_t> slash_stack;
  slash_stack.push(-1);
  while (handler < pathname.size()) {
    if (Path::separator == pathname[handler]) { // handler = 1st '/'
      size_t next = handler + 1;
      if (next >= pathname.size()) {
        handler = pathname.size();
        continue;
      }
      switch(pathname[next]) { // next = handler + 1;
        case Path::separator: { // '//'
          while (next < pathname.size() && Path::separator == pathname[next])
            ++next;
          // next is the last not '/'
          pathname.erase(handler, next - handler - 1);
          // handler is the first '/'
          slash_stack.push(handler);
          break;
        }
        case '.': { // '/.'
          ++next; // next = handler + 2
          if (next >= pathname.size()) {// '/.'
            pathname.erase(handler+1, 1);
            handler = pathname.size();
            continue;
          }
          switch (pathname[next]) {
            case Path::separator: { // '/./'
              pathname.erase(handler, 2);
              break;
            }
            case '.': { // '/..'
              ++next; // next = handler + 3;
              if (next >= pathname.size()) {// '/..?'
                handler = pathname.size();
                continue;
              }
              switch(pathname[next]) {
                case Path::separator: { // '/../'
                  if (slash_stack.size() > 1) {
                    handler = slash_stack.top();
                    slash_stack.pop();
                  }
                  pathname.erase(handler+1, next-handler);
                  if (static_cast<size_t>(-1) == handler) {
                    slash_stack.push(-1);
                    handler = pathname.find_first_of(Path::separator, handler);
                  }
                  break;
                }
                default : { // '/..a'
                  slash_stack.push(handler);
                  handler = pathname.find_first_of(Path::separator, handler+3);
                  break;
                }
              }
              break;
            }
            default : { // '/.a'
              slash_stack.push(handler);
              handler = pathname.find_first_of(Path::separator, handler+2);
              break;
            }
          }
          break;
        }
        default : { // '/a
          slash_stack.push(handler);
          handler = pathname.find_first_of(Path::separator, handler+1);
          break;
        }
      }
    }
    else {
      handler = pathname.find_first_of(Path::separator, handler);
    }
  } // end of while

  if (handler >= pathname.size()) {
    Path result(pathname);
    path = pathname;
    return true;
  }
  return false;
}

/// Checks if \ref pValue is the separator of the path in the system.
bool is_separator(char value)
{
  return (value == Path::separator || value == Path::preferred_separator);
}

} // namespace of sys

//===----------------------------------------------------------------------===//
// chmod
//===----------------------------------------------------------------------===//
bool chmod(const Path& pPath, uint32_t pMode)
{
  int ret = ::chmod(pPath.c_str(), pMode);
  return (0 == ret);
}

//===----------------------------------------------------------------------===//
// copy_file
//===----------------------------------------------------------------------===//
// copy file or symbolic link from @ref pFrom to @ref pTo
SystemError copy_file(const Path& pFrom, const Path& pTo, CopyOptions pOption)
{
  // check source file
  if (!exists(pFrom))
    return SystemError::kNoSuchFileOrDirectory;

  if (!(is_regular(pFrom) || is_symlink(pFrom)))
    return SystemError::kNotSupported;

  // check target file
  if (exists(pTo)) {
    // fail if exists
    if (pOption == kFailIfExists)
      return SystemError::kInvalidArgument;

    // not a regular file nor a symbolic link
    if (!(is_regular(pTo) || is_symlink(pTo)))
      return SystemError::kNotSupported;
  }

#if defined(__APPLE__)
  copyfile_flags_t flags = COPYFILE_ALL | COPYFILE_STAT | COPYFILE_NOFOLLOW;
  if (pOption == kFailIfExists)
    flags |= COPYFILE_EXCL;
  int ret = copyfile(pFrom.c_str(), pTo.c_str(), NULL, flags);
  if (0 > ret)
    return errno;
  else
    return SystemError::kSuccess;
  // end of code in Apple system
#else
  if (is_symlink(pFrom)) {
    Path target;
    SystemError err = readlink(target, pFrom);
    if (err != SystemError::kSuccess)
      return err;

    int ret = ::symlink(target.c_str(), pTo.c_str());
    if (0 > ret)
      return errno;
    else
      return SystemError::kSuccess;
  }
  else {
    int fd_in = ::open(pFrom.c_str(), O_RDONLY);
    if (-1 == fd_in)
      return errno;

    struct stat file_stat;
    fstat(fd_in, &file_stat);

    int fd_out = ::open(pTo.c_str(), O_WRONLY | O_CREAT, file_stat.st_mode);
    if (-1 == fd_out) {
      int ret = errno;
      close(fd_in);
      return ret;
    }

#if defined(__linux__)
    off_t bytes_copied = 0;
    ssize_t ret = sendfile(fd_out, fd_in, &bytes_copied, file_stat.st_size);
    if(-1 == ret)
      return errno;
#else
    int ch;
    while (0 < read(fd_in, &ch, 1))
      ::write(fd_out, &ch, 1); 
#endif

    close(fd_in);
    close(fd_out);
    return SystemError::kSuccess;
    // end of code in Unix system
  }
#endif
}

//===----------------------------------------------------------------------===//
// mkdir
//===----------------------------------------------------------------------===//
// make a director file
// @param[in] pDir  The path of directory
// @param[in] pMode The possible permission bits
// @retval false Fails to create a directory file.
// @see FileStatus
SystemError mkdir(const Path& pDir, uint32_t pMode)
{
  if (0 > ::mkdir(pDir.c_str(), pMode))
    return errno;
  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// readlink
//===----------------------------------------------------------------------===//
SystemError readlink(Path& pTarget, const Path& pPath)
{
  char buf[PATH_MAX];
  ssize_t len;
  len = ::readlink(pPath.c_str(), buf, sizeof(buf)-1);
  if (0 > len)
    return errno;
  buf[len] = '\0';
  pTarget.assign(buf, len);
  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// getwd
//===----------------------------------------------------------------------===//
SystemError getwd(Path& pPath)
{
  char* pwd = ::getcwd(NULL, 0);
  if (nullptr == pwd)
    return SystemError(errno);

  pPath.assign(pwd, ::strlen(pwd));
  ::free(pwd);
  return SystemError::kSuccess;
}

//===----------------------------------------------------------------------===//
// open_file
//===----------------------------------------------------------------------===//
int open_file(const Path& pPath, int pOFlag) {
  return ::open(pPath.native().c_str(), pOFlag);
}

int open_file(const Path& pPath, int pOFlag, int pPerm) {
  mode_t perm = 0;
  if (pPerm & FileHandle::kReadOwner)
    perm |= S_IRUSR;
  if (pPerm & FileHandle::kWriteOwner)
    perm |= S_IWUSR;
  if (pPerm & FileHandle::kExeOwner)
    perm |= S_IXUSR;
  if (pPerm & FileHandle::kReadGroup)
    perm |= S_IRGRP;
  if (pPerm & FileHandle::kWriteGroup)
    perm |= S_IWGRP;
  if (pPerm & FileHandle::kExeGroup)
    perm |= S_IXGRP;
  if (pPerm & FileHandle::kReadOther)
    perm |= S_IROTH;
  if (pPerm & FileHandle::kWriteOther)
    perm |= S_IWOTH;
  if (pPerm & FileHandle::kExeOther)
    perm |= S_IXOTH;

  return ::open(pPath.native().c_str(), pOFlag, perm);
}

} // namespace of onnc
