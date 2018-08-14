//===- StringHasher.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This file collects string hash functions
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRING_HASHER_H
#define ONNC_ADT_STRING_HASHER_H
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/DataTypes.h>
#include <cassert>
#include <cctype>
#include <cstring>
#include <functional>

namespace onnc {
namespace internal {

enum StringHasherType {
  RS,
  JS,
  PJW,
  ELF,
  BKDR,
  SDBM,
  DJB,
  DEK,
  BP,
  FNV,
  AP,
  ES
};

/** \class StringHasher
 *  \brief the template StringHasher class, for specification
 */
template<uint32_t NAME>
struct StringHasher : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    assert(false && "Undefined StringHasher function.\n");
    return 0;
  }
};

/** \class StringHasher<RS>
 *  \brief RS StringHasher function
 */
template<>
struct StringHasher<RS> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    const unsigned int b = 378551;
    uint32_t a = 63689;
    uint32_t hash_val = 0;

    for(unsigned int i = 0; i < pKey.size(); ++i) {
      hash_val = hash_val * a + pKey[i];
      a = a * b;
    }
    return hash_val;
  }
};

/** \class StringHasher<JS>
 *  \brief JS hash function
 */
template<>
struct StringHasher<JS> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = 1315423911;

    for(unsigned int i = 0; i < pKey.size(); ++i) {
       hash_val ^= ((hash_val << 5) + pKey[i] + (hash_val >> 2));
    } 
    return hash_val;
  }
};

/** \class StringHasher<PJW>
 *  \brief P.J. Weinberger hash function
 */
template<>
struct StringHasher<PJW> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    uint32_t hash_val = 0;
    uint32_t test = 0;

    for(unsigned int i = 0; i < pKey.size(); ++i) {
      hash_val = (hash_val << OneEighth) + pKey[i];

      if((test = hash_val & HighBits) != 0) {
        hash_val = (( hash_val ^ (test >> ThreeQuarters)) & (~HighBits));
      }
    }
    return hash_val;
  }
};

/** \class StringHasher<ELF>
 *  \brief ELF hash function.
 */
template<>
struct StringHasher<ELF> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = 0;
    uint32_t x = 0;

    for (unsigned int i = 0; i < pKey.size(); ++i) {
      hash_val = (hash_val << 4) + pKey[i];
      if((x = hash_val & 0xF0000000L) != 0)
        hash_val ^= (x >> 24); 
      hash_val &= ~x;
    }
    return hash_val;
  }
};

/** \class StringHasher<BKDR>
 *  \brief BKDR hash function
 */
template<>
struct StringHasher<BKDR> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    const uint32_t seed = 131;
    uint32_t hash_val = 0;
      
    for(uint32_t i = 0; i < pKey.size(); ++i)
      hash_val = (hash_val * seed) + pKey[i];
    return hash_val;
  }
};


/** \class StringHasher<SDBM>
 *  \brief SDBM hash function
 *  0.049s in 100000 test
 */
template<>
struct StringHasher<SDBM> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = 0;

    for(uint32_t i = 0; i < pKey.size(); ++i)
      hash_val = pKey[i] + (hash_val << 6) + (hash_val << 16) - hash_val;
    return hash_val;
  }
};

/** \class StringHasher<DJB>
 *  \brief DJB hash function
 *  0.057s in 100000 test
 */
template<>
struct StringHasher<DJB> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = 5381;

    for(uint32_t i = 0; i < pKey.size(); ++i)
      hash_val = ((hash_val << 5) + hash_val) + pKey[i];

    return hash_val;
  }
};

/** \class StringHasher<DEK>
 *  \brief DEK hash function
 *  0.60s
 */
template<>
struct StringHasher<DEK> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = pKey.size();

    for(uint32_t i = 0; i < pKey.size(); ++i)
      hash_val = ((hash_val << 5) ^ (hash_val >> 27)) ^ pKey[i];

    return hash_val;
  }
};

/** \class StringHasher<BP>
 *  \brief BP hash function
 *  0.057s
 */
