/*
 * Copyright (c) 2017-2018, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NVDLA_PRIV_TYPE_H
#define NVDLA_PRIV_TYPE_H

#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <exception>

#include "nvdla/IType.h"
//#include "priv/Check.h"

namespace nvdla
{

namespace priv
{

// Note: these are used as array indices.
enum IOD { IOD_Input = 0U, IOD_Output = 1U, IOD_Max = 3U };
enum IO  { IO_Input = 0U, IO_Output = 1U, IO_Max = 2U };

enum ELST { ELST_Elem = 0U, ELST_Line = 1U, ELST_Surf = 2U, ELST_Tensor = 3U, ELST_Max = 4U };

//
// this isn't meant to be complicated. just to reduce typing and errors.
//
template <typename L, typename R>
class BiMap
{
public:
    BiMap()  { }
    ~BiMap() { }

    typedef typename std::map<L, R>::iterator left_iterator;
    typedef typename std::map<R, L>::iterator right_iterator;

    void insert(L l, R r) { m_left_right[l] = r; m_right_left[r] = l; }
    void remove(L l) {
        R r = m_left_right[l];
        m_left_right.erase(l);
        m_right_left.erase(r);
    }

    left_iterator begin_left()   { return m_left_right.begin(); }
    left_iterator end_left()     { return m_left_right.end();   }
    left_iterator find_left(L l) { return m_left_right.find(l); }

    right_iterator begin_right()   { return m_right_left.begin(); }
    right_iterator end_right()     { return m_right_left.end();   }
    right_iterator find_right(R r) { return m_right_left.find(r); }

protected:
    std::map<L, R> m_left_right;
    std::map<R, L> m_right_left;
};

//
// PrivPair and PrivDiamond simplify management of the pointers necessary
// to track public interfaces, their private implementations and derivations
// of such which result in a diamond inheritance pattern.  These are simply
// fancy 2 and 4-tuples implemented by std::pair and 2x same.
// Note: with RTTI enabled this can all disappear as dynamic_cast<>()
// would be available instead ;(
//
template <typename I, typename P>
class PrivPair
{
public:
    typedef I InterfaceType;
    typedef P PrivateType;

    PrivPair() : m_i_priv(0, 0) { }

    PrivPair(I i, P priv) :
        m_i_priv(i, priv)
    { }

    PrivPair(const PrivPair &p) :
        m_i_priv(p.m_i_priv)
    { }

    inline bool operator !() const { return (!m_i_priv.first) || (!m_i_priv.second); }
    inline bool operator ==(const PrivPair &rhs) const { return m_i_priv == rhs.m_i_priv; }
    inline bool operator <(const PrivPair &rhs) const { return m_i_priv < rhs.m_i_priv; }

    inline I i() const      { return m_i_priv.first;  }
    inline P priv() const   { return m_i_priv.second; }

protected:
    std::pair<I, P> m_i_priv;
};

} // nvdla::priv

} // nvdla

#endif // NVDLA_PRIV_TYPE_H
