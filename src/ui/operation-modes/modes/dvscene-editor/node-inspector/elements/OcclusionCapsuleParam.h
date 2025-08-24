#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::OCCLUSION_CAPSULE_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementOcclusionCapsuleParam::Description*>(element);
		changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            float color[4] = { static_cast<float>(data->occlusionColor[0]) / 255, static_cast<float>(data->occlusionColor[1]) / 255, static_cast<float>(data->occlusionColor[2]) / 255, static_cast<float>(data->occlusionColor[3]) / 255 };
            if(changed |= ImGui::ColorEdit4("Color", color)){
                data->occlusionColor[0] = static_cast<unsigned int>(color[0] * 255);
                data->occlusionColor[1] = static_cast<unsigned int>(color[1] * 255);
                data->occlusionColor[2] = static_cast<unsigned int>(color[2] * 255);
                data->occlusionColor[3] = static_cast<unsigned int>(color[3] * 255);
            }
            changed |= Editor("Occlusion Power", data->occlusionPower);
        }
        return changed;
    }
}
