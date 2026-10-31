#include "ResEffectEditor.h"

bool Editor(const char* label, ucsl::resources::cemt::v100000::JitteredValue& value);
bool Editor(const char* label, ucsl::resources::cemt::v100000::JitteredVector3& value);
bool Editor(const char* label, ucsl::resources::cemt::v100000::TextureParam& texture);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ChildEffect& effect);
bool Editor(const char* label, ucsl::resources::cemt::v100000::TextureParam::ScrollColorParam& param);
bool Editor(const char* label, ucsl::resources::cemt::v100000::TextureParam::UVAnimationParam& param);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::GravitySettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::SpeedSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::MagnetSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::NewtonSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::VortexSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::SpinSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::Spin2Settings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::RandomSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::TailSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam::FluctuationSettings& settings);
bool Editor(const char* label, ucsl::resources::cemt::v100000::FieldParam& modifier);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ColorRandomSet& table);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ColorParam& v);
bool Editor(const char* label, ucsl::resources::cemt::v100000::ElementParam& element);
bool Editor(const char* label, ucsl::resources::cemt::v100000::EmitterParam::EmissionParam& element);
bool Editor(const char* label, ucsl::resources::cemt::v100000::EmitterParam::EmitParam& element);
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

bool Editor(const char* label, JitteredValue& value) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("Value", value.value);
		edited |= Editor("Jitter", value.jitter);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, JitteredVector3& value) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("X", value.x);
		edited |= Editor("Y", value.y);
		edited |= Editor("Z", value.z);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, TextureParam::ScrollColorParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("scrollColor", param.scrollColor);
		edited |= Editor("scrollStartJitter", param.scrollStartJitter);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, TextureParam::UVAnimationParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("scale", param.scale);
		edited |= Editor("rotation", param.rotation);
		edited |= Editor("translation", param.translation);
		edited |= Editor("gap15", param.gap15);
		ImGui::TreePop();
	}
	return edited;
}

static const float minColorJitter = -1.0f;
static const float maxColorJitter = 1.0f;
bool JitteredColorEditor(const char* label, JitteredValue& red, JitteredValue& green, JitteredValue& blue) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		float editableValue[3]{ red.value, green.value, blue.value };
		float editableJitter[3]{ red.jitter, green.jitter, blue.jitter };

		bool editedValue = ImGui::ColorEdit3("value", editableValue, ImGuiColorEditFlags_Float);
		bool editedJitter = ImGui::DragScalarN("jitter", ImGuiDataType_Float, editableJitter, 3, 0.01f, &minColorJitter, &maxColorJitter);

		if (editedValue) {
			red.value = editableValue[0];
			green.value = editableValue[1];
			blue.value = editableValue[2];
			edited |= true;
		}

		if (editedJitter) {
			red.jitter = editableJitter[0];
			green.jitter = editableJitter[1];
			blue.jitter = editableJitter[2];
			edited |= true;
		}

		ImGui::TreePop();
	}
	return edited;
}
bool JitteredColorEditor(const char* label, JitteredValue& red, JitteredValue& green, JitteredValue& blue, JitteredValue& alpha) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		float editableValue[4]{ red.value, green.value, blue.value, alpha.value };
		float editableJitter[4]{ red.jitter, green.jitter, blue.jitter, alpha.jitter };

		bool editedValue = ImGui::ColorEdit4(label, editableValue, ImGuiColorEditFlags_Float);
		bool editedJitter = ImGui::DragScalarN(label, ImGuiDataType_Float, editableJitter, 4, 0.01f, &minColorJitter, &maxColorJitter);

		if (editedValue) {
			red.value = editableValue[0];
			green.value = editableValue[1];
			blue.value = editableValue[2];
			alpha.value = editableValue[3];
			edited |= true;
		}

		if (editedJitter) {
			red.jitter = editableJitter[0];
			green.jitter = editableJitter[1];
			blue.jitter = editableJitter[2];
			alpha.jitter = editableJitter[3];
			edited |= true;
		}

		ImGui::TreePop();
	}
	return edited;
}

static const char* patternAnimationParamTypeNames[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
	"UNK4",
	"UNK5",
	"UNK6",
	"UNK7",
	"UNK8",
	"UNK9",
	"UNK10",
};

