#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    const char* weatherTypeNames[] = {
        "SUNNY",
        "CLOUDY",
        "RAINY",
        "SANDSTORM",
        "METEOR_SHOWER"
    };
#endif
    
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::WEATHER>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementWeather::Description*>(element);
#ifdef DEVTOOLS_TARGET_SDK_miller
        unsigned int weatherType = static_cast<unsigned int>(data->weatherType);
		if(changed |= Editor("Weather Type", weatherType))
            data->weatherType = static_cast<app::dv::DvElementWeather::Description::WeatherType>(weatherType);
#else
        changed |= ComboEnum("Weather Type", data->weatherType, weatherTypeNames);
#endif
        return changed;
    }
}
