#ifndef VANILLA_MYLOADABLE_LOADABLE_GENERATED_H
#define VANILLA_MYLOADABLE_LOADABLE_GENERATED_H

#include <onnc/Support/IOStream.h>
//#include "faltbuffer.h"

typedef struct vanilla_weight{
  uint8_t type;
  const void* values;
  int64_t count;
}vanilla_weight;

namespace cortexm_loadable{
namespace loadable{
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
      //assert(0);
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
 
  static uint8_t data_[4] = {1,1,1,1};

  static const uint8_t *GetVTable() {
    return data_ - ReadScalar<int16_t>(data_);
  }

  static uint32_t GetOptionalFieldOffset(uint32_t field) {
    auto vtable = GetVTable();
    auto vtsize = ReadScalar<uint32_t>(vtable);
    return field < vtsize ? ReadScalar<uint32_t>(vtable + field) : 0;
  }

  template<typename T> T GetField(uint32_t o) {
    return ReadScalar<T>(&data_[o]);
  }

  template<typename T> T GetField(uint32_t field,T defaultval) {
    auto field_offset = GetOptionalFieldOffset(field);
    return field_offset ? ReadScalar<T>(data_ + field_offset) : defaultval;
  }

  struct Blob;

  enum Interface{
    Interface_NONE = 0,
    Interface_DLA1 = 1,
    Interface_EMU1 = 2,
    Interface_MIN = Interface_NONE,
    Interface_MAX = Interface_EMU1
  };

  struct Blob{
    enum{
      VT_NAME = 4,
      VT_SIZE = 6,
      VT_INTERFACE = 8,
      VT_SUB_INTERFACE = 10,
      VT_VERSION = 12,
      VT_DATA = 14
    };

    std::string name;
    int size;
    int major;
    int minor;
    int sub_minor;
    //Interface
    Interface interface() const{
      return static_cast<Interface>(GetField<uint32_t>(VT_INTERFACE,0));
    }
    int subInterface;
  };

  struct MemoryListEntry{
    int id;
    int size;
    int alignment;
    //flags
    //domain
    int bind_id;
    int tensor_desc_id;
    //contents.push_back
    //offsets.push_back
  };
}//loadable
}//vanilla_loadable

#endif
