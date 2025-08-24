#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::BLOOM_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementBloomParam::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Parameters", data->params);
        if(data->curveEnabled)
            changed |= Editor("Finish Parameters", data->finishParams);
        return changed;
    }
}
