//===- Quadruple.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Quadruple.h>
#include <onnc/ADT/Rope.h>
#include <onnc/ADT/StringSwitch.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Parse Methods
//===----------------------------------------------------------------------===//
Quadruple::ArchType onnc::ParseArch(StringRef pArchName)
{
  return StringSwitch<Quadruple::ArchType>(pArchName)
    .Cases("i386", "i486", "i586", "i686", Quadruple::x86)
    // FIXME: Do we need to support these?
    .Case("x32", Quadruple::x32)
    .Cases("i786", "i886", "i986", Quadruple::x86)
    .Cases("amd64", "x86_64", Quadruple::x86_64)
    .Case("powerpc", Quadruple::ppc)
    .Cases("powerpc64", "ppu", Quadruple::ppc64)
    .Case("mblaze", Quadruple::mblaze)
    .Case("aarch64", Quadruple::aarch64)
    .Cases("arm", "xscale", Quadruple::arm)
    // FIXME: It would be good to replace these with explicit names for all the
    // various suffixes supported.
    .StartsWith("armv", Quadruple::arm)
    .Case("thumb", Quadruple::thumb)
    .StartsWith("thumbv", Quadruple::thumb)
    .Case("msp430", Quadruple::msp430)
    .Cases("mips", "mipseb", "mipsallegrex", Quadruple::mips)
    .Cases("mipsel", "mipsallegrexel", Quadruple::mipsel)
    .Cases("mips64", "mips64eb", Quadruple::mips64)
    .Case("mips64el", Quadruple::mips64el)
    .Case("r600", Quadruple::r600)
    .Case("hexagon", Quadruple::hexagon)
    .Case("s390x", Quadruple::systemz)
    .Case("sparc", Quadruple::sparc)
    .Case("sparcv9", Quadruple::sparcv9)
    .Case("tce", Quadruple::tce)
    .Case("xcore", Quadruple::xcore)
    .Case("nvptx", Quadruple::nvptx)
    .Case("nvptx64", Quadruple::nvptx64)
    .Case("le32", Quadruple::le32)
    .Case("amdil", Quadruple::amdil)
    .Case("spir", Quadruple::spir)
    .Case("spir64", Quadruple::spir64)
    .Case("sophon", Quadruple::sophon)
    .StartsWith("sophonv", Quadruple::sophon)
    .Default(Quadruple::UnknownArch);
}

Quadruple::SubArchType onnc::ParseSubArch(StringRef pSubArchName)
{
  return StringSwitch<Quadruple::SubArchType>(pSubArchName)
    .EndsWith("v8", Quadruple::ARMSubArch_v8)
    .EndsWith("v8a", Quadruple::ARMSubArch_v8)
    .EndsWith("v7", Quadruple::ARMSubArch_v7)
    .EndsWith("v7a", Quadruple::ARMSubArch_v7)
    .EndsWith("v7em", Quadruple::ARMSubArch_v7em)
    .EndsWith("v7l", Quadruple::ARMSubArch_v7)
    .EndsWith("v7m", Quadruple::ARMSubArch_v7m)
    .EndsWith("v7r", Quadruple::ARMSubArch_v7)
    .EndsWith("v7s", Quadruple::ARMSubArch_v7s)
    .EndsWith("v6", Quadruple::ARMSubArch_v6)
    .EndsWith("v6m", Quadruple::ARMSubArch_v6m)
    .EndsWith("v6t2", Quadruple::ARMSubArch_v6t2)
    .EndsWith("v5", Quadruple::ARMSubArch_v5)
    .EndsWith("v5e", Quadruple::ARMSubArch_v5)
    .EndsWith("v5t", Quadruple::ARMSubArch_v5)
    .EndsWith("v5te", Quadruple::ARMSubArch_v5te)
    .EndsWith("v4t", Quadruple::ARMSubArch_v4t)
    .EndsWith("v1680", Quadruple::SophonSubArch_vBM1680)
    .EndsWith("v1682", Quadruple::SophonSubArch_vBM1682)
    .EndsWith("v1880", Quadruple::SophonSubArch_vBM1880)
    .Default(Quadruple::NoSubArch);
}

