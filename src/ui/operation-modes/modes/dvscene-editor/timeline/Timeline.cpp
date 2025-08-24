#include "Timeline.h"
#include "../Behaviors.h"
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <imtimeline.h>
#include <ui/GlobalSettings.h>
#include <algorithm>
#include <numeric>
#include "Nodes.h"
#include <numbers>

namespace ui::operation_modes::modes::dvscene_editor {
	Timeline::Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, timelineCtx{ ImTimeline::CreateContext() } { }

	Timeline::~Timeline() {
		ImTimeline::DestroyContext(timelineCtx);
	}

	void Timeline::RenderPanel()
	{
		auto& context = GetContext();

		if (context.goDVSC == nullptr){
            ImGui::Text("No DvSceneControl selected");
			return;
		}
        
		auto* selectionBehavior = GetBehavior<SelectionBehavior<Context>>();
		auto& selected = selectionBehavior->GetSelection();

        if (ImGui::BeginChild("Timeline", ImVec2(0,0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			bool changed = false;
			auto* dvTimeline = context.goDVSC->timeline;
			Viewer("Current Frame", dvTimeline->preCurrentFrame/100);
			ImGui::Checkbox("Looping", &dvTimeline->looping);
			auto playHeadFrame = std::fminf(dvTimeline->postCurrentFrame/100, static_cast<float>(dvTimeline->frameEnd/100));
			bool currentTimeChanged{};
			bool play = context.goDVSC->play;
			if ((context.evtScene->flags.bits & 0x80) != 0)
				play = context.evtScene->flags.test(app::evt::EventScene::Flags::MOVIE_PLAYING);

			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImTimeline::Begin(timelineCtx);
			ImGui::SameLine();
			bool beforePlay = play;
			float beforeTime = playHeadFrame;
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, static_cast<float>(dvTimeline->frameEnd/100), 60.0, &play, &currentTimeChanged)) {
				if (selected.size() > 0) {
					DvNode focusedNode = selected[0];
					dv::DvNode* node = focusedNode.fileNode;
					NodeTimelineFuncType render = GetNodeTimelineRender(static_cast<hh::dv::DvNodeBase::NodeType>(node->category));
					if(render)
						if(render(this, node->data))
							focusedNode.UpdateRuntimeNode();
				}

				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
			ImPlot::PopStyleVar(2);

			changed |= beforePlay != play;

			if(currentTimeChanged)
				SetFrame(playHeadFrame * 100);

			if (auto* movieSrv = hh::game::GameManager::GetInstance()->GetService<hh::fmv::MovieManager>())
				for (auto x : movieSrv->movies)
					if (changed) {
						if ((context.evtScene->flags.bits & 0x80) != 0)
							context.evtScene->SetMovie(play);
						x->moviePlayer->SetPause(!play);
						context.goDVSC->play = play;
					}
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
		context.goDVSC->timeline->preCurrentFrame = static_cast<int>(time);
		context.goDVSC->timeline->postCurrentFrame = static_cast<int>(time);
#ifdef DEVTOOLS_TARGET_SDK_miller
		context.goDVSC->timeline->currentFrame2 = static_cast<int>(time);
#endif
	}

	bool Timeline::RenderTimeline(int& start, int& end, float* curve, int size, bool axisLimit, float maxValue) {
		bool changed = false;
		if (ImTimeline::BeginTrack("")) {
			float startTime = static_cast<float>(start);
			float endTime = static_cast<float>(end);
			bool startTimeChanged{};
			bool endTimeChanged{};
			bool moved{};

			ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f);
			ImPlotPoint clickpos{};

			if (ImTimeline::BeginClip("", &startTime, &endTime, 80.0f, &startTimeChanged, &endTimeChanged, &moved)) {
				if (ImGui::BeginPopupContextItem("Properties")) {
					if (changed |= Editor("Start", start))
						if (start < 0)
							start = 0;
					if (changed |= Editor("End", end))
						if (end <= start)
							end = start + 1;
					ImGui::EndPopup();
				}
				if (curve && ImPlot::BeginPlot("##Track", ImTimeline::GetClipSize(), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
					if (ImGui::BeginPopupContextItem("Controls")) {
						ImGui::SeparatorText("Curve Editing Settings");
						Editor("Falloff", timelineFalloff);
						ImGui::SeparatorText("Custom Curve");
						Editor("Decreasing", decreasing);
						ImGui::SameLine();
						ImGui::Combo("Curve Type", &curveType, curveTypeNames, 7);
						if (ImGui::Selectable("Generate Curve"))
							GenerateCurve(curve, size, curveType, decreasing);
						ImGui::EndPopup();
					}

					ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock | ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax);
					ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
					ImPlot::SetupAxisLimits(ImAxis_X1, 0, size, ImPlotCond_Always);
					if(axisLimit)
						ImPlot::SetupAxisLimits(ImAxis_Y1, 0, maxValue, ImPlotCond_Always);

					auto* tempAllocator = hh::fnd::MemoryRouter::GetTempAllocator();
					float* time = static_cast<float*>(tempAllocator->Alloc(size*sizeof(float), alignof(float)));
					std::iota(time, time + size, 0.0f);
					ImPlot::SetNextFillStyle(color, 0.3f);
					ImPlot::PlotLine<float>("X", time, curve, size, ImPlotLineFlags_Shaded);
					tempAllocator->Free(time);
					for (auto x = 0; x < size; x++) {
						double frame = x;
						double value = curve[x];
						bool clicked;
						bool hovered;
						bool held;

						if (changed |= ImPlot::DragPoint(x, &frame, &value, color, 4.0f, 0, &clicked, &hovered, &held)) {
							if (axisLimit)
								value = std::clamp(value, 0.0, static_cast<double>(maxValue));

							double sigma = timelineFalloff;
							double delta = value - curve[x];

							for (int i = 0; i < size; i++) {
								double dist = i - x;
								double weight = std::exp(-(dist * dist) / (sigma * sigma));
								curve[i] += weight*delta;
								if (axisLimit)
									curve[i] = std::clamp(static_cast<double>(curve[i]), 0.0, static_cast<double>(maxValue));
							}
						}

						if (hovered || held)
							ImGui::SetTooltip("Frame: %d\nValue: %f", x, value);
					}

					clickpos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);

					ImPlot::EndPlot();
				}
				ImTimeline::EndClip();
			}

			if (startTimeChanged)
				start = static_cast<int>(startTime);

			if (endTimeChanged)
				end = static_cast<int>(endTime);

			changed |= startTimeChanged | endTimeChanged;
			ImTimeline::EndTrack();
		}
		return changed;
	}

