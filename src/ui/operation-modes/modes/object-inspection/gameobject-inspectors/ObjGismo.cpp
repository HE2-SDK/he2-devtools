#include "ObjGismo.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>

void RenderGameObjectInspector(app::ObjGismo& gismo) {
	ReflectionEditor("Config Design", gismo.configDesign, &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigDesignData::rflClass));
	ReflectionEditor("Config Plan", gismo.configPlan, &RESOLVE_STATIC_VARIABLE(heur::rfl::GismoConfigPlanData::rflClass));
}
