#pragma once
#include <ui/common/StandaloneWindow.h>
#include <imtimeline.h>
#include "NodeEditor.h"
#include "ASMInterface.h"

namespace ui::operation_modes::modes::asm_editor {
	class BlendTreeEditor : public StandaloneWindow {
	public:
		ASMInterface asmInterface;
		hh::fnd::Reference<hh::anim::BlendNodeBase> focusedRootBlendNode{};
		short focusedRootBlendNodeIndex{ -1 };
		NodeEditor nodeEditor{ GetAllocator(), asmInterface };
		bool collapseBlendSpaceNodes{ true };
		const char* errMsg{};
		ImTimeline::ImTimelineContext* timelineCtx;
		ImVec2 clickPos{};
		OutputPinId createInPin{};
		InputPinId createOutPin{};
		NodeId ctxNodeId{};
		LinkId ctxLinkId{};

		BlendTreeEditor(csl::fnd::IAllocator* allocator, hh::anim::GOCAnimator* gocAnimator, hh::anim::BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex);
		virtual ~BlendTreeEditor();
		virtual void RenderContents() override;

	private:
		void RenderNodes();
		void RenderVariable(short variableId);
		void RenderBlendMask(short blendMaskId);
		void RenderClip(short clipId);
		void RenderFlag(short flagId);
		void RenderState(short stateId);
		void RenderBlendSpace(short blendSpaceId);
		void RenderNode(hh::anim::BlendNodeBase* node, short nodeId);

		void RenderTimeline(ASMInterface::Id clipId);

		void ShowError(const char* msg);

		void HandleCreate();

		void RenderPopups();
		void RenderErrorPopup();
		void RenderBackgroundContextMenu();
		void RenderNodeContextMenu();
		void RenderLinkContextMenu();
		void RenderCreateInputContextMenu();
		void RenderCreateOutputContextMenu();

		bool IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, PinType inPinType, NodeType outNodeType, PinType outPinType);
		bool IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, NodeType outNodeType, PinType pinType);
		void AddLink(OutputPinId inPin, InputPinId outPin);
		void RemoveLink(OutputPinId inPin, InputPinId outPin);
		NodeId AddNode(NodeType type);
		void RemoveNode(NodeId node);
		void HandleAddNode(NodeType nodeType);
		void HandleAddNodeWithTarget(NodeType nodeType, InputPinId target);
		void HandleAddNodeFromTarget(InputPinId target);
	};
}