bool Editor(const char* label, TextureParam::PatternAnimationParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("startingFrame", param.startingFrame);
		edited |= Editor("stepCountX", param.stepCountX);
		edited |= Editor("stepCountY", param.stepCountY);
		edited |= ComboEnum("type", param.type, patternAnimationParamTypeNames);
		edited |= Editor("frameRangeStart", param.frameRangeStart);
		edited |= Editor("frameRangeEnd", param.frameRangeEnd);
		edited |= Editor("frameCount", param.frameCount);
		edited |= Editor("repeating", param.repeating);
		edited |= Editor("unk6", param.unk6);
		edited |= Editor("unk6a", param.unk6a);
		edited |= CheckboxFlags("scrollDirectionFlags[INVERT_X]", param.scrollDirectionFlags, TextureParam::DirectionFlag::INVERT_X);
		edited |= CheckboxFlags("scrollDirectionFlags[INVERT_Y]", param.scrollDirectionFlags, TextureParam::DirectionFlag::INVERT_Y);
		edited |= CheckboxFlags("scrollDirectionRandomizationFlags[RANDOMIZE_X]", param.scrollDirectionRandomizationFlags, TextureParam::DirectionRandomizationFlag::RANDOMIZE_X);
		edited |= CheckboxFlags("scrollDirectionRandomizationFlags[RANDOMIZE_Y]", param.scrollDirectionRandomizationFlags, TextureParam::DirectionRandomizationFlag::RANDOMIZE_Y);
		edited |= Editor("unk9", param.unk9);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, TextureParam& texture) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("name", texture.name);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_0]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_0);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_1]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_1);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_2]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_2);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_3]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_3);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_4]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_4);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_5]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_5);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_6]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_6);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_7]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_7);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_8]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_8);
		edited |= CheckboxFlags("textureFlags[TEXTURE_TYPE_9]", texture.textureFlags, TextureParam::TextureFlag::TEXTURE_TYPE_9);

		edited |= CheckboxFlags("animationFlags[ANIMATED]", texture.animationFlags, TextureParam::AnimationFlag::ANIMATED);
		edited |= CheckboxFlags("animationFlags[UNK1]", texture.animationFlags, TextureParam::AnimationFlag::UNK1);
		edited |= CheckboxFlags("animationFlags[UNK2]", texture.animationFlags, TextureParam::AnimationFlag::UNK2);
		edited |= CheckboxFlags("animationFlags[STEPWISE_PATTERN_ANIMATION]", texture.animationFlags, TextureParam::AnimationFlag::STEPWISE_PATTERN_ANIMATION);
		edited |= CheckboxFlags("animationFlags[ENABLE_SECONDARY_SCROLL]", texture.animationFlags, TextureParam::AnimationFlag::ENABLE_SECONDARY_SCROLL);
		edited |= CheckboxFlags("animationFlags[UNK5]", texture.animationFlags, TextureParam::AnimationFlag::UNK5);
		edited |= CheckboxFlags("animationFlags[RANDOMIZE_STARTING_FRAMES]", texture.animationFlags, TextureParam::AnimationFlag::RANDOMIZE_STARTING_FRAMES);
		edited |= CheckboxFlags("animationFlags[ENABLE_PATTERN_ANIMATION]", texture.animationFlags, TextureParam::AnimationFlag::ENABLE_PATTERN_ANIMATION);
		edited |= CheckboxFlags("animationFlags[UNK8]", texture.animationFlags, TextureParam::AnimationFlag::UNK8);
		edited |= CheckboxFlags("animationFlags[SCROLL_U]", texture.animationFlags, TextureParam::AnimationFlag::SCROLL_U);
		edited |= CheckboxFlags("animationFlags[SCROLL_V]", texture.animationFlags, TextureParam::AnimationFlag::SCROLL_V);
		edited |= CheckboxFlags("animationFlags[UNK11]", texture.animationFlags, TextureParam::AnimationFlag::UNK11);
		edited |= CheckboxFlags("animationFlags[USE_MATRICES_ON_TEXTURE_TYPE_0]", texture.animationFlags, TextureParam::AnimationFlag::USE_MATRICES_ON_TEXTURE_TYPE_0);
		edited |= CheckboxFlags("animationFlags[USE_MATRICES_ON_TEXTURE_TYPE_1]", texture.animationFlags, TextureParam::AnimationFlag::USE_MATRICES_ON_TEXTURE_TYPE_1);
		edited |= CheckboxFlags("animationFlags[SCALE_U]", texture.animationFlags, TextureParam::AnimationFlag::SCALE_U);
		edited |= CheckboxFlags("animationFlags[SCALE_V ]", texture.animationFlags, TextureParam::AnimationFlag::SCALE_V);

		edited |= Editor("patternAnimationParam", texture.patternAnimationParam);
		edited |= Editor("scrollColorParams", texture.scrollColorParams);
		edited |= Editor("uvAnimationParams", texture.uvAnimationParams);
		edited |= Editor("uvScale", texture.uvScale);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, ChildEffect& effect) {
	bool edited{};
	ImGui::PushID(label);
	//edited |= Editor("flags", effect.flags);
	edited |= Editor("unkType", effect.unkType);
	edited |= Editor("name", effect.name);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::GravitySettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("rotation", settings.rotation);
	edited |= Editor("usesDirection", settings.usesDirection);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::SpeedSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::MagnetSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("magnetPoint", settings.magnetPoint);
	edited |= Editor("scale", settings.scale);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::NewtonSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("targetPoint", settings.targetPoint);
	edited |= Editor("force", settings.force);
	edited |= Editor("falloffRadius", settings.falloffRadius);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::VortexSettings& settings) {
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

bool Editor(const char* label, FieldParam::SpinSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("rotation", settings.rotation);
	edited |= Editor("usesDirection", settings.usesDirection);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::Spin2Settings& settings) {
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

bool Editor(const char* label, FieldParam::RandomSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("scale", settings.scale);
	edited |= Editor("spreadScale", settings.spreadScale);
	edited |= Editor("updateInterval", settings.updateInterval);
	edited |= CheckboxFlags("RANDOMIZED_SCALE", settings.flags, FieldParam::RandomSettings::Flags::RANDOMIZED_SCALE);
	edited |= CheckboxFlags("X", settings.flags, FieldParam::RandomSettings::Flags::X);
	edited |= CheckboxFlags("Y", settings.flags, FieldParam::RandomSettings::Flags::Y);
	edited |= CheckboxFlags("Z", settings.flags, FieldParam::RandomSettings::Flags::Z);
	edited |= Editor("normalizedSpreadVector", settings.normalizedSpreadVector);
	edited |= Editor("randomPerAxis", settings.randomPerAxis);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::TailSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("multiplier", settings.multiplier);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::FluctuationSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= ComboEnum("waveformType", settings.waveformType, waveformNames);
	edited |= Editor("unk2", settings.unk2);
	edited |= Editor("baseOffset", settings.baseOffset);
	edited |= Editor("unk0", settings.unk0);
	edited |= Editor("unk1", settings.unk1);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam::UnkSettings& settings) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("unk1", settings.unk1);
	edited |= Editor("unk2", settings.unk2);
	edited |= CheckboxFlags("X", settings.axes, FieldParam::UnkSettings::AxisFlag::X);
	edited |= CheckboxFlags("Y", settings.axes, FieldParam::UnkSettings::AxisFlag::Y);
	edited |= CheckboxFlags("Z", settings.axes, FieldParam::UnkSettings::AxisFlag::Z);
	ImGui::PopID();
	return edited;
}

bool Editor(const char* label, FieldParam& modifier) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("enabled", modifier.enabled);
		edited |= ComboEnum("type", modifier.type, modifierNames);
		edited |= ComboEnum("origin", modifier.origin, originNames);
		edited |= Editor("option2", modifier.option2);
	
		switch (modifier.type) {
		case FieldParam::Type::GRAVITY: edited |= Editor("settings", modifier.settings.gravity); break;
		case FieldParam::Type::SPEED: edited |= Editor("settings", modifier.settings.speed); break;
		case FieldParam::Type::MAGNET: edited |= Editor("settings", modifier.settings.magnet); break;
		case FieldParam::Type::NEWTON: edited |= Editor("settings", modifier.settings.newton); break;
		case FieldParam::Type::VORTEX: edited |= Editor("settings", modifier.settings.vortex); break;
		case FieldParam::Type::SPIN: edited |= Editor("settings", modifier.settings.spin); break;
		case FieldParam::Type::SPIN2: edited |= Editor("settings", modifier.settings.spin2); break;
		case FieldParam::Type::RANDOM: edited |= Editor("settings", modifier.settings.random); break;
		case FieldParam::Type::TAIL: edited |= Editor("settings", modifier.settings.tail); break;
		case FieldParam::Type::FLUCTUATION: edited |= Editor("settings", modifier.settings.fluctuation); break;
		default: ImGui::Text("not yet implemented"); break;
		}

		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, ElementParam::TransformParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("rotation", param.rotation);
		edited |= Editor("rotationFlags", param.rotationFlags);
		edited |= Editor("randomizedDirectionRotation", param.randomizedDirectionRotation);
		edited |= Editor("randomizedDirectionRotationFlags", param.randomizedDirectionRotationFlags);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[X][-90]", param.rotationRandomInitialAngleFlags[0], ElementParam::TransformParam::RandomAngleFlag::CCW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[X][+90]", param.rotationRandomInitialAngleFlags[0], ElementParam::TransformParam::RandomAngleFlag::CW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[X][+180]", param.rotationRandomInitialAngleFlags[0], ElementParam::TransformParam::RandomAngleFlag::CW_180);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[X][0]", param.rotationRandomInitialAngleFlags[0], ElementParam::TransformParam::RandomAngleFlag::CW_0);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Y][-90]", param.rotationRandomInitialAngleFlags[1], ElementParam::TransformParam::RandomAngleFlag::CCW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Y][+90]", param.rotationRandomInitialAngleFlags[1], ElementParam::TransformParam::RandomAngleFlag::CW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Y][+180]", param.rotationRandomInitialAngleFlags[1], ElementParam::TransformParam::RandomAngleFlag::CW_180);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Y][0]", param.rotationRandomInitialAngleFlags[1], ElementParam::TransformParam::RandomAngleFlag::CW_0);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Z][-90]", param.rotationRandomInitialAngleFlags[2], ElementParam::TransformParam::RandomAngleFlag::CCW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Z][+90]", param.rotationRandomInitialAngleFlags[2], ElementParam::TransformParam::RandomAngleFlag::CW_90);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Z][+180]", param.rotationRandomInitialAngleFlags[2], ElementParam::TransformParam::RandomAngleFlag::CW_180);
		edited |= CheckboxFlags("rotationRandomInitialAngleFlags[Z][0]", param.rotationRandomInitialAngleFlags[2], ElementParam::TransformParam::RandomAngleFlag::CW_0);
		edited |= Editor("angularVelocity", param.angularVelocity);
		edited |= Editor("angularVelocityFlags", param.angularVelocityFlags);
		edited |= Editor("size", param.size);
		edited |= Editor("sizeFlags", param.sizeFlags);
		edited |= Editor("scale", param.scale);
		edited |= Editor("scaleFlags", param.scaleFlags);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, ColorRandomSet& colorRandomSet) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		if (ImGui::TreeNode("color1")) {
			for (int i = 0; i < 32; i++) {
				ImGui::PushID(i);
				edited |= JitteredColorEditor("color1", colorRandomSet.color1[0][i], colorRandomSet.color1[1][i], colorRandomSet.color1[2][i]);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("color2")) {
			for (int i = 0; i < 32; i++) {
				ImGui::PushID(i);
				edited |= JitteredColorEditor("color2", colorRandomSet.color2[0][i], colorRandomSet.color2[1][i], colorRandomSet.color2[2][i]);
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		edited |= Editor("luminance1", colorRandomSet.luminance1);
		edited |= Editor("luminance2", colorRandomSet.luminance2);
		edited |= Editor("alpha1", colorRandomSet.alpha1);
		edited |= Editor("alpha2", colorRandomSet.alpha2);
		edited |= Editor("color1Count", colorRandomSet.color1Count);
		edited |= Editor("color2Count", colorRandomSet.color2Count);
		edited |= Editor("luminance1Count", colorRandomSet.luminance1Count);
		edited |= Editor("luminance2Count", colorRandomSet.luminance2Count);
		edited |= Editor("alpha1Count", colorRandomSet.alpha1Count);
		edited |= Editor("alpha2Count", colorRandomSet.alpha2Count);

		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, ColorParam& colorParam) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= JitteredColorEditor("color", colorParam.color1[0], colorParam.color1[1], colorParam.color1[2]);
		edited |= Editor("alpha1", colorParam.alpha1);
		edited |= JitteredColorEditor("color2", colorParam.color2[0], colorParam.color2[1], colorParam.color2[2]);
		edited |= Editor("alpha2", colorParam.alpha2);
		edited |= Editor("luminance1", colorParam.luminance1);
		edited |= Editor("luminance2", colorParam.luminance2);
		edited |= Editor("colorRandomSet", colorParam.colorRandomSet);
		edited |= Editor("unk7", colorParam.unk7);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, ElementParam::ModelParam& modelParam) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("hasModel", modelParam.hasModel);
		edited |= Editor("modelSetting1", modelParam.modelSetting1);
		edited |= Editor("gap6b", modelParam.gap6b);
		edited |= Editor("modelName", modelParam.modelName);
		edited |= Editor("skeletonName", modelParam.skeletonName);
		edited |= Editor("nodeAnimName", modelParam.nodeAnimName);
		ImGui::TreePop();
	}
	return edited;
}

