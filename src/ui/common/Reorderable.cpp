#include "Reorderable.h"
#include "icons.h"

bool Reorderable(const char* label, int& index, int count) {
	auto pos = ImGui::GetCursorPos();

	ImGui::Text("%s", GetIconGlyph(IconId::REORDERABLE));
	ImGui::SetCursorPos(pos);
	ImGui::InvisibleButton(label, ImVec2(16.0f, 16.0f));

	if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
		int nextIndex = index + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);

		if (nextIndex >= 0 && nextIndex < count) {
			ImGui::ResetMouseDragDelta();

			index = nextIndex;
			return true;
		}
	}

	return false;
}
