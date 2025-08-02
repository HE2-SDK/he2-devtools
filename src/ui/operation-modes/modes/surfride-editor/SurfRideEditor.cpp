#include "SurfRideEditor.h"
#include "ProjectTree.h"
#include "Timeline.h"
#include "ElementInspector.h"
#include "CropLibrary.h"
#include "Behaviors.h"
#include "Actions.h"
#include "texture-editor/TextureEditor.h"
#ifdef DEVTOOLS_TARGET_SDK_wars
#include <ucsl-reflection/reflections/resources/swif/v5.h>
#else
#include <ucsl-reflection/reflections/resources/swif/v6.h>
#endif
#include <rip/binary/containers/swif/SWIF.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/GlobalSettings.h>
#include <utilities/math/MathUtils.h>
#include <utilities/BoundingBoxes.h>
#include <span>

namespace ui::operation_modes::modes::surfride_editor
{
	using namespace ucsl::resources::swif::swif_version;

	SurfRideEditor::SurfRideEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		AddPanel<ProjectTree>();
		AddPanel<Timeline>();
		AddPanel<ElementInspector>();
		AddPanel<CropLibrary>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<SelectionVisual2DBehavior>();
		AddBehavior<ScreenSpaceManipulationBehavior>();
		AddBehavior<MousePickingBehavior>();
		AddBehavior<SelectionMousePickingBehavior>();

		hh::game::GameManager::GetInstance()->AddComponentListener(this);
	}

	SurfRideEditor::~SurfRideEditor() {
		hh::game::GameManager::GetInstance()->RemoveComponentListener(this);
	}

	void SurfRideEditor::ProcessAction(const ActionBase& action) {
		OperationMode::ProcessAction(action);

		switch (action.id) {
		case SetResourceAction::id: {
			auto* resource = static_cast<const SetResourceAction&>(action).payload;

			GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
			GetContext().SetResource(resource);

			Dispatch(ResourceChangedAction{});
			break;
		}
		case SetFocusedSceneAction::id: {
			auto* scene = static_cast<const SetFocusedSceneAction&>(action).payload;

			GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
			GetContext().focusedScene = scene;

			Dispatch(FocusedSceneChangedAction{});
			break;
		}
		case AddCastToLayerAction::id: {
			auto payload = static_cast<const AddCastToLayerAction&>(action).payload;

			GetContext().AddCast(payload.layer, payload.type);
			break;
		}
		case AddCastToCastAction::id: {
			auto& payload = static_cast<const AddCastToCastAction&>(action).payload;

			GetContext().AddCast(payload.cast, payload.type);
			break;
		}
		case RemoveCastAction::id: {
			auto& cast = static_cast<const RemoveCastAction&>(action).payload;

			GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
			GetContext().RemoveCast(cast);
			break;
		}
		case OpenTextureEditorAction::id: {
			auto* host = new (GetAllocator()) StandaloneOperationModeHost<texture_editor::TextureEditor>{ GetAllocator() };
			host->operationMode.GetContext().gocSprite = GetContext().gocSprite;
			break;
		}
		case ExportResourceAction::id: {
			IGFD::FileDialogConfig cfg{};
			cfg.path = GlobalSettings::defaultFileDialogDirectory;
			cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
			cfg.userDatas = GetContext().project;
			ImGuiFileDialog::Instance()->OpenDialog("ResSurfRideProjectExportDialog", "Choose File", ".swif", cfg);
			break;
		}
		case CriticalParametersChangedAction::id: {
			GetContext().ReloadResource();
			break;
		}
		}
	}

	std::optional<csl::geom::Aabb> SurfRideEditor::RenderCasts(SurfRide::Cast& cast) {
#ifdef DEVTOOLS_TARGET_SDK_wars
		if (cast.flags & 0x1000 || !cast.transform->display)
#else
		if (cast.flags & 0x1000 || !cast.transform->fullDisplay)
#endif
			return std::nullopt;

		csl::geom::Aabb aabb{};
		aabb.min = { INFINITY, INFINITY, INFINITY };
		aabb.max = { -INFINITY, -INFINITY, -INFINITY };

		bool updated{};
		updated |= GetContext().UpdateAabbWithoutChildren(&cast, aabb);

		for (auto child : cast.GetChildren()) {
			auto childAabb = RenderCasts(*child);

			if (childAabb.has_value()) {
				AddAabb(aabb, childAabb.value());
				updated = true;
			}
		}

		if (updated) {
			auto min = NDCCoordsToImGui({ aabb.min.x(), aabb.max.y() });
			auto max = NDCCoordsToImGui({ aabb.max.x(), aabb.min.y() });

			ImGui::GetWindowDrawList()->AddRect({ min.x, max.y }, { max.x, min.y }, IM_COL32(255, 0, 255, 255));

			ImGui::SetCursorPos(min + ImVec2(2, 0));
			ImGui::Text("Layer: %s, Cast: %s", cast.layer->layerData->name, cast.castData->name);
		}

		return updated ? std::make_optional(aabb) : std::nullopt;
	}

	void SurfRideEditor::RenderCasts(SurfRide::Layer& layer) {
		if (layer.flags & 0x100)
			return;

		for (auto cast : layer.GetCasts())
			RenderCasts(*cast);
	}

	void SurfRideEditor::RenderCasts(SurfRide::Scene& scene) {
		for (auto layer : scene.GetLayers())
			RenderCasts(*layer);
	}

	void SurfRideEditor::RenderScene() {
		auto& context = GetContext();

		if (context.focusedScene == nullptr)
			return;

		auto* runtimeScene = context.FindRuntimeScene(context.focusedScene->id);

		if (runtimeScene == nullptr)
			return;

		ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 1.0f, 1.0f });
		RenderCasts(*runtimeScene);
		ImGui::PopStyleColor();

		if (ImGuiFileDialog::Instance()->Display("ResSurfRideProjectExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				auto* exportProjectData = static_cast<SRS_PROJECT*>(ImGuiFileDialog::Instance()->GetUserDatas());

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::trunc | std::ios::binary };
				rip::binary::containers::swif::v1::SWIFSerializer serializer{ ofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportProjectData);
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}

	void SurfRideEditor::ComponentRemovedCallback(hh::game::GOComponent* component) {
		if (GetContext().gocSprite == component)
			Dispatch(SetResourceAction{ nullptr });
	}
}
