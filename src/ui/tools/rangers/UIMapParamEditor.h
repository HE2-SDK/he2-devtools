#pragma once
#include <ui/common/StandaloneWindow.h>

class UIMapParamEditor : public StandaloneWindow {
	enum class Island {
		KRONOS,
		ARES,
		CHAOS,
		RHEA,
		OURANOS,
		OURANOS_FH,
	};

	hh::fnd::Reference<hh::fnd::ResReflectionT<heur::rfl::UIMapParameter>> resource{};
	hh::fnd::Reference<hh::gfnd::ResTexture> textureResource{};
	Island islandId{};
	int currentChallengeIdx{};
	float opacity{ 1.0f };

public:
	UIMapParamEditor(csl::fnd::IAllocator* allocator);
	~UIMapParamEditor();
	virtual void PreRender() override;
	virtual void RenderContents() override;
};
