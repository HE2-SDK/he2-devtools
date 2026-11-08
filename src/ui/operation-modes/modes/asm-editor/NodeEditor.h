#pragma once
#include <ui/common/NodeEditor.h>
#include <ranges>
#include <imgui_internal.h>
#include "ASMInterface.h"

namespace ui::operation_modes::modes::asm_editor {
	enum class NodeType {
		STATE,
		BLEND_NODE,
		VARIABLE,
		CLIP,
		LAYER_BLEND_TREE_OUTPUT,
		BLEND_SPACE,
		BLEND_MASK,
		FLAG,
	};

	enum class PinType {
		DEFAULT_TRANSITION,
		TRANSITION,
		EVENT,
		VARIABLE,
		BLEND_NODE,
		BLEND_MASK,
		CLIP,
		BLEND_SPACE,
		FLAG,
	};

	struct NodeId {
		NodeType type{};
		size_t id{};

		NodeId() = default;
		NodeId(NodeType type, ASMInterface::Id id);
		NodeId(unsigned long long nodeId);
		NodeId(ax::NodeEditor::NodeId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::NodeId() const;
	};

	struct PinId {
		NodeId nodeId{};
		ax::NodeEditor::PinKind kind{};
		PinType type{};
		unsigned short idx{};

		PinId() = default;
		PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx);
		PinId(unsigned long long nodeId);
		PinId(ax::NodeEditor::PinId nodeId);
		operator unsigned long long() const;
		operator ax::NodeEditor::PinId() const;
	};

	struct InputPinId : PinId {
		InputPinId() = default;
		InputPinId(const NodeId& nodeId, PinType type, unsigned short idx);
		InputPinId(unsigned long long nodeId);
		InputPinId(ax::NodeEditor::PinId nodeId);
	};

	struct OutputPinId : PinId {
		OutputPinId() = default;
		OutputPinId(const NodeId& nodeId, PinType type, unsigned short idx);
		OutputPinId(unsigned long long nodeId);
		OutputPinId(ax::NodeEditor::PinId nodeId);
	};

	struct LinkId {
		OutputPinId inputPinId{};
		InputPinId outputPinId{};

		LinkId() = default;
		LinkId(const OutputPinId& inputPinId, const InputPinId& outputPinId);
		LinkId(unsigned long long linkId);
		LinkId(ax::NodeEditor::LinkId linkId);
		operator unsigned long long() const;
		operator ax::NodeEditor::LinkId() const;
	};

	class NodeEditorInterface {
	public:
		using PinType = PinType;
		static void RenderPinIcon(PinType type);
		static ImVec4 GetColor(PinType type);
	};

	class RawNodeEditor : public ::NodeEditor<NodeEditorInterface> {
	public:
		using ::NodeEditor<NodeEditorInterface>::NodeEditor;
	};

	class NodeEditor : public CompatibleObject {
		ASMInterface& asmInterface;
		RawNodeEditor nodeEditor;
		bool currentNodeUnfolded{};
		const char* currentNodeTitle{};

	public:
		NodeEditor(csl::fnd::IAllocator* allocator, ASMInterface& asmInterface);

		void BeginContext();
		void EndContext();
		void Begin();
		void End();

		void RunAutoLayout();

		void State(short stateIdx);
		void BlendTreeState(short stateIdx);
		void StateTransition(short prevStateIdx, short nextStateIdx);
		void StateDefaultTransition(short prevStateIdx, short nextStateIdx);
		void StateEventTransition(short prevStateIdx, short nextStateIdx, unsigned short idx);
		void StateTransitionFlow(short prevStateIdx, short nextStateIdx);
		void StateDefaultTransitionFlow(short prevStateIdx, short nextStateIdx);
		void StateEventTransitionFlow(short prevStateIdx, short nextStateIdx, unsigned short idx);
		void StateTransitionFlowAuto(short prevStateIdx, short nextStateIdx);
		void StateActiveTransitionFlow();
		void StateClip(short clipIdx, short stateIdx);
		void StateBlendNode(short blendNodeIdx, short stateIdx);

