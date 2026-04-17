#pragma once
#include <ui/operation-modes/OperationModeBehavior.h>
#include "MousePicking.h"

template<typename OpModeContext>
class GroundContextMenuBehavior : public OperationModeBehavior {
public:
	typename MousePickingBehaviorTraits<OpModeContext>::LocationType pickedLocation{ 0.0f, 0.0f, 0.0f };

	static constexpr unsigned int id = 12;
	virtual unsigned int GetId() override { return id; }

	using OperationModeBehavior::OperationModeBehavior;

	virtual void Render() override {
#ifndef DEVTOOLS_TARGET_SDK_wars
		auto& updater = hh::game::GameApplication::GetInstance()->GetGameUpdater();
		static bool restoreObjectPause = false;
		if (restoreObjectPause) {
			updater.flags.set(hh::game::GameUpdater::Flags::OBJECT_PAUSE, true);
			restoreObjectPause = false;
		}

		auto* mousePicking = operationMode.GetBehavior<MousePickingBehavior<OpModeContext>>();

		if (mousePicking->picked && mousePicking->mouseButton == ImGuiMouseButton_Right)
			if (auto* levelInfo = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>())
				if (auto* player = static_cast<app::player::Player*>(hh::fnd::MessageManager::GetInstance()->GetMessengerByHandle(levelInfo->GetPlayerObject(0))))
					if (auto* playerKine = player->GetComponent<app::player::GOCPlayerKinematicParams>()) {
						pickedLocation = mousePicking->pickedLocation;
						ImGui::OpenPopup("WorldContext");
					}


		if (ImGui::BeginPopup("WorldContext")) {
			if (auto* levelInfo = hh::game::GameManager::GetInstance()->GetService<app::level::LevelInfo>())
				if (auto* player = static_cast<app::player::Player*>(hh::fnd::MessageManager::GetInstance()->GetMessengerByHandle(levelInfo->GetPlayerObject(0))))
					if (auto* playerKine = player->GetComponent<app::player::GOCPlayerKinematicParams>())
						if (ImGui::Selectable("Teleport player")) {
							playerKine->SetPosition({ pickedLocation.x(), pickedLocation.y(), pickedLocation.z(), 0.0f });
							if (updater.flags.test(hh::game::GameUpdater::Flags::OBJECT_PAUSE)) {
								updater.flags.set(hh::game::GameUpdater::Flags::OBJECT_PAUSE, false);
								updater.flags.set(hh::game::GameUpdater::Flags::DEBUG_STEP_FRAME, true);
								restoreObjectPause = true;
							} else if (updater.flags.test(hh::game::GameUpdater::Flags::DEBUG_PAUSE)) {
								updater.flags.set(hh::game::GameUpdater::Flags::DEBUG_STEP_FRAME, true);
							}
#ifdef DEVTOOLS_TARGET_SDK_miller
							if (auto* physAnim = player->GetComponent<hh::pba::GOCPhysicalAnimationBullet>()) {
								physAnim->SetEnabled(false);
								physAnim->SetEnabled(true);
								physAnim->Reset();
							}
#endif
						}
			ImGui::EndPopup();
		};
#endif
	}
};
