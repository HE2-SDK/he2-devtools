#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::GENERAL_TRIGGER>(char* element) {
        auto* data = reinterpret_cast<app::dv::DvElementGeneralTrigger::Description*>(element);
		return Editor("Trigger Name", data->triggerName);
    }
}
