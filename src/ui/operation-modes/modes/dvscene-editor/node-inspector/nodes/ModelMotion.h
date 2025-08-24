#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<hh::dv::DvNodeBase::NodeType::MODEL_MOTION>(char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeModelMotion::Description*>(node);
        changed |= CheckboxFlags("Unk0", data->flags, hh::dv::DvNodeModelMotion::Description::Flags::UNK0);
        changed |= CheckboxFlags("Update Transform", data->flags, hh::dv::DvNodeModelMotion::Description::Flags::UPDATE_TRANSFORM);
        changed |= CheckboxFlags("Use Root Bone", data->flags, hh::dv::DvNodeModelMotion::Description::Flags::USE_ROOT_BONE);
		changed |= Editor("ASM State Name", data->asmState);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
