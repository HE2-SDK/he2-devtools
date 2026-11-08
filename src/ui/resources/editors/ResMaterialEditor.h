#pragma once

#include <ui/common/StandaloneWindow.h>

class ResMaterialEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::gfx::ResMaterial> resource;
public:
	ResMaterialEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource);
	virtual void RenderContents();
	static ResMaterialEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResMaterial* resource);
	void RenderFloatParameterEditor(size_t idx, hh::needle::ParameterValueFloat* data);
	void RenderBoolParameterEditor(size_t idx, hh::needle::ParameterValueBool* data);
	void RenderIntParameterEditor(size_t idx, hh::needle::ParameterValueInt* data);
	void RenderSamplerParameterEditor(size_t idx, hh::needle::ParameterValueSampler* data);
	void RenderRsFlagMaskParameterEditor(size_t idx, hh::needle::ParameterValueRsFlagMask* data);
	void RenderShaderNameParameterEditor(size_t idx, hh::needle::ParameterValueShaderName* data);
	void RenderTextureNameParameterEditor(size_t idx, hh::needle::ParameterValueTextureName* data);
	void RenderUnimplementedTypeParameterEditor(size_t idx);
};