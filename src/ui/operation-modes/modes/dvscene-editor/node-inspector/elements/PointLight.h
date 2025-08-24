#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::POINT_LIGHT>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementPointLight::Description*>(element);
        changed |= Editor("Position", data->position);
        changed |= Editor("Finish Position", data->finishPosition);
        float color[3] = { static_cast<float>(data->color[0]) / 255, static_cast<float>(data->color[1]) / 255, static_cast<float>(data->color[2]) / 255 };
        if (changed |= ImGui::ColorEdit3("Color", color)) {
            data->color[0] = static_cast<int>(color[0] * 255);
            data->color[1] = static_cast<int>(color[1] * 255);
            data->color[2] = static_cast<int>(color[2] * 255);
        }
        float finishColor[3] = { static_cast<float>(data->finishColor[0]) / 255, static_cast<float>(data->finishColor[1]) / 255, static_cast<float>(data->finishColor[2]) / 255 };
        if (changed |= ImGui::ColorEdit3("Finish Color", finishColor)) {
            data->finishColor[0] = static_cast<int>(finishColor[0] * 255);
            data->finishColor[1] = static_cast<int>(finishColor[1] * 255);
            data->finishColor[2] = static_cast<int>(finishColor[2] * 255);
        }
        changed |= Editor("Parameters", data->params);
        changed |= Editor("Finish Parameters", data->finishParams);
        changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Unk14", data->unk14);
        return changed;
    }
}
