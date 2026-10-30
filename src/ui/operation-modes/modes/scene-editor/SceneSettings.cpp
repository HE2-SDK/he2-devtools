#include "SceneSettings.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>

namespace ui::operation_modes::modes::scene_editor {
	void SceneSettings::RenderPanel()
	{
		auto& context = GetContext();

		char sceneName[400];

		if (context.sceneCtrl != nullptr)
			snprintf(sceneName, sizeof(sceneName), "%s - %zx", context.sceneCtrl->sceneName.c_str(), reinterpret_cast<size_t>(context.sceneCtrl));
		else
			strcpy_s(sceneName, "none");

		if (ImGui::BeginCombo("Scene Control", sceneName)) {
			if (auto* sceneMgr = hh::game::GameManager::GetInstance()->GetService<hh::scene::SceneManager>()) {
				for (auto* sceneCtrl : sceneMgr->sceneControls) {
					if (ImGui::Selectable(sceneCtrl->sceneName.c_str(), sceneCtrl == context.sceneCtrl)) {
						context.sceneCtrl = sceneCtrl;
						Dispatch(SceneChangedAction{});
					}
					if (sceneCtrl == context.sceneCtrl)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (context.sceneCtrl == nullptr) {
            ImGui::Text("No Scene Control selected");
			return;
		}

		auto* resource = context.sceneCtrl->GetResource();
		auto* data = ((ucsl::resources::scene::v106::SceneData*)resource->unpackedBinaryData);
		csl::ut::MoveArray<csl::ut::Pair<const char*, csl::ut::MoveArray<ucsl::resources::scene::v106::ControlNode*>>> nodes{ hh::fnd::MemoryRouter::GetInstance()->GetTemp() };
		ImGui::SeparatorText("Control Nodes");
		for (int i = 0; i < data->sceneControl->nodeCount; i++) {
			auto* ctrl = data->sceneControl->nodes[i];
			if (nodes.size() == 0) {
				nodes.push_back({ ctrl->nodeName, { hh::fnd::MemoryRouter::GetInstance()->GetTemp() } });
				nodes[0].second.push_back(ctrl);
				continue;
			}
			bool hasSet{ false };
			for (auto& x : nodes) {
				if (strcmp(x.first, ctrl->nodeName) == 0) {
					x.second.push_back(ctrl);
					hasSet = true;
				}
			}
			if (!hasSet) {
				nodes.push_back({ ctrl->nodeName, { hh::fnd::MemoryRouter::GetInstance()->GetTemp() } });
				nodes[nodes.size() - 1].second.push_back(ctrl);
			}
		}

		for (auto& x : nodes) {
			ImGui::PushID(&x.second);
			if (ImGui::TreeNode(x.first)) {
				for (auto* y : x.second) {
					ImGui::PushID(y);
					if (ImGui::TreeNode(y->parameterName)) {
						bool changed{ false };
						switch (y->type) {
						case ucsl::resources::scene::v106::ControlNode::Type::BOOL:
							changed |= Editor("Value", y->value.b);
							break;
						case ucsl::resources::scene::v106::ControlNode::Type::FLOAT:
							changed |= Editor("Value", y->value.f);
							break;
						case ucsl::resources::scene::v106::ControlNode::Type::DOUBLE:
							changed |= Editor("Value", y->value.d);
							break;
						case ucsl::resources::scene::v106::ControlNode::Type::INTEGER:
							changed |= Editor("Value", y->value.i);
							break;
						case ucsl::resources::scene::v106::ControlNode::Type::STRING:
							changed |= InputText("Value", y->value.s, resource);
							break;
						}

						if (changed) {
							auto* ctrl = context.sceneCtrl->GetControlNode(x.first, y->parameterName);
							ctrl->value.value.s = y->value.s;
							ctrl->finalValue.value.s = y->value.s;
							context.sceneCtrl->FireUpdateControlNodesCallback();
						}

						ImGui::TreePop();
					}
					ImGui::PopID();
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	}

	PanelTraits SceneSettings::GetPanelTraits() const
	{
		return { "Scene Settings", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}
}
