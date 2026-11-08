#include "NodeEditor.h"
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	ImVec4 NodeEditorInterface::GetColor(PinType type)
	{
		switch (type) {
		case PinType::DEFAULT_TRANSITION: return { 1.0f, 1.0f, 1.0f, 1.0f };
		//case PinType::TRANSITION: return { 0.0f, 0.8f, 0.8f, 1.0f };
		case PinType::TRANSITION: return { 0.0f, 0.5f, 0.8f, 1.0f };
		case PinType::EVENT: return { 0.965f, 0.243f, 0.235f, 1.0f };
		case PinType::VARIABLE: return { 0.165f, 0.886f, 0.682f, 1.0f };
		case PinType::BLEND_NODE: return { 0.886f, 0.322f, 0.665f, 1.0f };
		case PinType::BLEND_MASK: return { 0.886f, 0.682f, 0.165f, 1.0f };
		case PinType::CLIP: return { 0.58f, 0.788f, 0.0f, 1.0f };
		case PinType::BLEND_SPACE: return { 0.651f, 0.325f, 0.0f, 1.0f };
		//case PinType::FLAG: return { 0.545f, 0.0f, 0.1f, 1.0f };
		//case PinType::FLAG: return { 0.847f, 0.788f, 0.608f, 1.0f };
		case PinType::FLAG: return { 1.0f, 0.49f, 0.0f, 1.0f };
		default: assert(false); return { 0.0f, 0.0f, 0.0f, 0.0f }; break;
		}
	}

	void NodeEditorInterface::RenderPinIcon(PinType type) {
		auto size = ImGui::GetTextLineHeight();
		auto color = ImGui::GetColorU32(GetColor(type));

		ImGui::Dummy({ size, size });

		auto itemRectMin = ImGui::GetItemRectMin();
		auto itemRectMax = ImGui::GetItemRectMax();

		switch (type) {
		case PinType::DEFAULT_TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		case PinType::TRANSITION: {
			ImVec2 arrowHead[] = { { itemRectMin.x, itemRectMin.y }, { itemRectMin.x + size / 2, itemRectMin.y }, { itemRectMin.x + size, itemRectMin.y + size / 2 }, { itemRectMin.x + size / 2, itemRectMin.y + size }, { itemRectMin.x, itemRectMin.y + size } };
			ImGui::GetWindowDrawList()->AddConvexPolyFilled(arrowHead, 5, color);
			break;
		}
		case PinType::EVENT: {
			ImGui::GetWindowDrawList()->AddRectFilled(itemRectMin, itemRectMax, color);
			break;
		}
		case PinType::VARIABLE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_NODE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_MASK: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::CLIP: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::BLEND_SPACE: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		case PinType::FLAG: {
			ImGui::GetWindowDrawList()->AddCircleFilled(itemRectMin + ImVec2{ size / 2, size / 2 }, size / 2, color);
			break;
		}
		default: {
			assert(false);
			break;
		}
		}
	}

	NodeEditor::NodeEditor(csl::fnd::IAllocator* allocator, ASMInterface& asmInterface) : CompatibleObject{ allocator }, asmInterface{ asmInterface }, nodeEditor{ allocator } {}

	void NodeEditor::BeginContext() {
		nodeEditor.BeginContext();
	}

	void NodeEditor::EndContext() {
		nodeEditor.EndContext();
	}

	void NodeEditor::Begin() {
		ImPlot::PushColormap(ImPlotColormap_Deep);
		nodeEditor.Begin();
	}

	void NodeEditor::End() {
		nodeEditor.End();
		ImPlot::PopColormap();
	}

	void NodeEditor::RunAutoLayout()
	{
		nodeEditor.RunAutoLayout();
	}

	void NodeEditor::State(short stateIdx) {
		auto& state = asmInterface.asmData.states[stateIdx];
		auto color = CalculateActiveStateColor(stateIdx);
		auto progress = CalculateActiveStateProgress(stateIdx);
		NodeId nodeId{ NodeType::STATE, asmInterface.stateRegistry.GetId(stateIdx) };

		float maxTextWidth = 0.0f;
		for (unsigned short j = 0; j < state.eventCount; j++) {
			auto& event = asmInterface.asmData.events[state.eventOffset + j];
			maxTextWidth = std::fmaxf(maxTextWidth, ImGui::CalcTextSize(event.name).x);
		}

		ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, color);

		BeginNode(nodeId, nullptr, maxTextWidth, false);

		nodeEditor.BeginInputPins();
		InputPin({ nodeId, PinType::DEFAULT_TRANSITION, 0 });
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f * ImGui::GetFontSize() / 14.0f, 0.0f }, state.name);
			Editor("Loop count", state.maxCycles);
			CheckboxFlags("Loop", state.flags, StateData::Flag::LOOPS);
			CheckboxFlags("Unknown 1", state.flags, StateData::Flag::UNK1);
			CheckboxFlags("Use PBA", state.flags, StateData::Flag::USE_PBA);

#ifdef DEVTOOLS_TARGET_SDK_miller
			CheckboxFlags("Use PBA blend factor", state.flags, StateData::Flag::USE_PBA_BLEND_FACTOR);

			float zero = 0.0f;
			float one = 1.0f;
			if (state.flags.test(StateData::Flag::USE_PBA_BLEND_FACTOR))
				SliderScalar("PBA blend factor", state.pbaBlendFactor, &zero, &one);
