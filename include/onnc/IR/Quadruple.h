//===- Quadruple.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_QUADRUPLE_H
#define ONNC_IR_QUADRUPLE_H
#include <string>
#include <ostream>
#include <onnc/ADT/StringRef.h>

namespace onnc {

/** \class Quadruple
 *  \brief Rrepresentation of a target program model
 *
 *  Quadruple is used to identify an unique target program model. A target program
 *  model is a combination of hardware, software and toolchain models. A quadruple
 *  is a sequence of ID which are connected by '-'.
 *
 *  For example,
 *  \code
 *  Quadruple quadruple("arm-none-linux-gnueabi-gcc");
 *  \endcode
 *  is a legal quadruple.
 *
 *  Quadruple is an extension of LLVM llvm::Triple class. It contains additional
 *  tool, architecture and software information. The order of ID is:
 *  - Arch(Sub-Arch), the ISA
 *  - Arch Vendor, who provide the architecture
 *  - OS Type, running on what operating system
 *  - Environment Type, with what kind of ABI
 *  - Tool Type, produced by what tool
 *  - Tool Version, the version of the tool
 *  - Tool Vendor, the vendor of the tool
 *  - Arch Core, the micro-architecture
 */
class Quadruple
{
public:
  /// Architecture type.
  enum ArchType {
    UnknownArch, // Unkwnon architecture

    aarch64,     // AArch64: aarch64
    arm,         // ARM: arm, armv.*, xscale
    hexagon,     // Hexagon: hexagon
    mips,        // MIPS: mips, mipsallegrex
    mipsel,      // MIPSEL: mipsel, mipsallegrexel
    mips64,      // MIPS64: mips64
    mips64el,    // MIPS64EL: mips64el
    msp430,      // MSP430: msp430
    ppc,         // PPC: powerpc
    ppc64,       // PPC64: powerpc64, ppu
    r600,        // R600: AMD GPUs HD2XXX - HD6XXX
    sparc,       // Sparc: sparc
    sparcv9,     // Sparcv9: Sparcv9
    systemz,     // SystemZ: s390x
    tce,         // TCE (http://tce.cs.tut.fi/): tce
    thumb,       // Thumb: thumb, thumbv.*
    x32,         // Intel X32
    x86,         // X86: i[3-9]86
    x86_64,      // X86-64: amd64, x86_64
    xcore,       // XCore: xcore
    mblaze,      // MBlaze: mblaze
    nvptx,       // NVPTX: 32-bit
    nvptx64,     // NVPTX: 64-bit
    le32,        // le32: generic little-endian 32-bit CPU (PNaCl / Emscripten)
    amdil,       // amdil: amd IL
    spir,        // SPIR: standard portable IR for OpenCL 32-bit version
    spir64,      // SPIR: standard portable IR for OpenCL 64-bit version
    sophon,      // Sophon
    tg           // Sophon without opt
  };

  /// ISA or architecture version of the architecture type.
  enum SubArchType {
    NoSubArch,

    ARMSubArch_v8,
    ARMSubArch_v7,
    ARMSubArch_v7em,
    ARMSubArch_v7m,
    ARMSubArch_v7s,
    ARMSubArch_v6,
    ARMSubArch_v6m,
    ARMSubArch_v6t2,
    ARMSubArch_v5,
    ARMSubArch_v5te,
    ARMSubArch_v4t
  };

  /// Vendor of the architecture
  enum ArchVendorType {
    UnknownArchVendor,

    Apple,
    PC,
    SCEI,
    BGP,
    BGQ,
    Freescale,
    IBM,
    Renesas
  };

  /// Operating System
  enum OSType {
    UnknownOS,

    AuroraUX,
    Cygwin,
    Darwin,
    DragonFly,
    FreeBSD,
    IOS,
    KFreeBSD,
    Linux,
    Lv2,        // PS3
    MacOSX,
    MinGW32,    // i*86-pc-mingw32, *-w64-mingw32
    NetBSD,
    OpenBSD,
    Solaris,
    Win32,
    Haiku,
    Minix,
    RTEMS,
    NaCl,       // Native Client
    CNK,        // BG/P Compute-Node Kernel
    Bitrig,
    AIX,
    NDK         // Android Java Native Development Kit
  };

  /// ABI
  enum EnvironmentType {
    UnknownEnvironment,

    GNU,
    GNUEABI,
    GNUEABIHF,
    GNUX32,
    EABI,
    MachO,
    Android,
    AndroidEABI,
    ELF
  };

  /// Tool in toolchain or software model
  enum ToolType {
    UnknownTool,

    ALL,
    AR,
    AS,
    CC,
    CPP,
    CXX,
    DRIVER,
    ELFEDIT,
    LD,
    RANLIB,
    STRIP
  };

  /// Vendor of the toolchain
  enum ToolVendorType {
    UnknownToolVendor,

