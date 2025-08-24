#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_IN_GAME>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraInGame::Description*>(element);
		changed |= Editor("Unk0", data->unk0);
        return changed;
    }
}
