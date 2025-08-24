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
					if (ImGui::Selectable(sceneCtrl->sceneName.c_str(), sceneCtrl == context.sceneCtrl))
						context.sceneCtrl = sceneCtrl;
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

		auto* data = ((ucsl::resources::scene::v106::SceneData*)context.sceneCtrl->resource->unpackedBinaryData);
		ImGui::SeparatorText("Control Nodes");
		for (int i = 0; i < data->sceneControl->nodeCount; i++) {
			auto& ctrl = *data->sceneControl->nodes[i];
			ImGui::PushID(&ctrl);
			char ctrlName[500];
			snprintf(ctrlName, sizeof(ctrlName), "%s - %s - %d", ctrl.nodeName, ctrl.parameterName, ctrl.type);
			if (ImGui::TreeNode(ctrlName)) {
				switch ((hh::scene::SceneControl::ControlNode::ValueType)ctrl.type) {
				case hh::scene::SceneControl::ControlNode::ValueType::BOOLEAN:
					Editor("Value", ctrl.value.b);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::FLOAT:
					Editor("Value", ctrl.value.f);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::DOUBLE:
					Editor("Value", ctrl.value.d);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::INTEGER:
					Editor("Value", ctrl.value.i);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::STRING:
					Viewer("Value", ctrl.value.s);
					break;
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::SeparatorText("Control Nodes");
		for (int i = 0; i < data->sceneControl->nodeCount; i++) {
			auto& ctrl = *data->sceneControl->nodes[i];
			ImGui::PushID(&ctrl);
			char ctrlName[500];
			snprintf(ctrlName, sizeof(ctrlName), "%s - %s - %d", ctrl.nodeName, ctrl.parameterName, ctrl.type);
			if (ImGui::TreeNode(ctrlName)) {
				switch ((hh::scene::SceneControl::ControlNode::ValueType)ctrl.type) {
				case hh::scene::SceneControl::ControlNode::ValueType::BOOLEAN:
					Editor("Value", ctrl.value.b);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::FLOAT:
					Editor("Value", ctrl.value.f);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::DOUBLE:
					Editor("Value", ctrl.value.d);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::INTEGER:
					Editor("Value", ctrl.value.i);
					break;
				case hh::scene::SceneControl::ControlNode::ValueType::STRING:
					Viewer("Value", ctrl.value.s);
					break;
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
