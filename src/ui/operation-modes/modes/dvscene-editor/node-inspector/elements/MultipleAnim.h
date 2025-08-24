#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* animationTypeNames[] = {
        "SKELETAL_ANIMATION",
        "UV_ANIMATION",
        "VISIBILITY_ANIMATION",
        "MATERIAL_ANIMATION",
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::MULTIPLE_ANIM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementMultipleAnim::Description*>(element);
		changed |= Editor("Unk0", data->unk0);
		changed |= Editor("ASM State Name", data->stateName);
		changed |= Editor("Unk1", data->unk1);
		if(ImGui::TreeNode("Animations")){
            for(auto i = 0; i < data->activeAnimationsCount; i++){
                auto& x = data->animations[i];
                char buffer[68];
                snprintf(buffer, sizeof(buffer), "%s##%s", x.fileName, i);
                if(ImGui::TreeNode(buffer)){
                    int curAnmType = static_cast<int>(x.animType)-1;
                    if (changed |= ImGui::Combo("Animation Type", &curAnmType, animationTypeNames, 4))
                        x.animType = static_cast<hh::dv::DvElementMultipleAnim::Description::Animation::AnimationType>(curAnmType+1);
                    changed |= Editor("Filename", x.fileName);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
        if(Editor("Active Animation Count", data->activeAnimationsCount)) {
            changed |= true;
            if(data->activeAnimationsCount > 16)
                data->activeAnimationsCount = 16;
            else if(data->activeAnimationsCount < 0)
                data->activeAnimationsCount = 0;
        }
        return changed;
    }
}