    CAF,       // Code Aurora Forum
    FSF,       // Free Software Foundation
    Google,    // Google
    LLVM,      // LLVM open source
    Linaro,    // Linaro
    Skymizer,  // Skymizer
    Sourcery,  // Code Sourcery
    XCode,     // XCode
    Homebrew,  // Homebrew
    MacPort,   // Mac Port
    Redhat,    // Redhat
    Fedora,    // Fedora
    CentOS,    // CentOS
    Ubuntu,    // Ubuntu
    Debian,    // Debian
    BITMAIN    // BITMAIN
  };

  /// Core name of the architecture
  enum ArchCoreType {
    UnknownArchCore,

    ARMArchCore_CortexA7,
    ARMArchCore_CortexA8,
    ARMArchCore_CortexA9,
    ARMArchCore_CortexA12,
    ARMArchCore_CortexA15,
    ARMArchCore_CortexA53,
    ARMArchCore_CortexA57,
    ARMArchCore_CortexA72,
    ARMArchCore_CortexA73
  };

public:
  /// @name Constructors
  /// @{
  Quadruple();

  Quadruple(const Quadruple& pOther);

  explicit Quadruple(const char* pStr);

  explicit Quadruple(const std::string& pStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr,
            StringRef pEnvironmentStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr,
            StringRef pEnvironmentStr,
            StringRef pToolStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr,
            StringRef pEnvironmentStr,
            StringRef pToolStr,
            StringRef pToolVersionStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr,
            StringRef pEnvironStr,
            StringRef pToolStr,
            StringRef pToolVersionStr,
            StringRef pToolVendorStr);

  Quadruple(StringRef pArchStr,
            StringRef pArchVendorStr,
            StringRef pOSStr,
            StringRef pEnvironStr,
            StringRef pToolStr,
            StringRef pToolVersionStr,
            StringRef pToolVendorStr,
            StringRef pArchCoreStr);
  /// @}

  Quadruple& operator=(const Quadruple& pQuadruple);

  ~Quadruple() { }

  /// @name Typed Component Access
  /// @{
  /// Get the data string
  const std::string& getRaw() const { return m_Raw; }

  /// Set up the native string and update all components.
  void setRaw(const std::string& pRaw);

  /// Check if the native string is set.
  bool hasRaw() const { return !m_Raw.empty(); }

  /// @retval true If @pRaw is equal to the quadruple
  bool equals(const std::string& pRaw) const { return (m_Raw == pRaw); }

  bool
  equals(const Quadruple& pOther) const { return m_Raw == pOther.getRaw(); }

  /// Get the canonical form of the quadruple
  void canonical(std::string& pResult) const;

  /// Get the parsed architecture type of this quadruple.
  ArchType getArch() const { return m_Arch; }

  /// Get the name of architecture
  StringRef getArchName() const;

  /// Set up architecture type
  void setArch(ArchType pType) { m_Arch = pType; }

  /// Get the parsed sub-architecture type of this quadruple.
  SubArchType getSubArch() const { return m_SubArch; }

  /// Set up sub-architecture type
  void setSubArch(SubArchType pType) { m_SubArch = pType; }

  /// Get the parsed architecture vendor type of this quadruple.
  ArchVendorType getArchVendor() const { return m_ArchVendor; }

  /// Get the name of architecture vendor
  StringRef getArchVendorName() const;

  /// Set up architecture vendor type
  void setArchVendor(ArchVendorType pType) { m_ArchVendor = pType; }

  /// Get the parsed operating system type of this quadruple.
  OSType getOS() const { return m_OS; }

  /// Get the name of operating system
  StringRef getOSName() const;

  /// Set up operating system type
  void setOS(OSType pType) { m_OS = pType; }

  /// Get the parsed environment type of this quadruple.
  EnvironmentType getEnvironment() const { return m_Environment; }

  /// Get the name of environment
  StringRef getEnvironmentName() const;

  /// Set up environment type
  void setEnvironment(EnvironmentType pType) { m_Environment = pType; }

  /// Get the parsed tool type of this quadruple.
  ToolType getTool() const { return m_Tool; }

  /// Get the name of tool
  StringRef getToolName() const;

  /// Set up tool type
  void setTool(ToolType pType) { m_Tool = pType; }

  /// Get the tool version component of the quadruple.
  StringRef getToolVersion() const { return m_ToolVersion; }

  /// Set up tool version
  void setToolVersion(const std::string& pVersion) { m_ToolVersion = pVersion; }

  /// Get the parsed tool vendor of this quadruple
  ToolVendorType getToolVendor() const { return m_ToolVendor; }

  /// Get the name of tool vendor
  StringRef getToolVendorName() const;

  /// Set up tool vendor
  void setToolVendor(ToolVendorType pType) { m_ToolVendor = pType; }

  /// Get the name of arhictecture model
  StringRef getArchModelName() const;

  /// Get the parsed architecture core of this quadruple.
  ArchCoreType getArchCore() const { return m_ArchCore; }

  /// Get the name of architecture core
  StringRef getArchCoreName() const;

