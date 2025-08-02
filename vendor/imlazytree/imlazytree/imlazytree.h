#pragma once
#include <imgui.h>

namespace ImLazyTree {
	struct ImLazyTreeContext;
	
	ImLazyTreeContext* CreateContext();
	void DestroyContext(ImLazyTreeContext* context);
	void Begin(ImLazyTreeContext* context);
	void End();
}
