#include "UIMapParamEditor.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/Textures.h>
#include <imgui_internal.h>

using namespace hh::fnd;

UIMapParamEditor::UIMapParamEditor(csl::fnd::IAllocator* allocator) : StandaloneWindow(allocator)
{
	SetTitle("UIMapParam Editor");
}

UIMapParamEditor::~UIMapParamEditor() {
}

void UIMapParamEditor::PreRender() {
	ImGui::SetNextWindowSize(ImVec2(1400, 1100), ImGuiCond_Once);
}

static const char* islandNames[]{ "Kronos", "Ares", "Chaos", "Rhea", "Ouranos", "Ouranos (Final Horizon)" };
void UIMapParamEditor::RenderContents() {
	if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->ContentRegionRect, ImGui::GetCurrentWindow()->ID)) {
		if (auto* payload = ImGui::AcceptDragDropPayload("Resource")) {
			auto* res = *static_cast<ManagedResource**>(payload->Data);
			const ResourceTypeInfo* typeInfo = &res->GetClass();

			if (typeInfo == ResReflection::GetTypeInfo())
				resource = static_cast<ResReflectionT<heur::rfl::UIMapParameter>*>(res);
			else if (typeInfo == hh::gfnd::ResTexture::GetTypeInfo())
				textureResource = static_cast<hh::gfnd::ResTexture*>(res);
		}
		ImGui::EndDragDropTarget();
	}

	if (resource != nullptr) {
		auto cellSize = ImVec2(24, 24);
		auto* dl = ImGui::GetWindowDrawList();
		auto* uimapParam = resource->GetData();
		auto& island = uimapParam->islandMapParam[static_cast<unsigned int>(islandId)];

		ImGui::BeginGroup();
		auto startPos = ImGui::GetCursorPos();

		if (textureResource != nullptr) {
			ImGui::Image(GetTextureIDFromMIRAGETexture(textureResource->GetTexture()), ImVec2(
				cellSize.x * sizeof(island.challengeIDHorizonGridGroup) / sizeof(decltype(island.challengeIDHorizonGridGroup[0])),
				cellSize.y * sizeof(island.challengeIDHorizonGridGroup[0].challengeID) / sizeof(decltype(island.challengeIDHorizonGridGroup[0].challengeID[0]))
			));
		}

		ImGui::SetCursorPos(startPos);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		size_t row{};
		for (auto& gridGroup : island.challengeIDHorizonGridGroup) {
			ImGui::PushID(row);

			size_t column{};
			for (auto& challengeId : gridGroup.challengeID) {
				if (column != 0)
					ImGui::SameLine();

				ImGui::PushID(column);

				ImColor color{ 255, 255, 255, static_cast<int>(255.0f * opacity) };
				for (size_t i = 0; i < sizeof(uimapParam->idColors) / sizeof(heur::rfl::IDColor); i++) {
					auto& col = uimapParam->idColors[i];

					if (uimapParam->idColors[i].id == challengeId) {
						color = ImColor{ col.color.r, col.color.g, col.color.b, static_cast<int>(static_cast<float>(col.color.a) * opacity) };
						break;
					}
				}

				auto screenPos = ImGui::GetCursorScreenPos();

				ImGui::Dummy(cellSize);

				auto id = ImGui::GetID("hover");

				if (ImGui::ItemHoverable(ImRect(screenPos, screenPos + cellSize), id, ImGuiItemFlags_None)) {
					if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(screenPos, screenPos + cellSize)) {
						ImGui::SetActiveID(id, ImGui::GetCurrentWindow());
						challengeId = uimapParam->idColors[currentChallengeIdx].id;
					}
					if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(screenPos, screenPos + cellSize)) {
						ImGui::ClearActiveID();
					}
				}

				dl->AddRectFilled(screenPos, screenPos + cellSize, color);

				ImGui::PopID();
				column++;
			}

			ImGui::PopID();

			row++;
		}
		ImGui::PopStyleVar();
		ImGui::EndGroup();
		
		ImGui::SameLine();

		if (ImGui::BeginChild("Challenge IDs", ImVec2(0.0f, 0.0f))) {
			ComboEnum("Island", islandId, islandNames);

			float zero{ 0.0f };
			float one{ 1.0f };
			SliderScalar("Preview opacity", opacity, &zero, &one);

			if (ImGui::BeginTable("Challenge IDs", 3)) {
				ImGui::TableSetupColumn("##checkbox", ImGuiTableColumnFlags_WidthFixed, 30.0f);
				ImGui::TableSetupColumn("Challenge ID", ImGuiTableColumnFlags_WidthFixed, 70.0f);
				ImGui::TableSetupColumn("Color");
				ImGui::TableHeadersRow();

				for (size_t i = 0; i < sizeof(uimapParam->idColors) / sizeof(heur::rfl::IDColor); i++) {
					auto& col = uimapParam->idColors[i];
					
					ImGui::PushID(i);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					bool selected{ i == currentChallengeIdx };
					if (Editor("##selected", selected) && selected)
						currentChallengeIdx = i;
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(64.0f);
					Editor("##id", col.id);
					ImGui::TableNextColumn();
					Editor("##color", (ucsl::colors::Color8ABGR&)col.color);

					ImGui::PopID();
				}

				ImGui::EndTable();
			}
		}
		ImGui::EndChild();
	}
}