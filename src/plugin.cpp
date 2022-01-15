#include "plugin.hpp"


Plugin *pluginInstance;

void init(rack::Plugin *p) {
	pluginInstance = p;

	p->addModel(model_AC_Folding);
	p->addModel(model_AC_LFold);
}



