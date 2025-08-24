#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CHROMATIC_ABERRATION_FILTER_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementChromaticAberrationFilterParam::Description*>(element);
        changed |= Editor("Curve Enabled", data->curveEnabled);
		changed |= Editor("Parameters", data->param);
        if(data->curveEnabled)
            changed |= Editor("Finish Parameters", data->finishParam);
        return changed;
    }
}
