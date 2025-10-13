#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/viewport/Viewport.h>

class ResAnimTexSrtEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::gfx::ResAnimTexSrt> resource;
	hh::fnd::Reference<Viewport> viewport;
	hh::needle::PBRModelInstance* previewModel;
	hh::fnd::Reference<hh::gfx::TexSrtBlenderHH> blender;

	void SetModel(hh::needle::PBRModelInstance* model);
	void SetModel(hh::gfx::ResModel* model);
public:
	ResAnimTexSrtEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResAnimTexSrt* resource);
	static ResAnimTexSrtEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResAnimTexSrt* resource);
	virtual void RenderContents();
};
