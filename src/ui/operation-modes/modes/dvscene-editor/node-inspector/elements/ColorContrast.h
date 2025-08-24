#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::COLOR_CONTRAST>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementColorContrast::Description*>(element);
        changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementColorContrast::Description::Flags::ENABLED);
        if (data->flags.test(app::dv::DvElementColorContrast::Description::Flags::ENABLED)) {
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementColorContrast::Description::Flags::CURVE_ENABLED);
            changed |= CheckboxFlags("Use HLS Correction", data->flags, app::dv::DvElementColorContrast::Description::Flags::USE_HLS_CORRECTION);
#ifdef DEVTOOLS_TARGET_SDK_rangers
            changed |= Editor("Contrast", data->contrast);
#elif DEVTOOLS_TARGET_SDK_miller
            changed |= Editor("LUT Ratio", data->lutRatio);
#endif
            changed |= Editor("Dynamic Range", data->dynamicRange);
            changed |= Editor("Crush Shadows", data->crushShadows);
            changed |= Editor("Crush Highlights", data->crushHighlights);
            if (data->flags.test(app::dv::DvElementColorContrast::Description::Flags::USE_HLS_CORRECTION)) {
                changed |= Editor("HLS Hue Offset", data->hlsHueOffset);
                changed |= Editor("HLS Lightness Offset", data->hlsLightnessOffset);
                changed |= Editor("HLS Saturation Offset", data->hlsSaturationOffset);
            }
        }
        return changed;
    }
}
