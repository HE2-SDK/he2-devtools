#include "ToolBar.h"
#include <debug-rendering/DebugRenderer.h>
#include <rip/schemas/hedgeset.h>
#include <ui/common/Editors/Basic.h>
#include "Desktop.h"
#include "SettingsManager.h"
#include "GlobalSettings.h"
#include "common/Theme.h"
#include "common/inputs/Basic.h"
#include "resources/ResourceBrowser.h"
#include "game-services/GameServiceInspector.h"
#include "core-services/MemoryInspector.h"
#include "tools/RflComparer.h"
#include <ui/common/viewers/Basic.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
#include "tools/wars/NeedleFxSceneDataTesterV2.h"
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "core-services/GameModeInspector.h"
#include "core-services/GameUpdaterInspector.h"
#include "core-services/GraphicsContextInspector.h"
#include "core-services/RenderManagerInspector.h"
#include "core-services/RenderingEngineInspector.h"
#include "core-services/CameraManagerInspector.h"
#include "tools/rangers/NeedleFxSceneDataTester.h"
#include "tools/rangers/NeedleFxSceneDataTesterV2.h"
#include "tools/rangers/UIMapParamEditor.h"
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
#include "core-services/GraphicsContextInspector.h"
#include "core-services/RenderManagerInspector.h"
#include "tools/miller/NeedleFxSceneDataTesterV2.h"
#endif

#include "operation-modes/modes/object-inspection/ObjectInspection.h"
#include "operation-modes/modes/level-editor/LevelEditor.h"
#include "operation-modes/modes/fxcol-editor/FxColEditor.h"
#include "operation-modes/modes/svcol-editor/SvColEditor.h"
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "operation-modes/modes/pointcloud-editor/PointcloudEditor.h"
#endif
#include "operation-modes/modes/surfride-editor/SurfRideEditor.h"
#ifndef DEVTOOLS_TARGET_SDK_wars
#include "operation-modes/modes/dvscene-editor/DvSceneEditor.h"
#else
#include "operation-modes/modes/scene-editor/SceneEditor.h"
#endif

using namespace hh::game;

void ToolBar::Render() {
	auto& style = ImGui::GetStyle();
	float menuBarHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;
	float toolBarHeight = ImGui::GetFontSize() + style.FramePadding.y * 2.0f + style.WindowPadding.y * 2.0f;

	if (!ImGui::BeginViewportSideBar("Main menu", ImGui::GetMainViewport(), ImGuiDir_Up, menuBarHeight + toolBarHeight, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::MenuItem("Resource Browser"))
			new (Desktop::instance->GetAllocator()) ResourceBrowser(Desktop::instance->GetAllocator());

		if (ImGui::BeginMenu("Inspectors")) {
			if (ImGui::MenuItem("GameService"))
				new (Desktop::instance->GetAllocator()) GameServiceInspector(Desktop::instance->GetAllocator());
#ifndef DEVTOOLS_TARGET_SDK_miller
			if (ImGui::MenuItem("Memory"))
				new (Desktop::instance->GetAllocator()) MemoryInspector(Desktop::instance->GetAllocator());
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::MenuItem("GameUpdater"))
				new (Desktop::instance->GetAllocator()) GameUpdaterInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("GraphicsContext"))
				new (Desktop::instance->GetAllocator()) GraphicsContextInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RenderManager"))
				new (Desktop::instance->GetAllocator()) RenderManagerInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RenderingEngine"))
				new (Desktop::instance->GetAllocator()) RenderingEngineInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("GameMode"))
				new (Desktop::instance->GetAllocator()) GameModeInspector(Desktop::instance->GetAllocator());
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
			if (ImGui::MenuItem("GraphicsContext"))
				new (Desktop::instance->GetAllocator()) GraphicsContextInspector(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RenderManager"))
				new (Desktop::instance->GetAllocator()) RenderManagerInspector(Desktop::instance->GetAllocator());
#endif
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools")) {
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::MenuItem("UIMapParam Editor"))
				new (Desktop::instance->GetAllocator()) UIMapParamEditor(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("NeedleFxSceneData Tester") && ImGui::FindWindowByName("NeedleFxSceneData testing tool") == nullptr)
				new (Desktop::instance->GetAllocator()) NeedleFxSceneDataTester(Desktop::instance->GetAllocator());
