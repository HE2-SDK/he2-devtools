#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Basic.h>
#include <resources/managed-memory/ManagedCArray.h>
#include <ucsl/resources/light/v2.h>

class ResMirageLightEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::gfx::ResMirageLight> resource;
	hh::fnd::Reference<hh::gfx::GOCPointLight> gocPointLight;
	unsigned int lightIdx;
	hh::gfx::GOCPointLight::Light* gocLight;
public:
	ResMirageLightEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResMirageLight* resource, hh::gfx::GOCPointLight* goc = nullptr, unsigned int idx = 0);
	virtual void RenderContents();
	static ResMirageLightEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResMirageLight* resource, hh::gfx::GOCPointLight* goc = nullptr, unsigned int idx = 0);
};
