#include <ui/common/editors/Basic.h>
#include <ui/operation-modes/modes/dvscene-editor/Context.h>
#include "DvScene.h"

bool Editor(const char* label, char* guid, hh::dv::DvSceneNodeTree* nodeTree)
{
    bool changed = false;
    csl::ut::MoveArray32<hh::dv::DvNodeBase*> value{ nodeTree->GetAllocator() };
    ui::operation_modes::modes::dvscene_editor::Context::GetNodes(nodeTree, value, false);
    hh::dv::DvNodeBase* selectedNode;
    for (auto* x : value)
        if (strcmp(x->GetGUID(), guid) == 0) {
            selectedNode = x;
            break;
        }
    if (ImGui::BeginCombo(label, value.empty() ? "None" : selectedNode->nodeName.c_str())) {
        for (auto* x : value) {
            bool isSelected = x == selectedNode;
            if (changed |= ImGui::Selectable(x->nodeName.c_str(), isSelected))
                selectedNode = x;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    value.clear();
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraHedgehog::Description::Camera& cam) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Unk0", cam.unk0);
        changed |= Editor("Position", cam.position);
        changed |= Editor("Target Position", cam.targetPosition);
        changed |= Editor("Rotation", cam.rotation);
        changed |= Editor("Unk1", cam.unk1);
        changed |= Editor("Near Clip", cam.nearClip);
        changed |= Editor("Far Clip", cam.farClip);
        changed |= Editor("FOV", cam.fov);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Description::Camera& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Position", data.position);
        changed |= Editor("Target Position", data.targetPosition);
        changed |= Editor("FOV", data.fov);
        changed |= Editor("Direction", data.direction);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementCameraParams::Description::Camera& data, csl::ut::Bitset<hh::dv::DvElementCameraParams::Description::Flags>& flags, bool finishParams) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        if(flags.test(finishParams ? hh::dv::DvElementCameraParams::Description::Flags::FINISH_POSITION : hh::dv::DvElementCameraParams::Description::Flags::POSITION))
            changed |= Editor("Position", data.position);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Description::Flags::FINISH_TARGET_POSITION : hh::dv::DvElementCameraParams::Description::Flags::TARGET_POSITION))
            changed |= Editor("Target Position", data.targetPosition);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Description::Flags::FINISH_FOV : hh::dv::DvElementCameraParams::Description::Flags::FOV))
            changed |= Editor("FOV", data.fov);
        if (flags.test(finishParams ? hh::dv::DvElementCameraParams::Description::Flags::FINISH_ROTATION : hh::dv::DvElementCameraParams::Description::Flags::ROTATION))
            changed |= Editor("Rotation", data.direction);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, hh::dv::DvElementPathInterpolation::Description::Interpolation& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Position", data.position);
        changed |= Editor("Rotation", data.rotation);
        changed |= Editor("Scale", data.scale);
        ImGui::TreePop();
    }
    return changed;
}

#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, hh::dv::DvElementPointLight::Description::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Attenuation Range", data.attenuationRadius);
        changed |= Editor("Intensity", data.intensity);
        changed |= Editor("Radius", data.radius);
        changed |= Editor("Rotation Multiplier", data.rotationMultiplier);
        ImGui::TreePop();
    }
    return changed;
}
#endif


bool Editor(const char* label, app::dv::DvElementDOFParam::Description::DOFParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)) {
        if(ImGui::TreeNode("Foreground")) {
            changed |= Editor("Bokeh Start Depth", param.foregroundBokehStartDepth);
            changed |= Editor("Bokeh Max Depth", param.foregroundBokehMaxDepth);
        }
        if (ImGui::TreeNode("Background")) {
            changed |= Editor("Bokeh Start Depth", param.backgroundBokehStartDepth);
            changed |= Editor("Bokeh Max Depth", param.backgroundBokehMaxDepth);
        }
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementChromaticAberrationFilterParam::Description::ChromaticAberrationParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)) {
        changed |= Editor("Color Offset", param.colorOffset);
        changed |= Editor("Sphere Curve", param.sphereCurve);
        changed |= Editor("Scale", param.scale);
        changed |= Editor("Position", param.position);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementLookAtIK::Description::Object& obj) {
    static const char* typeNames[]{
        "OFFSET",
        "OFFSET_GUID_TARGET",
        "CAMERA",
        "OFFSET_GUID_POSITION"
    };

    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= ComboEnum("Type", obj.type, typeNames);
        changed |= Editor("GUID", obj.guid);
        changed |= Editor("Offset", obj.offset);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementAura::Description::AuraNode& node) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        unsigned int color[4] = { node.color[1], node.color[2], node.color[3], node.color[0] };
        changed |= ColorEditor("Color", color);
        changed |= Editor("Distance", node.distance);
        changed |= Editor("Noise Texture Scroll Speed", node.noiseTextureScrollSpeed);
        changed |= Editor("Blur Scale", node.blurScale);
        changed |= Editor("Color Gain", node.colorGain);
        changed |= Editor("Noise Gain", node.noiseGain);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementVariablePointLight::Description::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Attenuation Range", data.attenuationRadius);
        changed |= Editor("Intensity", data.intensity);
        changed |= Editor("Radius", data.radius);
        changed |= Editor("Rotation Multiplier", data.rotationMultiplier);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementBloomParam::Description::Parameters& data) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Bloom", data.bloom);
        changed |= Editor("Sample Radius", data.sampleRadius);
        changed |= Editor("Blur Quality", data.blurQuality);
        ImGui::TreePop();
    }
    return changed;
}