static const char* particleTypeNames[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
	"UNK4",
	"UNK5_CPU",
	"HISTORICAL_STRIPE",
	"POINT_LIGHT",
};

bool Editor(const char* label, ElementParam& element) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("transformParam", element.transformParam);
		edited |= Editor("colorParams", element.colorParams);
		edited |= Editor("lifetime", element.lifetime);
		edited |= Editor("modelParam", element.modelParam);
		edited |= ComboEnum("particleType", element.particleType, particleTypeNames);
		//edited |= Editor("particleType2", element.particleType2);
		//edited |= Editor("particleInfo2", element.particleInfo2);
		edited |= Editor("textures", element.textures);
		edited |= Editor("textureCount", element.textureCount);
		edited |= Editor("childEffects", element.childEffects);
		edited |= Editor("fields", element.fields);
		edited |= CheckboxFlags("updateFlags[HAS_CHILDREN]", element.updateFlags, ElementParam::UpdateFlag::HAS_CHILDREN);
		edited |= CheckboxFlags("updateFlags[HAS_RANDOMIZED_ROTATION_DIRECTION]", element.updateFlags, ElementParam::UpdateFlag::HAS_RANDOMIZED_ROTATION_DIRECTION);
		edited |= CheckboxFlags("updateFlags[HAS_FIXED_ANGULAR_VELOCITY]", element.updateFlags, ElementParam::UpdateFlag::HAS_FIXED_ANGULAR_VELOCITY);
		edited |= CheckboxFlags("updateFlags[USE_GLOBAL_TIME]", element.updateFlags, ElementParam::UpdateFlag::USE_GLOBAL_TIME);
		edited |= CheckboxFlags("updateFlags[HAS_MODEL]", element.updateFlags, ElementParam::UpdateFlag::HAS_MODEL);
		edited |= CheckboxFlags("updateFlags[UNK5]", element.updateFlags, ElementParam::UpdateFlag::UNK5);
		edited |= CheckboxFlags("updateFlags[UNK6]", element.updateFlags, ElementParam::UpdateFlag::UNK6);
		edited |= CheckboxFlags("updateFlags[UNK7]", element.updateFlags, ElementParam::UpdateFlag::UNK7);
		edited |= CheckboxFlags("updateFlags[UNK8]", element.updateFlags, ElementParam::UpdateFlag::UNK8);
		edited |= CheckboxFlags("updateFlags[UNK9]", element.updateFlags, ElementParam::UpdateFlag::UNK9);
		edited |= CheckboxFlags("updateFlags[UNK10]", element.updateFlags, ElementParam::UpdateFlag::UNK10);
		edited |= CheckboxFlags("updateFlags[DISABLE_ANIMATIONS]", element.updateFlags, ElementParam::UpdateFlag::DISABLE_ANIMATIONS);
		edited |= CheckboxFlags("updateFlags[UNK12]", element.updateFlags, ElementParam::UpdateFlag::UNK12);
		edited |= CheckboxFlags("updateFlags[DISABLE_FIELDS]", element.updateFlags, ElementParam::UpdateFlag::DISABLE_FIELDS);
		edited |= CheckboxFlags("gpuParticleFlags[DISABLE_ANIMATIONS]", element.gpuParticleFlags, ElementParam::GpuParticleFlag::IS_GPU);
		edited |= CheckboxFlags("gpuParticleFlags[UNK12]", element.gpuParticleFlags, ElementParam::GpuParticleFlag::UNK1);
		edited |= CheckboxFlags("gpuParticleFlags[DISABLE_FIELDS]", element.gpuParticleFlags, ElementParam::GpuParticleFlag::UNK2);
		edited |= Editor("vectorFieldName", element.vectorFieldName);
		edited |= Editor("vectorFieldSize", element.vectorFieldSize);
		edited |= Editor("unkVector2", element.unkVector2);
		edited |= Editor("vectorFieldParam", element.vectorFieldParam);
		edited |= Editor("unkVector1", element.unkVector1);
		edited |= Editor("depthCollision", element.depthCollision);
		//edited |= Editor("totalAnimationCount", element.totalAnimationCount);
		edited |= Editor("unk7e", element.unk7e);

		//ImGui::Text("Child effects type: %s", element.unkChildEffectFlags.test(ElementParam::UnkChildEffectsFlag::BASIC_CHILD_EFFECTS) ? "BASIC" : "ANIMATED");

		//if (element.unkChildEffectFlags.test(ElementParam::UnkChildEffectsFlag::BASIC_CHILD_EFFECTS)) {
		//	edited |= Editor("unkChildEffects", element.childEffectsParam.basic);
		//}
		//else if (element.unkChildEffectFlags.test(ElementParam::UnkChildEffectsFlag::ANIMATED_CHILD_EFFECTS)) {
		//	edited |= Editor("unkChildEffects", element.childEffectsParam.animated);
		//}
		ImGui::TreePop();
	}
	return edited;
}

