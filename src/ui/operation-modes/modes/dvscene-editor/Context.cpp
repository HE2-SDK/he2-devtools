#include "Context.h"
#include "timeline/Elements.h"

using namespace hh::dv;
using namespace app::dv;

namespace ui::operation_modes::modes::dvscene_editor {
    constexpr std::pair<size_t, size_t> NodeDataSize[] = {
        {},
        {sizeof(DvNodePath), sizeof(DvNodePath::Description)},
        {},
        {sizeof(DvNodeCamera), sizeof(DvNodeCamera::Description)},
        {sizeof(DvNodeCameraMotion), sizeof(DvNodeCameraMotion::Description)},
        {sizeof(DvNodeCharacter), sizeof(DvNodeCharacter::Description)},
        {sizeof(DvNodeCharacterMotion), sizeof(DvNodeCharacterMotion::Description)},
        {},
        {sizeof(DvNodeModel), sizeof(DvNodeModel::Description)},
        {},
        {sizeof(DvNodeModelMotion), sizeof(DvNodeModelMotion::Description)},
        {sizeof(DvNodeModelNode), sizeof(DvNodeModelNode::Description)},
        {sizeof(DvNodeElement), sizeof(DvNodeElement::Description<DvElementBase::Description>)}
    };

#ifdef DEVTOOLS_TARGET_SDK_rangers
    constexpr size_t ElementDataSize[] = {
        0, // 0
        sizeof(DvElementCameraParams::Description), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(DvElementDrawOff::Description), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(DvElementPathOffset::Description), // PATH_OFFSET
        sizeof(DvElementCameraShake::Description), // CAMERA_SHAKE
        sizeof(DvElementCameraShakeLoop::Description), // CAMERA_SHAKE_LOOP
        /*sizeof(DvElementEffect::Description)*/676, // EFFECT
        sizeof(DvElementDirectionalLight::Description), // DIRECTIONAL_LIGHT
        sizeof(DvElementPathInterpolation::Description), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(DvElementCameraNearFar::Description), // CAMERA_NEAR_FAR
        sizeof(DvElementUVAnim::Description), // UV_ANIM
        sizeof(DvElementVisibilityAnim::Description), // VISIBILITY_ANIM
        sizeof(DvElementMaterialAnim::Description), // MATERIAL_ANIM
        sizeof(DvElementMultipleAnim::Description), // MULTIPLE_ANIM
        sizeof(DvElementCameraOffset::Description), // CAMERA_OFFSET
        sizeof(DvElementModelFade::Description), // MODEL_FADE
        sizeof(DvElementDebugMotion::Description), // DEBUG_MOTION
        sizeof(DvElementCameraHedgehog::Description), // CAMERA_HEDGEHOG
        sizeof(DvElementCameraInGame::Description), // CAMERA_IN_GAME
        sizeof(DvElementPointLight::Description), // POINT_LIGHT
        sizeof(DvElementVertexAnimationTexture::Description), // VERTEX_ANIMATION_TEXTURE
        sizeof(DvElementSpotlight::Description), // SPOTLIGHT
        sizeof(DvElementControllerVibration::Description), // CONTROLLER_VIBRATION
        sizeof(DvElementSpotlightModel::Description), // SPOTLIGHT_MODEL

        sizeof(DvElementBloomParam::Description), // BLOOM_PARAM
        sizeof(DvElementDOFParam::Description), // DOF_PARAM
        sizeof(DvElementColorContrast::Description), // COLOR_CONTRAST
        sizeof(DvElementCameraControlParam::Description), // CAMERA_CONTROL_PARAM
        sizeof(DvElementShadowResolution::Description), // SHADOW_RESOLUTION
        sizeof(DvElementGodrayParam::Description), // GODRAY_PARAM
        sizeof(DvElementAtmosphereGodrayParam::Description), // ATMOSPHERE_GODRAY_PARAM
        sizeof(DvElementAtmosphereHeightFogParam::Description), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(DvElementChromaticAberrationFilterParam::Description), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(DvElementVignetteParam::Description), // VIGNETTE_PARAM
        sizeof(DvElementFade::Description), // FADE
        sizeof(DvElementLetterBox::Description), // LETTER_BOX
        sizeof(DvElementModelClipping::Description), // MODEL_CLIPPING
        sizeof(DvElementPbaReset::Description), // PBA_RESET
        sizeof(DvElementBossName::Description), // BOSS_NAME
        sizeof(DvElementCaption::Description), // CAPTION
        sizeof(DvElementSound::Description), // SOUND
        sizeof(DvElementTime::Description), // TIME
        sizeof(DvElementSun::Description), // SUN
        sizeof(DvElementLookAtIK::Description), // LOOK_AT_IK
        sizeof(DvElementCameraBlurParam::Description), // CAMERA_BLUR_PARAM
        sizeof(DvElementGeneralTrigger::Description), // GENERAL_TRIGGER
        sizeof(DvElementFootIk::Description), // FOOT_IK
        sizeof(DvElementDitherParam::Description), // DITHER_PARAM
        sizeof(DvElementQTE::Description), // QTE
        sizeof(DvElementFacialAnimation::Description), // FACIAL_ANIMATION
        sizeof(DvElementOverrideASM::Description), // OVERRIDE_ASM
        sizeof(DvElementAura::Description), // AURA
        sizeof(DvElementChangeTimeScale::Description), // CHANGE_TIME_SCALE
        sizeof(DvElementCyberSpaceNoise::Description), // CYBER_SPACE_NOISE
        sizeof(DvElementLipAnimation::Description), // LIP_ANIMATION
        sizeof(DvElementAuraRoad::Description), // AURA_ROAD
        0, // MOVIE_VIEW
        sizeof(DvElementCrossFade::Description), // CROSS_FADE
        sizeof(DvElementWeather::Description), // WEATHER
        sizeof(DvElementShadowMapParam::Description), // SHADOW_MAP_PARAM
        sizeof(DvElementVariablePointLight::Description), // VARIABLE_POINT_LIGHT
        0, // OPENING_LOGO
        sizeof(DvElementDensitySectorPoint::Description), // DENSITY_SECTOR_POINT
        0, // FX_COL_UPDATE
        sizeof(DvElementQTEAccel::Description), // QTE_ACCEL
        sizeof(DvElementTheEndCable::Description), // THE_END_CABLE_OBJECT
        sizeof(DvElementRifleBeastLighting::Description), // RIFLE_BEAST_LIGHTING
    };
#elif DEVTOOLS_TARGET_SDK_miller
    constexpr size_t ElementDataSize[] = {
        0, // 0 (Unused)
        sizeof(DvElementCameraParams::Description), // CAMERA_PARAMS
        0, // 2 (Empty)
        sizeof(DvElementDrawOff::Description), // DRAW_OFF
        0, // 4 (Empty)
        sizeof(DvElementPathOffset::Description), // PATH_OFFSET
        sizeof(DvElementCameraShake::Description), // CAMERA_SHAKE
        sizeof(DvElementCameraShakeLoop::Description), // CAMERA_SHAKE_LOOP
        sizeof(DvElementEffect::Description), // EFFECT
        sizeof(DvElementDirectionalLight::Description), // DIRECTIONAL_LIGHT
        sizeof(DvElementPathInterpolation::Description), // PATH_INTERPOLATION
        0, // CULLING_DISABLE
        sizeof(DvElementCameraNearFar::Description), // CAMERA_NEAR_FAR
        sizeof(DvElementUVAnim::Description), // UV_ANIM
        sizeof(DvElementVisibilityAnim::Description), // VISIBILITY_ANIM
        sizeof(DvElementMaterialAnim::Description), // MATERIAL_ANIM
        sizeof(DvElementMultipleAnim::Description), // MULTIPLE_ANIM
        sizeof(DvElementCameraOffset::Description), // CAMERA_OFFSET
        sizeof(DvElementModelFade::Description), // MODEL_FADE
        sizeof(DvElementDebugMotion::Description), // DEBUG_MOTION
        sizeof(DvElementCameraHedgehog::Description), // CAMERA_HEDGEHOG
        sizeof(DvElementCameraInGame::Description), // CAMERA_IN_GAME
        sizeof(DvElementPointLight::Description), // POINT_LIGHT
        sizeof(DvElementVertexAnimationTexture::Description), // VERTEX_ANIMATION_TEXTURE
        sizeof(DvElementSpotlight::Description), // SPOTLIGHT
        sizeof(DvElementControllerVibration::Description), // CONTROLLER_VIBRATION
        sizeof(DvElementTexturePatternAnim::Description), // TEXTURE_PATTERN_ANIM
        sizeof(DvElementMaterialParam::Description), // MATERIAL_PARAM

        sizeof(DvElementBloomParam::Description), // BLOOM_PARAM
        sizeof(DvElementDOFParam::Description), // DOF_PARAM
        sizeof(DvElementColorContrast::Description), // COLOR_CONTRAST
        sizeof(DvElementCameraControlParam::Description), // CAMERA_CONTROL
        sizeof(DvElementShadowResolution::Description), // SHADOW_RESOLUTION
        sizeof(DvElementSSAOParam::Description), // SSAO_PARAM
        sizeof(DvElementOcclusionCapsuleParam::Description), // OCCLUSION_CAPSULE_PARAM
        sizeof(DvElementGodrayParam::Description), // GODRAY_PARAM
        sizeof(DvElementAtmosphereGodrayParam::Description), // ATMOSPHERE_GODRAY_PARAM
        sizeof(DvElementAtmosphereHeightFogParam::Description), // ATMOSPHERE_HEIGHT_FOG_PARAM
        sizeof(DvElementChromaticAberrationFilterParam::Description), // CHROMATIC_ABERRATION_FILTER_PARAM
        sizeof(DvElementVignetteParam::Description), // VIGNETTE_PARAM
        sizeof(DvElementFade::Description), // FADE
        sizeof(DvElementLetterBox::Description), // LETTER_BOX
        sizeof(DvElementModelClipping::Description), // MODEL_CLIPPING
        sizeof(DvElementPbaReset::Description), // PBA_RESET
        sizeof(DvElementBossName::Description), // BOSS_NAME
        sizeof(DvElementCaption::Description), // CAPTION
        sizeof(DvElementSound::Description), // SOUND
        sizeof(DvElementTime::Description), // TIME
        sizeof(DvElementSun::Description), // SUN
        sizeof(DvElementLookAtIK::Description), // LOOK_AT_IK
        sizeof(DvElementCameraBlurParam::Description), // CAMERA_BLUR_PARAM
        sizeof(DvElementGeneralTrigger::Description), // GENERAL_TRIGGER
        sizeof(DvElementFootIk::Description), // FOOT_IK
        sizeof(DvElementDitherParam::Description), // DITHER_PARAM
        sizeof(DvElementQTE::Description), // QTE
        sizeof(DvElementFacialAnimation::Description), // FACIAL_ANIMATION
        sizeof(DvElementOverrideAsm::Description), // OVERRIDE_ASM
        sizeof(DvElementAura::Description), // AURA
        sizeof(DvElementChangeTimeScale::Description), // CHANGE_TIME_SCALE
        sizeof(DvElementCyberSpaceNoise::Description), // CYBER_SPACE_NOISE
        sizeof(DvElementLipAnimation::Description), // LIP_ANIMATION
        sizeof(DvElementAuraRoad::Description), // AURA_ROAD
        sizeof(DvElementMovieView::Description), // MOVIE_VIEW
        sizeof(DvElementCrossFade::Description), // CROSS_FADE
        sizeof(DvElementWeather::Description), // WEATHER
        sizeof(DvElementShadowMapParam::Description), // SHADOW_MAP_PARAM
        sizeof(DvElementVariablePointLight::Description), // VARIABLE_POINT_LIGHT
        sizeof(DvElementOpeningLogo::Description), // OPENING_LOGO
        sizeof(DvElementDensitySectorPoint::Description), // DENSITY_SECTOR_POINT
        sizeof(DvElementBulletTime::Description), // BULLET_TIME
        sizeof(DvElementTimeStop::Description), // TIME_STOP
        sizeof(DvElementObjectTimeStop::Description), // OBJECT_TIME_STOP
        sizeof(DvElementShadowShape::Description), // SHADOW_SHAPE
        sizeof(DvElementFalloff::Description), // FALLOFF
        sizeof(DvElementFog::Description), // FOG
        sizeof(DvElementDOF::Description), // DOF
        sizeof(DvElementFxColUpdate::Description), // FX_COL_UPDATE
    };
#endif


