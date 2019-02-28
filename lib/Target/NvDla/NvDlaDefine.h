//===- NvDlaDefine.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_DEFINE_H
#define TARGET_NVDLA_NVDLA_DEFINE_H

// ------------------------------

#define DLA_NV_SMALL 0
#define HAS_IMAGE_MODE 1

//-----------------------------

#if DLA_NV_SMALL
  #define DLA_TYPE_INT8
#else
//#define DLA_TYPE_INT16
  #define DLA_TYPE_FP16
#endif

#ifdef DLA_TYPE_INT8
    #define DLA_PRECISION PRECISION_INT8
#endif
#ifdef DLA_TYPE_INT16
    #define DLA_PRECISION PRECISION_INT16
#endif
#ifdef DLA_TYPE_FP16
    #define DLA_PRECISION PRECISION_FP16
#endif


// FIXME: use macro or variable?
// FIXME: currently duplicated declaration at fp16.c & NvDlaMeta.h
#if DLA_NV_SMALL
    // nv_small
    #define FEATURE_ATOM_CUBE_SIZE  8
    #define WEIGHT_ATOM_CUBE_SIZE   8

    #define ELEMENT_SIZE       1
    #define MAC_ATOMIC_C       8
    #define MAC_ATOMIC_K       8
    #define CBUF_BANK_NUM     32
    #define CBUF_BANK_WIDTH    8
    #define CBUF_BANK_DEPTH  512

    typedef char nv_weight_t;
#else
    // nv_full
    #define FEATURE_ATOM_CUBE_SIZE   32
    #define WEIGHT_ATOM_CUBE_SIZE   128

    #define ELEMENT_SIZE       2
    #define MAC_ATOMIC_C      64
    #define MAC_ATOMIC_K      16
    #define CBUF_BANK_NUM     16
    #define CBUF_BANK_WIDTH   64
    #define CBUF_BANK_DEPTH  256

    typedef short nv_weight_t;
#endif


#endif
