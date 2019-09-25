#include "my_loadable/loadable.h"

namespace cortexm_loadable {
namespace my_loadable {

int LoadableFactory_cortexm::newLoadable() {
  ILoadable* loadable;
  Loadable* loadable_priv;
  loadable = new Loadable();
  loadable_priv = new Loadable();
  return 0;
}

int Loadable::setSymbolContent(std::string name, struct Blob& b,
                               uint8_t* data) {
  return 0;
}

Loadable::Loadable() {}
Loadable::~Loadable() {}
}
}