static const char* transformTypeNames[]{ "UNK0", "UNK1", "ROTATED" };

bool Editor(const char* label, EmitterParam::TransformParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("position", param.position);
		edited |= Editor("rotation", param.rotation);
		edited |= Editor("scale", param.scale);
		edited |= ComboEnum("type", param.type, transformTypeNames);
		ImGui::TreePop();
	}
	return edited;
}

bool Editor(const char* label, EmitterParam::EmissionParam& emission) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= ComboEnum("shape", emission.shape, shapeNames);
		edited |= Editor("positionJitter", emission.positionJitter);
		edited |= Editor("spread", emission.spread);
		edited |= Editor("startAngle", emission.startAngle);
		edited |= Editor("endAngle", emission.endAngle);
		edited |= Editor("useRadialDistribution", emission.useRadialDistribution);
		edited |= Editor("consistentAngle", emission.consistentAngle);
		edited |= Editor("useSubdivisions", emission.useSubdivisions);
		edited |= Editor("subdivisionCount", emission.subdivisionCount);
		ImGui::TreePop();
	}
	return edited;
}

static const char* emitModeNames[]{ "UNK0", "UNK1" };
static const char* attenuationModeNames[]{ "FIXED", "POSE_POSITION" };

bool Editor(const char* label, EmitterParam::EmitParam& emit) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= ComboEnum("emissionMode", emit.emissionMode, emitModeNames);
		edited |= Editor("emitInterval", emit.emitInterval);
		edited |= Editor("emissionCount", emit.emissionCount);
		edited |= ComboEnum("attenuationMode", emit.attenuationMode, attenuationModeNames);
		edited |= Editor("minDistance", emit.minDistance);
		edited |= Editor("maxDistance", emit.maxDistance);
		edited |= Editor("distanceAttenuation", emit.distanceAttenuation); // 0->1
		edited |= Editor("duration", emit.duration);
		edited |= Editor("startDelay", emit.startDelay);
		edited |= Editor("fadeSpeed", emit.fadeSpeed);
		ImGui::TreePop();
	}
	return edited;
}

