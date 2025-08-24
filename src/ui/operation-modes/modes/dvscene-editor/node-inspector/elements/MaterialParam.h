#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::MATERIAL_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementMaterialParam::Description*>(element);
		changed |= Editor("Material Name", data->materialName);
		changed |= Editor("Parameter Name", data->paramName);
		changed |= CheckboxFlags("Use Parameter X", data->flags, hh::dv::DvElementMaterialParam::Description::Flags::PARAM_X);
		changed |= CheckboxFlags("Use Parameter Y", data->flags, hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Y);
		changed |= CheckboxFlags("Use Parameter Z", data->flags, hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Z);
		changed |= CheckboxFlags("Use Parameter W", data->flags, hh::dv::DvElementMaterialParam::Description::Flags::PARAM_W);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_X))
            changed |= Editor("Parameter X", data->params[0]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Y))
            changed |= Editor("Parameter Y", data->params[1]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Z))
            changed |= Editor("Parameter Z", data->params[2]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_W))
            changed |= Editor("Parameter W", data->params[3]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_X))
            changed |= Editor("Finish Parameter X", data->finishParams[0]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Y))
            changed |= Editor("Finish Parameter Y", data->finishParams[1]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_Z))
            changed |= Editor("Finish Parameter Z", data->finishParams[2]);
        if(data->flags.test(hh::dv::DvElementMaterialParam::Description::Flags::PARAM_W))
            changed |= Editor("Finish Parameter W", data->finishParams[3]);
        return changed;
    }
}
