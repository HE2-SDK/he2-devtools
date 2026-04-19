#include "BossGiant.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>

void RenderGameObjectInspector(app::BossGiant& object) {
    auto& ctx = object.context;
    Viewer("qword60 size", ctx->qword60.size());
    Viewer("qword80 size", ctx->qword80.size());
    Viewer("qwordC0 size", ctx->qwordC0.size());
    Viewer("qwordE0 size", ctx->qwordE0.size());
    Viewer("qword100 size", ctx->qword100.size());
    ReflectionEditor("Camera Params", &ctx->cameraParams, &RESOLVE_STATIC_VARIABLE(heur::rfl::BossLockOnCameraParam::rflClass));
    Editor("bossEventTime", ctx->bossEventTime);
    Editor("cameraTimeRemaining", ctx->cameraTimeRemaining);
    Editor("cameraTimeElapsed", ctx->cameraTimeElapsed);
    Editor("objectBossTransform", ctx->startObjectBossTransform);
    Editor("dword200", ctx->dword200);
    Editor("lastHitPosition", ctx->lastHitPosition);
    Editor("qword220", ctx->csl__math__vector4220);
    Editor("relatedToLastHit", ctx->relatedToLastHit);
    Viewer("Current Camera name", ctx->currentCamera->cameraFrame->name.c_str());
    if (ImGui::TreeNode("qword268")) {
        for (auto& x : ctx->qword268)
            ImGui::Text(x->name.c_str());
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("qword288")) {
        for (auto& x : ctx->qword288)
            ImGui::Text(x->name.c_str());
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("qword2A8")) {
        for (auto& x : ctx->qword2A8)
            ImGui::Text(x->name.c_str());
        ImGui::TreePop();
    }
    Editor("qword328 (flags)", ctx->flags.bits);
    Editor("currentPhase", ctx->currentPhase);
    Editor("dword334", ctx->dword334);
    Editor("dword338", ctx->dword338);
    Viewer("qword340 size", ctx->qword340.size());
    Editor("world pos 380", ctx->hh__fnd__worldposition380);
    Editor("word3A0", ctx->word3A0);
    Viewer("qword3A8 size", ctx->qword3A8.size());
}
