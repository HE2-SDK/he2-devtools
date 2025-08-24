#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::GODRAY_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementGodrayParam::Description*>(element);
        changed |= ImGui::ColorEdit3("Color", data->color);
        changed |= Editor("Intensity", data->intensity);
        return changed;
    }
}
