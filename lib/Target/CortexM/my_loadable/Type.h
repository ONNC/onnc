#ifndef VANILLA_MYLOADABLE_TYPE_H
#define VANILLA_MYLOADABLE_TYPE_H

#include <algorithm>
#include <exception>
#include <map>
#include <sstream>
#include <string>

namespace cortexm_loadable {
namespace my_loadable {

template <typename I, typename P> class PrivPair {
public:
  typedef I interface_type;
  typedef P private_type;
};

} // priv
} // vanilla

#endif
