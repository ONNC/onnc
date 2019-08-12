#include "my_loadable/loadable.h"

namespace cortexm_loadable{
namespace my_loadable{
  /*
  LoadableFactory_vanilla::LoadablePrivPair LoadableFactory_vanilla::newLoadable(){
    ILoadable* loadable;
    Loadable* loadable_priv;
    //loadable = loadable_priv = new Loadable();
		loadable = new Loadable();
    loadable_priv = new Loadable();
		return LoadableFactory_vanilla::LoadablePrivPair(loadable , loadable_priv);
	}
	*/

  int LoadableFactory_cortexm::newLoadable(){
    ILoadable* loadable;
    Loadable* loadable_priv;
    //loadable = loadable_priv = new Loadable();
		loadable = new Loadable();
    loadable_priv = new Loadable();
		return 0;
	}

  int Loadable::setSymbolContent(std::string name, struct Blob& b,uint8_t *data){
    return 0;
  }

	Loadable::Loadable(){}
  Loadable::~Loadable(){
		
	}

}
}
