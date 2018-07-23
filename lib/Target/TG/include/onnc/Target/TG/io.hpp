#ifndef _UTILS_IO_H_
#define _UTILS_IO_H_

#include <boost/filesystem.hpp>
#include <iomanip>
#include <iostream> // NOLINT(readability/streams)
#include <string>

#include "google/protobuf/message.h"

namespace bmnet {

using ::boost::filesystem::path;
using ::google::protobuf::Message;
using std::string;

bool ReadProtoFromTextFile(const char *pFilename, Message *pProto);
inline bool ReadProtoFromTextFile(const string &pFilename, Message *pProto)
{
  return ReadProtoFromTextFile(pFilename.c_str(), pProto);
}

void WriteProtoToTextFile(const Message &pProto, const char *pFilename);
inline void WriteProtoToTextFile(const Message &pProto, const string &pFilename)
{
  WriteProtoToTextFile(pProto, pFilename.c_str());
}

bool ReadProtoFromBinaryFile(const char *pFilename, Message *pProto);
inline bool ReadProtoFromBinaryFile(const string &pFilename, Message *pProto)
{
  return ReadProtoFromBinaryFile(pFilename.c_str(), pProto);
}

void WriteProtoToBinaryFile(const Message &pProto, const char *pFilename);
inline void WriteProtoToBinaryFile(const Message &pProto,
                                   const string &pFilename)
{
  WriteProtoToBinaryFile(pProto, pFilename.c_str());
}

void ReadFloatDataFromBinaryFile(const string &pFilename,
                                 std::vector<float> &pData);

void WriteFloatDataToBinaryFile(std::vector<float> *pData,
                                const string &pFilename);

void WriteFloatDataToBinaryFile(void *pData, uint32_t pSize,
                                const string &pFilename);

/*
 * Int8 version
 */

void ReadInt8DataFromBinaryFile(const string &pFilename,
                                std::vector<int8_t> &pData);

void WriteInt8DataToBinaryFile(std::vector<int8_t> *pData,
                               const string &pFilename);

/*
 * Uint8 version
 */

void ReadUint8DataFromBinaryFile(const string &pFilename,
                                 std::vector<uint8_t> &pData);

} // namespace bmnet

#endif // _UTILS_IO_H_
