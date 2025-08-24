#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::OBJECT_TIME_STOP>(char* element) {
        auto* data = reinterpret_cast<app::dv::DvElementObjectTimeStop::Description*>(element);
		return Editor("Type", data->type);
    }
}
