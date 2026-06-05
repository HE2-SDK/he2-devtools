#pragma once
#include <optional>
#include <ui/Desktop.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "ForwardDeclarations.h"
#include "MousePicking.h"
#include "GroundContextMenu.h"

template<typename OpModeContext>
class PlacementBehavior : public OperationModeBehavior {
public:
	using Traits = PlacementBehaviorTraits<OpModeContext>;
	using LocationType = typename MousePickingBehavior<OpModeContext>::LocationType;

private:
	Traits traits;
	bool placing{};

	void PerformPlace(const LocationType& location) {
		auto obj = traits.PlaceObject(location);

		if (auto* selection = operationMode.GetBehavior<SelectionBehavior<OpModeContext>>())
			selection->Select(obj);

		Dispatch(ObjectPlacedAction{ obj });
		Dispatch(SceneChangedAction{});
	}

public:
	static constexpr unsigned int id = 13;
	virtual unsigned int GetId() override { return id; }

	using ObjectPlacedAction = Action<ActionId::OBJECT_PLACED, typename MousePickingBehavior<OpModeContext>::ObjectType>;
	using TogglePlaceModeAction = Action<ActionId::TOGGLE_PLACE_MODE>;

	PlacementBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode }, traits { operationMode.GetContext() } {}

	void Init() {
		Desktop::instance->BindShortcut<TogglePlaceModeAction>(ShortcutId::TOGGLE_PLACE_MODE);
	}

	void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::TOGGLE_PLACE_MODE);
	}
		
	void SetPlaceMode(bool placing) {
		this->placing = placing;
	}

	bool CanPlace() const {
		return traits.CanPlace();
	}

	bool IsInPlaceMode() const {
		return traits.CanPlace() && placing;
	}

	void Render() override {
		auto* mousePicking = operationMode.GetBehavior<MousePickingBehavior<OpModeContext>>();

		if (CanPlace()) {
			if ((placing || ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) && mousePicking->locationPicked)
				PerformPlace(mousePicking->pickedLocation);

			if constexpr (Traits::is3D) {
				auto* groundContextMenu = operationMode.GetBehavior<GroundContextMenuBehavior<OpModeContext>>();
				if (groundContextMenu && ImGui::BeginPopup("WorldContext")) {
					if (ImGui::MenuItem("Place object", nullptr, nullptr))
						PerformPlace(groundContextMenu->pickedLocation);

					ImGui::EndPopup();
				}
			}
		}
	}
};
