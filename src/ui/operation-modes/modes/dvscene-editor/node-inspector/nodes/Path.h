#pragma once
#include "../Nodes.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderNodeInspector<hh::dv::DvNodeBase::NodeType::PATH>(char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvNodePath::Description*>(node);
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f{ data->matrix.matrix() });
        if (changed |= Editor("Transform", transform))
            data->matrix = Eigen::Affine3f{ TransformToAffine3f(transform) };
        return changed;
    }
}
