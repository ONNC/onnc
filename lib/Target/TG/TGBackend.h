//===- TGBackend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_BACKEND_H
#define TARGET_TG_TG_BACKEND_H
#include <string>
#include <onnc/Target/DLATargetBackend.h>

namespace onnc {

class TGBackend : public DLATargetBackend
{
public:
  TGBackend(const CompilerConfig& pConfig);

  virtual ~TGBackend();
};

class BM1680Backend : public TGBackend
{
public:
  BM1680Backend(const CompilerConfig& pConfig);

  virtual ~BM1680Backend();
};

class BM1682Backend : public TGBackend
{
public:
  BM1682Backend(const CompilerConfig& pConfig);

  virtual ~BM1682Backend();
};

}  // namespace onnc

#endif  // TARGET_TG_TG_BACKEND_H