    void Context::ContainsElement(bool& contains, const unsigned int& elementId, DvNodeBase* node)
    {
        if (contains)
            return;

        if (node->nodeType == DvNodeBase::NodeType::ELEMENT) {
            auto* element = reinterpret_cast<DvNodeElement*>(node);
            if (static_cast<unsigned int>(element->binaryData.elementId) == elementId){
                contains = true;
                return;
            }
        }

        for (auto* x : node->childrenElements0)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements1)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenElements2)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenPath)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenCamera)
            ContainsElement(contains, elementId, x);
        for (auto* x : node->childrenMotion)
            ContainsElement(contains, elementId, x);
    }

    void Context::GetFileNode(dv::DvNode& curNode, DvNodeBase* node, dv::DvNode*& result)
    {
        if (strcmp(curNode.name, node->nodeName.c_str()) == 0 && memcmp(reinterpret_cast<char*>(&curNode.guid), node->guid, 16) == 0 && curNode.category == static_cast<int>(node->nodeType)) {
            result = &curNode;
            return;
        }
        for (auto& x : curNode.childNodes)
            GetFileNode(x, node, result);
    }

    void Context::GetFilePage(dv::DvPage& curPage, hh::dv::DvPage* page, dv::DvPage*& result)
    {
        if (memcmp(curPage.name, page->binaryData.pageName, 32) == 0 && curPage.index == page->binaryData.pageIndex)
            result = &curPage;
    }

    void Context::GetRuntimeNode(DvNodeBase* curNode, dv::DvNode* node, DvNodeBase*& result)
    {
        if (strcmp(node->name, curNode->nodeName.c_str()) == 0 && memcmp(reinterpret_cast<char*>(&node->guid), curNode->guid, 16) == 0 && node->category == static_cast<int>(curNode->nodeType)) {
            result = curNode;
            return;
        }
        for (auto* x : curNode->childrenCamera)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenPath)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenMotion)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements0)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements1)
            GetRuntimeNode(x, node, result);
        for (auto* x : curNode->childrenElements2)
            GetRuntimeNode(x, node, result);
    }

    void Context::GetParentNode(dv::DvNode& curNode, dv::DvNode* node, dv::DvNode*& result)
    {
        for(auto& x : curNode.childNodes) {
            if (strcmp(x.name, node->name) == 0 && memcmp(reinterpret_cast<char*>(&x.guid), reinterpret_cast<char*>(&node->guid), 16) == 0)
            {
                result = &curNode;
                return;
            }
            GetParentNode(x, node, result);
        }
    }

    void Context::GetChildren(DvNodeBase* node, csl::ut::MoveArray32<DvNodeBase*>& value, bool& includeElements)
    {
        if (node->nodeType == DvNodeBase::NodeType::ELEMENT && !includeElements)
            return;

        value.push_back(node);

        for (auto* x : node->childrenCamera)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenPath)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenMotion)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements0)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements1)
            GetChildren(x, value, includeElements);
        for (auto* x : node->childrenElements2)
            GetChildren(x, value, includeElements);
    }

    bool Context::ContainsElement(const unsigned int& elementId)
    {
        bool contains = false;
        if (goDVSC)
            ContainsElement(contains, elementId, goDVSC->nodeTree->mainNode);
        return contains;
    }

    void Context::GetNodes(DvSceneNodeTree* nodeTree, csl::ut::MoveArray32<DvNodeBase*>& value, bool includeElements)
    {
        GetChildren(nodeTree->mainNode, value, includeElements);
    }

    void Context::DispatchSetupElement(DvNodeElement* node, DvNodeElement::ElementID elementId)
    {
        using ElementID = DvNodeElement::ElementID;

        switch (elementId) {
#ifdef DEVTOOLS_TARGET_SDK_rangers
        case ElementID::CAMERA_PARAMS:               SetupElement<DvElementCameraParams>(node, elementId); break;
        case ElementID::DRAW_OFF:                    SetupElement<DvElementDrawOff>(node, elementId); break;
        case ElementID::PATH_OFFSET:                 SetupElement<DvElementPathOffset>(node, elementId); break;
        case ElementID::CAMERA_SHAKE:                SetupElement<DvElementCameraShake>(node, elementId); break;
        case ElementID::CAMERA_SHAKE_LOOP:           SetupElement<DvElementCameraShakeLoop>(node, elementId); break;
        case ElementID::EFFECT:                      SetupElement<DvElementEffect>(node, elementId); break;
        case ElementID::DIRECTIONAL_LIGHT:           SetupElement<DvElementDirectionalLight>(node, elementId); break;
        case ElementID::PATH_INTERPOLATION:          SetupElement<DvElementPathInterpolation>(node, elementId); break;
        case ElementID::CAMERA_NEAR_FAR:             SetupElement<DvElementCameraNearFar>(node, elementId); break;
        case ElementID::UV_ANIM:                     SetupElement<DvElementUVAnim>(node, elementId); break;
        case ElementID::VISIBILITY_ANIM:             SetupElement<DvElementVisibilityAnim>(node, elementId); break;
        case ElementID::MATERIAL_ANIM:               SetupElement<DvElementMaterialAnim>(node, elementId); break;
        case ElementID::MULTIPLE_ANIM:               SetupElement<DvElementMultipleAnim>(node, elementId); break;
        case ElementID::CAMERA_OFFSET:               SetupElement<DvElementCameraOffset>(node, elementId); break;
        case ElementID::MODEL_FADE:                  SetupElement<DvElementModelFade>(node, elementId); break;
        case ElementID::DEBUG_MOTION:                SetupElement<DvElementDebugMotion>(node, elementId); break;
        case ElementID::CAMERA_HEDGEHOG:             SetupElement<DvElementCameraHedgehog>(node, elementId); break;
        case ElementID::CAMERA_IN_GAME:              SetupElement<DvElementCameraInGame>(node, elementId); break;
        case ElementID::POINT_LIGHT:                 SetupElement<DvElementPointLight>(node, elementId); break;
        case ElementID::VERTEX_ANIMATION_TEXTURE:    SetupElement<DvElementVertexAnimationTexture>(node, elementId); break;
        case ElementID::SPOTLIGHT:                   SetupElement<DvElementSpotlight>(node, elementId); break;
        case ElementID::CONTROLLER_VIBRATION:        SetupElement<DvElementControllerVibration>(node, elementId); break;
        case ElementID::SPOTLIGHT_MODEL:             SetupElement<DvElementSpotlightModel>(node, elementId); break;

        case ElementID::BLOOM_PARAM:                 SetupElement<DvElementBloomParam>(node, elementId); break;
        case ElementID::DOF_PARAM:                   SetupElement<DvElementDOFParam>(node, elementId); break;
        case ElementID::COLOR_CONTRAST:              SetupElement<DvElementColorContrast>(node, elementId); break;
        case ElementID::CAMERA_CONTROL_PARAM:        SetupElement<DvElementCameraControlParam>(node, elementId); break;
        case ElementID::SHADOW_RESOLUTION:           SetupElement<DvElementShadowResolution>(node, elementId); break;
        case ElementID::GODRAY_PARAM:                SetupElement<DvElementGodrayParam>(node, elementId); break;
        case ElementID::ATMOSPHERE_GODRAY_PARAM:     SetupElement<DvElementAtmosphereGodrayParam>(node, elementId); break;
        case ElementID::ATMOSPHERE_HEIGHT_FOG_PARAM: SetupElement<DvElementAtmosphereHeightFogParam>(node, elementId); break;
        case ElementID::CHROMATIC_ABERRATION_FILTER_PARAM: SetupElement<DvElementChromaticAberrationFilterParam>(node, elementId); break;
        case ElementID::VIGNETTE_PARAM:              SetupElement<DvElementVignetteParam>(node, elementId); break;
        case ElementID::FADE:                        SetupElement<DvElementFade>(node, elementId); break;
        case ElementID::LETTER_BOX:                  SetupElement<DvElementLetterBox>(node, elementId); break;
        case ElementID::MODEL_CLIPPING:              SetupElement<DvElementModelClipping>(node, elementId); break;
        case ElementID::PBA_RESET:                   SetupElement<DvElementPbaReset>(node, elementId); break;
        case ElementID::BOSS_NAME:                   SetupElement<DvElementBossName>(node, elementId); break;
        case ElementID::CAPTION:                     SetupElement<DvElementCaption>(node, elementId); break;
        case ElementID::SOUND:                       SetupElement<DvElementSound>(node, elementId); break;
        case ElementID::TIME:                        SetupElement<DvElementTime>(node, elementId); break;
        case ElementID::SUN:                         SetupElement<DvElementSun>(node, elementId); break;
        case ElementID::LOOK_AT_IK:                  SetupElement<DvElementLookAtIK>(node, elementId); break;
        case ElementID::CAMERA_BLUR_PARAM:           SetupElement<DvElementCameraBlurParam>(node, elementId); break;
        case ElementID::GENERAL_TRIGGER:             SetupElement<DvElementGeneralTrigger>(node, elementId); break;
        case ElementID::FOOT_IK:                     SetupElement<DvElementFootIk>(node, elementId); break;
        case ElementID::DITHER_PARAM:                SetupElement<DvElementDitherParam>(node, elementId); break;
        case ElementID::QTE:                         SetupElement<DvElementQTE>(node, elementId); break;
        case ElementID::FACIAL_ANIMATION:            SetupElement<DvElementFacialAnimation>(node, elementId); break;
        case ElementID::OVERRIDE_ASM:                SetupElement<DvElementOverrideASM>(node, elementId); break;
        case ElementID::AURA:                        SetupElement<DvElementAura>(node, elementId); break;
        case ElementID::CHANGE_TIME_SCALE:           SetupElement<DvElementChangeTimeScale>(node, elementId); break;
        case ElementID::CYBER_SPACE_NOISE:           SetupElement<DvElementCyberSpaceNoise>(node, elementId); break;
        case ElementID::LIP_ANIMATION:               SetupElement<DvElementLipAnimation>(node, elementId); break;
        case ElementID::AURA_ROAD:                   SetupElement<DvElementAuraRoad>(node, elementId); break;
        case ElementID::CROSS_FADE:                  SetupElement<DvElementCrossFade>(node, elementId); break;
        case ElementID::WEATHER:                     SetupElement<DvElementWeather>(node, elementId); break;
        case ElementID::SHADOW_MAP_PARAM:            SetupElement<DvElementShadowMapParam>(node, elementId); break;
        case ElementID::VARIABLE_POINT_LIGHT:        SetupElement<DvElementVariablePointLight>(node, elementId); break;
        case ElementID::DENSITY_SECTOR_POINT:        SetupElement<DvElementDensitySectorPoint>(node, elementId); break;
        case ElementID::QTE_ACCEL:                   SetupElement<DvElementQTEAccel>(node, elementId); break;
        case ElementID::THE_END_CABLE_OBJECT:        SetupElement<DvElementTheEndCable>(node, elementId); break;
        case ElementID::RIFLE_BEAST_LIGHTING:        SetupElement<DvElementRifleBeastLighting>(node, elementId); break;
#elif DEVTOOLS_TARGET_SDK_miller
        case ElementID::CAMERA_PARAMS:               SetupElement<DvElementCameraParams>(node, elementId); break;
        case ElementID::DRAW_OFF:                    SetupElement<DvElementDrawOff>(node, elementId); break;
        case ElementID::PATH_OFFSET:                 SetupElement<DvElementPathOffset>(node, elementId); break;
        case ElementID::CAMERA_SHAKE:                SetupElement<DvElementCameraShake>(node, elementId); break;
        case ElementID::CAMERA_SHAKE_LOOP:           SetupElement<DvElementCameraShakeLoop>(node, elementId); break;
        case ElementID::EFFECT:                      SetupElement<DvElementEffect>(node, elementId); break;
        case ElementID::DIRECTIONAL_LIGHT:           SetupElement<DvElementDirectionalLight>(node, elementId); break;
        case ElementID::PATH_INTERPOLATION:          SetupElement<DvElementPathInterpolation>(node, elementId); break;
        case ElementID::CAMERA_NEAR_FAR:             SetupElement<DvElementCameraNearFar>(node, elementId); break;
        case ElementID::UV_ANIM:                     SetupElement<DvElementUVAnim>(node, elementId); break;
        case ElementID::VISIBILITY_ANIM:             SetupElement<DvElementVisibilityAnim>(node, elementId); break;
        case ElementID::MATERIAL_ANIM:               SetupElement<DvElementMaterialAnim>(node, elementId); break;
        case ElementID::MULTIPLE_ANIM:               SetupElement<DvElementMultipleAnim>(node, elementId); break;
        case ElementID::CAMERA_OFFSET:               SetupElement<DvElementCameraOffset>(node, elementId); break;
        case ElementID::MODEL_FADE:                  SetupElement<DvElementModelFade>(node, elementId); break;
        case ElementID::DEBUG_MOTION:                SetupElement<DvElementDebugMotion>(node, elementId); break;
        case ElementID::CAMERA_HEDGEHOG:             SetupElement<DvElementCameraHedgehog>(node, elementId); break;
        case ElementID::CAMERA_IN_GAME:              SetupElement<DvElementCameraInGame>(node, elementId); break;
        case ElementID::POINT_LIGHT:                 SetupElement<DvElementPointLight>(node, elementId); break;
        case ElementID::VERTEX_ANIMATION_TEXTURE:    SetupElement<DvElementVertexAnimationTexture>(node, elementId); break;
        case ElementID::SPOTLIGHT:                   SetupElement<DvElementSpotlight>(node, elementId); break;
        case ElementID::CONTROLLER_VIBRATION:        SetupElement<DvElementControllerVibration>(node, elementId); break;
        case ElementID::TEXTURE_PATTERN_ANIM:        SetupElement<DvElementTexturePatternAnim>(node, elementId); break;
        case ElementID::MATERIAL_PARAM:              SetupElement<DvElementMaterialParam>(node, elementId); break;

        case ElementID::BLOOM_PARAM:                 SetupElement<DvElementBloomParam>(node, elementId); break;
        case ElementID::DOF_PARAM:                   SetupElement<DvElementDOFParam>(node, elementId); break;
        case ElementID::COLOR_CONTRAST:              SetupElement<DvElementColorContrast>(node, elementId); break;
        case ElementID::CAMERA_CONTROL:              SetupElement<DvElementCameraControlParam>(node, elementId); break;
        case ElementID::SHADOW_RESOLUTION:           SetupElement<DvElementShadowResolution>(node, elementId); break;
        case ElementID::SSAO_PARAM:                  SetupElement<DvElementSSAOParam>(node, elementId); break;
        case ElementID::OCCLUSION_CAPSULE_PARAM:     SetupElement<DvElementOcclusionCapsuleParam>(node, elementId); break;
        case ElementID::GODRAY_PARAM:                SetupElement<DvElementGodrayParam>(node, elementId); break;
        case ElementID::ATMOSPHERE_GODRAY_PARAM:     SetupElement<DvElementAtmosphereGodrayParam>(node, elementId); break;
        case ElementID::ATMOSPHERE_HEIGHT_FOG_PARAM: SetupElement<DvElementAtmosphereHeightFogParam>(node, elementId); break;
        case ElementID::CHROMATIC_ABERRATION_FILTER_PARAM: SetupElement<DvElementChromaticAberrationFilterParam>(node, elementId); break;
        case ElementID::VIGNETTE_PARAM:              SetupElement<DvElementVignetteParam>(node, elementId); break;
        case ElementID::FADE:                        SetupElement<DvElementFade>(node, elementId); break;
        case ElementID::LETTER_BOX:                  SetupElement<DvElementLetterBox>(node, elementId); break;
        case ElementID::MODEL_CLIPPING:              SetupElement<DvElementModelClipping>(node, elementId); break;
        case ElementID::PBA_RESET:                   SetupElement<DvElementPbaReset>(node, elementId); break;
        case ElementID::BOSS_NAME:                   SetupElement<DvElementBossName>(node, elementId); break;
        case ElementID::CAPTION:                     SetupElement<DvElementCaption>(node, elementId); break;
        case ElementID::SOUND:                       SetupElement<DvElementSound>(node, elementId); break;
        case ElementID::TIME:                        SetupElement<DvElementTime>(node, elementId); break;
        case ElementID::SUN:                         SetupElement<DvElementSun>(node, elementId); break;
        case ElementID::LOOK_AT_IK:                  SetupElement<DvElementLookAtIK>(node, elementId); break;
        case ElementID::CAMERA_BLUR_PARAM:           SetupElement<DvElementCameraBlurParam>(node, elementId); break;
        case ElementID::GENERAL_TRIGGER:             SetupElement<DvElementGeneralTrigger>(node, elementId); break;
        case ElementID::FOOT_IK:                     SetupElement<DvElementFootIk>(node, elementId); break;
        case ElementID::DITHER_PARAM:                SetupElement<DvElementDitherParam>(node, elementId); break;
        case ElementID::QTE:                         SetupElement<DvElementQTE>(node, elementId); break;
        case ElementID::FACIAL_ANIMATION:            SetupElement<DvElementFacialAnimation>(node, elementId); break;
        case ElementID::OVERRIDE_ASM:                SetupElement<DvElementOverrideAsm>(node, elementId); break;
        case ElementID::AURA:                        SetupElement<DvElementAura>(node, elementId); break;
        case ElementID::CHANGE_TIME_SCALE:           SetupElement<DvElementChangeTimeScale>(node, elementId); break;
        case ElementID::CYBER_SPACE_NOISE:           SetupElement<DvElementCyberSpaceNoise>(node, elementId); break;
        case ElementID::LIP_ANIMATION:               SetupElement<DvElementLipAnimation>(node, elementId); break;
        case ElementID::AURA_ROAD:                   SetupElement<DvElementAuraRoad>(node, elementId); break;
        case ElementID::MOVIE_VIEW:                  SetupElement<DvElementMovieView>(node, elementId); break;
        case ElementID::CROSS_FADE:                  SetupElement<DvElementCrossFade>(node, elementId); break;
        case ElementID::WEATHER:                     SetupElement<DvElementWeather>(node, elementId); break;
        case ElementID::SHADOW_MAP_PARAM:            SetupElement<DvElementShadowMapParam>(node, elementId); break;
        case ElementID::VARIABLE_POINT_LIGHT:        SetupElement<DvElementVariablePointLight>(node, elementId); break;
        case ElementID::OPENING_LOGO:                SetupElement<DvElementOpeningLogo>(node, elementId); break;
        case ElementID::DENSITY_SECTOR_POINT:        SetupElement<DvElementDensitySectorPoint>(node, elementId); break;
        case ElementID::BULLET_TIME:                 SetupElement<DvElementBulletTime>(node, elementId); break;
        case ElementID::TIME_STOP:                   SetupElement<DvElementTimeStop>(node, elementId); break;
        case ElementID::OBJECT_TIME_STOP:            SetupElement<DvElementObjectTimeStop>(node, elementId); break;
        case ElementID::SHADOW_SHAPE:                SetupElement<DvElementShadowShape>(node, elementId); break;
        case ElementID::FALLOFF:                     SetupElement<DvElementFalloff>(node, elementId); break;
        case ElementID::FOG:                         SetupElement<DvElementFog>(node, elementId); break;
        case ElementID::DOF:                         SetupElement<DvElementDOF>(node, elementId); break;
        case ElementID::FX_COL_UPDATE:               SetupElement<DvElementFxColUpdate>(node, elementId); break;
#endif
        }
    }

    void Context::SetNodeBasicProps(DvNodeBase* node, const char* nodeName, unsigned int nodeType, DvNodeBase* parent)
    {
        GenerateGUID(node->guid);
        node->dvsceneNodeTree = parent->dvsceneNodeTree;
        node->parent = parent;
        node->nodeType = static_cast<DvNodeBase::NodeType>(nodeType);
        for (int x = 0; x < strlen(nodeName) + 1; x++)
            node->nodeName.push_back(nodeName[x]);
    }

    DvNodeBase* Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId, DvNodeBase* parent)
    {
        DvNodeBase* node;
        auto* allocator = parent->dvsceneNodeTree->GetAllocator();
        switch (static_cast<DvNodeBase::NodeType>(nodeType)) {
        case DvNodeBase::NodeType::PATH: {
            node = new (allocator) DvNodePath{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodePath::Description setup{};
            setup.matrix = csl::math::Matrix44::Identity();
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CAMERA: {
            node = new (allocator) DvNodeCamera{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCamera::Description setup{};
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CAMERA_MOTION: {
            node = new (allocator) DvNodeCameraMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCameraMotion::Description setup{};
            setup.end = 100;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CHARACTER: {
            node = new (allocator) DvNodeCharacter{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCharacter::Description setup{};
            strcpy(setup.modelName, "chr_sonic");
            strcpy(setup.skeletonName, "chr_sonic");
            strcpy(setup.name3, "sn");
            setup.useName3 = true;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::CHARACTER_MOTION: {
            node = new (allocator) DvNodeCharacterMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeCharacterMotion::Description setup{};
            setup.end = 100;
            strcpy(setup.asmState, "Dst0000");
            setup.speed = 1.0f;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL: {
            node = new (allocator) DvNodeModel{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModel::Description setup{};
            strcpy(setup.modelName, "chr_sonic");
            strcpy(setup.skeletonName, "chr_sonic");
            strcpy(setup.name3, "sn");
            setup.useName3 = true;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL_MOTION: {
            node = new (allocator) DvNodeModelMotion{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModelMotion::Description setup{};
            setup.end = 100;
            strcpy(setup.asmState, "Dst0000");
            setup.speed = 1.0f;
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::MODEL_NODE: {
            node = new (allocator) DvNodeModelNode{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            DvNodeModelNode::Description setup{};
            node->Setup(setup);
            break;
        }
        case DvNodeBase::NodeType::ELEMENT: {
            node = new (allocator) DvNodeElement{ allocator };
            SetNodeBasicProps(node, nodeName, nodeType, parent);
            switch (static_cast<DvNodeElement::ElementID>(elementId)) {
            case DvNodeElement::ElementID::PATH_OFFSET: {
                DvNodeElement::Description<DvElementPathOffset::Description>* setup = CreateElementDesc<DvElementPathOffset::Description>(elementId);
                setup->elementDescription.offsetMatrix = csl::math::Matrix44::Identity();
                node->Setup(*setup);
                break;
            }
            case DvNodeElement::ElementID::PATH_INTERPOLATION: {
                DvNodeElement::Description<DvElementPathInterpolation::Description>* setup = CreateElementDesc<DvElementPathInterpolation::Description>(elementId);
                setup->elementDescription.interpolation.scale = { 1,1,1 };
                setup->elementDescription.finishInterpolation.scale = { 1,1,1 };
                node->Setup(*setup);
                break;
            }
            case DvNodeElement::ElementID::EFFECT: {
                DvNodeElement::Description<DvElementEffect::Description>* setup = CreateElementDesc<DvElementEffect::Description>(elementId);
                setup->elementDescription.r = 255;
                setup->elementDescription.g = 255;
                setup->elementDescription.b = 255;
                setup->elementDescription.a = 255;
                setup->elementDescription.effectMatrix = csl::math::Matrix44::Identity();
                node->Setup(*setup);
                break;
            }
            case DvNodeElement::ElementID::FADE: {
                DvNodeElement::Description<DvElementFade::Description>* setup = CreateElementDesc<DvElementFade::Description>(elementId);
                setup->elementDescription.enabled = true;
                node->Setup(*setup);
                break;
            }
            case DvNodeElement::ElementID::UV_ANIM: 
            case DvNodeElement::ElementID::VISIBILITY_ANIM: 
            case DvNodeElement::ElementID::MATERIAL_ANIM: 
            case DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE: 
            {
                DvNodeElement::Description<DvElementUVAnim::Description>* setup = CreateElementDesc<DvElementUVAnim::Description>(elementId);
                setup->elementDescription.speed = 1.0f;
                node->Setup(*setup);
                break;
            }
            default: {
                DispatchSetupElement(reinterpret_cast<DvNodeElement*>(node), static_cast<DvNodeElement::ElementID>(elementId));
                break;
            }
            }
            break;
        }
        }
        return node;
    }

    dv::DvNode Context::CreateNode(const char* nodeName, unsigned int nodeType, unsigned int elementId)
    {
        dv::DvNode node{};
        size_t setupSize = NodeDataSize[nodeType].second;       
        if (static_cast<DvNodeBase::NodeType>(nodeType) == DvNodeBase::NodeType::ELEMENT) {
            if (elementId >= 1000)
                setupSize += ElementDataSize[elementId + hhElementCount - 1000];
            else
                setupSize += ElementDataSize[elementId];
        }
        node.data = new char[setupSize];
        node.dataSize = setupSize;
        char* setup = node.data;
        memset(setup, 0x00, setupSize);
        switch (static_cast<DvNodeBase::NodeType>(nodeType)) {
        case DvNodeBase::NodeType::PATH:
            reinterpret_cast<DvNodePath::Description*>(setup)->matrix = csl::math::Matrix44::Identity();
            break;
        case DvNodeBase::NodeType::CAMERA_MOTION:
            reinterpret_cast<DvNodeCameraMotion::Description*>(setup)->end = 10000;
            break;
        case DvNodeBase::NodeType::CHARACTER: {
            auto* setupData = reinterpret_cast<DvNodeCharacter::Description*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case DvNodeBase::NodeType::CHARACTER_MOTION: {
            auto* setupData = reinterpret_cast<DvNodeCharacterMotion::Description*>(setup);
            setupData->end = 10000;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case DvNodeBase::NodeType::MODEL: {
            auto* setupData = reinterpret_cast<DvNodeModel::Description*>(setup);
            strcpy(setupData->modelName, "chr_sonic");
            strcpy(setupData->skeletonName, "chr_sonic");
            strcpy(setupData->name3, "sn");
            setupData->useName3 = 1;
            break;
        }
        case DvNodeBase::NodeType::MODEL_MOTION: {
            auto* setupData = reinterpret_cast<DvNodeModelMotion::Description*>(setup);
            setupData->end = 10000;
            strcpy(setupData->asmState, "Dst0000");
            setupData->speed = 1.0f;
            break;
        }
        case DvNodeBase::NodeType::ELEMENT: {
            
            auto* setupData = reinterpret_cast<DvNodeElement::DescriptionBase*>(setup);
            setupData->end = 100.0f;
            setupData->playType = DvNodeElement::PlayType::NORMAL;
            setupData->updateTiming = DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            setupData->elementId = static_cast<DvNodeElement::ElementID>(elementId);
            size_t sizeofElement = sizeof(DvNodeElement::DescriptionBase);
            switch (setupData->elementId) {
            case DvNodeElement::ElementID::PATH_OFFSET: {
                auto* elementData = reinterpret_cast<DvElementPathOffset::Description*>(reinterpret_cast<size_t>(setup) + sizeofElement);
                elementData->offsetMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case DvNodeElement::ElementID::PATH_INTERPOLATION: {
                auto* elementData = reinterpret_cast<DvElementPathInterpolation::Description*>(reinterpret_cast<size_t>(setup) + sizeofElement);
                elementData->interpolation.scale = { 1,1,1 };
                elementData->finishInterpolation.scale = { 1,1,1 };
                break;
            }
            case DvNodeElement::ElementID::EFFECT: {
                auto* elementData = reinterpret_cast<DvElementEffect::Description*>(reinterpret_cast<size_t>(setup) + sizeofElement);
                elementData->r = 255;
                elementData->g = 255;
                elementData->b = 255;
                elementData->a = 255;
                elementData->effectMatrix = csl::math::Matrix44::Identity();
                break;
            }
            case DvNodeElement::ElementID::FADE: {
                auto* elementData = reinterpret_cast<DvElementFade::Description*>(reinterpret_cast<size_t>(setup) + sizeofElement);
                elementData->enabled = true;
                break;
            }
            case DvNodeElement::ElementID::UV_ANIM:
            case DvNodeElement::ElementID::VISIBILITY_ANIM:
            case DvNodeElement::ElementID::MATERIAL_ANIM:
            case DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE:
            {
                auto* elementData = reinterpret_cast<DvElementUVAnim::Description*>(reinterpret_cast<size_t>(setup) + sizeofElement);
                elementData->speed = 1.0f;
                break;
            }
            }
            auto curveDataInfo = GetElementTimelineRender(static_cast<DvNodeElement::ElementID>(elementId));
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(setup) + sizeofElement + curveDataInfo.first), curveDataInfo.second / sizeof(float), 0, false);
            break;
        }
        }
        GenerateGUID(reinterpret_cast<char*>(&node.guid));
        node.category = nodeType;
        strcpy(node.name, nodeName);
        return node;
    }

    void Context::ParentNode(DvNodeBase* parent, DvNodeBase* child)
    {
        switch (child->nodeType) {
        case DvNodeBase::NodeType::PATH:
            parent->childrenPath.push_back(child);
            break;
        case DvNodeBase::NodeType::CAMERA:
            parent->childrenCamera.push_back(child);
            break;
        case DvNodeBase::NodeType::CAMERA_MOTION:
        case DvNodeBase::NodeType::CHARACTER_MOTION:
        case DvNodeBase::NodeType::MODEL_MOTION:
            parent->childrenMotion.push_back(child);
            break;
        default:
            parent->childrenElements0.push_back(child);
            break;
        }
    }

    void Context::ParentNode(dv::DvNode& parent, dv::DvNode& child)
    {
        parent.childNodes.push_back(child);
    }

    dv::DvNode* Context::GetFileNode(DvNodeBase* node)
    {
        dv::DvNode* result;
        GetFileNode(*parsedScene->dvCommon->node, node, result);
        return result;
    }

    dv::DvPage* Context::GetFilePage(hh::dv::DvPage* page)
    {
        dv::DvPage* result{};
        for (auto& x : parsedScene->dvCommon->pages) {
            if (!result)
                GetFilePage(x, page, result);
            else
                break;
        }
        return result;
    }

    DvNodeBase* Context::GetRuntimeNode(dv::DvNode* node)
    {
        DvNodeBase* result;
        GetRuntimeNode(goDVSC->nodeTree->mainNode, node, result);
        return result;
    }

    void Context::GenerateGUID(char* guid)
    {
        std::uniform_int_distribution<int> dis(0, 255);
        for (char x = 0; x < 16; x++)
            guid[x] = static_cast<char>(dis(mt));
    }

    dv::DvNode* Context::GetParentNode(dv::DvNode* node)
    {
        dv::DvNode* result;
        GetParentNode(*parsedScene->dvCommon->node, node, result);
        return result;
    }

    dv::Resource* Context::GetResourceByFileNode(dv::DvNode* node)
    {
        for(auto& res : parsedScene->dvResource)
            if(memcmp(&node->guid, &res.guid, 16) == 0)
                return &res;
        return nullptr;
    }

    dv::DvPage Context::CreatePage(const char* pageName, unsigned int idx)
    {
        dv::DvPage page{};
        page.frameEnd = 100;
        page.index = idx;
        page.skipFrame = 150;
        memset(page.name, 0, 32);
        strcpy(page.name, pageName);
        return page;
    }

    hh::dv::DvPage* Context::CreatePage(const char* pageName, unsigned int idx, DvSceneControl* dvsc)
    {
        auto* allocator = dvsc->GetAllocator();
        auto* rawMem = allocator->Alloc(sizeof(hh::dv::DvPage), alignof(hh::dv::DvPage));
        hh::dv::DvPage* page = new (rawMem) hh::dv::DvPage(allocator);
        page->dvSceneControl = dvsc;
        page->binaryData.end = 100;
        page->binaryData.pageIndex = idx;
        page->binaryData.skipFrame = 150;
        strcpy(page->binaryData.pageName, pageName);
        return page;
    }

    void Context::CreateWrapperPages()
    {
        dvPages.clear();
        for (auto* page : goDVSC->timeline->pages)
            dvPages.push_back(new DvPage(page, *this));
    }

    void Context::DeallocateNode(hh::dv::DvNodeBase* node)
    {
        if (node) {
            if (addedNodes.size() > 0) {
                if (addedNodes.find(node) != -1) {
                    if (node->nodeType == DvNodeBase::NodeType::ELEMENT) {
                        auto* nodeElement = static_cast<DvNodeElement*>(node);
                        if (static_cast<unsigned int>(nodeElement->binaryData.elementId) >= 1000) {
                            auto& appElement = *reinterpret_cast<hh::fnd::Reference<AppDvElementBase>*>(&nodeElement->element);
                            void* appElementData = appElement->elementBinaryData;
                            appElement->DeleteData();
                            hh::fnd::MemoryRouter::GetModuleAllocator()->Free(appElementData);
                        }
                    }
                }
                for (auto* x : node->childrenCamera)
                    DeallocateNode(node);
                for (auto* x : node->childrenPath)
                    DeallocateNode(node);
                for (auto* x : node->childrenMotion)
                    DeallocateNode(node);
                for (auto* x : node->childrenElements0)
                    DeallocateNode(node);
                for (auto* x : node->childrenElements1)
                    DeallocateNode(node);
                for (auto* x : node->childrenElements2)
                    DeallocateNode(node);
            }
        }
    }

    Context::Context(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator }, cutsceneName{ allocator }, nodeName{ allocator }, dvPages{ allocator }, addedNodes{ allocator } {}

    template<typename T>
    void Context::SetupElement(DvNodeElement* node, hh::dv::DvNodeElement::ElementID elementId)
    {
        DvNodeElement::Description<typename T::Description>* desc = CreateElementDesc<typename T::Description>(static_cast<unsigned int>(elementId));
        node->Setup(*desc);
    }

    template<typename T>
    DvNodeElement::Description<T>* Context::CreateElementDesc(unsigned int elementId)
    {
        if (elementId < 1000) {
            DvNodeElement::Description<T> desc{};
            desc.end = 100.0f;
            desc.playType = DvNodeElement::PlayType::NORMAL;
            desc.updateTiming = DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            desc.elementId = static_cast<DvNodeElement::ElementID>(elementId);
            desc.elementDescription = T{};
            auto curveDataInfo = GetElementTimelineRender(static_cast<DvNodeElement::ElementID>(elementId));
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(&desc.elementDescription) + curveDataInfo.first), curveDataInfo.second / sizeof(float), 0, false);
            return &desc;
        }
        else {
            DvNodeElement::Description<T>* desc = static_cast<DvNodeElement::Description<T>*>(hh::fnd::MemoryRouter::GetModuleAllocator()->Alloc(sizeof(DvNodeElement::Description<T>), alignof(DvNodeElement::Description<T>)));
            desc->end = 100.0f;
            desc->playType = DvNodeElement::PlayType::NORMAL;
            desc->updateTiming = DvNodeElement::UpdateTiming::ON_EXEC_PATH;
            desc->elementId = static_cast<DvNodeElement::ElementID>(elementId);
            desc->elementDescription = T{};
            auto curveDataInfo = GetElementTimelineRender(static_cast<DvNodeElement::ElementID>(elementId));
            if (curveDataInfo != std::pair<size_t, size_t>{})
                Timeline::GenerateCurve(reinterpret_cast<float*>(reinterpret_cast<size_t>(&desc->elementDescription) + curveDataInfo.first), curveDataInfo.second / sizeof(float), 0, false);
            return desc;
        }
    }
}
