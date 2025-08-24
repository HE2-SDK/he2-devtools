#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::PATH_INTERPOLATION>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementPathInterpolation::Description*>(element);
		changed |= Editor("Use Absolute Position", data->useAbsolutePosition);
        changed |= Editor("Interpolation", data->interpolation);
        changed |= Editor("Finish Interpolation", data->finishInterpolation);
        return changed;
    }
}
