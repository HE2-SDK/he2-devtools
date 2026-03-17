#pragma once
#include <imgui_node_editor.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"
#include <ui/common/NodeEditorAutoLayout.h>
#include "NodeEditor.h"

namespace ui::operation_modes::modes::asm_editor
{
	class ASMEditor : public OperationMode<Context>
	{
		ASMInterface asmInterface;
		hh::fnd::Reference<NodeEditor> nodeEditor{};
		csl::ut::MoveArray<float> outputPinTextWidths{ GetAllocator() };
		bool initialized{};
		NodeId ctxNodeId{};

	public:
		using OperationMode<Context>::OperationMode;

		virtual void RenderScene() override;
		void SetGOCAnimator(hh::anim::GOCAnimator* gocAnimator);

	private:

		void RenderNodes();
		void RenderTransitions();
		void RenderFlow();
	};
}
