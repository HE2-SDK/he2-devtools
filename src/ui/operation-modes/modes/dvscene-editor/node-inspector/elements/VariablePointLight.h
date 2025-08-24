#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* typeNames[]{
        "NORMAL",
        "CURVE_CONTROLLED",
        "TIME_CONTROLLED"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::VARIABLE_POINT_LIGHT>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementVariablePointLight::Description*>(element);
		changed |= Editor("Unk0", data->unk0);
        changed |= Editor("Position", data->position);
        changed |= Editor("Finish Position", data->finishPosition);
        changed |= ColorEditor("Color", data->color);
        changed |= ColorEditor("Finish Color", data->finishColor);
        changed |= Editor("Parameters", data->params);
        changed |= Editor("Finish Parameters", data->finishParams);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= ComboEnum("Type", data->type, typeNames);
#endif
        changed |= Editor("Unk5", data->unk5);
        return changed;
    }
}
