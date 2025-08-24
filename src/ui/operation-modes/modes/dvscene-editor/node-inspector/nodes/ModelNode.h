#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<hh::dv::DvNodeBase::NodeType::MODEL_NODE>(char* node) {
        auto* data = reinterpret_cast<hh::dv::DvNodeModelNode::Description*>(node);
        return Editor("Model Node Name", data->nodeName);
    }
}