	bool Timeline::RenderTimeline(float& start, float& end, float* curve, int size, bool axisLimit, float maxValue) {
		bool changed = false;
		int startI = static_cast<int>(start);
		int endI = static_cast<int>(end);
		if (changed |= RenderTimeline(startI, endI, curve, size, axisLimit, maxValue)) {
			start = static_cast<float>(startI);
			end = static_cast<float>(endI);
		}
		return changed;
	}

	void Timeline::GenerateCurve(float* curve, int size, int type, bool decreasing)
	{
		for (int x = 0; x < size; x++) {
			switch (type) {
			case 0:
				curve[x] = static_cast<float>(x) / static_cast<float>(size - 1);
				if (decreasing)
					curve[x] = 1 - curve[x];
				break;
			case 1: {
				float sqr = static_cast<float>(x) / static_cast<float>(size - 1);
				if (decreasing)
					curve[x] = (1 - sqr) * (1 - sqr);
				else
					curve[x] = sqr * sqr;
				break;
			}
			case 2: {
				float sqr = static_cast<float>(x) / static_cast<float>(size - 1);
				if (decreasing)
					curve[x] = 1 - sqr * sqr;
				else
					curve[x] = 1 - (1 - sqr) * (1 - sqr);
				break;
			}
			case 3: {
				float t = static_cast<float>(x) / static_cast<float>(size - 1);
				curve[x] = t < 0.5f ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
				if (decreasing)
					curve[x] = 1 - curve[x];
				break;
			}
			case 4: {
				curve[x] = 0.5f * (1 - cos(static_cast<float>(x) / static_cast<float>(size - 1) * std::numbers::pi));
				if (decreasing)
					curve[x] = 1 - curve[x];
				break;
			}
			case 5: {
				float t = static_cast<float>(x) / static_cast<float>(size - 1);
				if (!decreasing)
					t = 1 - t;
				curve[x] = 1 - log1p(t * 9) / log1p(9);
				break;
			}
			case 6: {
				float t = static_cast<float>(x) / static_cast<float>(size - 1);
				if (decreasing)
					t = 1 - t;
				curve[x] = log1p(t * 9) / log1p(9);
				break;
			}
			}
		}
	}
}
