#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::MATERIAL_ANIM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementMaterialAnim::Description*>(element);
		changed |= Editor("Material Anim Name", data->matAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
