#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::UV_ANIM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementUVAnim::Description*>(element);
		changed |= Editor("UV Anim Name", data->uvAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
