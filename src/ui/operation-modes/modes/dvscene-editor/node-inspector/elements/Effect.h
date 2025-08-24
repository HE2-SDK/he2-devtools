#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* transTypeNames[] = {
        "NODE",
        "NODE_AND_FRAME",
        "NODE_POSITION"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::EFFECT>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementEffect::Description*>(element);
        csl::math::Transform transform = Affine3fToTransform(Eigen::Affine3f{ data->effectMatrix.matrix() });
        if (changed |= Editor("Effect Transform", transform))
            data->effectMatrix = Eigen::Affine3f{ TransformToAffine3f(transform) };
        changed |= Editor("Effect Name", data->effectName);
        changed |= CheckboxFlags("Unk0", data->flags, hh::dv::DvElementEffect::Description::Flags::UNK0);
        changed |= CheckboxFlags("Quaternion", data->flags, hh::dv::DvElementEffect::Description::Flags::QUATERNION);
        changed |= CheckboxFlags("Unk1", data->flags, hh::dv::DvElementEffect::Description::Flags::UNK1);
        changed |= CheckboxFlags("Model Space Node", data->flags, hh::dv::DvElementEffect::Description::Flags::MODEL_SPACE_NODE);
        changed |= Editor("Eff Create Info Unk1a", data->effCreateInfoUnk1A);
        changed |= CheckboxFlags("Unk0 flags2", data->flags2, hh::dv::DvElementEffect::Description::Flags2::UNK0);
        changed |= CheckboxFlags("Persistent", data->flags2, hh::dv::DvElementEffect::Description::Flags2::PERSISTENT);
        unsigned char color[4] = { data->r, data->g, data->b, data->a };
        if (changed |= ColorEditor("Effect Color", color))
        {
            data->r = color[0];
            data->g = color[1];
            data->b = color[2];
            data->a = color[3];
        }
        changed |= Editor("Eff Create Info Unk5a", data->effCreateInfoUnk5A);
        changed |= Editor("Eff Create Info Unk5", data->effCreateInfoUnk5);
        changed |= ComboEnum("Eff Trans Type", data->effTransType, transTypeNames);
        changed |= Editor("Unk7", data->unk7);
        changed |= Editor("Unk8", data->unk8);
        return changed;
    }
}
