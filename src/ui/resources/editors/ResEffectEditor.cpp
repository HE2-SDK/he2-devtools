#include "ResEffectEditor.h"

bool Editor(const char* label, ucsl::resources::cemt::v100000::TextureParam& texture);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ChildEffect& effect);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::GravitySettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::SpeedSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::MagnetSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::NewtonSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::VortexSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::SpinSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::Spin2Settings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::RandomSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ModifierParam::TailSettings& settings);
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
	"GRAVITY",
	"SPEED",
	"MAGNET",
	"NEWTON",
	"VORTEX",
	"SPIN",
	"SPIN2",
	"RANDOM",
	"TAIL",
	"FLUCTUATION",
	"UNK0"
};

const char* shapeNames[] = {
	"POINT",
	"SPHERE",
	"DISC",
	"CYLINDER",
	"LINE",
	"TORUS",
	"CUBE",
	"null",
	"FIXED"
};

const char* originNames[] = {
	"WORLD",
	"EMISSION",
	"INHERIT",
	"UNK0"
};

const char* waveformNames[] = {
	"TRIANGLE",
	"INVERSE_TRIANGLE",
	"SAW",
	"STEP",
	"SINE"
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
	edited |= Editor("unkType", effect.unkType);
	edited |= Editor("name", effect.name);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::GravitySettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("rotation", settings.rotation);
	edited |= Editor("usesDirection", settings.usesDirection);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::SpeedSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::MagnetSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("magnetPoint", settings.magnetPoint);
	edited |= Editor("scale", settings.scale);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::NewtonSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("targetPoint", settings.targetPoint);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("maxDistance", settings.maxDistance);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::VortexSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("max", settings.max);
	edited |= Editor("min", settings.min);
	edited |= Editor("radius", settings.radius);
	edited |= Editor("rotation", settings.rotation);
	edited |= Editor("usesDirection", settings.usesDirection);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::SpinSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("rotation", settings.rotation);
	edited |= Editor("usesDirection", settings.usesDirection);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::Spin2Settings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("baseStrength", settings.baseStrength);
	edited |= Editor("spinStrength", settings.spinStrength);
	edited |= Editor("rotationAngle", settings.rotationAngle);
	edited |= Editor("axisFalloff", settings.axisFalloff);
	edited |= Editor("axisVector", settings.axisVector);
	edited |= Editor("useEulerRotation", settings.useEulerRotation);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::RandomSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("spreadScale", settings.spreadScale);
	edited |= Editor("updateInterval", settings.updateInterval);
	edited |= CheckboxFlags("RANDOMIZED_SCALE", settings.flags, ModifierParam::RandomSettings::Flags::RANDOMIZED_SCALE);
	edited |= CheckboxFlags("X", settings.flags, ModifierParam::RandomSettings::Flags::X);
	edited |= CheckboxFlags("Y", settings.flags, ModifierParam::RandomSettings::Flags::Y);
	edited |= CheckboxFlags("Z", settings.flags, ModifierParam::RandomSettings::Flags::Z);
	edited |= Editor("normalizedSpreadVector", settings.normalizedSpreadVector);
	edited |= Editor("randomPerAxis", settings.randomPerAxis);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::TailSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("multiplier", settings.multiplier);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam::FluctuationSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= ComboEnum("waveformType", settings.waveformType, waveformNames);
	edited |= Editor("baseOffset", settings.baseOffset);
	edited |= Editor("axisFlags", settings.axisFlags);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, ModifierParam& modifier) {
	bool edited{};
	ImGui::PushID(label);

	ImGui::Separator();
	edited |= Editor("enabled", modifier.enabled);
	edited |= ComboEnum("type", modifier.type, modifierNames);
	edited |= ComboEnum("origin", modifier.origin, originNames);
	edited |= Editor("option2", modifier.option2);
	
	switch (modifier.type) {
	case ModifierParam::Type::GRAVITY: edited |= Editor("settings", modifier.settings.gravity); break;
	case ModifierParam::Type::SPEED: edited |= Editor("settings", modifier.settings.speed); break;
	case ModifierParam::Type::MAGNET: edited |= Editor("settings", modifier.settings.magnet); break;
	case ModifierParam::Type::NEWTON: edited |= Editor("settings", modifier.settings.newton); break;
	case ModifierParam::Type::VORTEX: edited |= Editor("settings", modifier.settings.vortex); break;
	case ModifierParam::Type::SPIN: edited |= Editor("settings", modifier.settings.spin); break;
	case ModifierParam::Type::SPIN2: edited |= Editor("settings", modifier.settings.spin2); break;
	case ModifierParam::Type::RANDOM: edited |= Editor("settings", modifier.settings.random); break;
	case ModifierParam::Type::TAIL: edited |= Editor("settings", modifier.settings.tail); break;
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
	edited |= Editor("sizeX", element.sizeX);
	edited |= Editor("sizeXJitter", element.sizeXJitter);
	edited |= Editor("sizeY", element.sizeY);
	edited |= Editor("sizeYJitter", element.sizeYJitter);
	edited |= Editor("sizeZ", element.sizeZ);
	edited |= Editor("sizeZJitter", element.sizeZJitter);
	edited |= Editor("sizeFlags", element.sizeFlags);
	edited |= Editor("scaleX", element.scaleX);
	edited |= Editor("scaleXJitter", element.scaleXJitter);
	edited |= Editor("scaleY", element.scaleY);
	edited |= Editor("scaleYJitter", element.scaleYJitter);
	edited |= Editor("scaleZ", element.scaleZ);
	edited |= Editor("scaleZJitter", element.scaleZJitter);
	edited |= Editor("scaleFlags", element.scaleFlags);
	edited |= Editor("tables", element.tables);
	edited |= Editor("fps", element.fps);
	edited |= Editor("fpsJitter", element.fpsJitter);
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
	edited |= Editor("position", emitter.position);
	edited |= Editor("rotation", emitter.rotation);
	edited |= Editor("scale", emitter.scale);
	edited |= ComboEnum("shape", emitter.shape, shapeNames);
	edited |= Editor("spread", emitter.spread);
	edited |= Editor("startAngle", emitter.startAngle);
	edited |= Editor("endAngle", emitter.endAngle);
	edited |= Editor("useRadialDistribution", emitter.useRadialDistribution);
	edited |= Editor("consistentAngle", emitter.consistentAngle);
	edited |= Editor("useAngularSubdivisions", emitter.useAngularSubdivisions);
	edited |= Editor("numSubDivisions", emitter.numSubDivisions);
	edited |= Editor("disabled", emitter.disabled);
	edited |= Editor("frequency", emitter.frequency);
	edited |= Editor("frequencyJitter", emitter.frequencyJitter);
	edited |= Editor("emitterCount", emitter.emitterCount);
	edited |= Editor("emitterCountJitter", emitter.emitterCountJitter);
	edited |= Editor("duration", emitter.duration);
	edited |= Editor("startDelay", emitter.startDelay);
	edited |= Editor("fadeSpeed", emitter.fadeSpeed);
	edited |= Editor("lifeEndTime", emitter.lifeEndTime);
	edited |= Editor("accelarationMultiplier", emitter.accelarationMultiplier);
	edited |= Editor("accelarationNormalMultiplier", emitter.accelarationNormalMultiplier);
	edited |= Editor("initialSpeed", emitter.initialSpeed);
	edited |= Editor("velocityMultiplier", emitter.velocityMultiplier);
	edited |= Editor("shapeRadius", emitter.shapeRadius);
	edited |= Editor("velocityScale", emitter.velocityScale);
	edited |= Editor("emitSize", emitter.emitSize);
	edited |= Editor("useEmitVector", emitter.useEmitVector);
	edited |= Editor("emitVector", emitter.emitVector);
	edited |= Editor("emitVectorJitter", emitter.emitVectorJitter);
	edited |= Editor("directionJitter", emitter.directionJitter);
	edited |= Editor("inheritRatio", emitter.inheritRatio);
	edited |= Editor("randomSeed", emitter.randomSeed);
	edited |= Editor("unkFloat0", emitter.unkFloat0);
	edited |= Editor("unkFloat0Jitter", emitter.unkFloat0Jitter);
	edited |= Editor("unkFloat1", emitter.unkFloat1);
	edited |= Editor("unkFloat1Jitter", emitter.unkFloat1Jitter);
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
