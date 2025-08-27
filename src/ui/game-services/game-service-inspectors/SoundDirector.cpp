#include "SoundDirector.h"
#include <ui/Desktop.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/resources/editors/ResReflectionEditor.h>

void RenderUnk3Editor(const char* name, app::snd::SoundDirector::Unk3& unk3) {
    if (ImGui::TreeNode(&unk3, "%s", name)) {
        Editor("name", unk3.name);
        if (ImGui::TreeNode("Tween")) {
            Editor("Duration", unk3.tween.duration);
            Editor("Ratio", unk3.tween.ratio);
            Editor("StartRatio", unk3.tween.startRatio);
            Editor("TargetRatio", unk3.tween.targetRatio);
            Editor("State", unk3.tween.state);
            Editor("Step", unk3.tween.step);
            ImGui::TreePop();
        }
        Editor("unk102", unk3.unk102);
        Editor("unk103", unk3.unk103);
        Editor("unk104", unk3.unk104);
        Editor("unk105", unk3.unk105);
        Editor("unk106", unk3.unk106);
        Editor("unk107", unk3.unk107);
        Editor("unk107b", unk3.unk107b);
        Editor("unk107c", unk3.unk107c);
        Editor("unk107d", unk3.unk107d);
        ImGui::TreePop();
    }
}

void RenderGameServiceInspector(app::snd::SoundDirector& service) {
    RenderUnk3Editor("Static channel 1", service.unkA0[0]);
    RenderUnk3Editor("Static channel 2", service.unkA0[1]);
    RenderUnk3Editor("Static channel 3", service.unkA0[2]);

    size_t i{};
    for (auto& item : service.unk1A0) {
        char name[30];
        snprintf(name, sizeof(name), "Extra channel %zd", i++);
        RenderUnk3Editor("Static channel 1", item);
    }

    if (ImGui::Button("Open common sound parameters"))
        ResReflectionEditor::Create(Desktop::instance->GetAllocator(), service.soundCommonParameter);
    if (ImGui::Button("Open BGM info parameters"))
        ResReflectionEditor::Create(Desktop::instance->GetAllocator(), service.bgmInfoParameter);
    if (ImGui::Button("Open ambience sound parameters"))
        ResReflectionEditor::Create(Desktop::instance->GetAllocator(), service.ambSoundParameter);
    if (ImGui::Button("Open custom music parameters"))
        ResReflectionEditor::Create(Desktop::instance->GetAllocator(), service.customMusicParameter);

    Editor("dword88", service.dword88);
    //Editor("unk90.unk1", service.unk90.unk1);
    //Editor("unk90.unk2", service.unk90.unk2);
    Editor("dword260", service.dword260);
    Editor("qword264", service.qword264);
    Editor("qword268", service.qword268);
    Editor("qword26C", service.qword26C);
    Editor("qword270", service.qword270);
    Editor("byte274", service.byte274);
    Editor("dword298", service.dword298);
}
