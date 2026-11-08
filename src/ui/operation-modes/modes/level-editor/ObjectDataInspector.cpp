#include "ObjectDataInspector.h"
#include "Behaviors.h"
#include "Context.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/ObjectData.h>
#include <utilities/ObjectDataUtils.h>
#include <imgui_internal.h>
#include "Actions.h"

namespace ui::operation_modes::modes::level_editor {
	using namespace hh::fnd;
	using namespace hh::game;

	void ObjectDataInspector::RenderPanel() {
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("%zd objects selected", selection.size());
			for (auto obj : selection)
				ImGui::Text("%s", GetObjectName(obj));
			ImGui::SeparatorText("Align / Distribute");
			ImGui::PushID("Align / Distribute");
			Editor("##AlignX", alignX);
			ImGui::SameLine();
			Editor("SpacingX", distributeSpacing.x());
			Editor("##AlignY", alignY);
			ImGui::SameLine();
			Editor("SpacingY", distributeSpacing.y());
			Editor("##AlignZ", alignZ);
			ImGui::SameLine();
			Editor("SpacingZ", distributeSpacing.z());
			if (ImGui::Button("Align / Distribute"))
				Distribute();
			ImGui::PopID();
		}
		else {
			auto focusedObject = selection[0];
			// This can get imperformant at larger scales, when selecting the last object of the last layer, but it works for now.
			// Best solution would be to set the object type of the Selection behavior to a custom struct, which would have a member for the parent layer.
#ifdef DEVTOOLS_TARGET_SDK_wars
			bool edited = Editor("Focused object", *focusedObject, GetContext().GetParentLayer(focusedObject)->GetResource());
#else
			bool edited = Editor("Focused object", *focusedObject);
#endif

			if (edited || ImGui::IsItemDeactivatedAfterEdit()) {
				Dispatch(SelectionTransformationBehavior<Context>::SelectionTransformChangedAction{});
				Dispatch(ChangingParametersAction{});
			}

			if (ImGui::IsItemDeactivatedAfterEdit())
				Dispatch(StopChangingParametersAction{});
		}
	}

	PanelTraits ObjectDataInspector::GetPanelTraits() const
	{
		return { "Object data inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(800, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}

	void ObjectDataInspector::Distribute()
	{
		if (alignX) DistributeAlongBasis(Eigen::Vector3f::UnitX(), distributeSpacing.x());
		if (alignY) DistributeAlongBasis(Eigen::Vector3f::UnitY(), distributeSpacing.y());
		if (alignZ) DistributeAlongBasis(Eigen::Vector3f::UnitZ(), distributeSpacing.z());

		Dispatch(SelectionTransformationBehavior<Context>::SelectionTransformChangedAction{});
	}

	void ObjectDataInspector::DistributeAlongBasis(const Eigen::Vector3f& basis, float spacing)
	{
		Eigen::Vector3f avg{ 0.0f, 0.0f, 0.0f };

		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0)
			return;

		for (auto* obj : selection)
			avg += obj->transform.position.cwiseProduct(basis);
		avg /= static_cast<float>(selection.size());

		size_t idx{};
		float startOffset{ -static_cast<float>(selection.size() - 1) / 2.0f };
		for (auto* obj : selection) {
			auto transform = obj->transform;
			transform.position = transform.position - transform.position.cwiseProduct(basis) + avg + basis * spacing * (startOffset + static_cast<float>(idx));
			UpdateAbsoluteTransform(ObjectTransformDataToAffine3f(transform), *obj);
			idx++;
		}
	}
}