Quadruple::ArchVendorType onnc::ParseArchVendor(StringRef pArchVendorName)
{
  return StringSwitch<Quadruple::ArchVendorType>(pArchVendorName)
    .Case("apple", Quadruple::Apple)
    .Case("pc", Quadruple::PC)
    .Case("scei", Quadruple::SCEI)
    .Case("bgp", Quadruple::BGP)
    .Case("bgq", Quadruple::BGQ)
    .Case("fsl", Quadruple::Freescale)
    .Case("ibm", Quadruple::IBM)
    .Case("renesas", Quadruple::Renesas)
    .Case("bitmain", Quadruple::BITMAIN)
    .Default(Quadruple::UnknownArchVendor);
}

Quadruple::OSType onnc::ParseOS(StringRef pOSName)
{
  return StringSwitch<Quadruple::OSType>(pOSName)
    .StartsWith("auroraux", Quadruple::AuroraUX)
    .StartsWith("cygwin", Quadruple::Cygwin)
    .StartsWith("darwin", Quadruple::Darwin)
    .StartsWith("dragonfly", Quadruple::DragonFly)
    .StartsWith("freebsd", Quadruple::FreeBSD)
    .StartsWith("ios", Quadruple::IOS)
    .StartsWith("kfreebsd", Quadruple::KFreeBSD)
    .StartsWith("linux", Quadruple::Linux)
    .StartsWith("lv2", Quadruple::Lv2)
    .StartsWith("macosx", Quadruple::MacOSX)
    .StartsWith("mingw32", Quadruple::MinGW32)
    .StartsWith("netbsd", Quadruple::NetBSD)
    .StartsWith("openbsd", Quadruple::OpenBSD)
    .StartsWith("solaris", Quadruple::Solaris)
    .StartsWith("win32", Quadruple::Win32)
    .StartsWith("haiku", Quadruple::Haiku)
    .StartsWith("minix", Quadruple::Minix)
    .StartsWith("rtems", Quadruple::RTEMS)
    .StartsWith("nacl", Quadruple::NaCl)
    .StartsWith("cnk", Quadruple::CNK)
    .StartsWith("bitrig", Quadruple::Bitrig)
    .StartsWith("aix", Quadruple::AIX)
    .StartsWith("ndk", Quadruple::NDK)
    .Default(Quadruple::UnknownOS);
}

Quadruple::EnvironmentType
onnc::ParseEnvironment(StringRef pEnvironmentName)
{
  return StringSwitch<Quadruple::EnvironmentType>(pEnvironmentName)
    .StartsWith("eabi", Quadruple::EABI)
    .StartsWith("gnueabihf", Quadruple::GNUEABIHF)
    .StartsWith("gnueabi", Quadruple::GNUEABI)
    .StartsWith("gnux32", Quadruple::GNUX32)
    .StartsWith("gnu", Quadruple::GNU)
    .StartsWith("macho", Quadruple::MachO)
    .StartsWith("androideabi", Quadruple::AndroidEABI)
    .StartsWith("android", Quadruple::Android)
    .StartsWith("elf", Quadruple::ELF)
    .StartsWith("uff", Quadruple::UFF)
    .StartsWith("bmnet", Quadruple::BMNet)
    .Default(Quadruple::UnknownEnvironment);
}

Quadruple::ToolType onnc::ParseTool(StringRef pToolName)
{
  return StringSwitch<Quadruple::ToolType>(pToolName)
    .Case("all", Quadruple::ALL)
    .Case("ar", Quadruple::AR)
    .Cases("as", "gas", Quadruple::AS)
    .Case("cc1", Quadruple::CC)
    .Case("cc1plus", Quadruple::CXX)
    .Case("cpp", Quadruple::CPP)
    .Cases("gcc", "g++", Quadruple::DRIVER)
    .Cases("cc", "c++", Quadruple::DRIVER)
    .Cases("clang", "clang++", Quadruple::DRIVER)
    .Case("elfedit", Quadruple::ELFEDIT)
    .Cases("ld.bfd", "ld.gold", "ld.mcld", "ld", Quadruple::LD)
    .Cases("lto", "lto1", Quadruple::LD)
    .Case("ranlib", Quadruple::RANLIB)
    .Case("strip", Quadruple::STRIP)
    .Case("wrapper", Quadruple::WRAPPER)
    .Default(Quadruple::UnknownTool);
}

