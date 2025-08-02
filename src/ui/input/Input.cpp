#include <ui/Context.h>
#include <ui/SettingsManager.h>
#include "Input.h"

bool ShouldCaptureKeyboardInputs() {
	return Context::imguiInited && Context::visible && ImGui::GetIO().WantCaptureKeyboard;
}

bool ShouldCaptureMouseInputs() {
    return Context::imguiInited && Context::visible && ((!Context::passThroughMouse && !ImGui::IsMouseDragging(ImGuiMouseButton_Right)) || ImGui::GetIO().WantCaptureMouse);
}

void AddDevToolsInputBindings(hh::hid::InputMapSettings* inputSettings) {
	//inputSettings->BindActionMapping("DmenuFastStep", 0x10000u);
	//inputSettings->BindActionMapping("DmenuFastStep", 0x10004u);
	//inputSettings->BindActionMapping("DmenuCursorUp", 0x10001u);
	//inputSettings->BindActionMapping("DmenuCursorDown", 0x10003u);
	//inputSettings->BindActionMapping("DmenuCursorLeft", 0x10000u);
	//inputSettings->BindActionMapping("DmenuCursorRight", 0x10002u);
	//inputSettings->BindActionMapping("DmenuCursorUp", 0x20052u);
	//inputSettings->BindActionMapping("DmenuCursorDown", 0x20051u);
	//inputSettings->BindActionMapping("DmenuCursorLeft", 0x20050u);
	//inputSettings->BindActionMapping("DmenuCursorRight", 0x2004Fu);
	//inputSettings->BindActionMapping("DmenuDecide", 0x10004u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x20028u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x20058u);
	//inputSettings->BindActionMapping("DmenuDecide", 0x10005u);
	//inputSettings->BindActionMapping("DmenuCancel", 0x20029u);
	//inputSettings->BindActionMapping("DmenuCancel", 0x40003u);

	// Debug camera, gamepad bindings
	//inputSettings->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x10007u);

	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x10005u);
	inputSettings->BindActionMapping("HHFreeCameraReset", 0x10004u);
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x1000cu);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x1000du);
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x1000fu);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x1000du);
	inputSettings->BindActionMapping("HHFreeCameraSwitchViewport", 0x10006u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10001u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10003u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10000u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10002u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x10009u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x10008u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x1000bu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x1000au, 1.0);

	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Debug camera, keyboard bindings
	//inputSettings->BindActionMapping("HHFreeCameraSwitchArcballCamera", 0x2001eu);

	// Movement: WASD & arrows
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20052u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20051u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20050u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x2004fu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2001au, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x20016u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20004u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x20007u, 1.0);

	// Look: keypad arrows
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20060u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2005au, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005cu, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x2005eu, 1.0);

	// Up/down: hold ctrl + move & pgup/dn
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x200e0u);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x200e4u);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x2004bu);
	inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x2004eu);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004bu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveVertical", 0x2004eu, -1.0);

	// Speed
	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x200e1u);
	inputSettings->BindActionMapping("HHFreeCameraSpeedChange", 0x200e5u);

	// Reset
	inputSettings->BindActionMapping("HHFreeCameraReset", 0x2003au);

	// Roll: q/e
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x20014u);
	inputSettings->BindActionMapping("HHFreeCameraRoll", 0x20008u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20014u, -1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x20008u, 1.0);

	// Zoom: +/-
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x2002eu);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x2002du);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002eu, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2002du, -1.0);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x20057u);
	inputSettings->BindActionMapping("HHFreeCameraDistance", 0x20056u);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20057u, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x20056u, -1.0);

	// Fovy: home/end
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x2004au);
	inputSettings->BindActionMapping("HHFreeCameraFovy", 0x2004du);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004au, 1.0);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x2004du, -1.0);

	// Change viewport: KP *
	inputSettings->BindActionMapping("HHFreeCameraSwitchViewport", 0x20055u);

	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraRotate", 0x2001fu);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransXY", 0x20020u);
	//inputSettings->BindActionMapping("HHFreeCameraArcballCameraTransZ", 0x20021u);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20052u, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveVertical", 0x20051u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x20050u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraMoveHorizontal", 0x2004fu, 1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002du, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraArcballCameraZoom", 0x2002eu, 1.0);

	// Mouse (to be used in conjuction with keyboard)
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubVertical", 0x40001u, -SettingsManager::settings.debugCameraMouseSensitivityY);
	inputSettings->BindAxisMapping("HHFreeCameraMoveSubHorizontal", 0x40000u, SettingsManager::settings.debugCameraMouseSensitivityX);

	// Move up/down
	//inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x30005u);
	//inputSettings->BindActionMapping("HHFreeCameraUpDown", 0x30006u);
	//inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30005u, -1.0);
	//inputSettings->BindAxisMapping("HHFreeCameraMoveHorizontal", 0x30006u, 1.0);

	inputSettings->BindActionMapping("DevToolsPhotoMode", 0x10011u);
}