  /// Set up architecture core
  void setArchCore(ArchCoreType pType) { m_ArchCore = pType; }

  /// @}

  /// @name Convenience Predicates
  /// @{

  /// Test whether the data-path width is 64-bit
  bool isArch64Bit() const;

  /// Test whether the data-path width is 32-bit
  bool isArch32Bit() const;

  /// Test whether the data-path width is 16-bit
  bool isArch16Bit() const;

  /// \brief Test whether the pointer width is 64-bit
  /// Note that some 64-bit architecture remains 32-bit pointer width, for
  /// example, Intel X32 is 64-bit architecture, but its pointer is 32-bit.
  bool isPointer64Bit() const;

  /// Test for 32-bit pointer width
  bool isPointer32Bit() const;

  /// Test for 16-bit pointer width
  bool isPointer16Bit() const;
  /// @}

  /// @name String operators
  /// @{
  bool empty() const { return m_Raw.empty(); }

  const char* c_str() const { return m_Raw.c_str(); }

  unsigned int size() const { return m_Raw.size(); }
  /// @}

private:
  /// The wrapped string
  std::string m_Raw;

  /// The parsed arch type.
  ArchType m_Arch;

  /// The parsed sub-architecture type
  SubArchType m_SubArch;

  /// The parsed vendor type.
  ArchVendorType m_ArchVendor;

  /// The parsed OS type.
  OSType m_OS;

  /// The parsed environment type.
  EnvironmentType m_Environment;

  /// The parsed tool type.
  ToolType m_Tool;

  /// The parsed version
  std::string m_ToolVersion;

  /// The parsed tool vendor type.
  ToolVendorType m_ToolVendor;

  /// The parsed architecture core
  ArchCoreType m_ArchCore;
};

//===----------------------------------------------------------------------===//
// Parsing Methods
//===----------------------------------------------------------------------===//
/// Returns corresponding architecture type by \p pArchName.
Quadruple::ArchType ParseArch(StringRef pArchName);

/// Returns corresponding sub-architecture type by \p pSubArchName.
Quadruple::SubArchType ParseSubArch(StringRef pSubArchName);

/// Returns corresponding architecture vendor by \p pArchVendorName.
Quadruple::ArchVendorType ParseArchVendor(StringRef pArchVendorName);

/// Returns corresponding operating system by \p pOSName.
Quadruple::OSType ParseOS(StringRef pOSName);

/// Returns corresponding environment type by \p pEnvironmentName.
Quadruple::EnvironmentType ParseEnvironment(StringRef pEnvironmentName);

/// Returns corresponding tool type by \p pToolName.
Quadruple::ToolType ParseTool(StringRef pToolName);

/// Returns corresponding tool vendor type by \p pToolVendorName.
Quadruple::ToolVendorType ParseToolVendor(StringRef pToolVendorName);

Quadruple::ArchCoreType ParseArchCore(StringRef pArchCoreName);

//===----------------------------------------------------------------------===//
// Fetching Methods
//===----------------------------------------------------------------------===//
/// Returns the 1st component of pName. First component includes sub-
/// architecture.
StringRef FetchArchName(StringRef pName);

/// Returns the 2nd compoenet of pName.
StringRef FetchArchVendorName(StringRef pName);

/// Returns the 3rd component of pName
StringRef FetchOSName(StringRef pName);

/// Returns the 4th component of pName
StringRef FetchEnvironmentName(StringRef pName);

/// Returns the 5th component of pName
StringRef FetchToolName(StringRef pName);

/// Returns the 6th component of pName
StringRef FetchToolVersion(StringRef pName);

/// Returns the 7th component of pName
StringRef FetchToolVendorName(StringRef pName);

/// Returns the 8nd component of pName
StringRef FetchArchModelName(StringRef pName);

/// Returns the 9th component of pName
StringRef FetchArchCoreName(StringRef pName);

//===----------------------------------------------------------------------===//
// Turn Methods
//===----------------------------------------------------------------------===//
const char* ArchToName(Quadruple::ArchType pType,
                       Quadruple::SubArchType pSubType = Quadruple::NoSubArch);

const char* ArchVendorToName(Quadruple::ArchVendorType pType);

const char* OSToName(Quadruple::OSType pType);

const char* EnvironmentToName(Quadruple::EnvironmentType pType);

const char* ToolToName(Quadruple::ToolType pType);

const char* ToolVendorToName(Quadruple::ToolVendorType pType);

const char* ArchCoreToName(Quadruple::ArchCoreType pType);

/// @}

//===----------------------------------------------------------------------===//
// The rest non-member functions
//===----------------------------------------------------------------------===//
bool operator==(const Quadruple& pX, const Quadruple& pY);
bool operator!=(const Quadruple& pX, const Quadruple& pY);

} // namespace of onnc

namespace std {

inline ostream&
operator<<(ostream& pOS, const onnc::Quadruple& pQuadruple)
{
  pOS << pQuadruple.getRaw();
  return pOS;
}

} // namespace of std

#endif
