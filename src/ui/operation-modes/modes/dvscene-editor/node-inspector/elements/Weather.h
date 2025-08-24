#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::WEATHER>(char* element) {
        bool changed = false;
        //auto* data = reinterpret_cast<app::dv::DvElementWeather::Description*>(element);
        //unsigned int weatherType = static_cast<unsigned int>(data->weatherType);
        //if (changed |= Editor("Weather Type", weatherType))
        //    data->weatherType = static_cast<app::dv::DvElementWeather::Description::WeatherType>(weatherType);
        ImGui::Text("Ashrindy fix this please.");
        return changed;
    }
}