Quadruple::ToolVendorType onnc::ParseToolVendor(StringRef pToolVendorName)
{
  return StringSwitch<Quadruple::ToolVendorType>(pToolVendorName)
    .StartsWith("caf", Quadruple::CAF)
    .StartsWith("fsf", "gnu", Quadruple::FSF)
    .StartsWith("google", "aosp", Quadruple::Google)
    .StartsWith("llvm", Quadruple::LLVM)
    .StartsWith("linaro", Quadruple::Linaro)
    .StartsWith("onnc", Quadruple::Skymizer)
    .StartsWith("sourcery", Quadruple::Sourcery)
    .StartsWith("apple", "xcode", Quadruple::XCode)
    .StartsWith("brew", "homebrew", Quadruple::Homebrew)
    .StartsWith("macport", Quadruple::MacPort)
    .StartsWith("redhat", Quadruple::Redhat)
    .StartsWith("fedora", Quadruple::Fedora)
    .StartsWith("centos", Quadruple::CentOS)
    .StartsWith("ubuntu", Quadruple::Ubuntu)
    .StartsWith("debian", Quadruple::Debian)
    .StartsWith("skymizer", Quadruple::Skymizer)
    .StartsWith("bitmain", Quadruple::Skymizer)
    .Default(Quadruple::UnknownToolVendor);
}

Quadruple::ArchCoreType onnc::ParseArchCore(StringRef pArchCoreName)
{
  return StringSwitch<Quadruple::ArchCoreType>(pArchCoreName)
    .Cases("ca7", "cortexa7", Quadruple::ARMArchCore_CortexA7)
    .Cases("ca8", "cortexa8", Quadruple::ARMArchCore_CortexA8)
    .Cases("ca9", "cortexa9", Quadruple::ARMArchCore_CortexA9)
    .Cases("ca12", "cortexa12", Quadruple::ARMArchCore_CortexA12)
    .Cases("ca15", "cortexa15", Quadruple::ARMArchCore_CortexA15)
    .Cases("ca53", "cortexa15", Quadruple::ARMArchCore_CortexA53)
    .Cases("ca57", "cortexa15", Quadruple::ARMArchCore_CortexA57)
    .Cases("ca72", "cortexa15", Quadruple::ARMArchCore_CortexA72)
    .Cases("ca73", "cortexa15", Quadruple::ARMArchCore_CortexA73)
    .Case("tg", Quadruple::SophonArchCore_TG)
    .Default(Quadruple::UnknownArchCore);
}

//===----------------------------------------------------------------------===//
// Getting Methods
//===----------------------------------------------------------------------===//
StringRef onnc::FetchArchName(StringRef pName)
{
  // The 1st component
  return pName.split('-').first;
}

StringRef onnc::FetchArchVendorName(StringRef pName)
{
  // The 2nd component
  StringRef tmp = pName.split('-').second; // strip 1st component
  return tmp.split('-').first;             // isolate 2nd component
}

StringRef onnc::FetchOSName(StringRef pName)
{
  // The 3rd component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  return tmp.split('-').first;             // isolate 3rd component
}

StringRef onnc::FetchEnvironmentName(StringRef pName)
{
  // The 4th component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  tmp = tmp.split('-').second;             // strip 3rd component
  return tmp.split('-').first;             // isolate 4th component
}

StringRef onnc::FetchToolName(StringRef pName)
{
  // The 5th component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  tmp = tmp.split('-').second;             // strip 3rd component
  tmp = tmp.split('-').second;             // strip 4th component
  return tmp.split('-').first;             // isolate 5th component
}

StringRef onnc::FetchToolVersion(StringRef pName)
{
  // The 6th component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  tmp = tmp.split('-').second;             // strip 3rd component
  tmp = tmp.split('-').second;             // strip 4th component
  tmp = tmp.split('-').second;             // strip 5th component
  return tmp.split('-').first;             // isolate 6th component
}

StringRef onnc::FetchToolVendorName(StringRef pName)
{
  // The 7th component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  tmp = tmp.split('-').second;             // strip 3rd component
  tmp = tmp.split('-').second;             // strip 4th component
  tmp = tmp.split('-').second;             // strip 5th component
  tmp = tmp.split('-').second;             // strip 6th component
  return tmp.split('-').first;             // isolate 7th component
}

