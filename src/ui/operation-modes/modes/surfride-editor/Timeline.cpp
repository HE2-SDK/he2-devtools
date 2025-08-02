#include "Timeline.h"
#include "Context.h"
#include "Behaviors.h"
#include "SurfRideElement.h"
#include "implot_internal.h"
#include <span>
#include <ui/common/editors/SurfRide.h>

namespace ui::operation_modes::modes::surfride_editor {
	using namespace ucsl::resources::swif::swif_version;

	Timeline::Timeline(csl::fnd::IAllocator* allocator, OperationMode<Context>& operationMode) : Panel{ allocator, operationMode }, timelineCtx{ ImTimeline::CreateContext() } {}

	Timeline::~Timeline() {
		ImTimeline::DestroyContext(timelineCtx);
	}

	void Timeline::RenderPanel() {
		SRS_LAYER* focusedLayer{};
		SurfRide::Layer* focusedRuntimeLayer{};

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();
		auto& context = GetContext();

		for (auto& element : selection) {
			SRS_LAYER* newLayer{};
			SurfRide::Layer* newRuntimeLayer{};
			if (element.type == SurfRideElement::Type::LAYER) {
				newLayer = context.ResolveLayer(element);
				newRuntimeLayer = context.ResolveRuntimeLayer(element);
			}
			else if (element.type == SurfRideElement::Type::CAST) {
				newLayer = context.FindCastLayer(context.ResolveCast(element)->id);
				auto* rtCast = context.ResolveRuntimeCast(element);
				newRuntimeLayer = !rtCast ? nullptr : rtCast->layer;
			}
			if (newLayer != nullptr) {
				if (focusedLayer != nullptr) {
					ImGui::Text("Multiple layers selected");
					return;
				}
				else {
					focusedLayer = newLayer;
					focusedRuntimeLayer = newRuntimeLayer;
				}
			}
		}

		if (focusedLayer == nullptr) {
			ImGui::Text("Select a layer in the left pane.");
			return;
		}

		if (animationIdx >= focusedLayer->animationCount)
			animationIdx = 0;

		if (ImGui::BeginChild("Animation list", ImVec2(100.0f, 0.0f))) {
			for (int i = 0; i < focusedLayer->animationCount; i++) {
				auto& animation = focusedLayer->animations[i];

				bool clicked = ImGui::Selectable(animation.name, animationIdx == i);

				if (focusedRuntimeLayer && focusedRuntimeLayer->currentAnimationIndex == i)
					ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin() + ImVec2(4.0f, 0.0f), ImGui::GetItemRectMax() - ImVec2(4.0f, 0.0f), IM_COL32_WHITE);

				if (ImGui::BeginPopupContextItem()) {
					if (focusedRuntimeLayer && ImGui::MenuItem("Switch to"))
						context.StartAnimation(focusedRuntimeLayer, animation.id);
					if (ImGui::BeginMenu("Add motion")) {
						for (auto& cast : std::span(focusedLayer->casts, focusedLayer->castCount)) {
							bool anyFound{};

							for (auto& motion : std::span(animation.motions, animation.motionCount))
								if (motion.castId == cast.id)
									anyFound = true;

							if (anyFound)
								continue;

							ImGui::PushID(cast.id);
							if (ImGui::MenuItem(cast.name))
								context.AddMotion(animation, cast);
							ImGui::PopID();
						}

						ImGui::EndMenu();
					}
					ImGui::EndPopup();
				}

				if (clicked) {
					animationIdx = i;

					if (focusedRuntimeLayer && ImGui::IsKeyDown(ImGuiKey_ModCtrl))
						context.StartAnimation(focusedRuntimeLayer, animation.id);
				}

				if (animationIdx == i)
					ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndChild();

		auto& animation = focusedLayer->animations[animationIdx];
		bool animationIsCurrentRuntimeAnimation = focusedRuntimeLayer && focusedRuntimeLayer->currentAnimationIndex == animationIdx;

		ImGui::SameLine();

		if (ImGui::BeginChild("Properties", ImVec2(250.0f, 0.0f))) {
			ImGui::SeparatorText("Animation");
			if (Editor("Properties", context.projectResource, animation))
				if (animationIsCurrentRuntimeAnimation)
					context.StartAnimation(focusedRuntimeLayer, animation.id);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (ImGui::BeginChild("Timeline", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar)) {
			bool playing = animationIsCurrentRuntimeAnimation ? !focusedRuntimeLayer->pause : false;
			auto playHeadFrame = std::fminf(animationIsCurrentRuntimeAnimation ? context.GetAnimationFrame(focusedRuntimeLayer) : 0.0f, static_cast<float>(animation.frameCount));
			bool currentTimeChanged{};

			ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
			ImPlot::PushStyleVar(ImPlotStyleVar_PlotBorderSize, 0);
			ImTimeline::Begin(timelineCtx);
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, static_cast<float>(animation.frameCount), 60.0f, &playing, &currentTimeChanged)) {
				for (size_t i = 0; i < animation.motionCount; i++)
					RenderMotion(*focusedLayer, animation, animation.motions[i]);
				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
			ImPlot::PopStyleVar(2);

			if (animationIsCurrentRuntimeAnimation) {
				focusedRuntimeLayer->pause = !playing;

				if (currentTimeChanged)
					context.SetAnimationFrame(focusedRuntimeLayer, playHeadFrame);
			}
		}
		ImGui::EndChild();
	}

	PanelTraits Timeline::GetPanelTraits() const
	{
		return { "Timeline", ImVec2(0, ImGui::GetMainViewport()->WorkSize.y - 575), ImVec2(1200, 550), ImVec2(1, 0) };
	}

	void Timeline::RenderMotion(SRS_LAYER& layer, SRS_ANIMATION& animation, SRS_MOTION& motion)
	{
		auto& context = GetContext();
		SRS_CASTNODE* cast = context.FindCast(layer, motion.castId);

		char name[200];
		snprintf(name, sizeof(name), "%s", cast == nullptr ? "MISSINGNO." : cast->name);

		ImGui::PushID(&motion);
		bool isOpen = ImTimeline::BeginTrackGroup(name);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImTimeline::IsNameColumnHovered())
			ImGui::OpenPopup("contextmenu");

		if (ImGui::BeginPopup("contextmenu")) {
			if (ImGui::BeginMenu("Add track")) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(ECurveType::IlluminationColorA); i++)
					if (ImGui::MenuItem(TrackName(static_cast<ECurveType>(i))))
						context.AddTrack(motion, static_cast<ECurveType>(i), 0, animation.frameCount, EInterpolationType::LINEAR);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Add event")) {
				for (unsigned int i = 0; i < static_cast<unsigned int>(ECurveType::IlluminationColorA); i++) {
					if (ImGui::MenuItem(TrackName(static_cast<ECurveType>(i)))) {
						auto& track = context.AddTrack(motion, static_cast<ECurveType>(i), 0, 0, EInterpolationType::CONSTANT);
						context.AddKeyFrame(track, 0);
					}
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Remove")) {
				context.RemoveMotion(animation, motion);
			}
			ImGui::EndPopup();
		}

