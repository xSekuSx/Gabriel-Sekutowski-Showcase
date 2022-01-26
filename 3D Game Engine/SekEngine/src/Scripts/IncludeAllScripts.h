//include all scirpts here and call their initialise
//make and isntance of them
#pragma once
#include "CheckPointsScript.h"

CheckPointsScript checkpointsscript;

namespace IncludeScripts {
	void IncludeAll() {
		checkpointsscript.Initialise();
	};
}