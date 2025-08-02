#ifndef DEVTOOLS_TARGET_SDK_wars
#include "Effects.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

bool Editor(const char* label, hh::eff::EffectHandle& handle) {
	bool edited{};

	if (ImGui::TreeNode(label)) {
		Viewer("Cyan handle", handle.unk1);
		Viewer("Name", handle.GetName());
		
		bool paused{ handle.IsPaused() };
		bool visible{ handle.IsVisible() };

		if (ImGui::Button("Stop")) {
			handle.Stop();
			edited |= true;
		}

		ImGui::SameLine();

		if (Editor("Paused", paused)) {
			handle.SetPause(paused);
			edited |= true;
		}

		ImGui::SameLine();

		if (Editor("Visible", visible)) {
			handle.SetVisible(visible);
			edited |= true;
		}

		ImGui::TreePop();
	}
	
	return edited;
}
#endif
