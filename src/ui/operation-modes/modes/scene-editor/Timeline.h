#pragma once
#include <ui/operation-modes/Panel.h>
#include <imtimeline.h>
#include "Context.h"

namespace ui::operation_modes::modes::scene_editor {
	class Timeline : public Panel<Context> {
    private:
        ImTimeline::ImTimelineContext* timelineCtx;
	public:

        Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode);
		virtual ~Timeline();

		virtual void RenderPanel() override;
		virtual PanelTraits GetPanelTraits() const override;

		void SetFrame(float time);
	};
}
