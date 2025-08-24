#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::FALLOFF>(char* element) {
        auto* data = reinterpret_cast<app::dv::DvElementFalloff::Description*>(element);
		return Editor("Value", data->value);
    }
}
