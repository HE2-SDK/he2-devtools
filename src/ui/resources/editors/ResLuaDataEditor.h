#pragma once
#include <ui/common/StandaloneWindow.h>

class ResLuaDataEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResLuaData> resource;
public:
	ResLuaDataEditor(csl::fnd::IAllocator* allocator, hh::fnd::ResLuaData* resource);
	virtual void RenderContents();
	static ResLuaDataEditor* Create(csl::fnd::IAllocator* allocator, hh::fnd::ResLuaData* resource);
};