static const char* deathModeNames[]{ "WAIT_FOR_ELEMENTS", "KILL_AFTER_DELAY", "WAIT_FOR_ELEMENTS_OR_KILL_AFTER_DELAY", "IMMEDIATE" };

bool Editor(const char* label, EmitterParam::DeathParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= ComboEnum("mode", param.mode, deathModeNames);
		edited |= Editor("killDelay", param.killDelay);
		ImGui::TreePop();
	}
	return edited;
}

static const char* spreadDirectionTypeNames[]{ "DIRECTION", "ANGLES" };

bool Editor(const char* label, EmitterParam::EmissionDynamicsParam& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= Editor("direction1Velocity", param.settings.direction1Velocity);
		edited |= Editor("direction2Velocity", param.settings.direction2Velocity);
		edited |= Editor("randomDirectionVelocity", param.settings.randomDirectionVelocity);
		edited |= Editor("localSpreadVelocity", param.settings.localSpreadVelocity);
		edited |= Editor("localSpread", param.settings.localSpread);
		edited |= Editor("worldSpreadVelocity", param.settings.worldSpreadVelocity);
		edited |= Editor("worldSpread", param.settings.worldSpread);
		edited |= ComboEnum("worldSpreadDirectionType", param.settings.worldSpreadDirectionType, spreadDirectionTypeNames);
		edited |= Editor("worldSpreadAnglesOrDirection", param.settings.worldSpreadAnglesOrDirection);
		edited |= Editor("initialVelocityJitter", param.settings.initialVelocityJitter);
		edited |= Editor("velocityJitter", param.settings.velocityJitter);
		ImGui::TreePop();
	}
	return edited;
}

