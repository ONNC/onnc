//===- DefaultModulePass.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_DEFAULT_MODULE_PASS_H
#define ONNC_CORE_DEFAULT_MODULE_PASS_H
#include <onnc/Core/ModulePass.h>

#include <type_traits>

namespace onnc {

/** \class onnc::DefaultModulePass
 *  \brief default module class which implements most of the pure virtual methods
 */
template <
  typename PassType,
  typename = typename std::enable_if<
    std::is_class<PassType>::value
  >::type
>
class DefaultModulePass : public ModulePass
                        , public GeneratePassIdFor<PassType>
{ };

} // namespace of onnc

#endif
