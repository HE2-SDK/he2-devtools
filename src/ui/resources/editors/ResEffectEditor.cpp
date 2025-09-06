#include "ResEffectEditor.h"

bool Editor(const char* label, ucsl::resources::cemt::v100000::TextureParam& texture);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ChildEffect& effect);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::GravityVectorSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::GravityOtherSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::VortexSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::FluctuationSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam& modifier);
bool Editor(const char* label, ucsl::resources::cemt::v100000::Table::Unk1& v);
bool Editor(const char* label, ucsl::resources::cemt::v100000::Table& table);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ElementParam& element);
bool Editor(const char* label, ucsl::resources::cemt::v100000::EmitterParam& emitter);

#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

using namespace ucsl::resources::cemt::v100000;

const char* modifierNames[] = {
	"GRAVITY_VECTOR",
	"GRAVITY_OTHER",
	"UNK0",
	"UNK1",
	"VORTEX",
	"UNK2",
	"UNK3",
	"UNK4",
	"UNK5",
	"FLUCTUATION",
};

bool Editor(const char* label, TextureParam& texture) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("name", texture.name);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ChildEffect& effect) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("flags", effect.flags);
	edited |= Editor("name", effect.name);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::GravityVectorSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("value", settings.value);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::GravityOtherSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("value", settings.value);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::VortexSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("unk1", settings.unk1);
	edited |= Editor("unk2", settings.unk2);
	edited |= Editor("unk3", settings.unk3);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::FluctuationSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("unk0", settings.unk0);
	edited |= Editor("unk1", settings.unk1);
	edited |= Editor("unk2", settings.unk2);
	edited |= Editor("unk3", settings.unk3);
	edited |= Editor("unk4", settings.unk4);
	edited |= Editor("unk5", settings.unk5);
	edited |= Editor("unk6", settings.unk6);
	edited |= Editor("unk7", settings.unk7);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam& modifier) {
	bool edited{};
	ImGui::PushID(label);

	edited |= Editor("enabled", modifier.enabled);
	edited |= ComboEnum("type", modifier.type, modifierNames);
	edited |= Editor("option1", modifier.option1);
	edited |= Editor("option2", modifier.option2);
	
	switch (modifier.type) {
	case ModifierParam::Type::GRAVITY_VECTOR: edited |= Editor("settings", modifier.settings.gravityVector); break;
	case ModifierParam::Type::GRAVITY_OTHER: edited |= Editor("settings", modifier.settings.gravityOther); break;
	case ModifierParam::Type::VORTEX: edited |= Editor("settings", modifier.settings.vortex); break;
	case ModifierParam::Type::FLUCTUATION: edited |= Editor("settings", modifier.settings.fluctuation); break;
	default: ImGui::Text("not yet implemented"); break;
	}

	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, Table::Unk1& v) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("value", v.value);
		edited |= Editor("unk1", v.unk1);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, Table& table) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("red1", table.red1);;
	edited |= Editor("green1", table.green1);;
	edited |= Editor("blue1", table.blue1);;
	edited |= Editor("alpha1", table.alpha1);;
	edited |= Editor("red2", table.red2);;
	edited |= Editor("green2", table.green2);;
	edited |= Editor("blue2", table.blue2);;
	edited |= Editor("alpha2", table.alpha2);;
	edited |= Editor("unk6", table.unk6);;
	edited |= Editor("unk8", table.unk8);;
	edited |= Editor("reds1", table.reds1);;
	edited |= Editor("greens1", table.greens1);;
	edited |= Editor("blues1", table.blues1);;
	edited |= Editor("reds2", table.reds2);;
	edited |= Editor("greens2", table.greens2);;
	edited |= Editor("blues2", table.blues2);;
	edited |= Editor("unks1", table.unks1);;
	edited |= Editor("unks2", table.unks2);;
	edited |= Editor("alphas1", table.alphas1);;
	edited |= Editor("alphas2", table.alphas2);;
	edited |= Editor("count1", table.count1);;
	edited |= Editor("count2", table.count2);;
	edited |= Editor("unk1", table.unk1);;
	edited |= Editor("unk2", table.unk2);;
	edited |= Editor("alphaCount1", table.alphaCount1);;
	edited |= Editor("alphaCount2", table.alphaCount2);;
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ElementParam& element) {
	bool edited{};
	ImGui::PushID(label);

	ImGui::SeparatorText("Element");
	edited |= Editor("initialRotation", element.initialRotation);
	edited |= Editor("unk18a0", element.unk18a0);
	edited |= Editor("angularVelocity", element.angularVelocity);
	edited |= Editor("unk18a01", element.unk18a01);
	edited |= Editor("emitSpeed2", element.emitSpeed2);
	edited |= Editor("unk18a02", element.unk18a02);
	edited |= Editor("unkCount2", element.unkCount2);
	edited |= Editor("unkCount3", element.unkCount3);
	edited |= Editor("tables", element.tables);
	edited |= Editor("fps", element.fps);
	edited |= Editor("unk19a23", element.unk19a23);
	edited |= Editor("unk18a6", element.unk18a6);
	edited |= Editor("unk18a8", element.unk18a8);
	edited |= Editor("unk18a1", element.unk18a1);
	edited |= Editor("unk18a2", element.unk18a2);
	edited |= Editor("unk18a", element.unk18a);
	edited |= Editor("modelName", element.modelName);
	edited |= Editor("particleTypeOrSomething", element.particleTypeOrSomething);
	edited |= Editor("particleInfo1", element.particleInfo1);
	edited |= Editor("particleInfo2", element.particleInfo2);
	edited |= Editor("unk18a3", element.unk18a3);
	edited |= Editor("particleInfo3", element.particleInfo3);
	edited |= Editor("textures", element.textures);
	edited |= Editor("textureCount", element.textureCount);
	edited |= Editor("childEffects", element.childEffects);
	edited |= Editor("modifiers", element.modifiers);
	edited |= Editor("flags3", element.flags3);
	edited |= Editor("flags4", element.flags4);
	edited |= Editor("vectorFieldName", element.vectorFieldName);
	edited |= Editor("vectorFieldSize", element.vectorFieldSize);
	edited |= Editor("vectorFieldParam", element.vectorFieldParam);
	edited |= Editor("depthCollision", element.depthCollision);
	edited |= Editor("flags2", element.flags2);
	edited |= Editor("unkCount", element.unkCount);
	edited |= Editor("unk19", element.unk19);

	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, EmitterParam& emitter) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("emitspeed", emitter.emitSpeed);
	edited |= Editor("emitVector", emitter.emitVector);
	edited |= Editor("emitVectorUnk", emitter.emitVectorUnk);
	edited |= Editor("randomSeed", emitter.randomSeed);
	edited |= Editor("element", emitter.elementParam);
	ImGui::PopID();
	return edited;
}


ResEffectEditor::ResEffectEditor(csl::fnd::IAllocator* allocator, hh::eff::ResEffect* resource) : StandaloneWindow{ allocator }, resource{ resource }
{
	char title[400];
	snprintf(title, sizeof(title), "%s - %s @ 0x%zx (EffectParam @ 0x%zx)", resource->GetName(), resource->GetClass().pName, (size_t)resource, (size_t)&resource->unpackedBinaryData);
	SetTitle(title);
}

void ResEffectEditor::RenderContents()
{
	auto* effect = static_cast<EffectParam*>(resource->unpackedBinaryData);
	auto* emitter = reinterpret_cast<EmitterParam*>(&effect[1]);

	Editor("name", effect->name);
	Editor("count", effect->numEffects);
	if (Editor("emitter", *emitter)) {
		hh::fnd::ResourceManagerResolver resolver{};
		resource->Reload(resource->unpackedBinaryData, resource->size);
		resource->Resolve(resolver);
	}
}

ResEffectEditor* ResEffectEditor::Create(csl::fnd::IAllocator* allocator, hh::eff::ResEffect* resource)
{
	return new (allocator) ResEffectEditor(allocator, resource);
}
