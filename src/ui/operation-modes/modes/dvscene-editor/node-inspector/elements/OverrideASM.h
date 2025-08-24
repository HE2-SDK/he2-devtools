#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::OVERRIDE_ASM>(char* element) {
        bool changed = false;
#ifdef DEVTOOLS_TARGET_SDK_miller
        auto* data = reinterpret_cast<app::dv::DvElementOverrideAsm::Description*>(element);
#elif DEVTOOLS_TARGET_SDK_rangers
        auto* data = reinterpret_cast<app::dv::DvElementOverrideASM::Description*>(element);
#endif
		changed |= Editor("State Name", data->asmName);
		changed |= Editor("Target State Name", data->asmNameTarget);
        return changed;
    }
}
