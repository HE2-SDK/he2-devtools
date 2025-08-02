#include "GOCAnimation.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

bool Editor(const char* label, hh::anim::GOCAnimationSimple::SimpleAnimationState& state) {
	bool edited{};

	Viewer("Play policy", static_cast<unsigned int>(state.playPolicy));
	Viewer("Is playing", state.IsPlaying());
	Viewer("Duration", state.GetDuration());

	if (ImGui::Button("Reset time"))
		state.ResetTime();
	
	float localTime = state.GetLocalTime();
	if (Editor("Local time", localTime)) {
		state.SetLocalTime(localTime);
		edited = true;
	}

	float speed = state.GetSpeed();
	if (Editor("Speed", speed)) {
		state.SetSpeed(speed);
		edited = true;
	}

	float weight = state.GetWeight();
	if (Editor("Weight", weight)) {
		state.SetWeight(weight);
		edited = true;
	}

	return edited;
}

void RenderComponentInspector(hh::anim::GOCAnimationSingle& component) {
	unsigned int type{ static_cast<unsigned int>(component.type) };
	if (Editor("Type", type))
		component.type = static_cast<hh::anim::GOCAnimationSingle::Type>(type);

	ImGui::Text("Flags: %x", component.flags);

	CheckboxFlags("Has skeleton override", component.flags, hh::anim::GOCAnimationSingle::Flag::HAS_SKELETON);
	CheckboxFlags("Set pose", component.flags, hh::anim::GOCAnimationSingle::Flag::SET_POSE);

	Viewer("Model component name", component.modelComponentName);

	if (auto& skeleton = component.skeleton)
		Viewer("Skeleton resource", skeleton->GetName());
	if (auto& skeleton = component.skeleton)
		Viewer("Pose resource", skeleton->GetName());

	Viewer("Transform", component.GetTransform());

	if (auto* pose = (hh::anim::PosePxd*)component.GetPose()) {
		Editor("Pose transforms", pose->unk1.transforms);
	}
}

void RenderComponentInspector(hh::anim::GOCAnimationSimple& component) {
	RenderComponentInspector(static_cast<hh::anim::GOCAnimationSingle&>(component));

	for (auto& state : component.animations) {
		if (ImGui::TreeNode(state.name.c_str())) {
			Editor("State", state);
			ImGui::TreePop();
		}
	}
}
