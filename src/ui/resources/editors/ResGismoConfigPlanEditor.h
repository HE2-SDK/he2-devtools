#pragma once
#include <ui/common/StandaloneWindow.h>

class ResGismoConfigPlanEditor : public StandaloneWindow {
	hh::fnd::Reference<app::ResGismoConfigPlan> resource;
public:
	ResGismoConfigPlanEditor(csl::fnd::IAllocator* allocator, app::ResGismoConfigPlan* resource);
	virtual void RenderContents();
	static ResGismoConfigPlanEditor* Create(csl::fnd::IAllocator* allocator, app::ResGismoConfigPlan* resource);
};
