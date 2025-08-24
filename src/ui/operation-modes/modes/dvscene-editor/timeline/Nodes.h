#pragma once
#include "Timeline.h"
#include "Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    using NodeTimelineFuncType = bool(*)(Timeline*, char*);
    using namespace hh::dv;
    using NodeType = DvNodeBase::NodeType;

    template<>
	bool RenderNodeTimeline<NodeType::CAMERA_MOTION>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<DvNodeCameraMotion::Description*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
        return changed;
	}

	template<>
	bool RenderNodeTimeline<NodeType::CHARACTER_MOTION>(Timeline* timeline, char* node) {
        bool changed = false;
        auto* data = reinterpret_cast<DvNodeCharacterMotion::Description*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
		return changed;
	}

	template<>
	bool RenderNodeTimeline<NodeType::MODEL_MOTION>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<DvNodeModelMotion::Description*>(node);
        int start = data->start / 100;
        int end = data->end / 100;
        if (changed |= timeline->RenderTimeline(start, end)) {
            data->start = start * 100;
            data->end = end * 100;
        }
        return changed;
	}

	template<>
	bool RenderNodeTimeline<NodeType::ELEMENT>(Timeline* timeline, char* node) {
        bool changed = false;
		auto* data = reinterpret_cast<DvNodeElement::Description<hh::dv::DvElementBase>*>(node);
        int type = static_cast<int>(data->elementId);
        const std::pair<size_t, size_t> render = GetElementTimelineRender(data->elementId);
        
        if (render != std::pair<size_t, size_t>{})
        {
            float* curveData = &reinterpret_cast<float*>(&data->elementDescription)[render.first / sizeof(float)];
            changed |= timeline->RenderTimeline(data->start, data->end, curveData, render.second/sizeof(float));
        }
        else
            changed |= timeline->RenderTimeline(data->start, data->end);

        return changed;
	}

    constexpr std::pair<NodeType, NodeTimelineFuncType> RenderTimelineNodes[] = {
        {NodeType::CAMERA_MOTION,       RenderNodeTimeline<NodeType::CAMERA_MOTION>},
        {NodeType::CHARACTER_MOTION,    RenderNodeTimeline<NodeType::CHARACTER_MOTION>},
        {NodeType::MODEL_MOTION,        RenderNodeTimeline<NodeType::MODEL_MOTION>},
        {NodeType::ELEMENT,             RenderNodeTimeline<NodeType::ELEMENT>}
    };

    constexpr NodeTimelineFuncType GetNodeTimelineRender(NodeType type) {
        for (const auto& [key, func] : RenderTimelineNodes) {
            if (key == type) return func;
        }
        return nullptr;
    }
}
