#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<hh::dv::DvNodeBase::NodeType::CHARACTER>(char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodeCharacter::Description*>(node);
		changed |= Editor("Model Name", data->modelName);
		changed |= Editor("Skeleton Name", data->skeletonName);
		changed |= Editor("Use Internal Name", data->useName3);
		if (data->useName3)
            changed |= Editor("Internal Name", data->name3);
        return changed;
    }
}
