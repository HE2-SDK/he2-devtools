#pragma once
#include <ui/common/StandaloneWindow.h>

class ResParticleLocationEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::animeff::ResParticleLocation> resource;
public:
	ResParticleLocationEditor(csl::fnd::IAllocator* allocator, hh::animeff::ResParticleLocation* resource);
	virtual void RenderContents();
	static ResParticleLocationEditor* Create(csl::fnd::IAllocator* allocator, hh::animeff::ResParticleLocation* resource);
};
