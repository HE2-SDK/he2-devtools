#include "GOCEffect.h"
bool Editor(const char* label, hh::eff::GOCEffect::EffectInfo& info);
bool Editor(const char* label, hh::eff::GOCEffect::NewEffectInfo& info);
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Effects.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::eff;

bool Editor(const char* label, hh::eff::GOCEffect::EffectInfo& info) {
	bool edited{};
	
	edited |= Editor("Handle", info.handle);
	edited |= CheckboxFlags("Unk 0 flag", info.flags, hh::eff::GOCEffect::EffectInfo::Flag::UNK0);
	edited |= CheckboxFlags("Unk 1 flag", info.flags, hh::eff::GOCEffect::EffectInfo::Flag::UNK1);

	return edited;
}

bool Editor(const char* label, hh::eff::GOCEffect::NewEffectInfo& info) {
	bool edited{};
	edited |= Editor("Base", (hh::eff::GOCEffect::EffectInfo&)info);
	edited |= Editor("Delay", info.delay);
	return edited;
}

void RenderComponentInspector(GOCEffect& component) {
	if (component.frame != nullptr)
		Viewer("Frame", *component.frame);
	Editor("Scale", component.scale);
	Editor("Color", component.color);
	Viewer("Model name hash", component.modelNameHash);

	ImGui::Separator();
	
	Editor("New effects", component.newEffects);
	Editor("Effects", component.effects);
}
