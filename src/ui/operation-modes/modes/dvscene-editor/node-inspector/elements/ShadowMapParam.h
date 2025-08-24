#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* shadowFilterNames[] = {
        "SHADOW_FILTER_POINT",
        "SHADOW_FILTER_PCF",
        "SHADOW_FILTER_PCSS",
        "SHADOW_FILTER_ESM",
        "SHADOW_FILTER_MSM",
        "SHADOW_FILTER_VSM_POINT",
        "SHADOW_FILTER_VSM_LINEAR",
        "SHADOW_FILTER_VSM_ANISO_2",
        "SHADOW_FILTER_VSM_ANISO_4",
        "SHADOW_FILTER_VSM_ANISO_8",
        "SHADOW_FILTER_VSM_ANISO_16",
        "SHADOW_FILTER_COUNT",
        "SHADOW_FILTER_VSM_FIRST",
        "SHADOW_FILTER_VSM_LAST"
    };
    
    const char* shadowRangeTypeNames[] = {
        "SHADOW_RANGE_TYPE_CAMERA_LOOKAT",
        "SHADOW_RANGE_TYPE_POSITION_MANUAL",
        "SHADOW_RANGE_TYPE_FULL_MANUAL",
        "SHADOW_RANGE_TYPE_COUNT",
        "SHADOW_RANGE_TYPE_DEFAULT"
    };
    
    const char* fitProjectionNames[] = {
        "FIT_PROJECTION_TO_CASCADES",
        "FIT_PROJECTION_TO_SCENE",
        "FIT_PROJECTION_TO_ROTATE_CASCADES"
    };
    
    const char* fitNearFarNames[] = {
        "FIT_NEARFAR_ZERO_ONE",
        "FIT_NEARFAR_AABB",
        "FIT_NEARFAR_SCENE_AABB"
    };
    
    const char* partitionTypeNames[] = {
        "PARTITION_PSSM",
        "PARTITION_MANUAL"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SHADOW_MAP_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementShadowMapParam::Description*>(element);
        changed |= ComboEnum("Shadow Filter", data->shadowFilter, shadowFilterNames);
        changed |= ComboEnum("Shadow Range Type", data->shadowRangeType, shadowRangeTypeNames);
        changed |= ComboEnum("Fit Projection", data->fitProjection, fitProjectionNames);
        changed |= ComboEnum("Fit Near Far", data->fitNearFar, fitNearFarNames);
        changed |= ComboEnum("Partition Type", data->partitionType, partitionTypeNames);
        changed |= Editor("Scene Range", data->sceneRange);
        changed |= Editor("Scene Center", data->sceneCenter);
        changed |= Editor("Manual Light Position", data->manualLightPos);
        changed |= Editor("PSSM Lambda", data->pssmLambda);
        changed |= Editor("Cascade Offset", data->cascadeOffset);
        changed |= Editor("Cascade Level", data->cascadeLevel);
        changed |= Editor("Cascade Splits", data->cascadeSplits);
        changed |= Editor("Cascade Bias", data->cascadeBias);
        changed |= Editor("Bias", data->bias);
        changed |= Editor("Offset", data->offset);
        changed |= Editor("Normal Bias", data->normalBias);
        changed |= Editor("Blur Quality", data->blurQuality);
        changed |= Editor("Blur Size", data->blurSize);
        changed |= Editor("Fadeout Distance", data->fadeoutDistance);
        changed |= Editor("Cascade Transition Fade Distance", data->cascadeTransitionFadeDistance);
        changed |= CheckboxFlags("Enable Back Face Shadow", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_BACK_FACE_SHADOW);
        changed |= CheckboxFlags("Enable Shadow Camera", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_SHADOW_CAMERA);
        changed |= CheckboxFlags("Enable Draw Scene AABB", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_DRAW_SCENE_AABB);
        changed |= CheckboxFlags("Enable Draw Shadow Frustrum", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_DRAW_SHADOW_FRUSTRUM);
        changed |= CheckboxFlags("Enable Draw Cascade", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_DRAW_CASCADE);
        changed |= CheckboxFlags("Enable Draw Camera", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_DRAW_CAMERA_FRUSTRUM);
        changed |= CheckboxFlags("Enable Pause Camera", data->flags, app::dv::DvElementShadowMapParam::Description::Flags::ENABLE_PAUSE_CAMERA);
        return changed;
    }
}