static const char* unk0ParamModeNames[]{ "ANISOTROPIC", "ISOTROPIC" };

bool Editor(const char* label, EmitterParam::Unk0Param& param) {
	bool edited{};
	if (ImGui::TreeNode(label)) {
		edited |= CheckboxFlags("UNK0", param.flags, EmitterParam::Unk0Param::Flag::UNK0);
		edited |= CheckboxFlags("UNK1", param.flags, EmitterParam::Unk0Param::Flag::UNK1);
		edited |= ComboEnum("mode", param.mode, unk0ParamModeNames);
		edited |= Editor("position", param.position);
		edited |= Editor("scale", param.scale);
		ImGui::TreePop();
	}
	return edited;
}
static const char* inheritFlagNames[]{
	"TRANSLATION_RAW",
	"ROTATION",
	"TRANSLATION",
	"TRANSLATION_RELATIVE",
	"INHERIT_DIRECT",
};

static const char* unkFlagsNames[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
	"USE_TEXTURES",
	"USE_RENDER_PRIORITY",
	"UNK6",
	"USE_UNKFLOAT0_RANGE",
	"USE_UNKFLOAT1_RANGE",
	"UNK9",
	"UNK10",
	"UNK11",
};

static const char* cullModeNames[]{
	"NONE",
	"FRONT",
	"BACK",
};

