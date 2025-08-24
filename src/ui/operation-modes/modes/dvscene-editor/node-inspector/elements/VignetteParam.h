#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* gradationTypeNames[] = {
        "GRADATION_MODE_CIRCLE",
        "GRADATION_MODE_LINE"
    };

    const char* blendModeNames[] = {
        "BLEND_MODE_ALPHA_BLEND",
        "BLEND_MODE_ADD",
        "BLEND_MODE_MUL",
        "BLEND_MODE_SCREEN",
        "BLEND_MODE_OVERLAY"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::VIGNETTE_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementVignetteParam::Description*>(element);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= CheckboxFlags("Depth Enabled", data->flags, app::dv::DvElementVignetteParam::Description::Flags::DEPTH_ENABLED);
#endif
        changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementVignetteParam::Description::Flags::CURVE_ENABLED);
        changed |= ComboEnum("Gradation Type", data->gradationType, gradationTypeNames);
        changed |= ComboEnum("Blend Mode", data->blendMode, blendModeNames);
        changed |= Editor("Vignette Parameters", data->vignetteParams);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if(data->flags.test(app::dv::DvElementVignetteParam::Description::Flags::DEPTH_ENABLED))
            changed |= Editor("Depth Parameters", data->depthParams);
#endif
        if (data->flags.test(app::dv::DvElementVignetteParam::Description::Flags::CURVE_ENABLED))
            changed |= Editor("Finish Vignette Parameters", data->finishVignetteParams);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (data->flags.test(app::dv::DvElementVignetteParam::Description::Flags::CURVE_ENABLED) && data->flags.test(app::dv::DvElementVignetteParam::Description::Flags::DEPTH_ENABLED))
            changed |= Editor("Finish Depth Parameters", data->finishDepthParams);
#endif
        return changed;
    }
}
