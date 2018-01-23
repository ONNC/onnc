//===- Compress.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_COMPRESS_H
#define ONNC_SUPPORT_COMPRESS_H
#include <zlib.h>

namespace onnc {

class Compress
{
public:
  enum Result {
    kSuccess    = Z_OK,
    kStreamEnd  = Z_STREAM_END,
    kNeedDict   = Z_NEED_DICT,
    kErrNo      = Z_ERRNO,
    KStreamErr  = Z_STREAM_ERROR,
    kDataErr    = Z_DATA_ERROR,
    kMemErr     = Z_MEM_ERROR,
    kBufErr     = Z_BUF_ERROR,
    kVersionErr = Z_VERSION_ERROR
  };

  enum Level {
    kNoCompression      = Z_NO_COMPRESSION,
    kBestSpeed          = Z_BEST_SPEED,
    kBestCompression    = Z_BEST_COMPRESSION,
    kDefaultCompression = Z_DEFAULT_COMPRESSION
  };

public:
  /// compress from file pSrc to file pDest until EOF on pSrc
  static Result deflate(const Path& pSrc, const Path& pDest, Level pLevel);

  /// decompress from file pSrc to file pDest
  static Result inflate(const Path& pSrc, const Path& pDest);
};

} // namespace of onnc

#endif
