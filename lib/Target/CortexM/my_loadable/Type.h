#ifndef VANILLA_MYLOADABLE_TYPE_H
#define VANILLA_MYLOADABLE_TYPE_H

#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <exception>

namespace CortexM_loadable{
namespace my_loadable{
  
  template <typename I,typename P>
  class PrivPair{
    public:
      typedef I interface_type;
      typedef P private_type;
  };

}//priv
}//vanilla

#endif
