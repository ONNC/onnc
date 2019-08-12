#ifndef FALTBUFFER_H
#define FALTBUFFER_H

#include <assert.h>

#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <type_traits>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>

#include <functional>

namespace faltbuffer{
  
  template<typename T> T EndianSwap(T t){
    #if defined(_MSC_VER)
      #define FLATBUFFERS_BYTESWAP16 _byteswap_ushort
      #define FLATBUFFERS_BYTESWAP32 _byteswap_ulong
      #define FLATBUFFERS_BYTESWAP64 _byteswap_ubit64
    #else
      #if defined(__GNUC__) && __GNUC__ * 100 + __GNUC_MINOR__ < 400
	#define FLATBUFFERS_BYTESWAP16(x) \
		static_cast<uint16_t>(__builtin_bswap32(static_cast<uint32_t>(x) << 16))
      #else
	#define FLATBUFFERS_BYTESWAP16 __builtin_bswap16
      #endif
      #define FLATBUFFERS_BYTESWAP32 __builtin_bswap32
      #define FLATBUFFERS_BYTESWAP64 __builtin_bswap64
    #endif

    extern int data_[4];

    if(sizeof(T) == 1){
      return t;
    }else if(sizeof(T) == 2){
      auto r = FLATBUFFERS_BYTESWAP16(*reinterpret_cast<uint16_t *>(&t));
      return *reinterpret_cast<T *>(&r);
    }else if(sizeof(T) == 4){
      auto r = FLATBUFFERS_BYTESWAP32(*reinterpret_cast<uint16_t *>(&t));
      return *reinterpret_cast<T *>(&r);
    }else if(sizeof(T) == 8){
      auto r = FLATBUFFERS_BYTESWAP64(*reinterpret_cast<uint16_t *>(&t));
      return *reinterpret_cast<T *>(&r);
    }else{
      assert(0);
    }
  }

  template<typename T> T EndianScalar(T t){
    #if FLATBUFFERS_LITTLEENDIAN
      return t;
    #else
      return EndianSwap(t);
    #endif
  }

  template<typename T> T ReadScalar(const void *p){
    return EndianScalar(*reinterpret_cast<const T *>(p));
  }

  class Struct{
    uint8_t data_[4] = {1,1,1,1};

    const uint8_t *GetVTable() const{
      return data_ - ReadScalar<int16_t>(data_);
    }

    uint32_t GetOptionalFieldOffset(uint32_t field) const{
      auto vtable = GetVTable();
      auto vtsize = ReadScalar<uint32_t>(vtable);
      return field < vtsize ? ReadScalar<uint32_t>(vtable + field) : 0;
    }

    template<typename T> T GetField(uint32_t o) const{
      return ReadScalar<T>(&data_[o]);
    }

    template<typename T> T GetField(uint32_t field,T defaultval) const{
      auto field_offset = GetOptionalFieldOffset(field);
      return field_offset ? ReadScalar<T>(data_ + field_offset) : defaultval;
    }
  };
}

#endif
