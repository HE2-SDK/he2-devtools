#include <ui/input/Input.h>
#include <ui/Viewport.h>

HOOK(void, __fastcall, KeyboardUpdate, 0x140F22430, hh::hid::Keyboard* self, float unkParam)
{
	if (!ShouldCaptureKeyboardInputs()) {
		originalKeyboardUpdate(self, unkParam);
	}
}

HOOK(void, __fastcall, MouseUpdate, 0x140F16F00, hh::hid::MouseWin32* self, float unkParam)
{
	static bool capturing{};
	if (ShouldCaptureMouseInputs()) {
		self->state.deltaX = 0;
		self->state.deltaY = 0;
		self->deltaX = 0;
		self->deltaY = 0;
		self->prevX = self->absoluteX;
		self->prevY = self->absoluteY;
		capturing = true;
	}
    else {
		originalMouseUpdate(self, unkParam);
		if (capturing) {
			self->state.deltaX = 0;
			self->state.deltaY = 0;
			capturing = false;
		}
	}
}

HOOK(csl::math::Vector3*, __fastcall, PointingGetTarget, 0x140F22F70, hh::hid::InputDevice* self, csl::math::Vector3* resstr, unsigned int inputId)
{
	if (ShouldCaptureMouseInputs()) {
		*resstr = csl::math::Vector3{ 0.0f, 0.0f, 0.0f };
		return resstr;
	}
	else
		return originalPointingGetTarget(self, resstr, inputId);
}

HOOK(void, __fastcall, BindMaps, 0x14025ED30, hh::game::GameManager *gameManager, hh::hid::InputMapSettings *inputSettings) {
	originalBindMaps(gameManager, inputSettings);
    AddDevToolsInputBindings(inputSettings);
}

void InstallInputHooks() {
	INSTALL_HOOK(BindMaps);
	INSTALL_HOOK(KeyboardUpdate);
    INSTALL_HOOK(MouseUpdate);
    INSTALL_HOOK(PointingGetTarget);
}

void ReloadInputSettings() {
    hh::game::GameManager::GetInstance()->ReloadInputSettings(true);
}
