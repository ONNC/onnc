#ifndef GENMEM_HEADER_H
#define GENMEM_HEADER_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

namespace genmem {

#define GEN_assert(EXPR)                                                                                  \
  if (!(EXPR)) {                                                                                          \
    fprintf(stderr, "[genmem-FATAL] ASSERTION: %s\n", #EXPR);                                             \
    fprintf(stderr, "\t At File : %s \n\t Function : %s, line : %d\n", __FILE__, __FUNCTION__, __LINE__); \
    exit(-1);                                                                                             \
  }

using std::cin;
using std::cout;
using std::endl;

using std::ceil;

using std::map;
using std::string;
using std::vector;

using std::min;

enum DIM
{
  W_ID = 0,
  H_ID = 1,
  C_ID = 2
};

enum DATA_DIR
{
  DATAIN  = 0,
  DATAOUT = 1
};

enum RAM_TYPE
{
  CVIF = 0,
  MCIF = 1
};

enum RUBIK_MODE
{
  CONTRACTMODE = 0,
  SPLITMODE    = 1,
  MERGEMODE    = 2,

  BDMAMODE = 3
};

// define in genmem.cpp
extern unsigned long long start_address;
extern int                ATOMIC_K;
extern int                elementSize;
extern int                memWidth;
extern int                NUM_MAX_BDMA_TRANSFER;
} // namespace genmem

#endif
