#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_miller
    const char* camBlurTypeNames[] = {
        "BLURTYPE_CAMERA",
        "BLURTYPE_PREV_SURFACE",
        "BLURTYPE_RADIAL",
    };
#endif

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_BLUR_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCameraBlurParam::Description*>(element);
		changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementCameraBlurParam::Description::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementCameraBlurParam::Description::Flags::ENABLED)){
            changed |= CheckboxFlags("Single Direction OPT", data->flags, app::dv::DvElementCameraBlurParam::Description::Flags::SINGLE_DIRECTION_OPT);
            changed |= CheckboxFlags("Curve Enabled", data->flags, app::dv::DvElementCameraBlurParam::Description::Flags::CURVE_ENABLED);
            changed |= Editor("Sample Amount", data->sampleAmount);
#ifdef DEVTOOLS_TARGET_SDK_rangers
            changed |= Editor("Blur Amount", data->blurAmount);
            if(data->flags.test(app::dv::DvElementCameraBlurParam::Description::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Blur Amount", data->finishBlurAmount);
#elif DEVTOOLS_TARGET_SDK_miller
            changed |= Editor("Parameters", data->params);
            if(data->flags.test(app::dv::DvElementCameraBlurParam::Description::Flags::CURVE_ENABLED))
                changed |= Editor("Finish Parameters", data->finishParams);
            changed |= ComboEnum("Blur Type", data->blurType, camBlurTypeNames);
#endif
        }
        return changed;
    }
}
