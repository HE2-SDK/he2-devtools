#include <ui/operation-modes/modes/object-inspection/component-inspectors/GOCPlayerParameter.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/Desktop.h>
#include <ui/resources/editors/ResReflectionEditor.h>

template<typename R>
void RenderModeParameterInspector(const char* label, R (&parameters)[4]) {
    if (ImGui::TreeNode(label)) {
        if (ImGui::TreeNode("Normal")) {
            Editor("Parameters", *parameters[0]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Water")) {
            Editor("Parameters", *parameters[1]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Cyberspace Forward View")) {
            Editor("Parameters", *parameters[2]);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Cyberspace Side View")) {
            Editor("Parameters", *parameters[3]);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}

const char* playerParameterModes[] = { "Normal", "Water", "Cyberspace Forward View", "Cyberspace Side View" };
const char* playerParameterSuperStates[] = { "Normal", "Super Sonic", "Super Sonic 2" };

void RenderComponentInspector(app::player::GOCPlayerParameter& component) {
	ImGui::Text("Character:");
	ImGui::SameLine();
	switch (component.characterId) {
	case app::player::CharacterId::SONIC:
		ImGui::Text("Sonic");
		break;
	case app::player::CharacterId::AMY:
		ImGui::Text("Amy");
		break;
	case app::player::CharacterId::KNUCKLES:
		ImGui::Text("Knuckles");
		break;
	case app::player::CharacterId::TAILS:
		ImGui::Text("Tails");
		break;
	default:
		ImGui::Text("Unknown");
		break;
	}

	ComboEnum("Mode", component.mode, playerParameterModes);
	ComboEnum("Super state", component.superState, playerParameterSuperStates);

	if (ImGui::Button("Edit character parameters..."))
		switch (component.characterId) {
		case app::player::CharacterId::SONIC:
			ResReflectionEditor::Create(Desktop::instance->GetAllocator(), (hh::fnd::ResReflectionT<heur::rfl::SonicParameters>*)&*component.characterParameters);
			break;
		case app::player::CharacterId::AMY:
			ResReflectionEditor::Create(Desktop::instance->GetAllocator(), (hh::fnd::ResReflectionT<heur::rfl::AmyParameters>*)&*component.characterParameters);
			break;
		case app::player::CharacterId::KNUCKLES:
			ResReflectionEditor::Create(Desktop::instance->GetAllocator(), (hh::fnd::ResReflectionT<heur::rfl::KnucklesParameters>*)&*component.characterParameters);
			break;
		case app::player::CharacterId::TAILS:
			ResReflectionEditor::Create(Desktop::instance->GetAllocator(), (hh::fnd::ResReflectionT<heur::rfl::TailsParameters>*)&*component.characterParameters);
			break;
		}

	if (ImGui::Button("Edit Camera Set parameters..."))
		ResReflectionEditor::Create(Desktop::instance->GetAllocator(), &*component.cameraSetParameters);

	if (ImGui::TreeNode("Currently loaded mode parameters")) {
		ImGui::SeparatorText("Mode packages");
		RenderModeParameterInspector("Mode packages", component.modePackages);
		if (ImGui::TreeNode("Water mode package")) {
			Editor("Parameters", *component.waterModePackage);
			ImGui::TreePop();
		}
		ImGui::SeparatorText("Parameters");
		RenderModeParameterInspector("common", component.commonParameters);
		RenderModeParameterInspector("speed", component.speedParameters);
		RenderModeParameterInspector("jump", component.jumpParameters);
		RenderModeParameterInspector("jumpSpeed", component.jumpSpeedParameters);
		RenderModeParameterInspector("doubleJump", component.doubleJumpParameters);
		RenderModeParameterInspector("boost", component.boostParameters);
		RenderModeParameterInspector("airBoost", component.airBoostParameters);
		ImGui::TreePop();
	}
}
