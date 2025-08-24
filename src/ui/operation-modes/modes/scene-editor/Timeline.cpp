#include "Timeline.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <imtimeline.h>

namespace ui::operation_modes::modes::scene_editor {
	Timeline::Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, timelineCtx{ ImTimeline::CreateContext() } { }

	Timeline::~Timeline() {
		ImTimeline::DestroyContext(timelineCtx);
	}

	void Timeline::RenderPanel()
	{
		auto& context = GetContext();

		if (context.sceneCtrl == nullptr){
            ImGui::Text("No Scene Control selected");
			return;
		}

        if (ImGui::BeginChild("Timeline", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			bool changed = false;
			int currentFrame = (int)(context.sceneCtrl->playInfo.currentSecond * context.sceneCtrl->playInfo.fps);
			Viewer("Current Frame", currentFrame);
			auto playHeadFrame = std::fminf(currentFrame, context.sceneCtrl->playInfo.frameLength);
			bool currentTimeChanged{};
			bool play = true;

			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImTimeline::Begin(timelineCtx);
			ImGui::SameLine();
			bool beforePlay = play;
			float beforeTime = playHeadFrame;
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, context.sceneCtrl->playInfo.frameLength, 60.0, &play, &currentTimeChanged)) {

				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
			ImPlot::PopStyleVar(2);

			changed |= beforePlay != play;

			if(currentTimeChanged)
				SetFrame(playHeadFrame);
		}
		ImGui::EndChild();
	}

	PanelTraits Timeline::GetPanelTraits() const
	{
		return { "Timeline", ImVec2(250, 500), ImVec2(500, 250) };
	}

	void Timeline::SetFrame(float time)
	{
		auto& context = GetContext();
		context.sceneCtrl->playInfo.currentSecond = context.sceneCtrl->playInfo.fps * time;
	}
}
