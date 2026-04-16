#pragma once
#include <imgui_internal.h>
#include <ui/Action.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include <ui/operation-modes/OperationMode.h>
#include <utilities/math/MathUtils.h>
#include "ForwardDeclarations.h"
#include "Selection.h"

template<typename OpModeContext>
class DragDropPlacementBehavior : public OperationModeBehavior {
	using Traits = DragDropPlacementBehaviorTraits<OpModeContext>;
	Traits traits;

	hh::game::ObjectData* previewObject{};
	bool wasDraggingGameObjectClass{};

	void ClearPreview() {
		if (!previewObject)
			return;
		traits.DeletePreviewObject(previewObject);
		previewObject = nullptr;
	}

	static bool IsMouseOverScene() {
		ImGuiContext& g = *GImGui;
		return g.HoveredWindow == nullptr || (g.HoveredWindow->Flags & ImGuiWindowFlags_NoInputs) != 0;
	}

	bool GetDropLocation(csl::math::Vector3& outLocation) const {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraSrv = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraSrv->GetTopComponent(0)) {
			Eigen::Projective3f inverseCameraMatrix =
				camera->viewportData.GetInverseViewMatrix() * camera->viewportData.projMatrix.inverse();
			Ray3f ray = ScreenPosToWorldRay(ImGui::GetMousePos(), inverseCameraMatrix);

			if (auto* physWorld = gameManager->GetService<hh::physics::PhysicsWorld>()) {
				csl::ut::MoveArray<hh::physics::PhysicsQueryResult> results{ hh::fnd::MemoryRouter::GetTempAllocator() };
				if (physWorld->RayCastAllHits(ray.start, ray.end, 0xFFFFFFFF, results)) {
					for (auto& result : results) {
						if (result.collider->filterCategory == 10) {
							outLocation = result.hitLocation;
							return true;
						}
					}
				}
			}

			Eigen::Vector3f fallback = ray.start + (ray.end - ray.start).normalized() * 50.0f;
			outLocation = { fallback.x(), fallback.y(), fallback.z() };
			return true;
		}
		return false;
	}

public:
	static constexpr unsigned int id = 14;
	virtual unsigned int GetId() override { return id; }

	DragDropPlacementBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode)
		: OperationModeBehavior{ allocator, operationMode }, traits{ operationMode.GetContext() } {}

	~DragDropPlacementBehavior() { ClearPreview(); }

	void Render() override {
		auto* payload = ImGui::GetDragDropPayload();
		bool isDraggingGameObjectClass = payload != nullptr && strcmp(payload->DataType, "GameObjectClass") == 0;

		if (isDraggingGameObjectClass && traits.CanPlace()) {
			bool overScene = IsMouseOverScene();
			csl::math::Vector3 location{};
			bool hasLocation = overScene && GetDropLocation(location);

			if (hasLocation) {
				if (!previewObject) {
					auto* objectClass = *static_cast<const hh::game::GameObjectClass* const*>(payload->Data);
					previewObject = traits.SpawnPreviewObject(objectClass, location);
				} else {
					traits.MovePreviewObject(previewObject, location);
				}
			}

			if (overScene && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				hh::game::ObjectData* toFinalize = previewObject;
				if (!toFinalize && hasLocation) {
					auto* objectClass = *static_cast<const hh::game::GameObjectClass* const*>(payload->Data);
					toFinalize = traits.SpawnPreviewObject(objectClass, location);
				}
				if (toFinalize) {
					previewObject = nullptr;
					if (auto* selection = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>())
						selection->Select(toFinalize);
					Dispatch(SceneChangedAction{});
				}
			}
		} else if (!isDraggingGameObjectClass && wasDraggingGameObjectClass) {
			ClearPreview();
		}

		wasDraggingGameObjectClass = isDraggingGameObjectClass;
	}
};

