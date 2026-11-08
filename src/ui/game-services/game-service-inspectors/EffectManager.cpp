#include "EffectManager.h"
void Viewer(const char* label, Cyan::System::AnimCtrl& animCtrl);
void Viewer(const char* label, Cyan::System::AnimCtrl* animCtrlPtr);
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

void Viewer(const char* label, Cyan::System::AnimCtrl& animCtrl) {
    if (ImGui::TreeNode(label)) {
        Viewer("useGlobalTime", animCtrl.animParam->useGlobalTime);
        Viewer("timeScale", animCtrl.animParam->timeScale);
        Viewer("animParam", animCtrl.animParam->totalFrameCount);
        Viewer("time", animCtrl.time);
        Viewer("speed", animCtrl.speed);
        Viewer("durationScaling", animCtrl.durationScaling);
        Viewer("currentFrame", animCtrl.currentFrame);
        Viewer("loopCount", animCtrl.loopCount);
        Viewer("createParam.duration", animCtrl.createParam.duration);
        ImGui::TreePop();
    }
}
void Viewer(const char* label, Cyan::System::AnimCtrl* animCtrlPtr) {
    if (animCtrlPtr)
        Viewer(label, *animCtrlPtr);
}

void RenderGameServiceInspector(hh::eff::EffectManager& service)
{
    auto* cyanManager = (Cyan::ManagerImpl*)service.implementation->cyanManager;

    for (auto* effEntry = cyanManager->effects.sentinel.prev; effEntry; effEntry = effEntry->prev) {
        if (ImGui::TreeNode(effEntry->value->GetName())) {
            auto* emitter = effEntry->value->emitter;

            Viewer("lifetime", emitter->lifetime);
            Viewer("time", emitter->time);
            Viewer("elementLifetimeScale", emitter->elementLifetimeScale);
            //Viewer("lifeTime", emitter->lifeTime);
            //Viewer("animCtrl1", emitter->animCtrl1);
            //Viewer("animCtrl2", emitter->animCtrl2);
            //Viewer("animCtrl3", emitter->animCtrl3);
            //Viewer("emitCountAnimation", emitter->emitCountAnimation);
            //Viewer("accelerationMultiplierAnimation", emitter->accelerationMultiplierAnimation);
            //Viewer("accelerationNormalMultiplierAnimation", emitter->accelerationNormalMultiplierAnimation);
            //Viewer("velocityMultiplierAnimation", emitter->velocityMultiplierAnimation);
            //Viewer("shapeRadiusAnimation", emitter->shapeRadiusAnimation);
            //Viewer("velocityScaleAnimation", emitter->velocityScaleAnimation);
            //Viewer("emitSizeAnimation", emitter->emitSizeAnimation);
            //Viewer("emitVectorAnimation", emitter->emitVectorAnimation);
            //Viewer("randomPositionAnimation", emitter->randomPositionAnimation);
            //Viewer("spreadAnimation", emitter->spreadAnimation);
            //Viewer("startAngleAnimation", emitter->startAngleAnimation);
            //Viewer("endAngleAnimation", emitter->endAngleAnimation);
            //Viewer("sizeAnimation", emitter->sizeAnimation);

            for (auto* elEntry = emitter->elements.sentinel.prev; elEntry; elEntry = elEntry->prev) {
                Cyan::Element* element = elEntry->value;

                ImGui::PushID(element);
                if (BeginVectorViewerTable("table", false)) {
                    Viewer("element.emitParam.direction1", element->emitParam.direction1);
                    Viewer("element.emitParam.direction2", element->emitParam.direction2);
                    Viewer("element.emitParam.normal", element->emitParam.normal);

                    Viewer("element.previousPosition", element->previousPosition);
                    Viewer("element.position", element->position);
                    Viewer("element.velocity", element->velocity);
                    Viewer("element.unkVec", element->unkVec);
                    Viewer("element.rotation", element->rotation);
                    EndVectorViewerTable();
                }
                Viewer("element.lifetime", element->lifetime);
                Viewer("element.param.lifetime.value", element->param->lifetime.value);
                Viewer("element.param.lifetime.jitter", element->param->lifetime.jitter);
                //Viewer("scaleAnimation", element->scaleAnimation);
                //Viewer("modifierAnimation", element->modifierAnimation);
                Viewer("colorAnimations.colorAnimation", element->colorAnimations.colorAnimation);
                //Viewer("colorAnimations.alphaAnimation", element->colorAnimations.alphaAnimation);
                //Viewer("colorAnimations.unk6Animation", element->colorAnimations.unk6Animation);
                //Viewer("colorAnimations.unk7Animation", element->colorAnimations.unk7Animation);
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
    }
}
