#pragma once
#include "../Nodes.h"
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* elemPlayTypes[] = {
		"NORMAL",
		"ONESHOT",
		"ALWAYS"
	};

	const char* elemUpdateTimings[] = {
		"ON_EXEC_PATH",
		"ON_PRE_UPDATE",
		"CHARACTER_FIX_POSTURE",
		"ON_POST_UPDATE_CHARACTER",
		"ON_UPDATE_POS",
		"ON_FIX_BONE_POSTURE",
		"ON_EVALUATE_DETAIL_MOTION",
		"CHARACTER_JOB_UPDATE",
		"MODIFY_POSE_AFTER",
		"JOB_REGISTER",
		"MOTION_UPDATE"
	};

    template<>
    bool RenderNodeInspector<hh::dv::DvNodeBase::NodeType::ELEMENT>(char* node) {
		bool changed = false;

        auto* data = reinterpret_cast<hh::dv::DvNodeElement::DescriptionBase*>(node);

        int type = static_cast<int>(data->elementId);
		const char* elementIdName = GetElementName(type);
		Viewer("Element ID", elementIdName);

		changed |= ComboEnum("Play Type", data->playType, elemPlayTypes);
		changed |= ComboEnum("Update Timing", data->updateTiming, elemUpdateTimings);

		if (IsUnknownElement(type))
			return changed;

        ElementFuncType render = GetElementInspectorRender(data->elementId);
		if (render) {
			ImGui::SeparatorText("Element Properties");
			changed |= render(reinterpret_cast<char*>(&data[1]));
		}

		return changed;
    }
}
