#pragma once
#include "Timeline.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using namespace hh::dv;
    using namespace app::dv;
    using ElementID = DvNodeElement::ElementID;

    constexpr std::pair<ElementID, std::pair<size_t, size_t>> RenderTimelineElements[] = {
        {ElementID::CAMERA_PARAMS,                      {offsetof(DvElementCameraParams::Description, curveData),                      sizeof(DvElementCameraParams::Description::curveData)}},
        {ElementID::CAMERA_SHAKE_LOOP,                  {offsetof(DvElementCameraShakeLoop::Description, curveData),                   sizeof(DvElementCameraShakeLoop::Description::curveData)}},
        {ElementID::EFFECT,                             {offsetof(DvElementEffect::Description, curveData),                            sizeof(DvElementEffect::Description::curveData)}},
        {ElementID::PATH_INTERPOLATION,                 {offsetof(DvElementPathInterpolation::Description, curveData),                 sizeof(DvElementPathInterpolation::Description::curveData)}},
        {ElementID::CAMERA_OFFSET,                      {offsetof(DvElementCameraOffset::Description, curveData),                      sizeof(DvElementCameraOffset::Description::curveData)}},
        {ElementID::MODEL_FADE,                         {offsetof(DvElementModelFade::Description, curveData),                         sizeof(DvElementModelFade::Description::curveData)}},
        {ElementID::CAMERA_HEDGEHOG,                    {offsetof(DvElementCameraHedgehog::Description, curveData),                    sizeof(DvElementCameraHedgehog::Description::curveData)}},
        {ElementID::SPOTLIGHT,                          {offsetof(DvElementSpotlight::Description, curveData),                         sizeof(DvElementSpotlight::Description::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_miller
        {ElementID::POINT_LIGHT,                        {offsetof(DvElementPointLight::Description, curveData),                        sizeof(DvElementPointLight::Description::curveData)}},
        {ElementID::MATERIAL_PARAM,                     {offsetof(DvElementMaterialParam::Description, curveData),                     sizeof(DvElementMaterialParam::Description::curveData)}},
#endif
        {ElementID::BLOOM_PARAM,                        {offsetof(DvElementBloomParam::Description, curveData),                        sizeof(DvElementBloomParam::Description::curveData)}},
        {ElementID::DOF_PARAM,                          {offsetof(DvElementDOFParam::Description, curveData),                          sizeof(DvElementDOFParam::Description::curveData)}},
        {ElementID::COLOR_CONTRAST,                     {offsetof(DvElementColorContrast::Description, curveData),                     sizeof(DvElementColorContrast::Description::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::CAMERA_CONTROL_PARAM,               {offsetof(DvElementCameraControlParam::Description, curveData),                sizeof(DvElementCameraControlParam::Description::curveData)}},
#endif
        {ElementID::GODRAY_PARAM,                       {offsetof(DvElementGodrayParam::Description, curveData),                       sizeof(DvElementGodrayParam::Description::curveData)}},
        {ElementID::CHROMATIC_ABERRATION_FILTER_PARAM,  {offsetof(DvElementChromaticAberrationFilterParam::Description, curveData),    sizeof(DvElementChromaticAberrationFilterParam::Description::curveData)}},
        {ElementID::VIGNETTE_PARAM,                     {offsetof(DvElementVignetteParam::Description, curveData),                     sizeof(DvElementVignetteParam::Description::curveData)}},
        {ElementID::FADE,                               {offsetof(DvElementFade::Description, curveData),                              sizeof(DvElementFade::Description::curveData)}},
        {ElementID::LETTER_BOX,                         {offsetof(DvElementLetterBox::Description, curveData),                         sizeof(DvElementLetterBox::Description::curveData)}},
        {ElementID::TIME,                               {offsetof(DvElementTime::Description, curveData),                              sizeof(DvElementTime::Description::curveData)}},
        {ElementID::SUN,                                {offsetof(DvElementSun::Description, curveData),                               sizeof(DvElementSun::Description::curveData)}},
        {ElementID::LOOK_AT_IK,                         {offsetof(DvElementLookAtIK::Description, curveData),                          sizeof(DvElementLookAtIK::Description::curveData)}},
        {ElementID::CAMERA_BLUR_PARAM,                  {offsetof(DvElementCameraBlurParam::Description, curveData),                   sizeof(DvElementCameraBlurParam::Description::curveData)}},
        {ElementID::FACIAL_ANIMATION,                   {offsetof(DvElementFacialAnimation::Description, curveData),                   sizeof(DvElementFacialAnimation::Description::curveData)}},
        {ElementID::AURA,                               {offsetof(DvElementAura::Description, curveData),                              sizeof(DvElementAura::Description::curveData)}},
        {ElementID::CYBER_SPACE_NOISE,                  {offsetof(DvElementCyberSpaceNoise::Description, curveData),                   sizeof(DvElementCyberSpaceNoise::Description::curveData)}},
        {ElementID::CROSS_FADE,                         {offsetof(DvElementCrossFade::Description, curveData),                         sizeof(DvElementCrossFade::Description::curveData)}},
        {ElementID::WEATHER,                            {offsetof(DvElementWeather::Description, curveData),                           sizeof(DvElementWeather::Description::curveData)}},
        {ElementID::VARIABLE_POINT_LIGHT,               {offsetof(DvElementVariablePointLight::Description, curveData),                sizeof(DvElementVariablePointLight::Description::curveData)}},
#ifdef DEVTOOLS_TARGET_SDK_rangers
        {ElementID::THE_END_CABLE_OBJECT,               {offsetof(DvElementTheEndCable::Description, curveData),                       sizeof(DvElementTheEndCable::Description::curveData)}},
#elif DEVTOOLS_TARGET_SDK_miller
        {ElementID::FOG,                                {offsetof(DvElementFog::Description, curveData),                               sizeof(DvElementFog::Description::curveData)}},
        {ElementID::DOF,                                {offsetof(DvElementDOF::Description, curveData),                               sizeof(DvElementDOF::Description::curveData)}},
#endif
    };

    constexpr std::pair<size_t, size_t> GetElementTimelineRender(ElementID type) {
        for (const auto& [key, func] : RenderTimelineElements) {
            if (key == type) return func;
        }
        return {};
    }
}
