//===- Contract.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef ONNC_SUPPORT_CONTRACT_H_INCLUDED
#define ONNC_SUPPORT_CONTRACT_H_INCLUDED

#include <onnc/Diagnostic/MsgHandling.h>

#ifndef EXPECTS_MSG
#  define EXPECTS_MSG(cond, msg) EXPECTS_MSG_1(cond, msg)
#  define EXPECTS_MSG_1(cond, msg) EXPECTS_MSG_2(cond, msg)
#  define EXPECTS_MSG_2(cond, msg)              \
    do {                                        \
      if (!(cond)) {                            \
        fatal(cannot_meet_precondition) << msg; \
      }                                         \
    } while (false)
#else
#  error "already defined EXPECTS_MSG somewhere"
#endif

#ifndef EXPECTS
#  define EXPECTS(cond) EXPECTS_1(cond)
#  define EXPECTS_1(cond) EXPECTS_2(cond)
#  define EXPECTS_2(cond) EXPECTS_MSG(cond, #  cond)
#else
#  error "already defined EXPECTS somewhere"
#endif

#ifndef ENSURES_MSG
#  define ENSURES_MSG(cond, msg) ENSURES_MSG_1(cond, msg)
#  define ENSURES_MSG_1(cond, msg) ENSURES_MSG_2(cond, msg)
#  define ENSURES_MSG_2(cond, msg)               \
    do {                                         \
      if (!(cond)) {                             \
        fatal(cannot_meet_postcondition) << msg; \
      }                                          \
    } while (false)
#else
#  error "already defined ENSURES_MSG somewhere"
#endif

#ifndef ENSURES
#  define ENSURES(cond) ENSURES_1(cond)
#  define ENSURES_1(cond) ENSURES_2(cond)
#  define ENSURES_2(cond) ENSURES_MSG(cond, #  cond)
#else
#  error "already defined ENSURES somewhere"
#endif

#ifndef ASSERT_MSG
#  define ASSERT_MSG(cond, msg) ASSERT_MSG_1(cond, msg)
#  define ASSERT_MSG_1(cond, msg) ASSERT_MSG_2(cond, msg)
#  define ASSERT_MSG_2(cond, msg)        \
    do {                                 \
      if (!(cond)) {                     \
        fatal(assertion_failure) << msg; \
      }                                  \
    } while (false)
#else
#  error "already defined ASSERT_MSG somewhere"
#endif

#ifndef ASSERT
#  define ASSERT(cond) ASSERT_1(cond)
#  define ASSERT_1(cond) ASSERT_2(cond)
#  define ASSERT_2(cond) ASSERT_MSG(cond, #  cond)
#else
#  error "already defined ASSERT somewhere"
#endif

#endif
