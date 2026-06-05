#pragma once
#include <ui/common/StandaloneWindow.h>
#include <reflection/RflDiff.h>

class RflComparer : public StandaloneWindow {
	hh::fnd::Reference<hh::fnd::ResReflection> resource1{};
	hh::fnd::Reference<hh::fnd::ResReflection> resource2{};
	const hh::fnd::RflClass* rflClass{};
	char searchStr[200]{ "" };
	RflDiffResult diffResult{ GetAllocator() };
public:
	RflComparer(csl::fnd::IAllocator* allocator);
	//virtual void PreRender() override;
	virtual void RenderContents() override;
};
