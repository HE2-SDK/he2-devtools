#include "Camera.h"
#include <ui/common/editors/Basic.h>

using namespace app_cmn::camera;

bool Editor(const char* label, CameraParameter::Orientation::Target& target) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Up Vector", target.upVector);
	edited |= Editor("Unk8b", target.unk8b);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter::Orientation::Offset& offset) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Offset", reinterpret_cast<csl::math::Vector3&>(offset));
    edited |= Editor("Target Position", offset.targetPosition);
    edited |= Editor("Use Position", offset.usePosition);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter::Positioning& positioning) {
    bool edited{};
    ImGui::PushID(label);

    edited |= Editor("Position", positioning.position);
    edited |= Editor("Offset", positioning.offset);
    edited |= Editor("Unk3", positioning.unk3);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter::Orientation& orientation) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Offset", orientation.offset);
	edited |= Editor("Target", orientation.target);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraParameter& parameter) {
    bool edited{};
    ImGui::PushID(label);

	edited |= Editor("Positioning", parameter.positioning);
	edited |= Editor("Orientation", parameter.orientation);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, FrustumParameter& parameter) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("Near clipping plane", parameter.nearClip);
	edited |= Editor("Far clipping plane", parameter.farClip);
	edited |= Editor("Field of View", parameter.fov);

    ImGui::PopID();
    return edited;
}

bool Editor(const char* label, CameraPose& pose) {
    bool edited{};
    ImGui::PushID(label);
    
	edited |= Editor("LookAt position", pose.lookAtPos);
	edited |= Editor("Position", pose.position);
	edited |= Editor("Unk3", pose.unk3);

    ImGui::PopID();
    return edited;
}
