#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/previewer/Previewer.h>

class ResModelEditor : public StandaloneWindow {
public:
	hh::fnd::Reference<Previewer> previewer;

	virtual ~ResModelEditor();

	ResModelEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResModel* resource);
	virtual void RenderContents();
	static ResModelEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResModel* resource);
};
