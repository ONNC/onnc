//===- ONNC_MODULE_PRINTER.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_MODULE_PRINTER_H
#define ONNC_MODULE_PRINTER_H

#include <onnc/Core/ModulePass.h>

namespace onnc {
ModulePass *createONNCModulePrinterPass();
}


#endif // ONNC_MODULE_PRINTER_H
