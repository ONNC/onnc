//===- MappedFileRegion.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/MappedFileRegion.h>
#include <limits>
#include <sys/mman.h>
#include <unistd.h>
#include <cassert>

using namespace onnc;

MappedFileRegion::MappedFileRegion(int pFD, MapMode pMode,
                                   uint64_t pLength, uint64_t pOffset,
                                   SystemError &pErr)
  : m_Size(pLength), m_pMapping()
{
  // Make sure that the requested size fits within SIZE_T.
  if (pLength > std::numeric_limits<size_t>::max()) {
    pErr = SystemError::kInvalidArgument;
    return;
  }

  pErr = init(pFD, pOffset, pMode);
  if (!pErr.isGood())
    m_pMapping = nullptr;
}

MappedFileRegion::~MappedFileRegion()
{
  if (m_pMapping)
    ::munmap(m_pMapping, m_Size);
}

int MappedFileRegion::alignment()
{
  return ::getpagesize();
}

SystemError MappedFileRegion::init(int pFD, uint64_t pOffset, MapMode pMode)
{
  assert(m_Size != 0);

  int flags = (pMode == readwrite) ? MAP_SHARED : MAP_PRIVATE;
  int prot = (pMode == readonly) ? PROT_READ : (PROT_READ | PROT_WRITE);
  m_pMapping = ::mmap(nullptr, m_Size, prot, flags, pFD, pOffset);

  if (m_pMapping == MAP_FAILED)
    return SystemError::kIoError;

  return SystemError::kSuccess;
}
