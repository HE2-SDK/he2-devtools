#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::ATMOSPHERE_GODRAY_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementAtmosphereGodrayParam::Description*>(element);
		changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            changed |= Editor("Density", data->density);
            changed |= Editor("Decay", data->decay);
            changed |= Editor("Weight", data->weight);
        }
        return changed;
    }
}
