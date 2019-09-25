#ifndef VANILLA_MYLOADABLE_LOADABLE_H
#define VANILLA_MYLOADABLE_LOADABLE_H

#include <map>

#include "Type.h"
#include "loadable_generated.h"

#include "iloadable.h"

namespace cortexm_loadable {
namespace my_loadable {

class Loadable;

class LoadableFactory_cortexm {
public:
  typedef PrivPair<ILoadable *, Loadable *> LoadablePrivPair;

  static int newLoadable();

  static Loadable *priv(ILoadable *);
  static ILoadable *i(Loadable *);
  static ILoadable *self(void *s);
};

class Loadable : public ILoadable {
public:
  virtual int setSymbolContent(std::string name, struct Blob &,
                               uint8_t *data); // error : non-class type
  Loadable();
  virtual ~Loadable();
};

} // namespace my_loadable end
} // namespace vanilla end

#endif
