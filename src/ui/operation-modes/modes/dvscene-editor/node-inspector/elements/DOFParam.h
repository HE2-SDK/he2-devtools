#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* rtScaleNames[] = {
        "FULL_SCALE",
        "HALF_SCALE",
        "QUARTER_SCALE"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::DOF_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDOFParam::Description*>(element);
		changed |= CheckboxFlags("Use Focus Look At", data->flags, app::dv::DvElementDOFParam::Description::Flags::USE_FOCUS_LOOK_AT);
		changed |= CheckboxFlags("Enable Circle Dof", data->flags, app::dv::DvElementDOFParam::Description::Flags::ENABLE_CIRCLE_DOF);
		changed |= CheckboxFlags("Draw Focal Plane", data->flags, app::dv::DvElementDOFParam::Description::Flags::DRAW_FOCAL_PLANE);
		changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementDOFParam::Description::Flags::CURVE_ENABLED);
        changed |= Editor("Parameters", data->param);
        if(data->flags.test(app::dv::DvElementDOFParam::Description::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Parameters", data->finishParam);
        changed |= Editor("COC Max Radius", data->cocMaxRadius);
        changed |= Editor("Bokeh Radius Scale", data->bokehRadiusScale);
        changed |= Editor("Bokeh Sample Count", data->bokehSampleCount);
        changed |= Editor("Sky Focus Distance", data->skyFocusDistance);
        changed |= Editor("Bokeh Bias", data->bokehBias);
        changed |= ComboEnum("RenderTarget Scale", data->rtScale, rtScaleNames);
        return changed;
    }
}
