//===- TypeTraits.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TYPE_TRAITS_H
#define ONNC_TYPE_TRAITS_H

#include <onnc/Support/DataTypes.h>
#include <onnc/Support/ELF.h>
#include <cstdlib>

namespace onnc {

namespace dont_use
{
  // These two functions should never be used. They are helpers to
  // the is_class template below. They cannot be located inside
  // is_class because doing so causes at least GCC to think that
  // the value of the "value" enumerator is not constant. Placing
  // them out here (for some strange reason) allows the sizeof
  // operator against them to magically be constant. This is
  // important to make the is_class<T>::value idiom zero cost. it
  // evaluates to a constant 1 or 0 depending on whether the
  // parameter T is a class or not (respectively).
  template<typename T> char is_class_helper(void(T::*)());
  template<typename T> double is_class_helper(...);
} // dont_use namespace

template <typename T>
struct is_class
{
  static const bool value =
      sizeof(char) == sizeof(dont_use::is_class_helper<T>(0));
};

template<typename DataType>
struct NonConstTraits;

template<typename DataType>
struct ConstTraits
{
  typedef const DataType           value_type;
  typedef const DataType*          pointer;
  typedef const DataType&          reference;
  typedef size_t                   size_type;
  typedef ConstTraits<DataType>    const_traits;
  typedef NonConstTraits<DataType> nonconst_traits;

  template<typename T>
  struct rebind {
    typedef ConstTraits<T> other;
  };
};

template<typename DataType>
struct NonConstTraits
{
  typedef DataType                 value_type;
  typedef DataType*                pointer;
  typedef DataType&                reference;
  typedef size_t                   size_type;
  typedef ConstTraits<DataType>    const_traits;
  typedef NonConstTraits<DataType> nonconst_traits;

  template<typename T>
  struct rebind {
    typedef NonConstTraits<T> other;
  };
};

template<typename DataType>
struct ConstIteratorTraits
{
  typedef DataType                          value_type;
  typedef const DataType*                   pointer;
  typedef const DataType&                   reference;
  typedef size_t                            size_type;
  typedef ConstTraits<DataType>             const_traits;
  typedef NonConstTraits<DataType>          nonconst_traits;
  typedef typename DataType::const_iterator iterator;
};

template<typename DataType>
struct NonConstIteratorTraits
{
  typedef DataType                    value_type;
  typedef DataType*                   pointer;
  typedef DataType&                   reference;
  typedef size_t                      size_type;
  typedef ConstTraits<DataType>       const_traits;
  typedef NonConstTraits<DataType>    nonconst_traits;
  typedef typename DataType::iterator iterator;
};

template <size_t SIZE>
class ELFSizeTraits;

template <>
class ELFSizeTraits<32> {
 public:
  typedef onnc::ELF::Elf32_Addr Addr;  // Program address
  typedef onnc::ELF::Elf32_Off Off;    // File offset
  typedef onnc::ELF::Elf32_Half Half;
  typedef onnc::ELF::Elf32_Word Word;
  typedef onnc::ELF::Elf32_Sword Sword;

  typedef onnc::ELF::Elf32_Ehdr Ehdr;
  typedef onnc::ELF::Elf32_Shdr Shdr;
  typedef onnc::ELF::Elf32_Sym Sym;
  typedef onnc::ELF::Elf32_Rel Rel;
  typedef onnc::ELF::Elf32_Rela Rela;
  typedef onnc::ELF::Elf32_Phdr Phdr;
  typedef onnc::ELF::Elf32_Dyn Dyn;
};

template <>
class ELFSizeTraits<64> {
 public:
  typedef onnc::ELF::Elf64_Addr Addr;
  typedef onnc::ELF::Elf64_Off Off;
  typedef onnc::ELF::Elf64_Half Half;
  typedef onnc::ELF::Elf64_Word Word;
  typedef onnc::ELF::Elf64_Sword Sword;
  typedef onnc::ELF::Elf64_Xword Xword;
  typedef onnc::ELF::Elf64_Sxword Sxword;

  typedef onnc::ELF::Elf64_Ehdr Ehdr;
  typedef onnc::ELF::Elf64_Shdr Shdr;
  typedef onnc::ELF::Elf64_Sym Sym;
  typedef onnc::ELF::Elf64_Rel Rel;
  typedef onnc::ELF::Elf64_Rela Rela;
  typedef onnc::ELF::Elf64_Phdr Phdr;
  typedef onnc::ELF::Elf64_Dyn Dyn;
};

template <size_t Constraint>
uint64_t Align(uint64_t pAddress);

template <>
inline uint64_t Align<32>(uint64_t pAddress) {
  return (pAddress + 0x1F) & (~0x1F);
}

template <>
inline uint64_t Align<64>(uint64_t pAddress) {
  return (pAddress + 0x3F) & (~0x3F);
}

/// alignAddress - helper function to align an address with given alignment
/// constraint
///
/// @param pAddr - the address to be aligned
/// @param pAlignConstraint - the alignment used to align the given address
inline void alignAddress(uint64_t& pAddr, uint64_t pAlignConstraint) {
  if (pAlignConstraint != 0)
    pAddr = (pAddr + pAlignConstraint - 1) & ~(pAlignConstraint - 1);
}

class NullType;

struct EmptyType { };

#ifdef bswap16
#undef bswap16
#endif
#ifdef bswap32
#undef bswap32
#endif
#ifdef bswap64
#undef bswap64
#endif

/// bswap16 - byte swap 16-bit version
/// @ref binary utilities - elfcpp_swap
inline uint16_t bswap16(uint16_t pData) {
  return ((pData >> 8) & 0xFF) | ((pData & 0xFF) << 8);
}

/// bswap32 - byte swap 32-bit version
/// @ref elfcpp_swap
inline uint32_t bswap32(uint32_t pData) {
  return (((pData & 0xFF000000) >> 24) | ((pData & 0x00FF0000) >> 8) |
          ((pData & 0x0000FF00) << 8) | ((pData & 0x000000FF) << 24));
}

/// bswap64 - byte swap 64-bit version
/// @ref binary utilities - elfcpp_swap
inline uint64_t bswap64(uint64_t pData) {
  return (((pData & 0xFF00000000000000ULL) >> 56) |
          ((pData & 0x00FF000000000000ULL) >> 40) |
          ((pData & 0x0000FF0000000000ULL) >> 24) |
          ((pData & 0x000000FF00000000ULL) >> 8) |
          ((pData & 0x00000000FF000000ULL) << 8) |
          ((pData & 0x0000000000FF0000ULL) << 24) |
          ((pData & 0x000000000000FF00ULL) << 40) |
          ((pData & 0x00000000000000FFULL) << 56));
}

} // namespace of onnc

#endif
