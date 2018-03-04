//===- FileHandle.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/FileHandle.h>
#include <sys/mman.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// FileHandler
//===----------------------------------------------------------------------===//
SystemError FileHandle::mmap(void*& pMemBuffer, size_t pStartOffset, size_t pLength)
{
  if (!isOpen())
    return SystemError::kBadFileDescriptor;

  if (0 == pLength)
    return SystemError::kSuccess;

  int prot, flag;
  if (isReadable() && !isWritable()) {
    // read-only
    prot = PROT_READ;
    flag = MAP_FILE | MAP_PRIVATE;
  }
  else if (!isReadable() && isWritable()) {
    // write-only
    prot = PROT_WRITE;
    flag = MAP_FILE | MAP_SHARED;
  }
  else if (isReadWrite()) {
    // read and write
    prot = PROT_READ | PROT_WRITE;
    flag = MAP_FILE | MAP_SHARED;
  }
  else {
    // can not read/write
    return SystemError::kBadFileDescriptor;
  }

  pMemBuffer = ::mmap(NULL, pLength, prot, flag, m_Handler, pStartOffset);

  if (MAP_FAILED == pMemBuffer)
    return SystemError(errno);

  return SystemError::kSuccess;
}

SystemError FileHandle::munmap(void* pMemBuffer, size_t pLength)
{
  if (!isOpen())
    return SystemError::kBadFileDescriptor;

  if (-1 == ::munmap(pMemBuffer, pLength))
    return SystemError(errno);

  return SystemError::kSuccess;
}

} // namespace of onnc