		if (isOpen) {
			for (auto& track : std::span(motion.tracks, motion.trackCount))
				RenderTrack(layer, animation, motion, track);

			ImTimeline::EndTrackGroup();
		}
		ImGui::PopID();
	}

	void Timeline::RenderTrack(SRS_LAYER& layer, SRS_ANIMATION& animation, SRS_MOTION& motion, SRS_TRACK& track)
	{
		auto& context = GetContext();
		ImGui::PushID(&track);
		bool isOpen = ImTimeline::BeginTrack(TrackName(track));

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ImTimeline::IsNameColumnHovered())
			ImGui::OpenPopup("contextmenu");

		if (ImGui::BeginPopup("contextmenu")) {
			if (ImGui::MenuItem("Remove")) {
				context.RemoveTrack(motion, track);
			}

			bool repeat = track.GetRepeatType() == ETrackRepeatType::REPEAT;

			if (ImGui::MenuItem("Repeat", nullptr, &repeat))
				track.SetRepeatType(repeat ? ETrackRepeatType::REPEAT : ETrackRepeatType::ONCE);

			ImGui::EndPopup();
		}

		if (isOpen) {
			auto length = (track.lastFrame - track.firstFrame);

			if (length == 0) {
				float time{ static_cast<float>(track.firstFrame) };

				bool changed = ImTimeline::Event(TrackName(track), &time);

				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
					ImGui::OpenPopup("Editor");

				assert(track.keyCount == 1);

				if (ImGui::BeginPopup("Editor")) {
					context.VisitKeyFrames(track, [&track](auto* kfs) {
						using KeyFrameType = std::remove_pointer_t<std::remove_reference_t<decltype(kfs)>>;
						using ValueType = typename KeyFrameType::ValueType;

						unsigned int frame = kfs[0].frame;

						if (Editor("Frame", frame)) {
							track.firstFrame = frame;
							track.lastFrame = frame;
							kfs[0].frame = frame;
						}

						RenderKeyFrameEditor<ValueType>(track, kfs[0]);
					});
					ImGui::EndPopup();
				}

				if (changed) {
					auto newFrame = static_cast<unsigned int>(time);

					track.firstFrame = newFrame;
					track.lastFrame = newFrame;

					context.VisitKeyFrames(track, [newFrame](auto* kfs) {
						kfs[0].frame = newFrame;
					});
				}
			}
			else {
				float startTime = static_cast<float>(track.firstFrame);
				float endTime = static_cast<float>(track.lastFrame);
				bool startTimeChanged{};
				bool endTimeChanged{};
				bool moved{};

				ImPlotPoint thisClickpos{};
				if (ImTimeline::BeginClip(TrackName(track), &startTime, &endTime, 80.0f, &startTimeChanged, &endTimeChanged, &moved)) {
					if (ImPlot::BeginPlot("##Track", ImTimeline::GetClipSize(), ImPlotFlags_CanvasOnly | ImPlotFlags_NoInputs)) {
						ImPlot::SetupAxis(ImAxis_X1, "Time", ImPlotAxisFlags_NoDecorations);
						ImPlot::SetupAxis(ImAxis_Y1, "Value", ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
						ImPlot::SetupAxisLimits(ImAxis_X1, track.firstFrame, track.lastFrame, ImPlotCond_Always);

						RenderPlotLines(track);

						thisClickpos = ImPlot::GetPlotMousePos(ImAxis_X1, ImAxis_Y1);

						ImPlot::EndPlot();

						if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
							clickpos = thisClickpos;
							ImGui::OpenPopup("plot-context-menu");
						}

						if (ImGui::BeginPopup("plot-context-menu")) {
							if (ImGui::MenuItem("Add keyframe"))
								context.AddKeyFrame(track, static_cast<unsigned int>(clickpos.x));

							auto interpolationType = track.GetInterpolationType();
							ImGui::SeparatorText("Properties");
							if (ComboEnum("Interpolation type", interpolationType, interpolationTypesExtended))
								context.SetTrackInterpolationType(track, interpolationType);

							ImGui::EndPopup();
						}
					}
					ImTimeline::EndClip();
				}

				unsigned int newFirstFrame = static_cast<unsigned int>(startTime);
				unsigned int newLastFrame = static_cast<unsigned int>(endTime);

				if (moved)
					context.MoveTrack(track, static_cast<int>(newFirstFrame) - static_cast<int>(track.firstFrame));

				if (startTimeChanged)
					context.SetTrackStart(track, newFirstFrame);

				if (endTimeChanged)
					context.SetTrackEnd(track, newLastFrame);
			}

			ImTimeline::EndTrack();
		}
		ImGui::PopID();
	}




	double GetR(const Color& color) { return color.r; }
	double GetG(const Color& color) { return color.g; }
	double GetB(const Color& color) { return color.b; }
	double GetA(const Color& color) { return color.a; }
	void SetR(Color& color, double y) { color.r = static_cast<uint8_t>(std::round(y)); }
	void SetG(Color& color, double y) { color.g = static_cast<uint8_t>(std::round(y)); }
	void SetB(Color& color, double y) { color.b = static_cast<uint8_t>(std::round(y)); }
	void SetA(Color& color, double y) { color.a = static_cast<uint8_t>(std::round(y)); }

	void Timeline::RenderPlotLines(SRS_TRACK& track)
	{
		if (track.keyCount == 0)
			return;

		switch (track.GetDataType()) {
		case ETrackDataType::FLOAT:
			switch (track.trackType) {
			case ECurveType::MaterialColorR:
			case ECurveType::MaterialColorG:
			case ECurveType::MaterialColorB:
			case ECurveType::MaterialColorA:
			case ECurveType::VertexColorTopLeftR:
			case ECurveType::VertexColorTopLeftG:
			case ECurveType::VertexColorTopLeftB:
			case ECurveType::VertexColorTopLeftA:
			case ECurveType::VertexColorTopRightR:
			case ECurveType::VertexColorTopRightG:
			case ECurveType::VertexColorTopRightB:
			case ECurveType::VertexColorTopRightA:
			case ECurveType::VertexColorBottomLeftR:
			case ECurveType::VertexColorBottomLeftG:
			case ECurveType::VertexColorBottomLeftB:
			case ECurveType::VertexColorBottomLeftA:
			case ECurveType::VertexColorBottomRightR:
			case ECurveType::VertexColorBottomRightG:
			case ECurveType::VertexColorBottomRightB:
			case ECurveType::VertexColorBottomRightA:
			case ECurveType::IlluminationColorR:
			case ECurveType::IlluminationColorG:
			case ECurveType::IlluminationColorB:
			case ECurveType::IlluminationColorA:
				SetupYAxis(0, 1);
				RenderDragPoints<float>(track, 0.0f, 1.0f);
				break;
			default:
				SetupFloatingYAxis<float>(track);
				RenderDragPoints<float>(track);
				break;
			}

			RenderPlotLine<float>(track);
			break;
		case ETrackDataType::BOOL:
			SetupYAxis(false, true);
			RenderDragPoints<bool>(track, false, true);
			RenderPlotLine<bool>(track);
			break;
		case ETrackDataType::INT:
			SetupFloatingYAxis<int>(track);
			RenderDragPoints<int>(track);
			RenderPlotLine<int>(track);
			break;
		case ETrackDataType::INDEX:
			SetupFloatingYAxis<int>(track);
			RenderDragPoints<int>(track);
			RenderPlotLine<int>(track);
			break;
		case ETrackDataType::COLOR:
			SetupYAxis(0, 255);

			RenderDragPoints<Color, GetR, SetR>(track, 0, 255, "R", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			RenderDragPoints<Color, GetG, SetG>(track, 0, 255, "G", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			RenderDragPoints<Color, GetB, SetB>(track, 0, 255, "B", ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
			RenderDragPoints<Color, GetA, SetA>(track, 0, 255, "A", ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			RenderPlotLine<Color, GetR>(track, "R", ImVec4(1.0f, 0.0f, 0.0f, 1.0f), false);
			RenderPlotLine<Color, GetG>(track, "G", ImVec4(0.0f, 1.0f, 0.0f, 1.0f), false);
			RenderPlotLine<Color, GetB>(track, "B", ImVec4(0.0f, 0.0f, 1.0f, 1.0f), false);
			RenderPlotLine<Color, GetA>(track, "A", ImVec4(0.0f, 0.0f, 0.0f, 1.0f), false);
			break;
		}
	}

	const char* Timeline::TrackName(ECurveType curveType)
	{
		switch (curveType) {
		case ECurveType::TranslationX: return "Translation X";
		case ECurveType::TranslationY: return "Translation Y";
		case ECurveType::TranslationZ: return "Translation Z";
		case ECurveType::RotationX: return "Rotation X";
		case ECurveType::RotationY: return "Rotation Y";
		case ECurveType::RotationZ: return "Rotation Z";
		case ECurveType::ScaleX: return "Scale X";
		case ECurveType::ScaleY: return "Scale Y";
		case ECurveType::ScaleZ: return "Scale Z";
		case ECurveType::MaterialColorR: return "Material Color - Red";
		case ECurveType::MaterialColorG: return "Material Color - Green";
		case ECurveType::MaterialColorB: return "Material Color - Blue";
		case ECurveType::MaterialColorA: return "Material Color - Alpha";
		case ECurveType::VertexColorTopLeftR: return "Vertex Color Top Left - Red";
		case ECurveType::VertexColorTopLeftG: return "Vertex Color Top Left - Green";
		case ECurveType::VertexColorTopLeftB: return "Vertex Color Top Left - Blue";
		case ECurveType::VertexColorTopLeftA: return "Vertex Color Top Left - Alpha";
		case ECurveType::VertexColorTopRightR: return "Vertex Color Top Right - Red";
		case ECurveType::VertexColorTopRightG: return "Vertex Color Top Right - Green";
		case ECurveType::VertexColorTopRightB: return "Vertex Color Top Right - Blue";
		case ECurveType::VertexColorTopRightA: return "Vertex Color Top Right - Alpha";
		case ECurveType::VertexColorBottomLeftR: return "Vertex Color Bottom Left - Red";
		case ECurveType::VertexColorBottomLeftG: return "Vertex Color Bottom Left - Green";
		case ECurveType::VertexColorBottomLeftB: return "Vertex Color Bottom Left - Blue";
		case ECurveType::VertexColorBottomLeftA: return "Vertex Color Bottom Left - Alpha";
		case ECurveType::VertexColorBottomRightR: return "Vertex Color Bottom Right - Red";
		case ECurveType::VertexColorBottomRightG: return "Vertex Color Bottom Right - Green";
		case ECurveType::VertexColorBottomRightB: return "Vertex Color Bottom Right - Blue";
		case ECurveType::VertexColorBottomRightA: return "Vertex Color Bottom Right - Alpha";
		case ECurveType::IlluminationColorR: return "Illumination Color - Red";
		case ECurveType::IlluminationColorG: return "Illumination Color - Green";
		case ECurveType::IlluminationColorB: return "Illumination Color - Blue";
		case ECurveType::IlluminationColorA: return "Illumination Color - Alpha";
		case ECurveType::Display: return "Display";
		case ECurveType::Width: return "Width";
		case ECurveType::Height: return "Height";
		case ECurveType::CropIndex0: return "Crop Index 0";
		case ECurveType::CropIndex1: return "Crop Index 1";
		case ECurveType::VertexColorTopLeft: return "Vertex Color Top Left";
		case ECurveType::VertexColorTopRight: return "Vertex Color Top Right";
		case ECurveType::VertexColorBottomLeft: return "Vertex Color Bottom Left";
		case ECurveType::VertexColorBottomRight: return "Vertex Color Bottom Right";
		case ECurveType::MaterialColor: return "Material Color";
		case ECurveType::IlluminationColor: return "Illumination Color";
		case ECurveType::Unknown: return "Unknown";
		default: return "Unknown";
		}
	}

	const char* Timeline::TrackName(SRS_TRACK& track) {
		return TrackName(track.trackType);
	}

	const char* Timeline::interpolationTypes[3] = { "Constant", "Linear", "Hermite" };
	const char* Timeline::interpolationTypesExtended[4] = { "Constant", "Linear", "Hermite", "Individual" };
}