		void Variable(short variableIdx);
		void BlendNodeVariable(short variableIdx, short blendNodeIdx, unsigned short idx);
		void BlendSpaceVariable(short variableIdx, short blendSpaceIdx, unsigned short idx);

		void Clip(short clipIdx);
		void BlendNodeClip(short clipIdx, short blendNodeIdx, unsigned short idx);
		void BlendSpaceClip(short clipIdx, short blendSpaceIdx, unsigned short idx);

		void BlendSpace(short blendSpaceIdx);
		void BlendNodeBlendSpace(short blendSpaceIdx, short blendNodeIdx);

		void BlendMask(short blendMaskIdx);
		void ClipBlendMask(short blendMaskIdx, short clipIdx);

		void LerpBlendNode(short blendNodeIdx, hh::anim::LerpBlendNode* liveNode);
		void AdditiveBlendNode(short blendNodeIdx, hh::anim::AdditiveBlendNode* liveNode);
		void ClipNode(short blendNodeIdx, hh::anim::ClipNode* liveNode);
		void OverrideBlendNode(short blendNodeIdx, hh::anim::OverrideBlendNode* liveNode);
		void LayerBlendNode(short blendNodeIdx, hh::anim::LayerBlendNode* liveNode);
		void MulBlendNode(short blendNodeIdx, hh::anim::MulBlendNode* liveNode);
		void BlendSpaceNode(short blendNodeIdx, hh::anim::BlendSpaceNode* liveNode);
		void CollapsedBlendSpaceNode(short blendNodeIdx, hh::anim::BlendSpaceNode* liveNode);
		void TwoPointLerpBlendNode(short blendNodeIdx, hh::anim::TwoPointLerpBlendNode* liveNode);
		void BlendNodeChildRelationship(short childNodeIdx, short parentNodeIdx, unsigned short idx);

		void Flag(short flagIdx);
		void StateFlag(short flagIdx, short stateIdx);

		void LayerBlendTreeOutput(short blendNodeIdx);

		bool IsStateSelected(short idx);

		bool ShowBackgroundContextMenu();
		bool ShowNodeContextMenu(NodeId& nodeId);
		bool ShowPinContextMenu(PinId& pinId);
		bool ShowLinkContextMenu(LinkId& linkId);

		bool BeginCreate();
		bool QueryNewLink(OutputPinId& startPinId, InputPinId& endPinId);
		bool QueryNewInputNode(InputPinId& pinId);
		bool QueryNewOutputNode(OutputPinId& pinId);
		void EndCreate();

	private:
		template<typename I, typename C>
		void BlendNode(short blendNodeIdx, const char* name, I renderInputPins, C renderControls) {
			NodeId nodeId{ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) };
			
			char nameBuf[256];
			snprintf(nameBuf, sizeof(nameBuf), "Blend node - %s", name);

			BeginNode(nodeId, nameBuf, 0.0f);

			nodeEditor.BeginInputPins();
			renderInputPins();
			nodeEditor.EndInputPins();

			if (BeginControls()) {
				ImGui::PushItemWidth(100.0f);
				renderControls();
				ImGui::PopItemWidth();
			}
			EndControls();

			nodeEditor.BeginOutputPins();
			OutputPin({ nodeId, PinType::BLEND_NODE, 0 });
			nodeEditor.EndOutputPins();

