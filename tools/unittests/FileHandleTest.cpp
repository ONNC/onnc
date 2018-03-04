//===- FileHandleTest.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Support/FileHandle.h>
#include <onnc/Support/Path.h>
#include <onnc/Support/IOStream.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

using namespace skypat;
using namespace onnc;

//===----------------------------------------------------------------------===//
// FileHandleTest
//===----------------------------------------------------------------------===//
SKYPAT_F(FileHandleTest, open_close)
{
  Path path(TOPDIR);
  path.append("tools").append("unittests").append("data").append("test.txt");
  FileHandle file;
  ASSERT_TRUE(file.open(path, FileHandle::OpenMode(FileHandle::kReadOnly)).isGood());
  ASSERT_TRUE(file.isOpen());

  ASSERT_TRUE(27 == file.size());

  ASSERT_TRUE(file.close().isGood());
  ASSERT_FALSE(file.isOpen());

  ASSERT_TRUE(0 == file.size());
}

SKYPAT_F(FileHandleTest, delegate_close)
{
  Path path(TOPDIR);
  path.append("tools").append("unittests").append("data").append("test.txt");

  int fd = ::open(path.native().c_str(), O_RDONLY);

  FileHandle file;
  ASSERT_TRUE(file.delegate(fd, FileHandle::kReadOnly).isGood());
  ASSERT_TRUE(file.isOpen());

  ASSERT_TRUE(27 == file.size());

  ASSERT_TRUE(file.close().isGood());
  ASSERT_FALSE(file.isOpen());

  ASSERT_TRUE(0 == file.size());

  ASSERT_EQ(::close(fd), -1);
}

SKYPAT_F(FileHandleTest, fail_close)
{
  Path path(TOPDIR);
  path.append("tools").append("unittests").append("data").append("test.txt");

  FileHandle file;
  ASSERT_TRUE(file.open(path, FileHandle::OpenMode(FileHandle::kReadOnly)).isGood());
  ASSERT_TRUE(file.isOpen());

  ASSERT_TRUE(27 == file.size());

  int close_result = ::close(file.handler());
  ASSERT_EQ(0, close_result);

  ASSERT_FALSE(file.close().isGood());
}
