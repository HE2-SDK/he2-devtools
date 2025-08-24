#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SPOTLIGHT>(char* element) {
		bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementSpotlight::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
		changed |= Editor("Position", data->position);
		if(data->curveEnabled)
			changed |= Editor("Finish Position", data->finishPosition);
		changed |= Editor("Rotation", data->rotation);
		changed |= ColorEditor("Color", data->color);
		changed |= Editor("Range", data->range);
		changed |= Editor("Intensity", data->intensity);
		changed |= Editor("Falloff", data->falloff);
		changed |= Editor("Angle", data->angle);
		return changed;
    }
}