StringRef onnc::FetchArchCoreName(StringRef pName)
{
  // The 8th component
  StringRef tmp = pName.split('-').second; // strip 1st component
  tmp = tmp.split('-').second;             // strip 2nd component
  tmp = tmp.split('-').second;             // strip 3rd component
  tmp = tmp.split('-').second;             // strip 4th component
  tmp = tmp.split('-').second;             // strip 5th component
  tmp = tmp.split('-').second;             // strip 6th component
  tmp = tmp.split('-').second;             // strip 7th component
  return tmp.split('-').first;             // isolate 8th component
}

//===----------------------------------------------------------------------===//
// Turn Methods
//===----------------------------------------------------------------------===//
const char*
onnc::ArchToName(Quadruple::ArchType pType, Quadruple::SubArchType pSubType)
{
  switch (pType) {
    case Quadruple::arm: {
      switch (pSubType) {
        case Quadruple::ARMSubArch_v8:   return "armv8";
        case Quadruple::ARMSubArch_v7:   return "armv7";
        case Quadruple::ARMSubArch_v7em: return "armv7em"; 
        case Quadruple::ARMSubArch_v7m:  return "armv7m";
        case Quadruple::ARMSubArch_v7s:  return "armv7s";
        case Quadruple::ARMSubArch_v6:   return "armv6";
        case Quadruple::ARMSubArch_v6m:  return "armv6m";
        case Quadruple::ARMSubArch_v6t2: return "armv6t2";
        case Quadruple::ARMSubArch_v5:   return "armv5";
        case Quadruple::ARMSubArch_v5te: return "armv5te";
        case Quadruple::ARMSubArch_v4t:  return "armv4t";
        case Quadruple::SophonSubArch_vBM1680: return "sophonv1680";
        case Quadruple::SophonSubArch_vBM1682: return "sophonv1682";
        case Quadruple::SophonSubArch_vBM1880: return "sophonv1880";
        case Quadruple::NoSubArch:
        default: return "arm";
      }
    }
    case Quadruple::aarch64:     return "aarch64";
    case Quadruple::hexagon:     return "hexagon";
    case Quadruple::mips:        return "mips";
    case Quadruple::mipsel:      return "mipsel";
    case Quadruple::mips64:      return "mips64";
    case Quadruple::mips64el:    return "mips64el";
    case Quadruple::msp430:      return "msp430";
    case Quadruple::ppc:         return "ppc";
    case Quadruple::ppc64:       return "ppc64";
    case Quadruple::r600:        return "r600";
    case Quadruple::sparc:       return "sparc";
    case Quadruple::sparcv9:     return "sparcv9";
    case Quadruple::systemz:     return "systemz";
    case Quadruple::tce:         return "tce";
    case Quadruple::thumb:       return "thumb";
    case Quadruple::x32:         return "x32";
    case Quadruple::x86:         return "x86";
    case Quadruple::x86_64:      return "x86_64";
    case Quadruple::xcore:       return "xcore";
    case Quadruple::mblaze:      return "mblaze";
    case Quadruple::nvptx:       return "nvptx";
    case Quadruple::nvptx64:     return "nvptx64";
    case Quadruple::le32:        return "le32";
    case Quadruple::amdil:       return "amdil";
    case Quadruple::spir:        return "spir";
    case Quadruple::spir64:      return "spir64";
    case Quadruple::sophon:      return "sophon";

    case Quadruple::UnknownArch:
    default: return "unknown";
  }
  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::ArchVendorToName(Quadruple::ArchVendorType pType)
{
  switch (pType) {
    case Quadruple::Apple:             return "apple";
    case Quadruple::PC:                return "pc";
    case Quadruple::SCEI:              return "scei";
    case Quadruple::BGP:               return "bgp";
    case Quadruple::BGQ:               return "bgq";
    case Quadruple::Freescale:         return "freescale";
    case Quadruple::IBM:               return "ibm";
    case Quadruple::BITMAIN:           return "bitmain";

    case Quadruple::UnknownArchVendor:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::OSToName(Quadruple::OSType pType)
{
  switch (pType) {
    case Quadruple::AuroraUX:  return "auroraux";
    case Quadruple::Cygwin:    return "cygwin";
    case Quadruple::Darwin:    return "darwin";
    case Quadruple::DragonFly: return "dragonfly";
    case Quadruple::FreeBSD:   return "freebsd";
    case Quadruple::IOS:       return "ios";
    case Quadruple::KFreeBSD:  return "kfreebsd";
    case Quadruple::Linux:     return "linux";
    case Quadruple::Lv2:       return "lv2";
    case Quadruple::MacOSX:    return "macosx";
    case Quadruple::MinGW32:   return "mingw32";
    case Quadruple::NetBSD:    return "netbsd";
    case Quadruple::OpenBSD:   return "openbsd";
    case Quadruple::Solaris:   return "solaris";
    case Quadruple::Win32:     return "win32";
    case Quadruple::Haiku:     return "haiku";
    case Quadruple::Minix:     return "minix";
    case Quadruple::RTEMS:     return "rtems";
    case Quadruple::NaCl:      return "nacl";
    case Quadruple::CNK:       return "cnk";
    case Quadruple::Bitrig:    return "bitrig";
    case Quadruple::AIX:       return "aix";
    case Quadruple::NDK:       return "ndk";

    case Quadruple::UnknownOS:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::EnvironmentToName(Quadruple::EnvironmentType pType)
{
  switch (pType) {
    case Quadruple::GNU:                return "gnu";
    case Quadruple::GNUEABI:            return "gnueabi";
    case Quadruple::GNUEABIHF:          return "gnueabihf";
    case Quadruple::GNUX32:             return "gnux32";
    case Quadruple::EABI:               return "eabi";
    case Quadruple::MachO:              return "macho";
    case Quadruple::Android:            return "android";
    case Quadruple::AndroidEABI:        return "androideabi";
    case Quadruple::ELF:                return "elf";
    case Quadruple::UFF:                return "uff";
    case Quadruple::BMNet:              return "bmnet";

    case Quadruple::UnknownEnvironment:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::ToolToName(Quadruple::ToolType pType)
{
  switch (pType) {
    case Quadruple::ALL:         return "all";
    case Quadruple::AR:          return "ar";
    case Quadruple::AS:          return "as";
    case Quadruple::CC:          return "cc";
    case Quadruple::CPP:         return "cpp";
    case Quadruple::CXX:         return "cxx";
    case Quadruple::ELFEDIT:     return "elfedit";
    case Quadruple::LD:          return "ld";
    case Quadruple::RANLIB:      return "ranlib";
    case Quadruple::STRIP:       return "strip";
    case Quadruple::WRAPPER:     return "wrapper";

    case Quadruple::DRIVER: //< Unable to decide the driver name.
    case Quadruple::UnknownTool:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::ToolVendorToName(Quadruple::ToolVendorType pType)
{
  switch (pType) {
    case Quadruple::FSF:               return "fsf";
    case Quadruple::Google:            return "google";
    case Quadruple::LLVM:              return "llvm";
    case Quadruple::Linaro:            return "linaro";
    case Quadruple::Skymizer:          return "skymizer";
    case Quadruple::Sourcery:          return "sourcery";
    case Quadruple::XCode:             return "xcode";
    case Quadruple::Homebrew:          return "brew";
    case Quadruple::MacPort:           return "macport";
    case Quadruple::Redhat:            return "redhat";
    case Quadruple::Fedora:            return "fedora";
    case Quadruple::CentOS:            return "centos";
    case Quadruple::Ubuntu:            return "ubuntu";
    case Quadruple::Debian:            return "debian";

    case Quadruple::UnknownToolVendor:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

const char* onnc::ArchCoreToName(Quadruple::ArchCoreType pType)
{
  switch (pType) {
    case Quadruple::ARMArchCore_CortexA7: return "ca7";
    case Quadruple::ARMArchCore_CortexA8: return "ca8";
    case Quadruple::ARMArchCore_CortexA9: return "ca9";
    case Quadruple::ARMArchCore_CortexA12: return "ca12";
    case Quadruple::ARMArchCore_CortexA15: return "ca15";
    case Quadruple::ARMArchCore_CortexA53: return "ca53";
    case Quadruple::ARMArchCore_CortexA57: return "ca57";
    case Quadruple::ARMArchCore_CortexA72: return "ca72";
    case Quadruple::ARMArchCore_CortexA73: return "ca73";
    case Quadruple::SophonArchCore_TG: return "tg";

    case Quadruple::UnknownArchCore:
    default: return "unknown";
  }

  // FIXME: add unreachable here
  assert(false && "Unreachable!");
  return NULL;
}

//===----------------------------------------------------------------------===//
// Static Methods
//===----------------------------------------------------------------------===//
static unsigned getArchBitWidth(Quadruple::ArchType pArch)
{
  switch (pArch) {
  case Quadruple::UnknownArch:
    return 0;

  case Quadruple::msp430:
    return 16;

  case Quadruple::amdil:
  case Quadruple::arm:
  case Quadruple::hexagon:
  case Quadruple::le32:
  case Quadruple::mblaze:
  case Quadruple::mips:
  case Quadruple::mipsel:
  case Quadruple::nvptx:
  case Quadruple::ppc:
  case Quadruple::r600:
  case Quadruple::sparc:
  case Quadruple::tce:
  case Quadruple::thumb:
  case Quadruple::x86:
  case Quadruple::xcore:
  case Quadruple::spir:
  case Quadruple::sophon:
    return 32;

  case Quadruple::aarch64:
  case Quadruple::mips64:
  case Quadruple::mips64el:
  case Quadruple::nvptx64:
  case Quadruple::ppc64:
  case Quadruple::sparcv9:
  case Quadruple::systemz:
  case Quadruple::x32:
  case Quadruple::x86_64:
  case Quadruple::spir64:
    return 64;
  }
  // FIXME: use unreachable
}
static unsigned getPointerBitWidth(Quadruple::ArchType pArch)
{
  switch (pArch) {
  case Quadruple::UnknownArch:
    return 0;

  case Quadruple::msp430:
    return 16;

  case Quadruple::amdil:
  case Quadruple::arm:
  case Quadruple::hexagon:
  case Quadruple::le32:
  case Quadruple::mblaze:
  case Quadruple::mips:
  case Quadruple::mipsel:
  case Quadruple::nvptx:
  case Quadruple::ppc:
  case Quadruple::r600:
  case Quadruple::sparc:
  case Quadruple::tce:
  case Quadruple::thumb:
  case Quadruple::x32:
  case Quadruple::x86:
  case Quadruple::xcore:
  case Quadruple::spir:
  case Quadruple::sophon:
    return 32;

  case Quadruple::aarch64:
  case Quadruple::mips64:
  case Quadruple::mips64el:
  case Quadruple::nvptx64:
  case Quadruple::ppc64:
  case Quadruple::sparcv9:
  case Quadruple::systemz:
  case Quadruple::x86_64:
  case Quadruple::spir64:
    return 64;
  }
  // FIXME: use unreachable
}

//===----------------------------------------------------------------------===//
// Quadruple
//===----------------------------------------------------------------------===//
Quadruple::Quadruple()
  : m_Raw(), m_Arch(), m_SubArch(), m_ArchVendor(), m_OS(), m_Environment(),
    m_Tool(), m_ToolVersion(), m_ToolVendor(), m_ArchCore() {
}

Quadruple::Quadruple(const Quadruple& pCopy)
  : m_Raw(pCopy.m_Raw),
    m_Arch(pCopy.m_Arch),
    m_SubArch(pCopy.m_SubArch),
    m_ArchVendor(pCopy.m_ArchVendor),
    m_OS(pCopy.m_OS),
    m_Environment(pCopy.m_Environment),
    m_Tool(pCopy.m_Tool),
    m_ToolVersion(pCopy.m_ToolVersion),
    m_ToolVendor(pCopy.m_ToolVendor),
    m_ArchCore(pCopy.m_ArchCore) {
}

Quadruple::Quadruple(const std::string& pStr)
  : m_Raw(pStr),
    m_Arch(ParseArch(FetchArchName(pStr))),
    m_SubArch(ParseSubArch(FetchArchName(pStr))),
    m_ArchVendor(ParseArchVendor(FetchArchVendorName(pStr))),
    m_OS(ParseOS(FetchOSName(pStr))),
    m_Environment(ParseEnvironment(FetchEnvironmentName(pStr))),
    m_Tool(ParseTool(FetchToolName(pStr))),
    m_ToolVersion(FetchToolVersion(pStr)),
    m_ToolVendor(ParseToolVendor(FetchToolVendorName(pStr))),
    m_ArchCore(ParseArchCore(FetchArchCoreName(pStr))) {
}

Quadruple::Quadruple(const char* pStr)
  : m_Raw(pStr),
    m_Arch(ParseArch(FetchArchName(pStr))),
    m_SubArch(ParseSubArch(FetchArchName(pStr))),
    m_ArchVendor(ParseArchVendor(FetchArchVendorName(pStr))),
    m_OS(ParseOS(FetchOSName(pStr))),
    m_Environment(ParseEnvironment(FetchEnvironmentName(pStr))),
    m_Tool(ParseTool(FetchToolName(pStr))),
    m_ToolVersion(FetchToolVersion(pStr)),
    m_ToolVendor(ParseToolVendor(FetchToolVendorName(pStr))),
    m_ArchCore(ParseArchCore(FetchArchCoreName(pStr))) {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(),
    m_Tool(),
    m_ToolVersion(),
    m_ToolVendor(),
    m_ArchCore() {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr,
                     StringRef pEnvironStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr +
           Rope('-') + pEnvironStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(ParseEnvironment(pEnvironStr)),
    m_Tool(),
    m_ToolVersion(),
    m_ToolVendor(),
    m_ArchCore() {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr,
                     StringRef pEnvironStr,
                     StringRef pToolStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr +
           Rope('-') + pEnvironStr + Rope('-') + pToolStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(ParseEnvironment(pEnvironStr)),
    m_Tool(ParseTool(pToolStr)),
    m_ToolVersion(),
    m_ToolVendor(),
    m_ArchCore() {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr,
                     StringRef pEnvironStr,
                     StringRef pToolStr,
                     StringRef pToolVersionStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr +
           Rope('-') + pEnvironStr + Rope('-') + pToolStr + Rope('-') +
           pToolVersionStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(ParseEnvironment(pEnvironStr)),
    m_Tool(ParseTool(pToolStr)),
    m_ToolVersion(pToolVersionStr),
    m_ToolVendor(),
    m_ArchCore() {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr,
                     StringRef pEnvironStr,
                     StringRef pToolStr,
                     StringRef pToolVersionStr,
                     StringRef pToolVendorStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr +
           Rope('-') + pEnvironStr + Rope('-') + pToolStr + Rope('-') +
           pToolVersionStr + Rope('-') + pToolVendorStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(ParseEnvironment(pEnvironStr)),
    m_Tool(ParseTool(pToolStr)),
    m_ToolVersion(pToolVersionStr),
    m_ToolVendor(ParseToolVendor(pToolVendorStr)),
    m_ArchCore() {
}

Quadruple::Quadruple(StringRef pArchStr,
                     StringRef pArchVendorStr,
                     StringRef pOSStr,
                     StringRef pEnvironStr,
                     StringRef pToolStr,
                     StringRef pToolVersionStr,
                     StringRef pToolVendorStr,
                     StringRef pArchCoreStr)
  : m_Raw((pArchStr + Rope('-') + pArchVendorStr + Rope('-') + pOSStr +
           Rope('-') + pEnvironStr + Rope('-') + pToolStr + Rope('-') +
           pToolVersionStr + Rope('-') + pToolVendorStr + Rope('-') +
           pArchCoreStr).str()),
    m_Arch(ParseArch(pArchStr)),
    m_SubArch(ParseSubArch(pArchStr)),
    m_ArchVendor(ParseArchVendor(pArchVendorStr)),
    m_OS(ParseOS(pOSStr)),
    m_Environment(ParseEnvironment(pEnvironStr)),
    m_Tool(ParseTool(pToolStr)),
    m_ToolVersion(pToolVersionStr),
    m_ToolVendor(ParseToolVendor(pToolVendorStr)),
    m_ArchCore(ParseArchCore(pArchCoreStr)) {
}

Quadruple& Quadruple::operator=(const Quadruple& pCopy)
{
  m_Raw         = pCopy.m_Raw;
  m_Arch        = pCopy.m_Arch;
  m_SubArch     = pCopy.m_SubArch;
  m_ArchVendor  = pCopy.m_ArchVendor;
  m_OS          = pCopy.m_OS;
  m_Environment = pCopy.m_Environment;
  m_Tool        = pCopy.m_Tool;
  m_ToolVersion = pCopy.m_ToolVersion;
  m_ToolVendor  = pCopy.m_ToolVendor;
  m_ArchCore    = pCopy.m_ArchCore;
  return *this;
}

void Quadruple::setRaw(const std::string& pRaw)
{
  m_Raw         = pRaw;
  m_Arch        = ParseArch(FetchArchName(pRaw));
  m_SubArch     = ParseSubArch(FetchArchName(pRaw));
  m_ArchVendor  = ParseArchVendor(FetchArchVendorName(pRaw));
  m_OS          = ParseOS(FetchOSName(pRaw));
  m_Environment = ParseEnvironment(FetchEnvironmentName(pRaw));
  m_Tool        = ParseTool(FetchToolName(pRaw));
  m_ToolVersion = FetchToolVersion(pRaw);
  m_ToolVendor  = ParseToolVendor(FetchToolVendorName(pRaw));
  m_ArchCore    = ParseArchCore(FetchArchCoreName(pRaw));
}

void Quadruple::canonical(std::string& pResult) const
{
  pResult = (ArchToName(getArch()) + Rope('-') +
             ArchVendorToName(getArchVendor()) + Rope('-') +
             OSToName(getOS()) + Rope('-') +
             EnvironmentToName(getEnvironment()) + Rope('-') +
             getToolName() + Rope('-') + //< use raw value if exists
             getToolVersion() + Rope('-') +
             ToolVendorToName(getToolVendor()) + Rope('-') +
             ArchCoreToName(getArchCore())).str();
}

StringRef Quadruple::getArchName() const
{
  if ((UnknownArch != getArch()) && hasRaw())
    return FetchArchName(m_Raw);
  return ArchToName(getArch(), getSubArch());
}

StringRef Quadruple::getArchVendorName() const
{
  if ((UnknownArchVendor != getArchVendor()) && hasRaw())
    return FetchArchVendorName(m_Raw);
  return ArchVendorToName(getArchVendor());
}

StringRef Quadruple::getOSName() const
{
  if ((UnknownOS != getOS()) && hasRaw())
    return FetchOSName(m_Raw);
  return OSToName(getOS());
}

StringRef Quadruple::getEnvironmentName() const
{
  if ((UnknownEnvironment != getEnvironment()) && hasRaw())
    return FetchEnvironmentName(m_Raw);
  return EnvironmentToName(getEnvironment());
}

StringRef Quadruple::getToolName() const
{
  if ((UnknownTool != getTool()) && hasRaw())
    return FetchToolName(m_Raw);
  return ToolToName(getTool());
}

StringRef Quadruple::getToolVendorName() const
{
  if ((UnknownToolVendor != getToolVendor()) && hasRaw())
    return FetchToolVendorName(m_Raw);
  return ToolVendorToName(getToolVendor());
}

StringRef Quadruple::getArchCoreName() const
{
  if ((UnknownArchCore != getArchCore()) && hasRaw())
    return FetchArchCoreName(m_Raw);
  return ArchCoreToName(getArchCore());
}

bool Quadruple::isArch64Bit() const
{
  return (64 == getArchBitWidth(getArch()));
}

bool Quadruple::isArch32Bit() const
{
  return (32 == getArchBitWidth(getArch()));
}

bool Quadruple::isArch16Bit() const
{
  return (16 == getArchBitWidth(getArch()));
}

bool Quadruple::isPointer64Bit() const
{
  return (64 == getPointerBitWidth(getArch()));
}

bool Quadruple::isPointer32Bit() const
{
  return (32 == getPointerBitWidth(getArch()));
}

bool Quadruple::isPointer16Bit() const
{
  return (16 == getPointerBitWidth(getArch()));
}

//===----------------------------------------------------------------------===//
// The rest non-member functions
//===----------------------------------------------------------------------===//
bool onnc::operator==(const Quadruple& pX, const Quadruple& pY)
{
  return (pX.getRaw() == pY.getRaw());
}

bool onnc::operator!=(const Quadruple& pX, const Quadruple& pY)
{
  return (pX.getRaw() != pY.getRaw());
}
