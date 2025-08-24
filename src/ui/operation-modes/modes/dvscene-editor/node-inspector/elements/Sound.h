#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* soundIdNames[] = {
        "BGM",
        "Voice",
        "SFX",
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SOUND>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSound::Description*>(element);
		changed |= Editor("Cue Name", data->cueName);
        changed |= ComboEnum("Sound ID", data->soundId, soundIdNames);
        changed |= CheckboxFlags("Facial Animation", data->flags, app::dv::DvElementSound::Description::Flags::FACIAL_ANIMATION);
        changed |= CheckboxFlags("Stop Immediately", data->flags, app::dv::DvElementSound::Description::Flags::STOP_IMMEDIATELY);
        return changed;
    }
}
