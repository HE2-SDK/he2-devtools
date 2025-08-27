#include "GOCPlayerVisual.h"
bool Editor(const char* label, app::player::GOCPlayerVisual::Unk1& val);
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Effects.h>

bool Editor(const char* label, app::player::GOCPlayerVisual::Unk1& val) {
	bool edited{};

	if (ImGui::TreeNode(&val, "%s", "Unk1")) {
		edited |= Editor("unk1", val.unk1);
		edited |= Editor("unk2", val.unk2);
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::player::Component& component) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		ImGui::SeparatorText("Effects");
		for (auto& eff : component.effects) {
			ImGui::PushID(&eff);
			Editor("Effect", eff);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::player::ComponentCollection& collection) {
	bool edited{};

	if (ImGui::TreeNode(&collection, "%s", label)) {
		edited |= CheckboxFlags("Unk0", collection.flags, app::player::ComponentCollection::Flag::UNK0);
		edited |= CheckboxFlags("Visible", collection.flags, app::player::ComponentCollection::Flag::VISIBLE);

		ImGui::SeparatorText("Components");
		size_t idx{};
		for (auto& component : collection.components) {
			ImGui::PushID(idx++);
			Editor("Component", component);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, app::player::PlayerVisual& visual) {
	bool edited{};

	if (ImGui::TreeNode(&visual, "%s", label)) {
		edited |= CheckboxFlags("Unk0", visual.flags, app::player::PlayerVisual::Flag::UNK0);
		edited |= CheckboxFlags("Is current", visual.stateFlags, app::player::PlayerVisual::StateFlag::IS_CURRENT);

		ImGui::SeparatorText("Components");
		Editor("Components", *visual.componentCollection);
		ImGui::TreePop();
	}

	return edited;
}

void RenderComponentInspector(app::player::GOCPlayerVisual& component) {
	Editor("Unk7", component.unk7);
	Editor("unk10", component.unk10);
	Editor("Flags", component.flags);
	
	ImGui::SeparatorText("Visuals");
	size_t idx{};
	for (auto& visual : component.visuals) {
		ImGui::PushID(idx++);
		Editor("Visual", *visual);
		ImGui::PopID();
	}
}
