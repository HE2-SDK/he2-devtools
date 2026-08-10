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

	static constexpr int POINTS_PER_SEGMENT = 40;

	static ImPlotPoint GeneratePlotLine(int i, void* userData) {
		auto* trackNode = (ucsl::resources::scene::v106::TrackNode*)userData;
		auto* curveData = trackNode->curveData;

		auto segment = i / POINTS_PER_SEGMENT;
		auto& kf = curveData->keyframes[segment];

		hh::scene::LinearCurveEvaluator linearCurveEvaluator{};
		hh::scene::HermiteCurveEvaluator hermiteCurveEvaluator{};
		hh::scene::ICurveEvaluator* curveEvaluator{ nullptr };

		switch (curveData->interpolationType) {
		case ucsl::resources::scene::v106::TrackNode::CurveData::InterpolationType::LINEAR:
			curveEvaluator = &linearCurveEvaluator;
			break;
		case ucsl::resources::scene::v106::TrackNode::CurveData::InterpolationType::HERMITE:
			curveEvaluator = &hermiteCurveEvaluator;
			break;
		}

		if (curveEvaluator) {
			curveEvaluator->SetParameters(curveData);
			return ImPlotPoint{ kf.frame - trackNode->frameStart, curveEvaluator->Calculate(kf.frame) };
		}

		return ImPlotPoint{ 0, 0 };
	}

	void Timeline::RenderPanel()
	{
		auto& context = GetContext();

		if (context.sceneCtrl == nullptr) {
            ImGui::Text("No Scene Control selected");
			return;
		}

		auto* resource = context.sceneCtrl->GetResource();
		auto* data = (ucsl::resources::scene::v106::SceneData*)resource->unpackedBinaryData;

        if (ImGui::BeginChild("Timeline", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			bool changed = false;
			int currentFrame = (int)(context.sceneCtrl->playInfo.currentSecond * context.sceneCtrl->playInfo.fps);
			Viewer("Current Frame", currentFrame);
			auto playHeadFrame = std::fminf(currentFrame, context.sceneCtrl->playInfo.frameLength);
			bool currentTimeChanged{};
			bool play = context.sceneCtrl->playInfo.unk1 != 0.0f;

			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImTimeline::Begin(timelineCtx);
			ImGui::SameLine();
			bool beforePlay = play;
			float beforeTime = playHeadFrame;
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, context.sceneCtrl->playInfo.frameLength, context.sceneCtrl->playInfo.fps, &play, &currentTimeChanged)) {
				for (auto x = 0; x < data->timeline->count; x++) {
					auto* node = data->timeline->nodes[x];
					if (node->type == ucsl::resources::scene::v106::TimelineNode::NodeType::SCENE_CONTROL ||
						node->type == ucsl::resources::scene::v106::TimelineNode::NodeType::SCENE_CONTROL_MODEL)
					{
						auto* ctrlNode = data->sceneControl->nodes[node->value.sceneControl];

						char buffer[0x100];
						snprintf(buffer, sizeof(buffer), "%s - %s", ctrlNode->nodeName, ctrlNode->parameterName);
						if (ImTimeline::BeginTrack(buffer)) {
							for (auto y = 0; y < node->trackNodeCount; y++) {
								auto* trackNode = node->trackNodes[y];

								float frameEnd{ trackNode->frameEnd };
								if (trackNode->trackType == 1 || trackNode->trackType == 2) {
									frameEnd = context.sceneCtrl->playInfo.frameLength;
								}

								bool endTimeChanged{ false };
								char bufferx[0x120];
								snprintf(bufferx, sizeof(bufferx), "%s %d", buffer, y);
								if (ImTimeline::BeginClip(bufferx, &trackNode->frameStart, &frameEnd, 40, nullptr, &endTimeChanged)) {
									if (trackNode->trackType == 0 &&
										trackNode->curveData &&
										ImPlot::BeginPlot("##Track", ImTimeline::GetClipSize(), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs))
									{
										ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock | ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
										ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
										ImPlot::SetupAxisLimits(ImAxis_X1, 0, frameEnd - trackNode->frameStart, ImPlotCond_Always);

										ImPlot::PlotLineG("X", GeneratePlotLine, trackNode, (trackNode->curveData->keyframeCount - 1) * POINTS_PER_SEGMENT + 3, ImPlotLineFlags_Shaded);

										for (auto z = 0; z < trackNode->curveData->keyframeCount; z++) {
											auto& keyframe = trackNode->curveData->keyframes[z];

											double time = keyframe.frame;
											double value = keyframe.value;

											constexpr ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f);

											if (changed |= ImPlot::DragPoint(z, &time, &value, color, 4.0f, 0)) {
												keyframe.frame = std::clamp((float)time, 0.0f, INFINITY);
												keyframe.value = std::clamp((float)value, 0.0f, INFINITY);
											}
										}

										ImPlot::EndPlot();
									}

									ImTimeline::EndClip();
								}

								if (endTimeChanged && trackNode->trackType == 0)
									trackNode->frameEnd = frameEnd;
							}

							ImTimeline::EndTrack();
						}
					}
					else {
						auto& sceneGraphNode = data->sceneGraphNodes[node->value.animation.sceneNodeIndex];
						auto& sceneNode = data->sceneNodes[sceneGraphNode.index];

						if (ImTimeline::BeginTrack(sceneNode.nodeName)) {
							for (auto y = 0; y < node->trackNodeCount; y++) {
								auto* trackNode = node->trackNodes[y];

								float frameEnd{ trackNode->frameEnd };
								if (trackNode->trackType == 1 || trackNode->trackType == 2) {
									frameEnd = context.sceneCtrl->playInfo.frameLength;
								}

								bool endTimeChanged{ false };
								char bufferx[0x120];
								snprintf(bufferx, sizeof(bufferx), "%s %d", sceneNode.nodeName, y);
								if (ImTimeline::BeginClip(bufferx, &trackNode->frameStart, &frameEnd, 40, nullptr, &endTimeChanged)) {
									if (trackNode->trackType == 0 &&
										trackNode->curveData &&
										ImPlot::BeginPlot("##Track", ImTimeline::GetClipSize(), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs))
									{
										ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock | ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
										ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
										ImPlot::SetupAxisLimits(ImAxis_X1, 0, frameEnd - trackNode->frameStart, ImPlotCond_Always);

										ImPlot::PlotLineG("X", GeneratePlotLine, trackNode, (trackNode->curveData->keyframeCount - 1) * POINTS_PER_SEGMENT + 3, ImPlotLineFlags_Shaded);

										for (auto z = 0; z < trackNode->curveData->keyframeCount; z++) {
											auto& keyframe = trackNode->curveData->keyframes[z];

											double time = keyframe.frame;
											double value = keyframe.value;

											constexpr ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f);

											if (changed |= ImPlot::DragPoint(z, &time, &value, color, 4.0f, 0)) {
												keyframe.frame = std::clamp((float)time, 0.0f, INFINITY);
												keyframe.value = std::clamp((float)value, 0.0f, INFINITY);
											}
										}

										ImPlot::EndPlot();
									}

									ImTimeline::EndClip();
								}

								if (endTimeChanged && trackNode->trackType == 0)
									trackNode->frameEnd = frameEnd;
							}

							ImTimeline::EndTrack();
						}
					}
					
				}

				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
			ImPlot::PopStyleVar(2);

			changed |= beforePlay != play;

			if (changed)
				context.sceneCtrl->SetPlaybackSpeed(play ? 0.0f : 1.0f);

			if(currentTimeChanged)
				SetFrame(playHeadFrame / context.sceneCtrl->playInfo.fps);
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
