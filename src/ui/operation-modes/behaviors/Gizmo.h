#pragma once
#include <ui/Action.h>
#include <ui/Desktop.h>
#include <ui/Shortcuts.h>
#include <ui/operation-modes/OperationModeBehavior.h>
#include "SelectionTransformation.h"

template<typename OpModeContext>
class GizmoBehavior : public OperationModeBehavior {
    ImGuizmo::OPERATION gizmoOperation{ ImGuizmo::TRANSLATE };
    ImGuizmo::MODE gizmoMode{ ImGuizmo::LOCAL };
    bool wasUsing{ false };

public:
	using ChangeCoordinateSystemAction = Action<ActionId::CHANGE_COORDINATE_SYSTEM>;
	using TransformModeAction = Action<ActionId::TRANSFORM_MODE>;
	using TranslateModeAction = Action<ActionId::TRANSLATE_MODE>;
	using NoTranslateModeAction = Action<ActionId::NO_TRANSLATE_MODE>;
	using RotateModeAction = Action<ActionId::ROTATE_MODE>;
	using NoRotateModeAction = Action<ActionId::NO_ROTATE_MODE>;
	using ScaleModeAction = Action<ActionId::SCALE_MODE>;
	using NoScaleModeAction = Action<ActionId::NO_SCALE_MODE>;
	using OnlyXAxisAction = Action<ActionId::ONLY_X_AXIS>;
	using LockXAxisAction = Action<ActionId::LOCK_X_AXIS>;
	using OnlyYAxisAction = Action<ActionId::ONLY_Y_AXIS>;
	using LockYAxisAction = Action<ActionId::LOCK_Y_AXIS>;
	using OnlyZAxisAction = Action<ActionId::ONLY_Z_AXIS>;
	using LockZAxisAction = Action<ActionId::LOCK_Z_AXIS>;

	GizmoBehavior(csl::fnd::IAllocator* allocator, OperationMode<OpModeContext>& operationMode) : OperationModeBehavior{ allocator, operationMode } {}

	static constexpr unsigned int id = 2;
	virtual unsigned int GetId() override { return id; }

	virtual void Init() {
		Desktop::instance->BindShortcut<ChangeCoordinateSystemAction>(ShortcutId::CHANGE_COORDINATE_SYSTEM);
		Desktop::instance->BindShortcut<TransformModeAction>(ShortcutId::TRANSFORM_MODE);
		Desktop::instance->BindShortcut<TranslateModeAction>(ShortcutId::TRANSLATE_MODE);
		Desktop::instance->BindShortcut<NoTranslateModeAction>(ShortcutId::NO_TRANSLATE_MODE);
		Desktop::instance->BindShortcut<RotateModeAction>(ShortcutId::ROTATE_MODE);
		Desktop::instance->BindShortcut<NoRotateModeAction>(ShortcutId::NO_ROTATE_MODE);
		Desktop::instance->BindShortcut<ScaleModeAction>(ShortcutId::SCALE_MODE);
		Desktop::instance->BindShortcut<NoScaleModeAction>(ShortcutId::NO_SCALE_MODE);
		Desktop::instance->BindShortcut<OnlyXAxisAction>(ShortcutId::ONLY_X_AXIS);
		Desktop::instance->BindShortcut<LockXAxisAction>(ShortcutId::LOCK_X_AXIS);
		Desktop::instance->BindShortcut<OnlyYAxisAction>(ShortcutId::ONLY_Y_AXIS);
		Desktop::instance->BindShortcut<LockYAxisAction>(ShortcutId::LOCK_Y_AXIS);
		Desktop::instance->BindShortcut<OnlyZAxisAction>(ShortcutId::ONLY_Z_AXIS);
		Desktop::instance->BindShortcut<LockZAxisAction>(ShortcutId::LOCK_Z_AXIS);
	}

