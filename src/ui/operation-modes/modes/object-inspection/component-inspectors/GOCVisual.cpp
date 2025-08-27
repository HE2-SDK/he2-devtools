#include "GOCVisual.h"

#ifdef DEVTOOLS_TARGET_SDK_rangers
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::TexSrtControlHH>& control);
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::TexPatControlHH>& control);
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::MatAnimControlHH>& control);
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::VisAnimControlHH>& control);
#endif

#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Needle.h>

#ifdef DEVTOOLS_TARGET_SDK_rangers
template<typename T>
bool AnimControlEditor(const char* label, T& control) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		if (auto& resource = control.resource)
			Viewer("Resource", resource->GetName());
		
		float frame = control.GetFrame();
		float localTime = control.GetLocalTime();
		float endFrame = control.GetEndFrame();
		float endLocalTime = control.GetEndLocalTime();
		float speed = control.GetSpeed();
		float unk5 = control.GetUnk5();
		float zero = 0.0f;

		Viewer("End frame", endFrame);
		Viewer("End local time", endLocalTime);

		if (SliderScalar("Frame", frame, &zero, &endFrame)) {
			control.SetFrame(frame);
			edited = true;
		}

		if (SliderScalar("Local time", localTime, &zero, &endLocalTime)) {
			control.SetLocalTime(localTime);
			edited = true;
		}

		if (Editor("Speed", speed)) {
			control.SetSpeed(speed);
			edited = true;
		}

		if (Editor("Unk5", unk5)) {
			control.SetUnk5(unk5);
			edited = true;
		}

		ImGui::TreePop();
	}

	return edited;
}

template<typename T>
bool AnimBlenderEditor(const char* label, T& blender) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		edited |= Editor("Animations", blender.animations);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, hh::gfx::TexSrtControlHH& control) { return AnimControlEditor(label, control); }
bool Editor(const char* label, hh::gfx::TexPatControlHH& control) { return AnimControlEditor(label, control); }
bool Editor(const char* label, hh::gfx::MatAnimControlHH& control) { return AnimControlEditor(label, control); }
bool Editor(const char* label, hh::gfx::VisAnimControlHH& control) { return AnimControlEditor(label, control); }

bool Editor(const char* label, hh::fnd::Reference<hh::gfx::TexSrtControlHH>& control) { return control == nullptr ? false : AnimControlEditor(label, *control); }
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::TexPatControlHH>& control) { return control == nullptr ? false : AnimControlEditor(label, *control); }
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::MatAnimControlHH>& control) { return control == nullptr ? false : AnimControlEditor(label, *control); }
bool Editor(const char* label, hh::fnd::Reference<hh::gfx::VisAnimControlHH>& control) { return control == nullptr ? false : AnimControlEditor(label, *control); }

bool Editor(const char* label, hh::gfx::TexSrtBlender& blender) { return AnimBlenderEditor(label, blender); }
bool Editor(const char* label, hh::gfx::TexPatBlender& blender) { return AnimBlenderEditor(label, blender); }
bool Editor(const char* label, hh::gfx::MatAnimBlender& blender) { return AnimBlenderEditor(label, blender); }
bool Editor(const char* label, hh::gfx::VisAnimBlender& blender) { return AnimBlenderEditor(label, blender); }
#endif

void RenderComponentInspector(hh::gfx::GOCVisual& component) {
    bool isVisible{ component.IsVisible() };

    if (Editor("Visible", isVisible))
        component.SetVisible(isVisible);
    Editor("unk102", component.unk102);
}

void RenderComponentInspector(hh::gfx::GOCVisualTransformed& component) {
	RenderComponentInspector(static_cast<hh::gfx::GOCVisual&>(component));

	auto transform = component.localTransform;

	ImGui::SeparatorText("Main local transform (editable form)");
	if (Editor("Local Transform Editable", transform))
		component.SetLocalTransform(transform);

	ImGui::SeparatorText("Main local transform (internal representation)");
	Viewer("Local Transform ", transform);

	auto worldMatrix = component.worldMatrix;
	ImGui::SeparatorText("World matrix");
	if (Editor("World matrix", worldMatrix))
		component.SetWorldMatrix(worldMatrix);
	
	if (component.frame2)
		Viewer("Calculated HFrame", *component.frame2);
	if (component.frame1)
		Viewer("Offset HFrame", *component.frame1);

	// custom AABB
}

void RenderComponentInspector(hh::gfx::GOCVisualModel& component) {
	RenderComponentInspector(static_cast<hh::gfx::GOCVisualTransformed&>(component));

	if (auto& model = component.model)
		Viewer("Model resource", model->GetName());
	if (auto& skeleton = component.skeleton)
		Viewer("Skeleton resource", skeleton->GetName());

#ifdef DEVTOOLS_TARGET_SDK_rangers
	if (auto& texSrtBlender = component.texSrtBlender)
		Editor("TexSrt animation blender", texSrtBlender);
	if (auto& texPatBlender = component.texPatBlender)
		Editor("TexPat animation blender", texPatBlender);
	if (auto& matAnimBlender = component.matAnimBlender)
		Editor("Material animation blender", matAnimBlender);
	if (auto& visAnimBlender = component.visAnimBlender)
		Editor("Visibility animation blender", visAnimBlender);
#endif

	Viewer("Flags", component.description.flags.bits);

	if (ImGui::TreeNode("Material instance parameters")) {
		if (auto* meshResource = component.model->GetMeshResource()) {
			for (auto materialIdx = 0; materialIdx < meshResource->GetMaterialCount(); materialIdx++) {
				auto* materialNameID = meshResource->GetMaterialNameID(materialIdx);
				if (ImGui::TreeNode(materialNameID, "%s", materialNameID->name)) {
					auto* modelInstance = component.implementation.modelInstance;
					auto paramsIdx = modelInstance->GetParameterValueObjectContainerByName(materialNameID);

					if (paramsIdx != -1)
						if (auto* params = modelInstance->GetParameterValueObjectContainer(paramsIdx))
							Editor("Parameters", *params);

					ImGui::TreePop();
				}
			}
		}
		ImGui::TreePop();
	}
}
