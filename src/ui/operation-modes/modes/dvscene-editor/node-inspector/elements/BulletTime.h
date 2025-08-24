#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::BULLET_TIME>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementBulletTime::Description*>(element);
		changed |= Editor("Unk0", data->unk0);
		changed |= Editor("Unk1", data->unk1);
		changed |= Editor("Type", data->type);
        return changed;
    }
}
