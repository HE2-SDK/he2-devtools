#include "SceneSettings.h"
#include "Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/inputs/Basic.h>
#include <ui/GlobalSettings.h>
#include <dieventlib/dvscene.h>

namespace ui::operation_modes::modes::dvscene_editor {
	bool DvCommonEditor(dv::DvCommon* dvCommon) {
		bool changed = false;
		int start = static_cast<int>(dvCommon->frameStart);
		if(changed |= Editor("Start", start))
			dvCommon->frameStart = start;
		int end = static_cast<int>(dvCommon->frameEnd);
		if (changed |= Editor("End", end))
			dvCommon->frameEnd = end;
		return changed;
	}

	void SceneSettings::RenderPanel()
	{
		auto& context = GetContext();
		
		if (auto* evtPlayer = hh::game::GameManager::GetInstance()->GetService<app::evt::EventPlayer>()) {
			
			Editor("Cutscene Name", context.cutsceneName);
			if (ImGui::Button("Play Cutscene")) {
				app::evt::EventSetupData setupData{};
				setupData.Setup(context.cutsceneName.c_str());
				evtPlayer->PlayEvent(setupData);
			}
		}

		char sceneName[400];

		if (context.goDVSC != nullptr)
			snprintf(sceneName, sizeof(sceneName), "%s - %zx", context.goDVSC->cutsceneName.c_str(), reinterpret_cast<size_t>(&*context.goDVSC));
		else
			strcpy_s(sceneName, "none");

		if (ImGui::BeginCombo("DvSceneControl", sceneName)) {
			for (auto* obj : hh::game::GameManager::GetInstance()->objects)
				if (obj->objectClass == hh::dv::DvSceneControl::GetClass()) {
					auto* dvsc = reinterpret_cast<hh::dv::DvSceneControl*>(obj);
					snprintf(sceneName, sizeof(sceneName), "%s - %zx", dvsc->cutsceneName.c_str(), reinterpret_cast<size_t>(obj));
					if (ImGui::Selectable(sceneName, obj == context.goDVSC)) {
						GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
						context.goDVSC = dvsc;
						for (auto* evtScn : hh::game::GameManager::GetInstance()->GetService<app::evt::EventPlayer>()->evtSceneMgr->evtScenes)
							if (strcmp(evtScn->setupData.playInfo.cutsceneName, dvsc->cutsceneName.c_str()) == 0)
								context.evtScene = evtScn;
						context.parsedScene = new dv::DvScene;
						context.parsedScene->read(static_cast<char*>(dvsc->resource->binaryData), dvsc->resource->size);
						context.CreateWrapperPages();
					}
					if (context.goDVSC == obj)
						ImGui::SetItemDefaultFocus();
				}
			ImGui::EndCombo();
		}

		if (context.goDVSC == nullptr) {
            ImGui::Text("No DvSceneControl selected");
			return;
		}
		
		if (ImGui::Button("Export")) {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = context.goDVSC;
			ImGuiFileDialog::Instance()->OpenDialog("ResDvSceneExportDialog", "Choose File", ".dvscene", cfg);
		}

		if (ImGuiFileDialog::Instance()->Display("ResDvSceneExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* dvsc = static_cast<hh::dv::DvSceneControl*>(ImGuiFileDialog::Instance()->GetUserDatas());
				dv::internal::Buffer buffer = context.parsedScene->write();
				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				ofs.write(buffer.data, buffer.size);
				ofs.close();
			}
			ImGuiFileDialog::Instance()->Close();
		}

		auto* timeline = context.goDVSC->timeline;
        ImGui::SeparatorText("DvCommon Settings");
		if (DvCommonEditor(context.parsedScene->dvCommon)) {
			timeline->frameStart = context.parsedScene->dvCommon->frameStart * 100;
			timeline->frameEnd = context.parsedScene->dvCommon->frameEnd * 100;
		}
	}

	PanelTraits SceneSettings::GetPanelTraits() const
	{
		return { "Scene Settings", ImVec2(ImGui::GetMainViewport()->WorkSize.x-250, ImGui::GetMainViewport()->WorkSize.y - 200), ImVec2(250, 200) };
	}
}
