//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include <cstdint>
#include <fcntl.h>
#include <glog/logging.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>

#include <algorithm>
#include <fstream> // NOLINT(readability/streams)
#include <string>
#include <vector>

#include <onnc/Target/TG/io.hpp>

const int kProtoReadBytesLimit = INT_MAX; // Max size of 2 GB minus 1 byte.

namespace bmnet {

using google::protobuf::Message;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;

bool ReadProtoFromTextFile(const char *pFilename, Message *pProto)
{
  int fd = open(pFilename, O_RDONLY);
  CHECK_NE(fd, -1) << "File not found: " << pFilename;
  FileInputStream *input = new FileInputStream(fd);
  bool success = google::protobuf::TextFormat::Parse(input, pProto);
  delete input;
  close(fd);
  return success;
}

void WriteProtoToTextFile(const Message &pProto, const char *pFilename)
{
  int fd = open(pFilename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  FileOutputStream *output = new FileOutputStream(fd);
  CHECK(google::protobuf::TextFormat::Print(pProto, output));
  delete output;
  close(fd);
}

bool ReadProtoFromBinaryFile(const char *pFilename, Message *pProto)
{
  int fd = open(pFilename, O_RDONLY);
  CHECK_NE(fd, -1) << "File not found: " << pFilename;
  ZeroCopyInputStream *raw_input = new FileInputStream(fd);
  CodedInputStream *coded_input = new CodedInputStream(raw_input);
  coded_input->SetTotalBytesLimit(kProtoReadBytesLimit, 536870912);

  bool success = pProto->ParseFromCodedStream(coded_input);

  delete coded_input;
  delete raw_input;
  close(fd);
  return success;
}

void WriteProtoToBinaryFile(const Message &pProto, const char *pFilename)
{
  std::fstream output(pFilename,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  CHECK(pProto.SerializeToOstream(&output));
}

void ReadFloatDataFromBinaryFile(const string &pFilename,
                                 std::vector<float> &pData)
{
  std::ifstream fin(pFilename, std::ios::binary);
  CHECK(fin) << "File not found: " << pFilename;

  fin.seekg(0, std::ios::end);
  const size_t num_elements = fin.tellg() / sizeof(float);
  fin.seekg(0, std::ios::beg);

  pData.resize(num_elements);
  fin.read(reinterpret_cast<char *>(pData.data()),
           num_elements * sizeof(float));
}

void WriteFloatDataToBinaryFile(std::vector<float> *pData,
                                const string &pFilename)
{
  std::ofstream output(pFilename, std::ios::out | std::ios::binary);
  LOG(INFO) << "data size: " << pData->size();
  output.write((char *)(pData->data()), pData->size() * sizeof(float));
}

void WriteFloatDataToBinaryFile(void *pData, uint32_t pSize,
                                const string &pFilename)
{
  std::fstream output(pFilename,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  output.write((char *)pData, pSize);
}

/*
 * Int8 version
 */
void ReadInt8DataFromBinaryFile(const string &pFilename,
                                std::vector<int8_t> &pData)
{
  std::ifstream fin(pFilename, std::ios::binary);
  CHECK(fin) << "File not found: " << pFilename;

  fin.seekg(0, std::ios::end);
  const size_t num_elements = fin.tellg() / sizeof(int8_t);
  fin.seekg(0, std::ios::beg);

  pData.resize(num_elements);
  fin.read(reinterpret_cast<char *>(pData.data()),
           num_elements * sizeof(int8_t));
}

void WriteInt8DataToBinaryFile(std::vector<int8_t> *pData,
                               const string &pFilename)
{
  std::ofstream output(pFilename, std::ios::out | std::ios::binary);
  LOG(INFO) << "data size: " << pData->size();
  output.write((char *)(pData->data()), pData->size() * sizeof(int8_t));
}

/*
 * Uint8 version
 */
void ReadUint8DataFromBinaryFile(const string &pFilename,
                                 std::vector<uint8_t> &pData)
{
  std::ifstream fin(pFilename, std::ios::binary);
  CHECK(fin) << "File not found: " << pFilename;

  fin.seekg(0, std::ios::end);
  const size_t num_elements = fin.tellg() / sizeof(uint8_t);
  fin.seekg(0, std::ios::beg);

  pData.resize(num_elements);
  fin.read(reinterpret_cast<char *>(pData.data()),
           num_elements * sizeof(uint8_t));
}

} // namespace bmnet
