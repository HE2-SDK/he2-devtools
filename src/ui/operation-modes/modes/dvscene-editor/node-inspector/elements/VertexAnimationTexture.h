#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::VERTEX_ANIMATION_TEXTURE>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementVertexAnimationTexture::Description*>(element);
		changed |= Editor("VAT Anim Name", data->vatAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
