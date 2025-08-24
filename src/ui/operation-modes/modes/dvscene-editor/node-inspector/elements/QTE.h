#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* qteTypeNames[] = {
        "PRESS_PROMPT",
        "MASH",
        "RED_CIRCLE",
        "THE_END_VARIANT",
        "UNK0"
    };

    const char* qteButtonNames[] = {
        "A",
        "B",
        "X",
        "Y",
        "LB_RB",
        "LB",
        "RB",
        "MASH_A",
        "MASH_B",
        "MASH_X",
        "MASH_Y",
        "MASH_LB_RB",
        "MASH_LB",
        "MASH_RB",
        "X_LEFTRIGHT",
        "X_LEFTRIGHT_ALT",
        "RB_ALT"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::QTE>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementQTE::Description*>(element);
        changed |= ComboEnum("QTE Type", data->qteType, qteTypeNames);
        changed |= ComboEnum("QTE Button", data->qteButton, qteButtonNames);
        if(ImGui::TreeNode("Red Circle")){
            changed |= Editor("Size", data->redCircleSize);
            changed |= Editor("Thickness", data->redCircleThickness);
            changed |= Editor("Outline Thickness", data->redCircleOutlineThickness);
            ImGui::TreePop();
        }
        if(ImGui::TreeNode("White Line")){
            changed |= Editor("Speed", data->whiteLineSpeed);
            changed |= Editor("Thickness", data->whiteLineThickness);
            changed |= Editor("Outline Thickness", data->whiteLineOutlineThickness);
            ImGui::TreePop();
        }
        changed |= Editor("Multiplier", data->multiplier);
        changed |= Editor("Fail Count", data->failCount);
        changed |= Editor("ASM Variable Name", data->asmVarName);
        changed |= Editor("Mash Count", data->mashCount);
        changed |= Editor("Start", data->start);
        changed |= Editor("End", data->end);
        changed |= Editor("Speed Multiplier", data->speedMultiplier);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        changed |= Editor("Unk5", data->unk5);
#elif DEVTOOLS_TARGET_SDK_miller
        changed |= Editor("Unk4", data->unk4);
#endif
        changed |= Editor("Position", data->offset);
        changed |= Editor("Sound Cue Name", data->soundCueName);
        return changed;
    }
}