#endif
		}
		else {
			ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f * ImGui::GetFontSize() / 14.0f, 0.0f }, state.name);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::DEFAULT_TRANSITION, 0 });
		OutputPin({ nodeId, PinType::TRANSITION, 0 });
		for (unsigned short j = 0; j < state.eventCount; j++)
			OutputPin({ nodeId, PinType::EVENT, j }, asmInterface.asmData.events[state.eventOffset + j].name);
		nodeEditor.EndOutputPins();

		EndNode();

		ax::NodeEditor::PopStyleColor();
	}

	static const char* stateTypeNames[]{ "NULL", "CLIP", "BLEND_TREE", "NONE" };
	void NodeEditor::BlendTreeState(short stateIdx)
	{
		auto& state = asmInterface.asmData.states[stateIdx];
		auto color = CalculateActiveStateColor(stateIdx);
		auto progress = CalculateActiveStateProgress(stateIdx);
		NodeId nodeId{ NodeType::STATE, asmInterface.stateRegistry.GetId(stateIdx) };

		ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, color);

		BeginNode(nodeId, "State", 0.0f, false);

		nodeEditor.BeginInputPins();
		if (state.type == StateType::CLIP)
			InputPin({ nodeId, PinType::CLIP, 0 });
		else if (state.type == StateType::BLEND_TREE)
			InputPin({ nodeId, PinType::BLEND_NODE, 0 });
		if (BeginInputPin({ nodeId, PinType::VARIABLE, 0 })) {
			Editor("Speed", state.speed);
			EndInputPin();
		}
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f * ImGui::GetFontSize() / 14.0f, 0.0f }, state.name);
			if (ImGui::BeginCombo("Type", stateTypeNames[static_cast<char>(state.type) + 1])) {
				for (char i = 0; i < 4; i++) {
					if (ImGui::Selectable(stateTypeNames[i], static_cast<char>(state.type) == i - 1)) {
						state.type = static_cast<StateType>(i - 1);
						state.rootBlendNodeOrClipIndex = -1;
					}

					if (static_cast<char>(state.type) == i - 1)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			Editor("Loop count", state.maxCycles);
			CheckboxFlags("Loop", state.flags, StateData::Flag::LOOPS);
			CheckboxFlags("Unknown 1", state.flags, StateData::Flag::UNK1);
			CheckboxFlags("Use PBA", state.flags, StateData::Flag::USE_PBA);

#ifdef DEVTOOLS_TARGET_SDK_miller
			CheckboxFlags("Use PBA blend factor", state.flags, StateData::Flag::USE_PBA_BLEND_FACTOR);

			float zero = 0.0f;
			float one = 1.0f;
			if (state.flags.test(StateData::Flag::USE_PBA_BLEND_FACTOR))
				SliderScalar("PBA blend factor", state.pbaBlendFactor, &zero, &one);
#endif
		}
		else {
			ImGui::ProgressBar(progress, { ImGui::CalcTextSize(state.name).x + 10.0f * ImGui::GetFontSize() / 14.0f, 0.0f }, state.name);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::FLAG, 0 });
		nodeEditor.EndOutputPins();

		EndNode();

		ax::NodeEditor::PopStyleColor();
	}

	void NodeEditor::StateTransition(short prevStateId, short nextStateId)
	{
		Link({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::TRANSITION, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateDefaultTransition(short prevStateId, short nextStateId)
	{
		LinkWithLayout({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::DEFAULT_TRANSITION, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateEventTransition(short prevStateId, short nextStateId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::EVENT, idx }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateTransitionFlow(short prevStateId, short nextStateId)
	{
		Flow({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::TRANSITION, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateDefaultTransitionFlow(short prevStateId, short nextStateId)
	{
		Flow({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::DEFAULT_TRANSITION, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateEventTransitionFlow(short prevStateId, short nextStateId, unsigned short idx)
	{
		Flow({ { NodeType::STATE, asmInterface.stateRegistry.GetId(prevStateId) }, PinType::EVENT, idx }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(nextStateId) }, PinType::DEFAULT_TRANSITION, 0 });
	}

	void NodeEditor::StateTransitionFlowAuto(short prevStateId, short nextStateId)
	{
		auto& state = asmInterface.GetState(prevStateId);

		if (state.stateEndTransition.transitionInfo.targetStateIndex == nextStateId) {
			StateDefaultTransitionFlow(prevStateId, nextStateId);
			return;
		}

		for (unsigned short j = 0; j < state.eventCount; j++) {
			auto& event = asmInterface.asmData.events[state.eventOffset + j];

			if (event.transition.transitionInfo.targetStateIndex == nextStateId) {
				StateEventTransitionFlow(prevStateId, nextStateId, j);
				return;
			}
		}

		if (state.transitionArrayIndex == -1 || !IsStateSelected(prevStateId))
			StateTransition(prevStateId, nextStateId);

		StateTransitionFlow(prevStateId, nextStateId);
	}

	void NodeEditor::StateActiveTransitionFlow()
	{
		for (auto l : GetActiveLayers()) {
			if (l.prevState && l.nextState && l.prevState != l.nextState) {
				short prevIdx = static_cast<short>(l.prevState->stateData - asmInterface.asmData.states);
				short nextIdx = static_cast<short>(l.nextState->stateData - asmInterface.asmData.states);

				StateTransitionFlowAuto(prevIdx, nextIdx);
			}
		}
	}

	void NodeEditor::StateClip(short clipId, short stateId)
	{
		LinkWithLayout({ { NodeType::CLIP, asmInterface.clipRegistry.GetId(clipId) }, PinType::CLIP, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(stateId) }, PinType::CLIP, 0 });
	}

	void NodeEditor::StateBlendNode(short blendNodeId, short stateId)
	{
		LinkWithLayout({ { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeId) }, PinType::BLEND_NODE, 0 }, { { NodeType::STATE, asmInterface.stateRegistry.GetId(stateId) }, PinType::BLEND_NODE, 0 });
	}

	void NodeEditor::Variable(short variableIdx)
	{
		NodeId nodeId{ NodeType::VARIABLE, asmInterface.variableRegistry.GetId(variableIdx) };

		BeginNode(nodeId, "Variable", 10.0f * ImGui::GetFontSize() / 14.0f);

		nodeEditor.BeginInputPins();
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			Viewer("Name", asmInterface.asmData.variables[variableIdx]);
			if (asmInterface.gocAnimator)
				Viewer("Value", asmInterface.gocAnimator->animationStateMachine->variables[variableIdx].bindables.collectionFloat);
		}
		else {
			ImGui::Text("%s", asmInterface.asmData.variables[variableIdx]);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::VARIABLE, 0 });
		nodeEditor.EndOutputPins();

		EndNode();
	}

	void NodeEditor::BlendNodeVariable(short variableId, short blendNodeId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::VARIABLE, asmInterface.variableRegistry.GetId(variableId) }, PinType::VARIABLE, 0 }, { { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeId) }, PinType::VARIABLE, idx });
	}

	void NodeEditor::BlendSpaceVariable(short variableId, short blendSpaceId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::VARIABLE, asmInterface.variableRegistry.GetId(variableId) }, PinType::VARIABLE, 0 }, { { NodeType::BLEND_SPACE, asmInterface.blendSpaceRegistry.GetId(blendSpaceId) }, PinType::VARIABLE, idx });
	}

	void NodeEditor::Clip(short clipIdx)
	{
		bool needsReload{};
		auto& clipData = asmInterface.asmData.clips[clipIdx];

		NodeId nodeId{ NodeType::CLIP, asmInterface.clipRegistry.GetId(clipIdx) };

		BeginNode(nodeId, "Clip", 10.0f * ImGui::GetFontSize() / 14.0f, false);

		nodeEditor.BeginInputPins();
		InputBlendMaskPin(nodeId, 0, "Blend mask", clipData.blendMaskIndex);
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			InputText("Name", clipData.name, asmInterface.resource);
			needsReload |= InputText("Resource", clipData.animationSettings.resourceName, asmInterface.resource);
			needsReload |= Editor("Start", clipData.animationSettings.start);
			needsReload |= Editor("End", clipData.animationSettings.end);
			needsReload |= Editor("Speed", clipData.animationSettings.speed);
			needsReload |= Editor("Loop", clipData.animationSettings.loops);
			needsReload |= CheckboxFlags("Mirror", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::MIRROR);
			needsReload |= CheckboxFlags("Play until end", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::PLAY_UNTIL_ANIMATION_END);
			needsReload |= CheckboxFlags("No resource resolution", clipData.animationSettings.flags, ClipData::AnimationSettings::Flag::NO_ANIMATION_RESOLUTION);
		}
		else {
			ImGui::Text("%s", clipData.name);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::CLIP, 0 });
		nodeEditor.EndOutputPins();

		EndNode();

		if (needsReload)
			asmInterface.ReloadResource();
	}

	void NodeEditor::BlendNodeClip(short clipId, short blendNodeId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::CLIP, asmInterface.clipRegistry.GetId(clipId) }, PinType::CLIP, 0 }, { { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeId) }, PinType::CLIP, idx });
	}

	void NodeEditor::BlendSpaceClip(short clipId, short blendSpaceId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::CLIP, asmInterface.clipRegistry.GetId(clipId) }, PinType::CLIP, 0 }, { { NodeType::BLEND_SPACE, asmInterface.blendSpaceRegistry.GetId(blendSpaceId) }, PinType::CLIP, idx });
	}

	void NodeEditor::BlendSpace(short blendSpaceIdx)
	{
		auto& blendSpace = asmInterface.asmData.blendSpaces[blendSpaceIdx];

		NodeId nodeId{ NodeType::BLEND_SPACE, asmInterface.blendSpaceRegistry.GetId(blendSpaceIdx) };

		BeginNode(nodeId, "BlendSpace", 10.0f * ImGui::GetFontSize() / 14.0f);

		nodeEditor.BeginInputPins();
		for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
			InputClipPin(nodeId, i, "Clip", blendSpace.clipIndices[i]);
		BlendSpaceVariablePins(nodeId, blendSpaceIdx, 0);
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			BlendSpaceControls(blendSpaceIdx);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::BLEND_SPACE, 0 });
		nodeEditor.EndOutputPins();

		EndNode();
	}

	void NodeEditor::BlendNodeBlendSpace(short blendSpaceId, short blendNodeId)
	{
		LinkWithLayout({ { NodeType::BLEND_SPACE, asmInterface.blendSpaceRegistry.GetId(blendSpaceId) }, PinType::BLEND_SPACE, 0 }, { { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeId) }, PinType::BLEND_SPACE, 0 });
	}

	void NodeEditor::BlendMask(short blendMaskIdx)
	{
		auto& blendMask = asmInterface.asmData.blendMasks[blendMaskIdx];

		NodeId nodeId{ NodeType::BLEND_MASK, asmInterface.blendMaskRegistry.GetId(blendMaskIdx) };

		BeginNode(nodeId, blendMask.name, 10.0f * ImGui::GetFontSize() / 14.0f);

		nodeEditor.BeginInputPins();
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			InputText("Name", blendMask.name, asmInterface.resource);
			ImGui::Text("Bones:");
			for (unsigned short i = 0; i < blendMask.maskBoneCount; i++)
				ImGui::Text("%s", asmInterface.asmData.maskBones[blendMask.maskBoneOffset + i]);
		}
		else {
			ImGui::Text("%s", blendMask.name);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		OutputPin({ nodeId, PinType::BLEND_MASK, 0 });
		nodeEditor.EndOutputPins();

		EndNode();
	}

	void NodeEditor::ClipBlendMask(short blendMaskId, short clipId)
	{
		LinkWithLayout({ { NodeType::BLEND_MASK, asmInterface.blendMaskRegistry.GetId(blendMaskId) }, PinType::BLEND_MASK, 0 }, { { NodeType::CLIP, asmInterface.clipRegistry.GetId(clipId) }, PinType::BLEND_MASK, 0 });
	}

	void NodeEditor::LerpBlendNode(short blendNodeIdx, hh::anim::LerpBlendNode* liveNode)
	{
		BranchBlendNode(blendNodeIdx, "Lerp", [=]() {
			auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

			BaseBlendNodeControls(blendNodeIdx);

			if (nodeData.childNodeArraySize < 2)
				return;

			auto childNodes = &asmInterface.asmData.blendNodes[nodeData.childNodeArrayOffset];

			if (ImPlot::BeginPlot("Lerp", { 300.0f * ImGui::GetFontSize() / 14.0f, 200.0f * ImGui::GetFontSize() / 14.0f })) {
				ImPlot::SetupAxes("Blend factor", "Contribution", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

				for (unsigned short i = 0; i < nodeData.childNodeArraySize; i++) {
					std::pair<unsigned short, BlendNodeData*> plotInfo{ i, childNodes };
					char name[100];
					snprintf(name, sizeof(name), "%d", i);

					ImPlot::PlotLineG(name, [](auto idx, void* userData) -> ImPlotPoint {
						auto* pInfo = static_cast<std::pair<unsigned short, BlendNodeData*>*>(userData);

						return { pInfo->second[idx].blendFactorTarget, pInfo->first == idx ? 1.0f : 0.0f };
						}, &plotInfo, nodeData.childNodeArraySize);
				}

				if (asmInterface.gocAnimator && nodeData.blendFactorVariableIndex != -1) {
					double cur = asmInterface.gocAnimator->animationStateMachine->variables[nodeData.blendFactorVariableIndex].bindables.collectionFloat;
					double values[]{ cur };
					ImPlot::PlotInfLines("Current", values, 1);
				}

				ImPlot::EndPlot();
			}
		});
	}

	void NodeEditor::AdditiveBlendNode(short blendNodeIdx, hh::anim::AdditiveBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeIdx, "Add");
	}

	void NodeEditor::ClipNode(short blendNodeIdx, hh::anim::ClipNode* liveNode)
	{
		BlendNode(
			blendNodeIdx,
			"Clip",
			[=]() {
				NodeId nodeId{ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) };
				auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

				BaseBlendNodeInputs(blendNodeIdx);

				InputClipPin(nodeId, 0, "Clip", nodeData.childNodeArrayOffset);
			},
			[=]() { BaseBlendNodeControls(blendNodeIdx); }
		);
	}

	void NodeEditor::OverrideBlendNode(short blendNodeIdx, hh::anim::OverrideBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeIdx, "Override");
	}

	void NodeEditor::LayerBlendNode(short blendNodeIdx, hh::anim::LayerBlendNode* liveNode)
	{
		SimpleBlendNode(blendNodeIdx, "Layer");
	}

	void NodeEditor::MulBlendNode(short blendNodeIdx, hh::anim::MulBlendNode* liveNode)
	{
		SimpleBranchBlendNode(blendNodeIdx, "Mul");
	}

	void NodeEditor::BlendSpaceNode(short blendNodeIdx, hh::anim::BlendSpaceNode* liveNode)
	{
		BlendNode(
			blendNodeIdx,
			"BlendSpace",
			[=]() {
				NodeId nodeId{ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) };
				auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

				BaseBlendNodeInputs(blendNodeIdx);

				InputPin({ nodeId, PinType::BLEND_SPACE, 0 }, "Blend space");

				if (nodeData.blendSpaceIndex != -1) {
					auto& blendSpace = asmInterface.asmData.blendSpaces[nodeData.blendSpaceIndex];

					for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
						InputPin({ nodeId, PinType::BLEND_NODE, i }, blendSpace.clipIndices[i] == -1 ? "<none>" : asmInterface.asmData.clips[blendSpace.clipIndices[i]].name);
				}
			},
			[=]() { BaseBlendNodeControls(blendNodeIdx); }
		);
	}

	void NodeEditor::CollapsedBlendSpaceNode(short blendNodeIdx, hh::anim::BlendSpaceNode* liveNode)
	{
		BlendNode(
			blendNodeIdx,
			"BlendSpace",
			[=]() {
				NodeId nodeId{ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) };
				auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

				BaseBlendNodeInputs(blendNodeIdx);

				if (nodeData.blendSpaceIndex != -1) {
					BlendSpaceVariablePins(nodeId, nodeData.blendSpaceIndex, 1);

					auto& blendSpace = asmInterface.asmData.blendSpaces[nodeData.blendSpaceIndex];

					for (unsigned short i = 0; i < blendSpace.nodeCount; i++)
						InputPin({ nodeId, PinType::CLIP, i }, blendSpace.clipIndices[i] == -1 ? "<none>" : asmInterface.asmData.clips[blendSpace.clipIndices[i]].name);
				}
			},
			[=]() {
				BaseBlendNodeControls(blendNodeIdx);
				if (asmInterface.asmData.blendNodes[blendNodeIdx].blendSpaceIndex != -1)
					BlendSpaceControls(asmInterface.asmData.blendNodes[blendNodeIdx].blendSpaceIndex);
				else {
					ImGui::Text("Missing blend space. Disable collapsing of blend space nodes and assign a blend space, or click the button below to create one.");
					if (ImGui::Button("Create blend space")) {
						auto blendSpaceId = asmInterface.AddBlendSpace();

						asmInterface.SetBlendNodeBlendSpace(asmInterface.blendNodeRegistry.GetId(blendNodeIdx), blendSpaceId);
					}
				}
			}
		);
	}

	void NodeEditor::TwoPointLerpBlendNode(short blendNodeIdx, hh::anim::TwoPointLerpBlendNode* liveNode)
	{
		SimpleBlendNode(blendNodeIdx, "Two Point Lerp");
	}

	void NodeEditor::SimpleBlendNode(short blendNodeIdx, const char* name)
	{
		BlendNode(
			blendNodeIdx,
			name,
			[=]() { BaseBlendNodeInputs(blendNodeIdx); },
			[=]() { BaseBlendNodeControls(blendNodeIdx); }
		);
	}

	void NodeEditor::SimpleBranchBlendNode(short blendNodeIdx, const char* name)
	{
		BranchBlendNode(
			blendNodeIdx,
			name,
			[=]() { BaseBlendNodeControls(blendNodeIdx); }
		);
	}

	void NodeEditor::BaseBlendNodeInputs(short blendNodeIdx)
	{
		auto& nodeData = asmInterface.asmData.blendNodes[blendNodeIdx];

		InputVariablePin({ NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeIdx) }, 0, "Blend factor", nodeData.blendFactorVariableIndex);
	}

	static const char* blendNodeTypeNames[]{
		"LERP",
		"ADDITIVE",
		"CLIP",
		"OVERRIDE",
		"LAYER",
		"MULTIPLY",
		"BLEND_SPACE",
		"TWO_POINT_LERP",
	};
	void NodeEditor::BaseBlendNodeControls(short blendNodeId)
	{
		auto& nodeData = asmInterface.asmData.blendNodes[blendNodeId];

		auto type = nodeData.type;
		if (ComboEnum("Type", type, blendNodeTypeNames)) {
			asmInterface.SetBlendNodeType(asmInterface.blendNodeRegistry.GetId(blendNodeId), type);
			asmInterface.ReloadResource();
		}
		Editor("Blend factor target", nodeData.blendFactorTarget);
	}

	void NodeEditor::BlendNodeChildRelationship(short childNodeId, short parentNodeId, unsigned short idx)
	{
		LinkWithLayout({ { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(childNodeId) }, PinType::BLEND_NODE, 0 }, { { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(parentNodeId) }, PinType::BLEND_NODE, idx });
	}

	void NodeEditor::Flag(short flagIdx)
	{
		NodeId nodeId{ NodeType::FLAG, asmInterface.flagRegistry.GetId(flagIdx) };

		BeginNode(nodeId, "Flag", 10.0f * ImGui::GetFontSize() / 14.0f, false);

		nodeEditor.BeginInputPins();
		InputPin({ nodeId, PinType::FLAG, 0 });
		nodeEditor.EndInputPins();

		if (BeginControls()) {
			InputText("Name", asmInterface.asmData.flags[flagIdx], asmInterface.resource);
		}
		else {
			ImGui::Text("%s", asmInterface.asmData.flags[flagIdx]);
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		nodeEditor.EndOutputPins();

		EndNode();
	}

	void NodeEditor::StateFlag(short flagId, short stateId)
	{
		LinkWithLayout({ { NodeType::STATE, asmInterface.stateRegistry.GetId(stateId) }, PinType::FLAG, 0 }, { { NodeType::FLAG, asmInterface.flagRegistry.GetId(flagId) }, PinType::FLAG, 0 });
	}

	void NodeEditor::LayerBlendTreeOutput(short blendNodeId)
	{
		NodeId nodeId{ NodeType::LAYER_BLEND_TREE_OUTPUT, 0 };
		InputPinId nextPin{ nodeId, PinType::BLEND_NODE, 0 };

		BeginNode(nodeId, "Layer blend tree output", 10.0f * ImGui::GetFontSize() / 14.0f);

		nodeEditor.BeginInputPins();
		InputPin(nextPin, "Blend node");
		nodeEditor.EndInputPins();

		if (BeginControls()) {
		}
		EndControls();

		nodeEditor.BeginOutputPins();
		nodeEditor.EndOutputPins();

		EndNode();

		LinkWithLayout({ { NodeType::BLEND_NODE, asmInterface.blendNodeRegistry.GetId(blendNodeId) }, PinType::BLEND_NODE, 0 }, nextPin);
	}

	bool NodeEditor::IsStateSelected(short id)
	{
		return ax::NodeEditor::IsNodeSelected(NodeId{ NodeType::STATE, asmInterface.stateRegistry.GetId(id) });
	}

	bool NodeEditor::ShowBackgroundContextMenu()
	{
		return ax::NodeEditor::ShowBackgroundContextMenu();
	}

	bool NodeEditor::ShowNodeContextMenu(NodeId& nodeId)
	{
		ax::NodeEditor::NodeId axNodeId;
		bool result = ax::NodeEditor::ShowNodeContextMenu(&axNodeId);
		if (result)
			nodeId = axNodeId;
		return result;
	}

	bool NodeEditor::ShowPinContextMenu(PinId& pinId)
	{
		ax::NodeEditor::PinId axPinId;
		bool result = ax::NodeEditor::ShowPinContextMenu(&axPinId);
		if (result)
			pinId = axPinId;
		return result;
	}

	bool NodeEditor::ShowLinkContextMenu(LinkId& linkId)
	{
		ax::NodeEditor::LinkId axLinkId;
		bool result = ax::NodeEditor::ShowLinkContextMenu(&axLinkId);
		if (result)
			linkId = axLinkId;
		return result;
	}

	bool NodeEditor::BeginCreate()
	{
		return ax::NodeEditor::BeginCreate();
	}

	void NodeEditor::EndCreate()
	{
		ax::NodeEditor::EndCreate();
	}

	bool NodeEditor::QueryNewLink(OutputPinId& startPinId, InputPinId& endPinId)
	{
		ax::NodeEditor::PinId startPin, endPin;

		bool result = ax::NodeEditor::QueryNewLink(&startPin, &endPin);

		if (!result || !startPin || !endPin)
			return false;

		PinId iStartPin = startPin;
		PinId iEndPin = endPin;

		if (iStartPin.kind == iEndPin.kind)
			return false;

		startPinId = iStartPin.kind == ax::NodeEditor::PinKind::Output ? OutputPinId{ static_cast<unsigned long long>(iStartPin) } : OutputPinId{ static_cast<unsigned long long>(iEndPin) };
		endPinId = iStartPin.kind == ax::NodeEditor::PinKind::Input ? InputPinId{ static_cast<unsigned long long>(iStartPin) } : InputPinId{ static_cast<unsigned long long>(iEndPin) };

		return true;
	}

	bool NodeEditor::QueryNewInputNode(InputPinId& pinId)
	{
		ax::NodeEditor::PinId pin;

		bool result = ax::NodeEditor::QueryNewNode(&pin);

		if (!result || !pin)
			return false;

		PinId iPin = pin;

		if (iPin.kind != ax::NodeEditor::PinKind::Input)
			return false;

		pinId = InputPinId{ static_cast<unsigned long long>(iPin) };

		return true;
	}

	bool NodeEditor::QueryNewOutputNode(OutputPinId& pinId)
	{
		ax::NodeEditor::PinId pin;

		bool result = ax::NodeEditor::QueryNewNode(&pin);

		if (!result || !pin)
			return false;

		PinId iPin = pin;

		if (iPin.kind != ax::NodeEditor::PinKind::Input)
			return false;

		pinId = OutputPinId{ static_cast<unsigned long long>(iPin) };

		return true;
	}

	ImVec4 NodeEditor::CalculateActiveStateColor(short stateIdx)
	{
		auto& state = asmInterface.asmData.states[stateIdx];

		for (auto l : GetActiveLayers()) {
			auto layerColor = ImPlot::GetColormapColor(l.layer.layerId);

			if (l.nextState && l.nextState->stateData == &state)
				return layerColor;
			else if (l.prevState && l.prevState->stateData == &state)
				return { layerColor.x * 0.7f, layerColor.y * 0.7f, layerColor.z * 0.7f, 1.0f };
		}

		return ax::NodeEditor::GetStyle().Colors[ax::NodeEditor::StyleColor_NodeBorder];
	}

	float NodeEditor::CalculateActiveStateProgress(short stateIdx)
	{
		auto& state = asmInterface.asmData.states[stateIdx];

		for (auto l : GetActiveLayers()) {
			if (l.nextState && l.nextState->stateData == &state)
				return l.nextState->implementation.duration == 0.0f ? 0.0f : l.nextState->implementation.currentTime / l.nextState->implementation.duration;
			else if (l.prevState && l.prevState->stateData == &state)
				return l.prevState->implementation.duration == 0.0f ? 0.0f : l.prevState->implementation.currentTime / l.prevState->implementation.duration;
		}

		return 0.0f;
	}

	void NodeEditor::BeginNode(ax::NodeEditor::NodeId nodeId, const char* title, float maxOutputPinLabelWidth, bool defaultUnfolded)
	{
		ImGui::PushID(reinterpret_cast<void*>(nodeId.Get()));
		auto* stateStrg = ImGui::GetStateStorage();
		currentNodeUnfolded = stateStrg->GetBool(ImGui::GetID("UnfoldedState"), defaultUnfolded);
		currentNodeTitle = title;
		ImGui::PopID();

		nodeEditor.BeginNode(nodeId, currentNodeUnfolded ? maxOutputPinLabelWidth : 0);

		if (title != nullptr) {
			nodeEditor.BeginTitle();
			auto pos = ImGui::GetCursorPos();

			if (ImGui::InvisibleButton("Title Button", ImGui::CalcTextSize(currentNodeTitle))) {
				stateStrg->SetBool(ImGui::GetID("UnfoldedState"), !currentNodeUnfolded);
			}

			ImGui::SetCursorPos(pos);
			ImGui::Text("%s", currentNodeTitle);
			nodeEditor.EndTitle();
		}
	}

	void NodeEditor::EndNode()
	{
		nodeEditor.EndNode();
	}

	bool NodeEditor::BeginControls()
	{
		ImGui::SameLine();
		ImGui::BeginGroup();

		return currentNodeUnfolded;
	}

	void NodeEditor::EndControls()
	{
		ImGui::EndGroup();
	}

	bool NodeEditor::BeginInputPin(const InputPinId& pinId)
	{
		nodeEditor.BeginInputPin(pinId, pinId.type);

		if (!currentNodeUnfolded) {
			nodeEditor.EndInputPin();
			return false;
		}

		return true;
	}

	void NodeEditor::EndInputPin() {
		nodeEditor.EndInputPin();
	}

	bool NodeEditor::BeginOutputPin(const OutputPinId& pinId, float labelWidth)
	{
		nodeEditor.BeginOutputPin(pinId, currentNodeUnfolded ? labelWidth : 0.0f, pinId.type);

		if (!currentNodeUnfolded) {
			nodeEditor.EndOutputPin();
			return false;
		}

		return true;
	}

	void NodeEditor::EndOutputPin() {
		nodeEditor.EndOutputPin();
	}

	void NodeEditor::InputPin(const InputPinId& pinId)
	{
		if (BeginInputPin(pinId)) {
			EndInputPin();
		}
	}

	void NodeEditor::OutputPin(const OutputPinId& pinId)
	{
		if (BeginOutputPin(pinId, 0.0f)) {
			EndOutputPin();
		}
	}

	void NodeEditor::InputVariablePin(const NodeId& nodeId, unsigned short idx, const char* label, short variableIdx)
	{
		if (asmInterface.gocAnimator && variableIdx != -1)
			InputPin({ nodeId, PinType::VARIABLE, idx }, label, "%f", asmInterface.gocAnimator->animationStateMachine->variables[variableIdx].bindables.collectionFloat);
		else
			InputPin({ nodeId, PinType::VARIABLE, idx }, label);
	}

	void NodeEditor::InputClipPin(const NodeId& nodeId, unsigned short idx, const char* label, short clipIdx)
	{
		if (clipIdx != -1)
			InputPin({ nodeId, PinType::CLIP, idx }, label, "%s", asmInterface.asmData.clips[clipIdx].name);
		else
			InputPin({ nodeId, PinType::CLIP, idx }, label);
	}

	void NodeEditor::InputBlendMaskPin(const NodeId& nodeId, unsigned short idx, const char* label, short blendMaskIdx)
	{
		if (blendMaskIdx != -1)
			InputPin({ nodeId, PinType::BLEND_MASK, idx }, label, "%s", asmInterface.asmData.blendMasks[blendMaskIdx].name);
		else
			InputPin({ nodeId, PinType::BLEND_MASK, idx }, label);
	}

	void NodeEditor::Link(const OutputPinId& fromPin, const InputPinId& toPin)
	{
		nodeEditor.Link(LinkId{ fromPin, toPin }, fromPin, toPin, fromPin.type);
	}

	void NodeEditor::LayoutLink(const NodeId& fromNode, const NodeId& toNode)
	{
		nodeEditor.LayoutLink(fromNode, toNode);
	}

	void NodeEditor::LinkWithLayout(const OutputPinId& fromPin, const InputPinId& toPin)
	{
		Link(fromPin, toPin);
		LayoutLink(fromPin.nodeId, toPin.nodeId);
	}

	void NodeEditor::Flow(const OutputPinId& fromPin, const InputPinId& toPin)
	{
		nodeEditor.Flow(LinkId{ fromPin, toPin });
	}

	void NodeEditor::BlendSpaceVariablePins(ax::NodeEditor::NodeId nodeId, short blendSpaceIdx, unsigned short startIdx)
	{
		auto& blendSpace = asmInterface.asmData.blendSpaces[blendSpaceIdx];

		InputVariablePin(nodeId, startIdx + 0, "X", blendSpace.xVariableIndex);
		InputVariablePin(nodeId, startIdx + 1, "Y", blendSpace.yVariableIndex);
	}

	void NodeEditor::BlendSpaceControls(short blendSpaceIdx)
	{
		auto& blendSpace = asmInterface.asmData.blendSpaces[blendSpaceIdx];

		unsigned short clickedNodeIdx{};
		ImPlotPoint clickPos{};
		bool openBgCtxMenu{};
		bool openNodeCtxMenu{};
		bool hov{};
		ImVec2 plotPos{};

		if (ImPlot::BeginPlot("Blend space", { 300.0f * ImGui::GetFontSize() / 14.0f, 300.0f * ImGui::GetFontSize() / 14.0f })) {
			ImPlot::SetupAxes(blendSpace.xVariableIndex == -1 ? "X" : asmInterface.asmData.variables[blendSpace.xVariableIndex], blendSpace.yVariableIndex == -1 ? "Y" : asmInterface.asmData.variables[blendSpace.yVariableIndex], ImPlotAxisFlags_None, ImPlotAxisFlags_None);
			ImPlot::SetupAxesLimits(blendSpace.xMin, blendSpace.xMax, blendSpace.yMin, blendSpace.yMax, ImPlotCond_Always);
			ImPlot::SetupFinish();

			for (unsigned short i = 0; i < blendSpace.nodeCount; i++) {
				auto& node = blendSpace.nodes[i];
				double x = node.x();
				double y = node.y();
				bool pointClicked{};

				if (ImPlot::DragPoint(i, &x, &y, { 1.0f, 1.0f, 1.0f, 1.0f }, 4.0f, 0, &pointClicked)) {
					node = {
						std::min(std::max((float)x, blendSpace.xMin), blendSpace.xMax),
						std::min(std::max((float)y, blendSpace.yMin), blendSpace.yMax)
					};

					asmInterface.TriangulateBlendSpace(asmInterface.blendSpaceRegistry.GetId(blendSpaceIdx));
				}
				ImPlot::Annotation(x, y, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, -5.0f }, true, "%s", blendSpace.clipIndices[i] == -1 ? "" : asmInterface.asmData.clips[blendSpace.clipIndices[i]].name);

				if (pointClicked) {
					clickedNodeIdx = i;
					openNodeCtxMenu = true;
				}
			}

			for (unsigned short i = 0; i < blendSpace.triangleCount; i++) {
				char name[100];
				snprintf(name, sizeof(name), "##%d", i);
				auto& triangle = blendSpace.triangles[i];
				double triangleXs[] = { blendSpace.nodes[triangle.nodeIndices[0]].x(), blendSpace.nodes[triangle.nodeIndices[1]].x(), blendSpace.nodes[triangle.nodeIndices[2]].x() };
				double triangleYs[] = { blendSpace.nodes[triangle.nodeIndices[0]].y(), blendSpace.nodes[triangle.nodeIndices[1]].y(), blendSpace.nodes[triangle.nodeIndices[2]].y() };
				ImPlot::PlotLine(name, triangleXs, triangleYs, 3, ImPlotLineFlags_Loop);
			}

			if (asmInterface.gocAnimator && blendSpace.xVariableIndex != -1 && blendSpace.yVariableIndex != -1) {
				double curX = asmInterface.gocAnimator->animationStateMachine->variables[blendSpace.xVariableIndex].bindables.collectionFloat;
				double curY = asmInterface.gocAnimator->animationStateMachine->variables[blendSpace.yVariableIndex].bindables.collectionFloat;

				ImPlot::PlotScatter("Current", &curX, &curY, 1, ImPlotScatterFlags_None);
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImPlot::IsPlotHovered()) {
				clickPos = ImPlot::GetPlotMousePos();
				openBgCtxMenu = true;
			}

			hov = ImPlot::IsPlotHovered();
			plotPos = ImPlot::GetPlotPos();

			ImPlot::EndPlot();
		}

		// Hacky shit to avoid incompatibilities in node editor...
		auto* ctx = ImGui::GetCurrentContext();
		ctx->HoveredIdAllowOverlap = false;

		if (openBgCtxMenu) {
			ImGui::GetStateStorage()->SetFloat(ImGui::GetID("clickX"), (float)clickPos.x);
			ImGui::GetStateStorage()->SetFloat(ImGui::GetID("clickY"), (float)clickPos.y);
			ImGui::OpenPopup("Background Context Menu");
		}
		if (openNodeCtxMenu) {
			ImGui::GetStateStorage()->SetInt(ImGui::GetID("clickedNodeIdx"), clickedNodeIdx);
			ImGui::OpenPopup("Node Context Menu");
		}

		clickPos = { ImGui::GetStateStorage()->GetFloat(ImGui::GetID("clickX")), ImGui::GetStateStorage()->GetFloat(ImGui::GetID("clickY")) };
		clickedNodeIdx = ImGui::GetStateStorage()->GetInt(ImGui::GetID("clickedNodeIdx"));

		if (ImGui::BeginPopup("Background Context Menu")) {
			if (ImGui::MenuItem("Add clip")) {
				asmInterface.AddBlendSpaceNode(asmInterface.blendSpaceRegistry.GetId(blendSpaceIdx), { clickPos.x, clickPos.y });
				asmInterface.ReloadResource();
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopup("Node Context Menu")) {
			if (ImGui::MenuItem("Remove")) {
				asmInterface.RemoveBlendSpaceNode(asmInterface.blendSpaceRegistry.GetId(blendSpaceIdx), clickedNodeIdx);
				asmInterface.ReloadResource();
			}
			ImGui::EndPopup();
		}

		if (ImGui::TreeNode("Bounds")) {
			Editor("X minimum", blendSpace.xMin);
			Editor("X maximum", blendSpace.xMax);
			Editor("Y minimum", blendSpace.yMin);
			Editor("Y maximum", blendSpace.yMax);
			ImGui::TreePop();
		}
	}

	NodeId::NodeId(NodeType type, ASMInterface::Id id) : type{ type }, id{ id } {}

	NodeId::NodeId(unsigned long long nodeId)
	{
		type = static_cast<NodeType>((nodeId >> 16) & 0xF);
		id = nodeId & 0xFFFF;
	}

	NodeId::NodeId(ax::NodeEditor::NodeId nodeId) : NodeId(nodeId.Get()) {}

	NodeId::operator unsigned long long() const
	{
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lId = id;

		return (lType << 16) | lId;
	}

	NodeId::operator ax::NodeEditor::NodeId() const
	{
		return static_cast<unsigned long long>(*this);
	}

	PinId::PinId(const NodeId& nodeId, ax::NodeEditor::PinKind kind, PinType type, unsigned short idx) : nodeId{ nodeId }, kind{ kind }, type{ type }, idx{ idx } {}

	PinId::PinId(unsigned long long pinId)
	{
		ax::NodeEditor::NodeId lNodeId = (pinId >> 12) & 0xFFFFF;

		nodeId = lNodeId;
		kind = static_cast<ax::NodeEditor::PinKind>((pinId >> 11) & 0x1);
		type = static_cast<PinType>((pinId >> 7) & 0xF);
		idx = pinId & 0x7F;
	}

	PinId::PinId(ax::NodeEditor::PinId pinId) : PinId(pinId.Get()) {}

	PinId::operator unsigned long long() const
	{
		unsigned long long lNodeId = nodeId;
		unsigned long long lKind = static_cast<unsigned long long>(kind);
		unsigned long long lType = static_cast<unsigned long long>(type);
		unsigned long long lIdx = idx;

		return (lNodeId << 12) | (lKind << 11) | (lType << 7) | lIdx;
	}

	PinId::operator ax::NodeEditor::PinId() const
	{
		return static_cast<unsigned long long>(*this);
	}

	InputPinId::InputPinId(const NodeId& nodeId, PinType type, unsigned short idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Input, type, idx } {}
	InputPinId::InputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	InputPinId::InputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	OutputPinId::OutputPinId(const NodeId& nodeId, PinType type, unsigned short idx) : PinId{ nodeId, ax::NodeEditor::PinKind::Output, type, idx } {}
	OutputPinId::OutputPinId(unsigned long long nodeId) : PinId{ nodeId } {}
	OutputPinId::OutputPinId(ax::NodeEditor::PinId nodeId) : PinId{ nodeId } {}

	LinkId::LinkId(const OutputPinId& inputPinId, const InputPinId& outputPinId) : inputPinId{ inputPinId }, outputPinId{ outputPinId } {}

	LinkId::LinkId(unsigned long long nodeId)
	{
		inputPinId = (nodeId >> 32) & 0xFFFFFFFF;
		outputPinId = nodeId & 0xFFFFFFFF;
	}

	LinkId::LinkId(ax::NodeEditor::LinkId nodeId) : LinkId(nodeId.Get()) {}

	LinkId::operator unsigned long long() const
	{
		unsigned long long lInputPinId = inputPinId;
		unsigned long long lOutputPinId = outputPinId;

		return (lInputPinId << 32) | lOutputPinId;
	}

	LinkId::operator ax::NodeEditor::LinkId() const
	{
		return static_cast<unsigned long long>(*this);
	}
}