	virtual void Deinit() {
		Desktop::instance->UnbindShortcut(ShortcutId::CHANGE_COORDINATE_SYSTEM);
		Desktop::instance->UnbindShortcut(ShortcutId::TRANSLATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::NO_TRANSLATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::ROTATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::NO_ROTATE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::SCALE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::NO_SCALE_MODE);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_X_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_X_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_Y_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_Y_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::ONLY_Z_AXIS);
		Desktop::instance->UnbindShortcut(ShortcutId::LOCK_Z_AXIS);
	}

	virtual void ProcessAction(const ActionBase& action) {
		switch (action.id) {
		case ChangeCoordinateSystemAction::id:
			gizmoMode = gizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
			break;
		case TransformModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowTranslate || GizmoBehaviorTraits<OpModeContext>::allowRotate || GizmoBehaviorTraits<OpModeContext>::allowScale)
				gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
					| (GizmoBehaviorTraits<OpModeContext>::allowTranslate ? ImGuizmo::TRANSLATE : static_cast<ImGuizmo::OPERATION>(0))
					| (GizmoBehaviorTraits<OpModeContext>::allowRotate ? ImGuizmo::ROTATE : static_cast<ImGuizmo::OPERATION>(0))
					| (GizmoBehaviorTraits<OpModeContext>::allowScale ? ImGuizmo::SCALE : static_cast<ImGuizmo::OPERATION>(0));
			break;
		case TranslateModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowTranslate)
				gizmoOperation = ImGuizmo::TRANSLATE;
			break;
		case NoTranslateModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowRotate || GizmoBehaviorTraits<OpModeContext>::allowScale)
				gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
					| (GizmoBehaviorTraits<OpModeContext>::allowRotate ? ImGuizmo::ROTATE : static_cast<ImGuizmo::OPERATION>(0))
					| (GizmoBehaviorTraits<OpModeContext>::allowScale ? ImGuizmo::SCALE : static_cast<ImGuizmo::OPERATION>(0));
			break;
		case RotateModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowRotate)
				gizmoOperation = ImGuizmo::ROTATE;
			break;
		case NoRotateModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowTranslate || GizmoBehaviorTraits<OpModeContext>::allowScale)
				gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (GizmoBehaviorTraits<OpModeContext>::allowTranslate ? ImGuizmo::TRANSLATE : static_cast<ImGuizmo::OPERATION>(0))
				| (GizmoBehaviorTraits<OpModeContext>::allowScale ? ImGuizmo::SCALE : static_cast<ImGuizmo::OPERATION>(0));
			break;
		case ScaleModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowScale)
				gizmoOperation = ImGuizmo::SCALE;
			break;
		case NoScaleModeAction::id:
			if constexpr (GizmoBehaviorTraits<OpModeContext>::allowTranslate || GizmoBehaviorTraits<OpModeContext>::allowRotate)
				gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (GizmoBehaviorTraits<OpModeContext>::allowTranslate ? ImGuizmo::TRANSLATE : static_cast<ImGuizmo::OPERATION>(0))
				| (GizmoBehaviorTraits<OpModeContext>::allowRotate ? ImGuizmo::ROTATE : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case OnlyXAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_X : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_X : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_X : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case LockXAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_Y | ImGuizmo::TRANSLATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_Y | ImGuizmo::SCALE_Z : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case OnlyYAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_Y : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_Y : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_Y : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case LockYAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_X | ImGuizmo::SCALE_Z : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case OnlyZAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_Z : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_Z : static_cast<ImGuizmo::OPERATION>(0));
			break;
        case LockZAxisAction::id:
			gizmoOperation = static_cast<ImGuizmo::OPERATION>(0)
				| (gizmoOperation & ImGuizmo::TRANSLATE ? ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::ROTATE ? ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y : static_cast<ImGuizmo::OPERATION>(0))
				| (gizmoOperation & ImGuizmo::SCALE ? ImGuizmo::SCALE_X | ImGuizmo::SCALE_Y : static_cast<ImGuizmo::OPERATION>(0));
			break;
		}
    }

    virtual void Render() override {
		if (auto* gameManager = hh::game::GameManager::GetInstance())
		if (auto* cameraManager = gameManager->GetService<hh::game::CameraManager>())
		if (auto* camera = cameraManager->GetTopComponent(0)) {
			auto* selTransform = operationMode.GetBehavior<SelectionTransformationBehaviorBase<false>>();

			if (!selTransform->HasTransformableSelection())
				return;

			auto selectionTransform = selTransform->GetSelectionTransform();

			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

			bool isUsing = ImGuizmo::IsUsing();
			if (isUsing && !wasUsing && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)))
				Dispatch(Action<ActionId::LEVEL_EDITOR_CLONE_SELECTION>{});
			wasUsing = isUsing;

			if (ImGuizmo::Manipulate(camera->viewportData.viewMatrix.data(), camera->viewportData.projMatrix.data(), gizmoOperation, gizmoMode, selectionTransform.data(), NULL, NULL))
				selTransform->SetSelectionTransform(selectionTransform);
		}
    }
};
