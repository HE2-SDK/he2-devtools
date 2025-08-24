#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Basic.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <ucsl/resources/vibration/v21.h>

using namespace ucsl::resources::vibration::v21;

class ResVibrationEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::hid::ResVibration> resource;
	const ImVec4 color = ImVec4(0.31f, 0.69f, 0.776f, 1.0f);

	bool Editor(Sound& value);
	bool Editor(VibrationKeyframe& value, unsigned int idx);
	bool Editor(VibrationMotor& value, unsigned int idx, Vibration& parent);
	bool Editor(Vibration& value, unsigned int idx, VibData& parent);
	bool Editor(VibData& value);
public:
	ResVibrationEditor(csl::fnd::IAllocator* allocator, hh::hid::ResVibration* resource);
	virtual void RenderContents();
	static ResVibrationEditor* Create(csl::fnd::IAllocator* allocator, hh::hid::ResVibration* resource);
};
