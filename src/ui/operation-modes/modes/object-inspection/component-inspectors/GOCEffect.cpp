#include "GOCEffect.h"
bool Editor(const char* label, hh::eff::GOCEffect::EffectInfo& info);
bool Editor(const char* label, hh::eff::GOCEffect::AttachedEffectInfo& info);
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Effects.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::eff;

bool Editor(const char* label, hh::eff::GOCEffect::EffectInfo& info) {
	bool edited{};

	edited |= Editor("Handle", info.handle);
	edited |= CheckboxFlags("Attached", info.flags, hh::eff::GOCEffect::EffectInfo::Flag::ATTACHED);
	edited |= CheckboxFlags("Ignore StopAttachedEffectAll", info.flags, hh::eff::GOCEffect::EffectInfo::Flag::IGNORE_STOP_ATTACHED_EFFECT_ALL);

	return edited;
}

bool Editor(const char* label, hh::eff::GOCEffect::AttachedEffectInfo& info) {
	bool edited{};
	edited |= Editor("Base", (hh::eff::GOCEffect::EffectInfo&)info);
	edited |= Editor("Attach time", info.attachTime);
	return edited;
}

void RenderComponentInspector(GOCEffect& component) {
	if (component.frame != nullptr)
		Viewer("Frame", *component.frame);
	Editor("Scale", component.scale);
	Editor("Color", component.color);
	Viewer("Model name hash", component.modelNameHash);

	ImGui::Separator();
	
	Editor("Attached effects", component.attachedEffects);
	Editor("Detached effects", component.detachedEffects);
}
