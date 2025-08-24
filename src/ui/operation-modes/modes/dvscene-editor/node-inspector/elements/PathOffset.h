#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::PATH_OFFSET>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementPathOffset::Description*>(element);
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f{ data->offsetMatrix.matrix() });
        if (changed |= Editor("Transform", transform))
            data->offsetMatrix = Eigen::Affine3f{ TransformToAffine3f(transform) };
        return changed;
    }
}