static const char* maskType1Names[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
};

static const char* maskType2Names[]{
	"UNK0",
	"UNK1",
	"UNK2",
	"UNK3",
	"UNK4",
};

static const char* maskType3Names[]{
	"UNK0",
	"UNK1",
	"UNK2",
};

static const char* depthModeNames[]{
	"BLENDING_ENABLED_WITH_DEPTH_TEST",
	"BLENDING_ENABLED",
	"BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE",
	"BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE_2",
	"BLENDING_ENABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE",
};

static const char* blendModeNames[]{
	"MIX",
	"ADD",
	"MULTIPLY",
	"SUBTRACT",
};

static const char* billboardTypeNames[] {
	"UNK0",
	"UNK1",
	"UNK2",
};

bool Editor(const char* label, EmitterParam& emitter) {
	bool edited{};
	ImGui::PushID(label);
	edited |= Editor("transform", emitter.transformParam);
	edited |= Editor("emission", emitter.emissionParam);
	edited |= Editor("emit", emitter.emitParam);
	edited |= Editor("deathParam", emitter.deathParam);
	edited |= Editor("emissionDynamicsParam", emitter.emissionDynamicsParam);
	edited |= Editor("inheritRatio", emitter.inheritTransformRatio);
	edited |= Editor("inheritRatio", emitter.inheritPoseRatio);
	edited |= Editor("randomSeed", emitter.randomSeed);
	edited |= ComboEnum("blendMode", emitter.blendMode, blendModeNames);
	edited |= ComboEnum("depthMode", emitter.depthMode, depthModeNames);
	edited |= ComboEnum("cullMode", emitter.cullMode, cullModeNames);
	edited |= CheckboxFlags("inheritTransformFlags[TRANSLATION_RAW]", emitter.inheritTransformFlags, EmitterParam::InheritFlag::TRANSLATION_RAW);
	edited |= CheckboxFlags("inheritTransformFlags[ROTATION]", emitter.inheritTransformFlags, EmitterParam::InheritFlag::ROTATION);
	edited |= CheckboxFlags("inheritTransformFlags[TRANSLATION]", emitter.inheritTransformFlags, EmitterParam::InheritFlag::TRANSLATION);
	edited |= CheckboxFlags("inheritTransformFlags[TRANSLATION_RELATIVE]", emitter.inheritTransformFlags, EmitterParam::InheritFlag::TRANSLATION_RELATIVE);
	edited |= CheckboxFlags("inheritTransformFlags[INHERIT_DIRECT ]", emitter.inheritTransformFlags, EmitterParam::InheritFlag::INHERIT_DIRECT);
	edited |= Editor("inheritTransformRatio", emitter.inheritTransformRatio);
	edited |= CheckboxFlags("inheritPoseFlags[TRANSLATION_RAW]", emitter.inheritPoseFlags, EmitterParam::InheritFlag::TRANSLATION_RAW);
	edited |= CheckboxFlags("inheritPoseFlags[ROTATION]", emitter.inheritPoseFlags, EmitterParam::InheritFlag::ROTATION);
	edited |= CheckboxFlags("inheritPoseFlags[TRANSLATION]", emitter.inheritPoseFlags, EmitterParam::InheritFlag::TRANSLATION);
	edited |= CheckboxFlags("inheritPoseFlags[TRANSLATION_RELATIVE]", emitter.inheritPoseFlags, EmitterParam::InheritFlag::TRANSLATION_RELATIVE);
	edited |= CheckboxFlags("inheritPoseFlags[INHERIT_DIRECT ]", emitter.inheritPoseFlags, EmitterParam::InheritFlag::INHERIT_DIRECT);
	edited |= Editor("inheritPoseRatio", emitter.inheritPoseRatio);
	edited |= CheckboxFlags("unkFlags[UNK0]", emitter.unkFlags, EmitterParam::UnkFlags::UNK0);
	edited |= CheckboxFlags("unkFlags[UNK1]", emitter.unkFlags, EmitterParam::UnkFlags::UNK1);
	edited |= CheckboxFlags("unkFlags[UNK2]", emitter.unkFlags, EmitterParam::UnkFlags::UNK2);
	edited |= CheckboxFlags("unkFlags[UNK3]", emitter.unkFlags, EmitterParam::UnkFlags::UNK3);
	edited |= CheckboxFlags("unkFlags[USE_TEXTURES]", emitter.unkFlags, EmitterParam::UnkFlags::USE_TEXTURES);
	edited |= CheckboxFlags("unkFlags[USE_RENDER_PRIORITY]", emitter.unkFlags, EmitterParam::UnkFlags::USE_RENDER_PRIORITY);
	edited |= CheckboxFlags("unkFlags[UNK6]", emitter.unkFlags, EmitterParam::UnkFlags::UNK6);
	edited |= CheckboxFlags("unkFlags[USE_UNKFLOAT0_RANGE]", emitter.unkFlags, EmitterParam::UnkFlags::USE_UNKFLOAT0_RANGE);
	edited |= CheckboxFlags("unkFlags[USE_UNKFLOAT1_RANGE]", emitter.unkFlags, EmitterParam::UnkFlags::USE_UNKFLOAT1_RANGE);
	edited |= CheckboxFlags("unkFlags[UNK9]", emitter.unkFlags, EmitterParam::UnkFlags::UNK9);
	edited |= CheckboxFlags("unkFlags[UNK10]", emitter.unkFlags, EmitterParam::UnkFlags::UNK10);
	edited |= CheckboxFlags("unkFlags[UNK11]", emitter.unkFlags, EmitterParam::UnkFlags::UNK11);
	edited |= Editor("unkFloat4", emitter.unkFloat4);
	edited |= Editor("unkFloat5", emitter.unkFloat5);
	edited |= Editor("unkFloat0RangeStart", emitter.unkFloat0RangeStart);
	edited |= Editor("unkFloat0RangeEnd", emitter.unkFloat0RangeEnd);
	edited |= Editor("unkFloat1RangeStart", emitter.unkFloat1RangeStart);
	edited |= Editor("unkFloat1RangeEnd", emitter.unkFloat1RangeEnd);
	edited |= Editor("unkFloat2RangeStart", emitter.unkFloat2RangeStart);
	edited |= Editor("unkFloat2RangeEnd", emitter.unkFloat2RangeEnd);
	edited |= Editor("unkFloat6RangeEnd", emitter.unkFloat6RangeEnd);
	edited |= Editor("unkFloat6RangeStart", emitter.unkFloat6RangeStart);
	edited |= Editor("gap6b", emitter.gap6b);
	edited |= Editor("unkFloatFoo0", emitter.unkFloatFoo0);
	edited |= Editor("unkFloatFoo1", emitter.unkFloatFoo1);
	edited |= Editor("unkFloatFoo2", emitter.unkFloatFoo2);
	edited |= Editor("randomSeed", emitter.randomSeed);
	edited |= Editor("renderLayer", emitter.renderLayer);
	edited |= Editor("unk6b2", emitter.unk6b2);
	edited |= Editor("unk6b3", emitter.unk6b3);
	edited |= Editor("unk6b4", emitter.unk6b4);
	edited |= Editor("unkType5", emitter.unkType5);
	edited |= ComboEnum("billboardType", emitter.billboardType, billboardTypeNames);
	edited |= Editor("enableUnkFloat2Range", emitter.enableUnkFloat2Range);
	edited |= Editor("gap6b2", emitter.gap6b2);
	edited |= Editor("unkAlpha", emitter.unkAlpha);
	edited |= Editor("unk0Param", emitter.unk0Param);
	edited |= Editor("gap6aa2", emitter.gap6aa2);
	edited |= ComboEnum("unkType0", emitter.unkType0, maskType1Names);
	edited |= Editor("unkType0a", emitter.unkType0a);
	edited |= ComboEnum("unkType0c", emitter.unkType0c, maskType2Names);
	edited |= Editor("unkType0d", emitter.unkType0d);
	edited |= ComboEnum("unkType1", emitter.unkType1, maskType3Names);
	edited |= ComboEnum("unkType1a", emitter.unkType1a, maskType1Names);
	edited |= Editor("unkType1b", emitter.unkType1b);
	edited |= ComboEnum("unkType2", emitter.unkType2, maskType3Names);
	edited |= ComboEnum("unkType3", emitter.unkType3, maskType2Names);
	edited |= Editor("unkType4", emitter.unkType4);
	edited |= Editor("elementParam", emitter.elementParam);
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
	auto* emitter = &effect->emitterParam;

	Editor("name", effect->name);
	Editor("emitterCount", effect->emitterCount);
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