template<>
struct StringHasher<BP> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    uint32_t hash_val = 0;
    for(uint32_t i = 0; i < pKey.size(); ++i)
      hash_val = hash_val << 7 ^ pKey[i];

    return hash_val;
  }
};

/** \class StringHasher<FNV>
 *  \brief FNV hash function
 *  0.058s
 */
template<>
struct StringHasher<FNV> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    const uint32_t fnv_prime = 0x811C9DC5;
    uint32_t hash_val = 0;
    for(uint32_t i = 0; i < pKey.size(); ++i) {
      hash_val *= fnv_prime;
      hash_val ^= pKey[i];
    }

    return hash_val;
  }
};

/** \class StringHasher<AP>
 *  \brief AP hash function
 *  0.060s
 */
template<>
struct StringHasher<AP> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pKey) const
  {
    unsigned int hash_val = 0xAAAAAAAA;
   
    for(uint32_t i = 0; i < pKey.size(); ++i) {  
      hash_val ^= ((i & 1) == 0)?
                          ((hash_val <<  7) ^ pKey[i] * (hash_val >> 3)):
                          (~((hash_val << 11) + (pKey[i] ^ (hash_val >> 5))));
    }
   
    return hash_val;
  }
};

/** \class StringHasher<ES>
 *  \brief This is a revision of Edward Sayers' string characteristic function.
 *
 *  31-28  27  26  25   -   0
 *  +----+---+---+------------+
 *  | .  | N | - | a/A  ~ z/Z |
 *  +----+---+---+------------+
 *
 *  . (bit 31~28) - The number of '.' characters
 *  N (bit 27)    - Are there any numbers in the string
 *  - (bit 26)    - Does the string have '-' character
 *  bit 25~0      - Bit 25 is set only if the string contains a 'a' or 'A', and
 *                  Bit 24 is set only if ...                   'b' or 'B', ...
 */
template<>
struct StringHasher<ES> : public std::unary_function<const onnc::StringRef&, uint32_t>
{
  uint32_t operator()(const onnc::StringRef& pString) const
  {
    uint32_t result = 0x0;
    unsigned int dot = 0;
    std::string::size_type idx;
    for (idx = 0; idx < pString.size(); ++idx) {
      int cur_char = pString[idx];

      if ('.' == cur_char) {
        ++dot;
        continue;
      }

      if (isdigit(cur_char)) {
        result |= (1 << 27);
        continue;
      }

      if ('_' == cur_char) {
        result |= (1 << 26);
        continue;
      }

      if (isupper(cur_char)) {
        result |= (1 << (cur_char - 'A'));
        continue;
      }

      if (islower(cur_char)) {
        result |= (1 << (cur_char - 'a'));
        continue;
      }
    }
    result |= (dot << 28);
    return result;
  }


  /** \func may_include
   *  \brief is it possible that pRule is a sub-string of pInString
   */
  static bool may_include(uint32_t pRule, uint32_t pInString)
  {
    uint32_t in_c = pInString << 4;
    uint32_t r_c  = pRule << 4;

    uint32_t res = (in_c ^ r_c) & r_c;
    if (0 != res)
      return false;

    uint32_t in_dot = pInString >> 28;
    uint32_t r_dot  = pRule >> 28;
    if (r_dot > in_dot)
      return false;

    return true;
  }
};

/** \class StringCompare
 *  \brief the template StringCompare class, for specification
 */
template<typename STRING_TYPE>
struct StringCompare : public std::binary_function<const STRING_TYPE&, const STRING_TYPE&, bool>
{
  bool operator()(const STRING_TYPE& X, const STRING_TYPE& Y) const
  { return X == Y; }
};

template<>
struct StringCompare<const char*> : public std::binary_function<const char*, const char*, bool>
{
  bool operator()(const char* X, const char* Y) const
  { return (0 == std::strcmp(X, Y)); }
};

template<>
struct StringCompare<char*> : public std::binary_function<const char*, const char*, bool>
{
  bool operator()(const char* X, const char* Y) const
  { return (0 == std::strcmp(X, Y)); }
};

} // namespace of internal
} // namespace of onnc

#endif
