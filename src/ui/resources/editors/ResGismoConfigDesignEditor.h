#pragma once
#include <ui/common/StandaloneWindow.h>

class ResGismoConfigDesignEditor : public StandaloneWindow {
	hh::fnd::Reference<app::ResGismoConfigDesign> resource;
public:
	ResGismoConfigDesignEditor(csl::fnd::IAllocator* allocator, app::ResGismoConfigDesign* resource);
	virtual void RenderContents();
	static ResGismoConfigDesignEditor* Create(csl::fnd::IAllocator* allocator, app::ResGismoConfigDesign* resource);
};
