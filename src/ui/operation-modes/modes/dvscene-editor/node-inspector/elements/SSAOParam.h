#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* ssaoTypeNames[] = {
        "SSAO",
        "HBAO",
        "HBAO_SSS",
        "SSS"
    };

    const char* renderTargetSizeNames[] = {
        "RTSIZE_SAME_AS_FRAMEBUFFER",
        "RTSIZE_ONE_SECOND",
        "RTSIZE_ONE_FORTH"
    };

    const char* blurTypeNames[] = {
        "None",
        "Gauss2x2",
        "Gauss3x3",
        "Gauss4x4",
        "Gauss5x5",
        "Gauss6x6",
        "Gauss7x7",
        "Gauss8x8",
        "Gauss9x9",
        "Bilateral",
        "NoisySample"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SSAO_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSSAOParam::Description*>(element);
        changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementSSAOParam::Description::Flags::ENABLED);
        if(data->flags.test(app::dv::DvElementSSAOParam::Description::Flags::ENABLED)){
            changed |= CheckboxFlags("Use Render Target Size", data->flags, app::dv::DvElementSSAOParam::Description::Flags::USE_RENDER_TARGET_SIZE);
            changed |= ComboEnum("SSAO Type", data->postType, ssaoTypeNames);
            changed |= ComboEnum("Render Target Size", data->renderTargetSize, renderTargetSizeNames);
            changed |= ComboEnum("Blur Type", data->blurStep, blurTypeNames);
            changed |= Editor("Intensity", data->intensity);
            changed |= Editor("Radius", data->radius);
            changed |= Editor("Fadeout Distance", data->fadeoutDistance);
            changed |= Editor("Fadeout Radius", data->fadeoutRadius);
            changed |= Editor("Power", data->power);
            changed |= Editor("Bias", data->bias);
            changed |= Editor("Occlusion Distance", data->occlusionDistance);
            changed |= Editor("Direct Lighting Influence", data->directLightingInfluence);
            changed |= Editor("Bilateral Threshold", data->bilateralThreshold);
        }
        return changed;
    }
}
