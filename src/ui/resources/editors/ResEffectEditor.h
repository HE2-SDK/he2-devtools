#pragma once
#include <ui/common/StandaloneWindow.h>

class ResEffectEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::eff::ResEffect> resource;
public:
	ResEffectEditor(csl::fnd::IAllocator* allocator, hh::eff::ResEffect* resource);
	static ResEffectEditor* Create(csl::fnd::IAllocator* allocator, hh::eff::ResEffect* resource);
	virtual void RenderContents();
};
