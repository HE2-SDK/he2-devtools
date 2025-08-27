#include "GOCUIComposition.h"
#include <ui/common/viewers/Basic.h>

void Viewer(const char* label, const hh::ui::UIElement& element) {
	if (ImGui::TreeNode(&element, "%s", label)) {
		Viewer("Name", element.name);
		Viewer("Type", element.type);
		ImGui::TreePop();
	}
}

void Viewer(const char* label, const hh::ui::UIElementGroup& group) {
	if (ImGui::TreeNode(&group, "%s", label)) {
		for (auto& element : group.elements)
			Viewer(element->name, *element);
		Viewer("Is current", group.isCurrentGroup);
		ImGui::TreePop();
	}
}

void Viewer(const char* label, const hh::ui::UIElementGroupContainer& container) {
	if (ImGui::TreeNode(&container, "%s", label)) {
		ImGui::SeparatorText("Child containers");
		for (auto it = container.childContainersByName.begin(); it != container.childContainersByName.end(); it++)
			Viewer(it.key(), **it);
		
		ImGui::SeparatorText("Elements");
		for (auto it = container.elementGroupsByName.begin(); it != container.elementGroupsByName.end(); it++)
			Viewer(it.key(), **it);

		ImGui::TreePop();
	}
}

void RenderComponentInspector(hh::ui::GOCUIComposition& component) {
	Viewer("Root container", *component.rootContainer);
}
