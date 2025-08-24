#pragma once

bool Editor(const char* label, char* guid, hh::dv::DvSceneNodeTree* nodeTree);

bool SearchableCombo(const char* label, int* current_item, const char* const* items, int item_count, int searchBufSize);

bool Editor(const char* label, hh::dv::DvElementCameraHedgehog::Description::Camera& cam);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Description::Camera& data);
bool Editor(const char* label, hh::dv::DvElementCameraParams::Description::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Description::Flags>& flags, bool finishParams = false);
bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Description::Interpolation& data);
#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::dv::DvElementPointLight::Description::Parameters& data);
#endif

bool Editor(const char* label, app::dv::DvElementBloomParam::Description::Parameters& data);
bool Editor(const char* label, app::dv::DvElementDOFParam::Description::DOFParam& param);
bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Description::ChromaticAberrationParam& param);
bool Editor(const char* label, app::dv::DvElementLookAtIK::Description::Object& obj);
bool Editor(const char* label, app::dv::DvElementAura::Description::AuraNode& node);
bool Editor(const char* label, app::dv::DvElementVariablePointLight::Description::Parameters& data);
#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::ImageCircleParam& param);
bool Editor(const char* label, app::dv::DvElementCameraBlurParam::Description::BlurParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter::DistanceFogParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter::HeightFogParameter& param);
bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter& param);
bool Editor(const char* label, app::dv::DvElementDOF::Description::DOFParam& param);
#endif
bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::VignetteParam& param);
#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::DepthParam& param);
#endif
bool Editor(const char* label, app::dv::DvElementTime::Description::Time& param);