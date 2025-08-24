#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* langNames[] = {
        "ENGLISH",
        "FRENCH",
        "ITALIAN",
        "GERMAN",
        "SPANISH",
        "POLISH",
        "PORTUGUESE",
        "RUSSIAN",
        "JAPANESE",
        "CHINESE",
        "CHINESE_SIMPLIFIED",
        "KOREAN",
#ifdef DEVTOOLS_TARGET_SDK_miller
        "THAI"
#endif
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAPTION>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCaption::Description*>(element);
#ifdef DEVTOOLS_TARGET_SDK_miller
        changed |= Editor("Caption Enabled", data->captionEnabled);
        changed |= Editor("Sound Enabled", data->soundEnabled);
        if(data->captionEnabled)
#endif
        changed |= Editor("Converse Name", data->converseName);
#ifdef DEVTOOLS_TARGET_SDK_miller
        if(data->soundEnabled)
            changed |= Editor("Sound Name", data->soundName);
#endif
        int curLangId = static_cast<int>(data->languageId);
#ifdef DEVTOOLS_TARGET_SDK_rangers
        if (ImGui::Combo("Language", &curLangId, langNames, 12))
#elif DEVTOOLS_TARGET_SDK_miller
        if (ImGui::Combo("Language", &curLangId, langNames, 13))
#endif
		{
            changed |= true;
            data->languageId = static_cast<app::dv::DvElementCaption::Description::LanguageID>(curLangId);
        }
        return changed;
    }
}
