//===- Application.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_APPLICATION_H
#define ONNC_CORE_APPLICATION_H
#include <onnc/ADT/Uncopyable.h>

namespace onnc {

/** \class onnc::CoreApplication
 *  \brief provides basic interfaces for a command line tool
 *
 *  CoreApplication initializes these systems:
 *  - command line parser
 */
class CoreApplication : protected Uncopyable
{
public:
  CoreApplication(int pArgc, char* pArgv[]);

  virtual ~CoreApplication();
};

} // namespace of onnc

#endif
