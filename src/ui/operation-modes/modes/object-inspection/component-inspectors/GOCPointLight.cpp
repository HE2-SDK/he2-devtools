#include "GOCPointLight.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderComponentInspector(hh::gfx::GOCPointLight& component) {
	if(component.hFrame)
		Viewer("HFrame", *component.hFrame);
	if (component.sceneContextManagerName)
		Viewer("Scene Context Manager Name", component.sceneContextManagerName);

	if (ImGui::TreeNode("Lights")) {
		for (auto& light : component.lights) {
			ImGui::PushID(light.hFrame);
			char buffer[16];
			snprintf(buffer, sizeof(buffer), "%d", light.idx);
			if (ImGui::TreeNode(buffer)) {
				Editor("Position", light.position);
				Editor("Rotation", light.rotation);
				Editor("Intensity", light.intensity);
				ColorEditor("Color", light.color);
				if (ImGui::TreeNode("HFrame")) {
					Viewer("HFrame", light.hFrame);
					ImGui::TreePop();
				}
				switch (light.type) {
				case ucsl::resources::light::v2::LightType::POINT: {
					Editor("Radius", light.pointProps.radius);
					Editor("Attenuation Radius", light.pointProps.attenuationRadius);
					Editor("Shadow Enabled", light.pointProps.shadowEnabled);
					break;
				}
				case ucsl::resources::light::v2::LightType::SPOT: {
					Editor("Inner Cone Angle", light.directionalProps.innerConeAngle);
					Editor("Outer Cone Angle", light.directionalProps.outerConeAngle);
					Editor("Attenuation Radius", light.directionalProps.attenuationRadius);
					Editor("Shadow Enabled", light.directionalProps.shadowEnabled);
					break;
				}
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
}