			EndNode();
		}
		template<typename C>
		void BranchBlendNode(short blendNodeIdx, const char* name, C renderControls) {
			BlendNode(
				blendNodeIdx,
				name,
				[=]() {
					NodeId nodeId{ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) };
					auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

					BaseBlendNodeInputs(blendNodeIdx);

					for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++)
						InputPin({ nodeId, PinType::BLEND_NODE, i }, "Child");
				},
				renderControls
			);
		}
		void SimpleBlendNode(short blendNodeId, const char* name);
		void SimpleBranchBlendNode(short blendNodeId, const char* name);

		void BaseBlendNodeInputs(short blendNodeId);
		void BaseBlendNodeControls(short blendNodeId);

		struct ActiveLayerInfo {
			hh::anim::AnimationStateMachine::LayerInfo& layer;
			hh::anim::AnimationState* prevState;
			hh::anim::AnimationState* nextState;
		};

		auto GetActiveLayers() {
			return std::views::all(asmInterface.gocAnimator->animationStateMachine->layers)
				| std::views::filter([](auto& l) { return l.layerState != nullptr; })
				| std::views::transform([](auto& l) { return ActiveLayerInfo{ l, l.layerState->GetPreviousAnimationState(), l.layerState->GetCurrentAnimationState() }; });
		}

		ImVec4 CalculateActiveStateColor(short stateId);
		float CalculateActiveStateProgress(short stateId);

		void BeginNode(ax::NodeEditor::NodeId nodeId, const char* title, float maxOutputPinLabelWidth, bool defaultUnfolded = true);
		void EndNode();
		bool BeginControls();
		void EndControls();
		bool BeginInputPin(const InputPinId& pinId);
		void EndInputPin();
		bool BeginOutputPin(const OutputPinId& pinId, float labelWidth);
		void EndOutputPin();

		template<typename... Args>
		void InputPin(const InputPinId& pinId, const char* fmt, Args... args) {
			if (BeginInputPin(pinId)) {
				ImGui::Text("%s", fmt, args...);
				EndInputPin();
			}
		}

		template<typename... Args>
		void OutputPin(const OutputPinId& pinId, const char* fmt, Args... args) {
			const char* text, * text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			if (BeginOutputPin(pinId, ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x)) {
				ImGui::Text("%s", text);
				EndOutputPin();
			}
		}

		template<typename... Args>
		void InputPin(const InputPinId& pinId, const char* label, const char* fmt, Args... args) {
			if (BeginInputPin(pinId)) {
				ImGui::LabelText(label, fmt, args...);
				EndInputPin();
			}
		}
		template<typename... Args>
		void OutputPin(const OutputPinId& pinId, const char* label, const char* fmt, Args... args) {
			const char* text, * text_end;
			ImFormatStringToTempBuffer(&text, &text_end, fmt, args...);
			if (BeginOutputPin(pinId, ImGui::CalcTextSize(label).x + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(text).x + ImGui::GetStyle().ItemSpacing.x)) {
				ImGui::LabelText(label, "%s", fmt);
				EndOutputPin();
			}
		}

		void InputPin(const InputPinId& pinId);
		void OutputPin(const OutputPinId& pinId);

		void InputVariablePin(const NodeId& nodeId, unsigned short idx, const char* label, short variableId);
		void InputClipPin(const NodeId& nodeId, unsigned short idx, const char* label, short clipId);
		void InputBlendMaskPin(const NodeId& nodeId, unsigned short idx, const char* label, short blendMaskId);

		void Link(const OutputPinId& fromPin, const InputPinId& toPin);
		void LayoutLink(const NodeId& fromNode, const NodeId& toNode);
		void LinkWithLayout(const OutputPinId& fromPin, const InputPinId& toPin);
		void Flow(const OutputPinId& fromPin, const InputPinId& toPin);

		//void Link(NodeType fromNodeType, unsigned short fromNodeIdx, PinType fromPinType, unsigned short fromPinIdx, NodeType toNodeType, unsigned short toNodeIdx, PinType toPinType, unsigned short toPinIdx);
		//void LayoutLink(NodeType fromNodeType, unsigned short fromNodeIdx, NodeType toNodeType, unsigned short toNodeIdx);
		//void Flow(NodeType fromNodeType, unsigned short fromNodeIdx, PinType fromPinType, unsigned short fromPinIdx, NodeType toNodeType, unsigned short toNodeIdx, PinType toPinType, unsigned short toPinIdx);

		void BlendSpaceVariablePins(ax::NodeEditor::NodeId nodeId, short blendSpaceId, unsigned short startIdx);
		void BlendSpaceControls(short blendSpaceId);
	};
}