#endif
			if (ImGui::MenuItem("NeedleFxSceneData Tester V2") && ImGui::FindWindowByName("NeedleFxSceneData testing tool V2") == nullptr)
				new (Desktop::instance->GetAllocator()) NeedleFxSceneDataTesterV2(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("RFL Comparer"))
				new (Desktop::instance->GetAllocator()) RflComparer(Desktop::instance->GetAllocator());
			if (ImGui::MenuItem("Export HSON template")) {
				IGFD::FileDialogConfig cfg{};
				cfg.path = GlobalSettings::defaultFileDialogDirectory;
				cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
				ImGuiFileDialog::Instance()->OpenDialog("HSONExportDialog", "Choose File", ".template.hson.json", cfg);
			}
			if (ImGui::MenuItem("Export RFL templates")) {
				IGFD::FileDialogConfig cfg{};
				cfg.path = ".";
				cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
				ImGuiFileDialog::Instance()->OpenDialog("RFLExportDialog", "Choose File", ".template.rfl.json", cfg);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Mode")) {
			if (ImGui::MenuItem("Object Inspection"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::object_inspection::ObjectInspection>();
			if (ImGui::MenuItem("Level Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::level_editor::LevelEditor>();
			if (ImGui::MenuItem("SvCol Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::svcol_editor::SvColEditor>();
#ifndef DEVTOOLS_TARGET_SDK_miller
			if (ImGui::MenuItem("FxCol Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::fxcol_editor::FxColEditor>();
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
			if (ImGui::MenuItem("PointcloudModel Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::pointcloud_editor::PointcloudEditor>(app::gfx::ResPointcloudModel::GetTypeInfo());
			if (ImGui::MenuItem("PointcloudLight Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::pointcloud_editor::PointcloudEditor>(app::gfx::ResPointcloudLight::GetTypeInfo());
#endif
			if (ImGui::MenuItem("SurfRide Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::surfride_editor::SurfRideEditor>();
#ifndef DEVTOOLS_TARGET_SDK_wars
			if (ImGui::MenuItem("DvScene Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::dvscene_editor::DvSceneEditor>();
#elif DEVTOOLS_TARGET_SDK_wars
			if (ImGui::MenuItem("Scene Editor"))
				Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::scene_editor::SceneEditor>();
#endif
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Settings"))
			SettingsManager::OpenConfigDialog();

		ImGui::EndMenuBar();
	}

	auto& gameUpdater = GameApplication::GetInstance()->GetGameUpdater();

	CheckboxFlags("Object pause", gameUpdater.flags, GameUpdater::Flags::OBJECT_PAUSE);
	ImGui::SetItemTooltip("Pauses only standard object layers. Certain layers and services (e.g. weather service) keep running. (Hotkey: F3)");
	ImGui::SameLine();
	CheckboxFlags("Debug pause", gameUpdater.flags, GameUpdater::Flags::DEBUG_PAUSE);
	ImGui::SetItemTooltip("Pauses (almost) every object layer. (Hotkey: F4)");
	ImGui::SameLine();

	if (ImGui::Button("Step frame"))
		gameUpdater.flags.set(GameUpdater::Flags::DEBUG_STEP_FRAME);
	ImGui::SetItemTooltip("When in debug pause, step through updates frame by frame. Note that if object pause is also enabled objects will not update even while stepping. (Hotkey: F5)");
	ImGui::SameLine();

	if (ImGui::IsKeyPressed(ImGuiKey_F3))
		gameUpdater.flags.flip(GameUpdater::Flags::OBJECT_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F4))
		gameUpdater.flags.flip(GameUpdater::Flags::DEBUG_PAUSE);
	if (ImGui::IsKeyPressed(ImGuiKey_F5))
		gameUpdater.flags.set(GameUpdater::Flags::DEBUG_STEP_FRAME);

	auto* debugCameraMgr = DebugCameraManager::GetInstance();

	bool debugCameraActive{ debugCameraMgr->isActive };

	ImGui::Checkbox("Debug camera", &debugCameraActive);
	ImGui::SetItemTooltip(
		"Enable the debug (free) camera. (F6)\n"
		"\n"
		"While in debug camera mode, all inputs are intercepted and the camera can be controlled using these key bindings:\n"
		"\n"
		"Gamepad:\n"
		"Left stick/dpad: move camera\n"
		"Right stick: rotate camera\n"
		"Hold left trigger + left stick/dpad up/down: move camera up/down\n"
		"Hold left trigger + right stick/dpad up/down: zoom in/out\n"
		"Hold left shoulder + right stick left/right: roll camera\n"
		"Hold right shoulder + right stick up/down: change fov\n"
		"X: reset camera\n"
		"Y: change camera movement speed\n"
		"B: change viewport\n"
		"\n"
		"Keyboard + mouse:\n"
		"WASD / arrow keys: move camera\n"
		"Mouse / numpad arrow keys: rotate camera\n"
		"Hold LCTRL/RCTRL + move forward backward OR PgUp/PgDn: move camera up/down\n"
		"Keypad +/-: zoom in/out\n"
		"Q/E: roll camera\n"
		"Home/End: change fov\n"
		"F1: reset camera\n"
		"LShift/RShift: toggle camera movement speed\n"
		"Keypad asterisk: change viewport"
	);
	ImGui::SameLine();

	if (ImGui::IsKeyPressed(ImGuiKey_F6, false))
		debugCameraActive = !debugCameraActive;

	if (debugCameraMgr->isActive != debugCameraActive) {
		if (debugCameraActive)
			debugCameraMgr->ActivateDebugCamera({ 1, 0, 1, 0 });
		else
			debugCameraMgr->DeactivateDebugCamera();
	}

	if (debugCameraActive) {
		bool debugCameraLocked{ debugCameraMgr->GetCamera()->IsLocked() };
		bool tempDebugCameraLocked{ debugCameraLocked };

		ImGui::Checkbox("Lock debug camera position", &tempDebugCameraLocked);
		ImGui::SameLine();

		if (debugCameraLocked != tempDebugCameraLocked)
			debugCameraMgr->GetCamera()->SetLocked(tempDebugCameraLocked);

		static bool showCameraInfo{};
		ImGui::Checkbox("Show camera information", &showCameraInfo);
		ImGui::SameLine();

		if (showCameraInfo) {
#ifdef DEVTOOLS_TARGET_SDK_wars
			auto& camCtrl = *((hh::game::FreeCameraController*)&*debugCameraMgr->GetCamera()->controller);
#else
			auto& camCtrl = *((hh::game::DefaultFreeCameraController*)&*debugCameraMgr->GetCamera()->controller)->padController;
#endif
			if (ImGui::Begin("Debug camera options")) {
				Editor("Origin / reset position", camCtrl.unk3.camera.origin);
				Editor("Position offset", camCtrl.unk3.camera.position);
				Editor("Yaw", camCtrl.unk3.camera.yaw);
				Editor("Pitch", camCtrl.unk3.camera.pitch);
				Editor("Roll", camCtrl.unk3.camera.roll);
				Editor("Zoom", camCtrl.unk3.camera.zoom);
				Editor("Near clip", camCtrl.unk3.viewport.nearClip);
				Editor("Far clip", camCtrl.unk3.viewport.farClip);
				Editor("Field of view", camCtrl.unk3.viewport.fov);
				ImGui::Separator();
				Editor("Current speed", camCtrl.currentSpeed);
				Editor("Speed options", camCtrl.speedOptions);
				Editor("Current speed option index", camCtrl.currentSpeedOptionIdx);
			}
			ImGui::End();
		}
	}

	ImGui::Checkbox("Render debug visuals", &devtools::debug_rendering::DebugRenderer::instance->enabled);

	if (auto* cockpit = hh::game::GameManager::GetInstance()->GetGameObject("UIGameCockpit"))
	if (auto* cockpitGocSprite = cockpit->GetComponent<hh::ui::GOCSprite>()) {
		bool enabled = cockpitGocSprite->flags.test(hh::ui::GOCSprite::Flag::ENABLED);

		ImGui::SameLine();
		if (ImGui::Checkbox("Render cockpit UI", &enabled))
			cockpitGocSprite->flags.set(hh::ui::GOCSprite::Flag::ENABLED, enabled);
	}

	if (ImGuiFileDialog::Instance()->Display("HSONExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
#ifdef DEVTOOLS_TARGET_SDK_wars
			rip::schemas::hedgeset::hson_template_builder<he2sdk::ucsl::GameInterface, rip::schemas::hedgeset::HSONFormat::GEDIT_V2> b{};
#else
			rip::schemas::hedgeset::hson_template_builder<he2sdk::ucsl::GameInterface, rip::schemas::hedgeset::HSONFormat::GEDIT_V3> b{};
#endif
			b.add_all();
			rip::schemas::hedgeset::write(ImGuiFileDialog::Instance()->GetFilePathName(), b.get_template());
		}
		ImGuiFileDialog::Instance()->Close();
	}

	if (ImGuiFileDialog::Instance()->Display("RFLExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			rip::schemas::hedgeset::rfl_template_builder<he2sdk::ucsl::GameInterface> b{};
			b.add_all();
			rip::schemas::hedgeset::write(ImGuiFileDialog::Instance()->GetFilePathName(), b.get_template());
		}
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::End();
}