#ifdef DEVTOOLS_TARGET_SDK_miller
bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::ImageCircleParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Position", param.position);
        changed |= Editor("Size", param.size);
        changed |= Editor("Scale", param.scale);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementCameraBlurParam::Description::BlurParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Blur Power", param.blurPower);
        changed |= Editor("Focus Range", param.focusRange);
        changed |= Editor("Alpha Slope", param.alphaSlope);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter::DistanceFogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Near Dist", param.nearDist);
        changed |= Editor("Far Dist", param.farDist);
        changed |= Editor("Influence", param.influence);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter::HeightFogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Min Height", param.minHeight);
        changed |= Editor("Max Height", param.maxHeight);
        changed |= Editor("Near Dist", param.nearDist);
        changed |= Editor("Far Dist", param.farDist);
        changed |= Editor("Influence", param.influence);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementFog::Description::FogParameter& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= ImGui::ColorEdit3("Color", param.color);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Sky Blend", param.skyBlend);
        changed |= Editor("Sky Mips", param.skyMips);
        changed |= Editor("Distance Fog Parameters", param.distanceFogParam);
        changed |= Editor("Height Fog Parameters", param.heightFogParam);
        ImGui::TreePop();
    }
    return changed;
}

bool Editor(const char* label, app::dv::DvElementDOF::Description::DOFParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
        changed |= Editor("Focal Length (mm)", param.focalLengthInMilliMeters);
        changed |= Editor("F-Number", param.fNumber);
        changed |= Editor("Fixed FOV Y", param.fixedFovY);
        changed |= Editor("Focus Length", param.focusLength);
        changed |= Editor("Force Focus Range", param.forceFocusRange);
        changed |= Editor("Foreground Bokeh Max Depth", param.foregroundBokehMaxDepth);
        changed |= Editor("Foreground Bokeh Start Depth", param.foregroundBokehStartDepth);
        changed |= Editor("Background Bokeh Start Depth", param.backgroundBokehStartDepth);
        changed |= Editor("Background Bokeh Max Depth", param.backgroundBokehMaxDepth);
        changed |= Editor("COC Max Radius", param.cocMaxRadius);
        changed |= Editor("Foreground Bokeh Scale", param.foregroundBokehScale);
        changed |= Editor("Background Bokeh Scale", param.backgroundBokehScale);
        ImGui::TreePop();
    }
    return changed;
}
#endif

bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::VignetteParam& param) {
    bool changed = false;
    if(ImGui::TreeNode(label)){
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= Editor("Position", param.position);
        changed |= Editor("Size", param.size);
        changed |= Editor("Scale", param.scale);
        changed |= Editor("Direction", param.lineDirection);
        float opacity = static_cast<float>(param.opacity) / 255.0f;
        if (changed |= Editor("Opacity", opacity))
            param.opacity = static_cast<unsigned int>(opacity * 255);
        changed |= ColorEditor("Color", param.color);
        changed |= Editor("Penumbra Scale", param.penumbraScale);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Rotation", param.rotation);
#elif DEVTOOLS_TARGET_SDK_miller
        changed |= Editor("Image Circle Parameters", param.imgCrclParam);
        changed |= Editor("Direction", param.direction);
        changed |= Editor("Opacity", param.opacity);
        changed |= ColorEditor("Color", param.color);
        changed |= Editor("Penumbra Scale", param.penumbraScale);
        changed |= Editor("Intensity", param.intensity);
        changed |= Editor("Rotation", param.rotation);
#endif
        ImGui::TreePop();
    }
    return changed;
}

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, app::dv::DvElementVignetteParam::Description::DepthParam& param) {
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Min Penumbra Scale", param.minMaxPenumbraScale[0]);
        changed |= Editor("Max Penumbra Scale", param.minMaxPenumbraScale[1]);
        changed |= Editor("Bokeh Scale", param.bokehScale);
        changed |= Editor("Min DOF Opacity Scale", param.minMaxDOFOpacityScale[0]);
        changed |= Editor("Max DOF Opacity Scale", param.minMaxDOFOpacityScale[1]);
        changed |= Editor("Min Opacity Scale", param.minMaxOpacityScale[0]);
        changed |= Editor("Max Opacity Scale", param.minMaxOpacityScale[1]);
        changed |= Editor("Min Opacity Distance", param.minMaxOpacityDist[0]);
        changed |= Editor("Max Opacity Distance", param.minMaxOpacityDist[1]);
        ImGui::TreePop();
    }
    return changed;
}
#endif

bool Editor(const char* label, app::dv::DvElementTime::Description::Time& param)
{
    bool changed = false;
    if (ImGui::TreeNode(label)) {
        changed |= Editor("Hour", param.hour);
        changed |= Editor("Minute", param.minute);
        ImGui::TreePop();
    }
    return changed;
}
