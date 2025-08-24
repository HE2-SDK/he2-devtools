#include "EventPlayer.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void RenderGameServiceInspector(app::evt::EventPlayer& service)
{
    Viewer("eventPlayerListeners amount", service.listeners.size());
    if (ImGui::TreeNode("EventSceneManager")) {
        Viewer("evtScenesCount", service.evtSceneMgr->evtScenes.size());
        if (ImGui::TreeNode("cutsceneNames")) {
            for (auto& x : service.evtSceneMgr->cutsceneNames)
                ImGui::Text(x.c_str());
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Event Scenes")) {
            for (auto* x : service.evtSceneMgr->evtScenes) {
                if (ImGui::TreeNode(x->setupData.cutsceneName)) {
                    Editor("unk2", x->unk2);
                    Editor("unk3", x->unk3);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("EventEnvironmentManager")) {
        Viewer("evtEnvsCount", service.evtEnvMgr->evtEnvs.size());
        ImGui::TreePop();
    }
}
