#include "GOCHsm2.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/HsmBase.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(app_cmn::fsm::GOCHsm2& component) {
	Editor("HSM", component.GetHsm());
}
