#include "BlendTreeEditor.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/GlobalSettings.h>
#ifdef DEVTOOLS_TARGET_SDK_rangers
#include <ucsl-reflection/reflections/resources/asm/v103-rangers.h>
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
#include <ucsl-reflection/reflections/resources/asm/v103-miller.h>
#endif
#include <rip/binary/containers/binary-file/v2.h>
#include <span>

namespace ui::operation_modes::modes::asm_editor {
	using namespace hh::anim;
	namespace NodeEd = ax::NodeEditor;

	const char* nodeNames[]{ "Lerp", "Add", "Clip", "Override", "Layer", "Multiply", "BlendSpace", "Two Point Lerp" };

	BlendTreeEditor::BlendTreeEditor(csl::fnd::IAllocator* allocator, GOCAnimator* gocAnimator, BlendNodeBase* focusedRootBlendNode, short focusedRootBlendNodeIndex)
		: StandaloneWindow{ allocator }, asmInterface{ allocator, gocAnimator->asmResourceManager->animatorResource, gocAnimator }, focusedRootBlendNode{ focusedRootBlendNode }, focusedRootBlendNodeIndex{ focusedRootBlendNodeIndex }
	{
		char title[400];
		snprintf(title, sizeof(title), "Blend tree @ %016zx", focusedRootBlendNode);
		SetTitle(title);
		timelineCtx = ImTimeline::CreateContext();
	}

	BlendTreeEditor::~BlendTreeEditor() {
		ImTimeline::DestroyContext(timelineCtx);
	}

	void BlendTreeEditor::RenderContents() {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::MenuItem("Export")) {
				IGFD::FileDialogConfig cfg{};
				cfg.path = GlobalSettings::defaultFileDialogDirectory;
				cfg.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
				cfg.userDatas = &asmInterface.asmData;
				ImGuiFileDialog::Instance()->OpenDialog("ResAnimatorExportDialog", "Choose File", ".asm", cfg);
			}
			if (ImGui::MenuItem("Toggle collapse blend space nodes"))
				collapseBlendSpaceNodes = !collapseBlendSpaceNodes;
			if (ImGui::MenuItem("Run auto-layout"))
				nodeEditor.RunAutoLayout();
			ImGui::EndMenuBar();
		}

		if (ImGuiFileDialog::Instance()->Display("ResAnimatorExportDialog", ImGuiWindowFlags_NoCollapse, ImVec2(800, 500))) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
#ifdef DEVTOOLS_TARGET_SDK_rangers
				auto* exportData = static_cast<ucsl::resources::animation_state_machine::v103_rangers::AsmData*>(ImGuiFileDialog::Instance()->GetUserDatas());
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
				auto* exportData = static_cast<ucsl::resources::animation_state_machine::v103_miller::AsmData*>(ImGuiFileDialog::Instance()->GetUserDatas());
#endif

				std::ofstream ofs{ ImGuiFileDialog::Instance()->GetFilePathName(), std::ios::binary };
				rip::binary::containers::binary_file::v2::BinaryFileSerializer<size_t> serializer{ ofs };
				serializer.serialize<he2sdk::ucsl::GameInterface>(*exportData);
			}
			ImGuiFileDialog::Instance()->Close();
		}

		nodeEditor.BeginContext();
		nodeEditor.Begin();

		RenderNodes();
		HandleCreate();
		RenderPopups();

		ax::NodeEditor::NodeId ids[1];
		auto selectedCount = ax::NodeEditor::GetSelectedNodes(ids, 1);

		nodeEditor.End();
		nodeEditor.EndContext();

		if (selectedCount > 0) {
			NodeId selectedNode = ids[0];

			if (selectedNode.type == NodeType::CLIP)
				RenderTimeline(selectedNode.id);
		}
	}

	void BlendTreeEditor::RenderNodes() {
		for (unsigned short i = 0; i < asmInterface.asmData.variableCount; i++)
			RenderVariable(i);
		for (unsigned short i = 0; i < asmInterface.asmData.blendMaskCount; i++)
			RenderBlendMask(i);
		for (unsigned short i = 0; i < asmInterface.asmData.clipCount; i++)
			RenderClip(i);
		for (unsigned short i = 0; i < asmInterface.asmData.flagCount; i++)
			RenderFlag(i);
		if (!collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < asmInterface.asmData.blendSpaceCount; i++)
				RenderBlendSpace(i);
		for (unsigned short i = 0; i < asmInterface.asmData.stateCount; i++)
			RenderState(i);

		// TODO: Game instead selects node 0 if this happens.
		if (focusedRootBlendNodeIndex != -1) {
			RenderNode(focusedRootBlendNode, focusedRootBlendNodeIndex);
			nodeEditor.LayerBlendTreeOutput(focusedRootBlendNodeIndex);
		}
	}

	void BlendTreeEditor::RenderVariable(short variableId) {
		nodeEditor.Variable(variableId);
	}

	void BlendTreeEditor::RenderBlendMask(short blendMaskId) {
		nodeEditor.BlendMask(blendMaskId);
	}

	void BlendTreeEditor::RenderClip(short clipId) {
		auto& clipData = asmInterface.asmData.clips[clipId];

		nodeEditor.Clip(clipId);

		if (clipData.blendMaskIndex != -1)
			nodeEditor.ClipBlendMask(clipData.blendMaskIndex, clipId);
	}

	void BlendTreeEditor::RenderFlag(short flagId) {
		nodeEditor.Flag(flagId);
	}

	void BlendTreeEditor::RenderState(short stateIdx) {
		auto& state = asmInterface.asmData.states[stateIdx];

		nodeEditor.BlendTreeState(stateIdx);

		if (state.rootBlendNodeOrClipIndex != -1) {
			switch (state.type) {
			case StateType::BLEND_TREE:
				RenderNode(nullptr, state.rootBlendNodeOrClipIndex);
				nodeEditor.StateBlendNode(state.rootBlendNodeOrClipIndex, stateIdx);
				break;
			case StateType::CLIP:
				nodeEditor.StateClip(state.rootBlendNodeOrClipIndex, stateIdx);
				break;
			}
		}

		for (unsigned short i = 0; i < state.flagIndexCount; i++)
			nodeEditor.StateFlag(asmInterface.asmData.flagIndices[state.flagIndexOffset + i], stateIdx);
	}

	void BlendTreeEditor::RenderBlendSpace(short blendSpaceId) {
		auto& blendSpaceData = asmInterface.asmData.blendSpaces[blendSpaceId];

		nodeEditor.BlendSpace(blendSpaceId);

		if (blendSpaceData.xVariableIndex != -1)
			nodeEditor.BlendSpaceVariable(blendSpaceData.xVariableIndex, blendSpaceId, 0);

		if (blendSpaceData.yVariableIndex != -1)
			nodeEditor.BlendSpaceVariable(blendSpaceData.yVariableIndex, blendSpaceId, 1);

		for (unsigned short i = 0; i < blendSpaceData.nodeCount; i++)
			nodeEditor.BlendSpaceClip(blendSpaceData.clipIndices[i], blendSpaceId, i);
	}

	void BlendTreeEditor::RenderNode(hh::anim::BlendNodeBase* node, short nodeId) {
		auto& blendNodeData = asmInterface.asmData.blendNodes[nodeId];

		if (blendNodeData.type != BlendNodeType::BLEND_SPACE || !collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
				RenderNode(node == nullptr ? nullptr : node->children[i], blendNodeData.childNodeArrayOffset + i);

		switch (blendNodeData.type) {
		case BlendNodeType::LERP: nodeEditor.LerpBlendNode(nodeId, static_cast<LerpBlendNode*>(node)); break;
		case BlendNodeType::ADDITIVE: nodeEditor.AdditiveBlendNode(nodeId, static_cast<AdditiveBlendNode*>(node)); break;
		case BlendNodeType::CLIP: nodeEditor.ClipNode(nodeId, static_cast<ClipNode*>(node)); break;
		case BlendNodeType::OVERRIDE: nodeEditor.OverrideBlendNode(nodeId, static_cast<OverrideBlendNode*>(node)); break;
		case BlendNodeType::LAYER: nodeEditor.LayerBlendNode(nodeId, static_cast<LayerBlendNode*>(node)); break;
		case BlendNodeType::MULTIPLY: nodeEditor.MulBlendNode(nodeId, static_cast<MulBlendNode*>(node)); break;
		case BlendNodeType::TWO_POINT_LERP: nodeEditor.TwoPointLerpBlendNode(nodeId, static_cast<TwoPointLerpBlendNode*>(node)); break;
		case BlendNodeType::BLEND_SPACE:
			if (!collapseBlendSpaceNodes)
				nodeEditor.BlendSpaceNode(nodeId, static_cast<BlendSpaceNode*>(node));
			else
				nodeEditor.CollapsedBlendSpaceNode(nodeId, static_cast<BlendSpaceNode*>(node));
			break;
		default: assert(false);
		}

		if (blendNodeData.type != BlendNodeType::BLEND_SPACE || !collapseBlendSpaceNodes)
			for (unsigned short i = 0; i < blendNodeData.childNodeArraySize; i++)
				nodeEditor.BlendNodeChildRelationship(blendNodeData.childNodeArrayOffset + i, nodeId, i);

		if (blendNodeData.blendFactorVariableIndex != -1)
			nodeEditor.BlendNodeVariable(blendNodeData.blendFactorVariableIndex, nodeId, 0);

		if (blendNodeData.type == BlendNodeType::CLIP && blendNodeData.childNodeArrayOffset != -1)
			nodeEditor.BlendNodeClip(blendNodeData.childNodeArrayOffset, nodeId, 0);

		if (blendNodeData.type == BlendNodeType::BLEND_SPACE && blendNodeData.blendSpaceIndex != -1) {
			if (!collapseBlendSpaceNodes)
				nodeEditor.BlendNodeBlendSpace(blendNodeData.blendSpaceIndex, nodeId);
			else {
				auto& blendSpaceData = asmInterface.asmData.blendSpaces[blendNodeData.blendSpaceIndex];

				if (blendSpaceData.xVariableIndex != -1)
					nodeEditor.BlendNodeVariable(blendSpaceData.xVariableIndex, nodeId, 1);

				if (blendSpaceData.yVariableIndex != -1)
					nodeEditor.BlendNodeVariable(blendSpaceData.yVariableIndex, nodeId, 2);

				for (unsigned short i = 0; i < blendSpaceData.nodeCount; i++)
					nodeEditor.BlendNodeClip(blendSpaceData.clipIndices[i], nodeId, i);
			}
		}
	}

	void BlendTreeEditor::RenderTimeline(ASMInterface::Id clipId) {
		if (ImGui::Begin("Timeline")) {
			auto& clip = asmInterface.GetClip(clipId);

			float playHeadFrame = 0.0f;
			bool playing{};
			bool currentTimeChanged{};

			ImTimeline::Begin(timelineCtx);
			if (ImTimeline::BeginTimeline("Timeline", &playHeadFrame, 500.0f, 60.0f, &playing, &currentTimeChanged)) {
				for (unsigned int triggerTypeIndex = 0; triggerTypeIndex < asmInterface.asmData.triggerTypeCount; triggerTypeIndex++) {
					if (ImTimeline::BeginTrack(asmInterface.asmData.triggerTypes[triggerTypeIndex])) {
						for (auto& trigger : std::span(asmInterface.asmData.triggers + clip.triggerOffset, clip.triggerCount)) {
							if (trigger.triggerTypeIndex != triggerTypeIndex)
								continue;

							ImGui::PushID(&trigger);
							ImTimeline::Event(trigger.name, &trigger.unknown2);

							if (ImGui::IsItemClicked())
								ImGui::OpenPopup("Editor");

							if (ImGui::BeginPopup("Editor")) {
								Viewer("name", trigger.name);
								Editor("type", reinterpret_cast<uint8_t&>(trigger.type));
								Viewer("trigger type name", trigger.triggerTypeIndex < 0 ? "<none>" : asmInterface.asmData.triggerTypes[trigger.triggerTypeIndex]);
								Viewer("collider", trigger.colliderIndex < 0 ? "<none>" : asmInterface.asmData.colliders[trigger.colliderIndex]);
								Editor("unknown2", trigger.unknown2);
								Editor("unknown3", trigger.unknown3);
								ImGui::EndPopup();
							}
							ImGui::PopID();
						}
						ImTimeline::EndTrack();
					}
				}
				ImTimeline::EndTimeline();
			}
			ImTimeline::End();
		}
		ImGui::End();
	}

	void BlendTreeEditor::ShowError(const char* msg) {
		errMsg = msg;
		ImGui::OpenPopup("ErrorMsg");
	}

	void BlendTreeEditor::HandleCreate() {
		if (nodeEditor.BeginCreate()) {
			OutputPinId inPin;
			InputPinId outPin;

			if (nodeEditor.QueryNewLink(inPin, outPin)) {
				if (ax::NodeEditor::AcceptNewItem()) {
					AddLink(inPin, outPin);
					asmInterface.ReloadResource();
				}
			}
			if (nodeEditor.QueryNewInputNode(outPin)) {
				if (ax::NodeEditor::AcceptNewItem()) {
					clickPos = ax::NodeEditor::ScreenToCanvas(ImGui::GetMousePos());
					createOutPin = outPin;
					ImGui::OpenPopup("CreateInputNode");
				}
			}
		}
		nodeEditor.EndCreate();
	}

	void BlendTreeEditor::RenderPopups() {
		ax::NodeEditor::Suspend();

		if (nodeEditor.ShowBackgroundContextMenu()) {
			clickPos = ax::NodeEditor::ScreenToCanvas(ImGui::GetMousePos());
			ImGui::OpenPopup("BackgroundPopup");
		}
		if (nodeEditor.ShowNodeContextMenu(ctxNodeId))
			ImGui::OpenPopup("Node Context Menu");
		if (nodeEditor.ShowLinkContextMenu(ctxLinkId))
			ImGui::OpenPopup("Link Context Menu");

		RenderErrorPopup();
		RenderBackgroundContextMenu();
		RenderNodeContextMenu();
		RenderLinkContextMenu();
		RenderCreateInputContextMenu();

		ax::NodeEditor::Resume();
	}

	void BlendTreeEditor::RenderErrorPopup() {
		if (ImGui::BeginPopupModal("ErrorMsg")) {
			ImGui::Text("%s", errMsg);

			if (ImGui::Button("OK"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	void BlendTreeEditor::RenderBackgroundContextMenu() {
		if (ImGui::BeginPopup("BackgroundPopup")) {
			if (ImGui::BeginMenu("Add")) {
				if (ImGui::MenuItem("Clip"))
					HandleAddNode(NodeType::CLIP);
				if (ImGui::MenuItem("Blend node"))
					HandleAddNode(NodeType::BLEND_NODE);
				if (ImGui::MenuItem("Blend mask"))
					HandleAddNode(NodeType::BLEND_MASK);
				if (!collapseBlendSpaceNodes && ImGui::MenuItem("Blend space"))
					HandleAddNode(NodeType::BLEND_SPACE);
				if (ImGui::MenuItem("Variable"))
					HandleAddNode(NodeType::VARIABLE);
				if (ImGui::MenuItem("Flag"))
					HandleAddNode(NodeType::FLAG);
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}

	void BlendTreeEditor::RenderNodeContextMenu() {
		if (ImGui::BeginPopup("Node Context Menu")) {
			auto& state = asmInterface.GetState(ctxNodeId.id);

			if (ImGui::MenuItem("Change to"))
				asmInterface.gocAnimator->ChangeState(state.name);
			if (ImGui::MenuItem("Change to without transition"))
				asmInterface.gocAnimator->ChangeStateWithoutTransition(state.name);
			if (ctxNodeId.type != NodeType::LAYER_BLEND_TREE_OUTPUT) {
				ImGui::Separator();
				if (ImGui::MenuItem("Remove")) {
					RemoveNode(ctxNodeId);
					asmInterface.ReloadResource();
				}
			}
			ImGui::EndPopup();
		}
	}

	void BlendTreeEditor::RenderLinkContextMenu() {
		if (ImGui::BeginPopup("Link Context Menu")) {
			auto& state = asmInterface.GetState(ctxNodeId.id);

			if (ImGui::MenuItem("Remove")) {
				RemoveLink(ctxLinkId.inputPinId, ctxLinkId.outputPinId);
				asmInterface.ReloadResource();
			}
			ImGui::EndPopup();
		}
	}

	void BlendTreeEditor::RenderCreateInputContextMenu() {
		if (ImGui::BeginPopup("CreateInputNode")) {
			if (ImGui::MenuItem("Add new"))
				HandleAddNodeFromTarget(createOutPin);
			ImGui::EndPopup();
		}
	}

	void BlendTreeEditor::RenderCreateOutputContextMenu() {
	}

	bool BlendTreeEditor::IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, PinType inPinType, NodeType outNodeType, PinType outPinType) {
		return inPin.type == inPinType && outPin.type == outPinType && inPin.nodeId.type == inNodeType && outPin.nodeId.type == outNodeType;
	}

	bool BlendTreeEditor::IsType(OutputPinId inPin, InputPinId outPin, NodeType inNodeType, NodeType outNodeType, PinType pinType) {
		return IsType(inPin, outPin, inNodeType, pinType, outNodeType, pinType);
	}

	void BlendTreeEditor::AddLink(OutputPinId inPin, InputPinId outPin) {
		if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_NODE, PinType::CLIP)) {
			auto& blendNode = asmInterface.GetBlendNode(outPin.nodeId.id);

			if (collapseBlendSpaceNodes && blendNode.type == BlendNodeType::BLEND_SPACE) {
				auto blendSpaceId = asmInterface.GetBlendNodeBlendSpace(outPin.nodeId.id);

				if (!blendSpaceId.has_value()) { ShowError("This blendspace node does not have an associated blend space. Toggle collapsing of BlendSpaceNode nodes off to add one."); return; }
				if (outPin.idx >= asmInterface.GetBlendNodeChildCount(outPin.nodeId.id)) { ShowError("The clip in this slot in the blend space does not correspond to a blend tree node. Toggle collapsing of BlendSpaceNode nodes off to edit it."); return; }

				auto childNodeId = asmInterface.GetBlendNodeChild(outPin.nodeId.id, outPin.idx);

				if (asmInterface.GetBlendNode(childNodeId).type != BlendNodeType::CLIP) { ShowError("The clip in this slot in the blend space does not correspond to a clip node. Toggle collapsing of BlendSpaceNode nodes off to edit it."); return; }

				asmInterface.SetBlendSpaceClip(blendSpaceId.value(), outPin.idx, inPin.nodeId.id);
				asmInterface.SetBlendNodeClip(childNodeId, inPin.nodeId.id);
			}
			else if (blendNode.type == BlendNodeType::CLIP)
				asmInterface.SetBlendNodeClip(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_SPACE, PinType::CLIP)) {
			asmInterface.SetBlendSpaceClip(outPin.nodeId.id, outPin.idx, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::STATE, PinType::CLIP)) {
			asmInterface.SetStateClip(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_NODE, NodeType::STATE, PinType::BLEND_NODE)) {
			asmInterface.SetStateBlendNode(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_SPACE, NodeType::BLEND_NODE, PinType::BLEND_SPACE)) {
			asmInterface.SetBlendNodeBlendSpace(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_MASK, NodeType::CLIP, PinType::BLEND_MASK)) {
			asmInterface.SetClipBlendMask(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_NODE, PinType::VARIABLE)) {
			if (outPin.idx == 0)
				asmInterface.SetBlendNodeBlendFactorVariable(outPin.nodeId.id, inPin.nodeId.id);
			else if (collapseBlendSpaceNodes && asmInterface.GetBlendNode(outPin.nodeId.id).type == BlendNodeType::BLEND_SPACE) {
				auto blendSpaceId = asmInterface.GetBlendNodeBlendSpace(outPin.nodeId.id);

				if (!blendSpaceId.has_value()) { ShowError("This blendspace node does not have an associated blend space. Toggle collapsing of BlendSpaceNode nodes off to add one."); return; }

				if (outPin.idx == 1)
					asmInterface.SetBlendSpaceXVariable(blendSpaceId.value(), inPin.nodeId.id);
				else if (outPin.idx == 2)
					asmInterface.SetBlendSpaceYVariable(blendSpaceId.value(), inPin.nodeId.id);
			}

		}
		else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_SPACE, PinType::VARIABLE)) {
			if (outPin.idx == 0)
				asmInterface.SetBlendSpaceXVariable(outPin.nodeId.id, inPin.nodeId.id);
			else if (outPin.idx == 1)
				asmInterface.SetBlendSpaceYVariable(outPin.nodeId.id, inPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::STATE, NodeType::FLAG, PinType::FLAG)) {
			asmInterface.AddStateFlag(inPin.nodeId.id, outPin.nodeId.id);
		}
	}

	void BlendTreeEditor::RemoveLink(OutputPinId inPin, InputPinId outPin) {
		if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_NODE, PinType::CLIP)) {
			auto& blendNode = asmInterface.GetBlendNode(outPin.nodeId.id);

			if (collapseBlendSpaceNodes && blendNode.type == BlendNodeType::BLEND_SPACE) {
				auto blendSpaceId = asmInterface.GetBlendNodeBlendSpace(outPin.nodeId.id);

				if (!blendSpaceId.has_value()) { ShowError("This blendspace node does not have an associated blend space. Toggle collapsing of BlendSpaceNode nodes off to add one."); return; }
				if (outPin.idx >= asmInterface.GetBlendNodeChildCount(outPin.nodeId.id)) { ShowError("The clip in this slot in the blend space does not correspond to a blend tree node. Toggle collapsing of BlendSpaceNode nodes off to edit it."); return; }

				auto childNodeId = asmInterface.GetBlendNodeChild(outPin.nodeId.id, outPin.idx);

				if (asmInterface.GetBlendNode(childNodeId).type != BlendNodeType::CLIP) { ShowError("The clip in this slot in the blend space does not correspond to a clip node. Toggle collapsing of BlendSpaceNode nodes off to edit it."); return; }

				asmInterface.ClearBlendSpaceClip(blendSpaceId.value(), outPin.idx);
				asmInterface.ClearBlendNodeClip(childNodeId);
			}
			else if (blendNode.type == BlendNodeType::CLIP)
				asmInterface.ClearBlendNodeClip(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::BLEND_SPACE, PinType::CLIP)) {
			asmInterface.ClearBlendSpaceClip(outPin.nodeId.id, outPin.idx);
		}
		else if (IsType(inPin, outPin, NodeType::CLIP, NodeType::STATE, PinType::CLIP)) {
			asmInterface.ClearStateClip(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_NODE, NodeType::STATE, PinType::BLEND_NODE)) {
			asmInterface.ClearStateBlendNode(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_SPACE, NodeType::BLEND_NODE, PinType::BLEND_SPACE)) {
			asmInterface.ClearBlendNodeBlendSpace(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::BLEND_MASK, NodeType::CLIP, PinType::BLEND_MASK)) {
			asmInterface.ClearClipBlendMask(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_NODE, PinType::VARIABLE)) {
			if (outPin.idx == 0)
				asmInterface.ClearBlendNodeBlendFactorVariable(outPin.nodeId.id);
			else if (collapseBlendSpaceNodes && asmInterface.GetBlendNode(outPin.nodeId.id).type == BlendNodeType::BLEND_SPACE) {
				auto blendSpaceId = asmInterface.GetBlendNodeBlendSpace(outPin.nodeId.id);

				if (!blendSpaceId.has_value()) { ShowError("This blendspace node does not have an associated blend space. Toggle collapsing of BlendSpaceNode nodes off to add one."); return; }

				if (outPin.idx == 1)
					asmInterface.ClearBlendSpaceXVariable(blendSpaceId.value());
				else if (outPin.idx == 2)
					asmInterface.ClearBlendSpaceYVariable(blendSpaceId.value());
			}

		}
		else if (IsType(inPin, outPin, NodeType::VARIABLE, NodeType::BLEND_SPACE, PinType::VARIABLE)) {
			if (outPin.idx == 0)
				asmInterface.ClearBlendSpaceXVariable(outPin.nodeId.id);
			else if (outPin.idx == 1)
				asmInterface.ClearBlendSpaceYVariable(outPin.nodeId.id);
		}
		else if (IsType(inPin, outPin, NodeType::STATE, NodeType::FLAG, PinType::FLAG)) {
			asmInterface.RemoveStateFlag(inPin.nodeId.id, outPin.nodeId.id);
		}
	}

	NodeId BlendTreeEditor::AddNode(NodeType type) {
		switch (type) {
		case NodeType::STATE: return { NodeType::STATE, asmInterface.AddState() };
		case NodeType::BLEND_MASK: return { NodeType::BLEND_MASK, asmInterface.AddBlendMask() };
		case NodeType::BLEND_SPACE: return { NodeType::BLEND_SPACE, asmInterface.AddBlendSpace() };
		case NodeType::CLIP: return { NodeType::CLIP, asmInterface.AddClip() };
		case NodeType::VARIABLE: return { NodeType::VARIABLE, asmInterface.AddVariable() };
		case NodeType::FLAG: return { NodeType::FLAG, asmInterface.AddFlag() };
		default: assert("unknown node type"); return {};
		}
	}

	void BlendTreeEditor::RemoveNode(NodeId node) {
		switch (node.type) {
		case NodeType::STATE: asmInterface.RemoveState(node.id); break;
		case NodeType::BLEND_MASK: asmInterface.RemoveBlendMask(node.id); break;
		case NodeType::BLEND_SPACE: asmInterface.RemoveBlendSpace(node.id); break;
		case NodeType::CLIP: asmInterface.RemoveClip(node.id); break;
		case NodeType::VARIABLE: asmInterface.RemoveVariable(node.id); break;
		case NodeType::FLAG: asmInterface.RemoveFlag(node.id); break;
		default: assert("unknown node type"); break;
		}
	}

	void BlendTreeEditor::HandleAddNode(NodeType nodeType) {
		NodeId nodeId = AddNode(nodeType);
		ax::NodeEditor::SetNodePosition(nodeId, clickPos);
		asmInterface.ReloadResource();
	}

	void BlendTreeEditor::HandleAddNodeWithTarget(NodeType nodeType, InputPinId target) {
		NodeId nodeId = AddNode(nodeType);
		AddLink({ nodeId, target.type, 0 }, target);
		ax::NodeEditor::SetNodePosition(nodeId, clickPos);
		asmInterface.ReloadResource();
	}

	void BlendTreeEditor::HandleAddNodeFromTarget(InputPinId target) {
		switch (target.type) {
		case PinType::CLIP: HandleAddNodeWithTarget(NodeType::CLIP, target); break;
		case PinType::BLEND_MASK: HandleAddNodeWithTarget(NodeType::BLEND_MASK, target); break;
		case PinType::BLEND_SPACE: HandleAddNodeWithTarget(NodeType::BLEND_SPACE, target); break;
		case PinType::VARIABLE: HandleAddNodeWithTarget(NodeType::VARIABLE, target); break;
		case PinType::FLAG: HandleAddNodeWithTarget(NodeType::FLAG, target); break;
		}
	}

	//void BlendTreeEditor::RenderNodeLinks(hh::anim::LayerBlendNode* node, short nodeId, hh::anim::BlendNodeData& nodeData)
	//{
	//	auto& layer = gocAnimator->animationStateMachine->layers[nodeData.childNodeArrayOffset];

	//	if (AnimationStateMachine::LayerStateBase* layerState = layer.layerState) {
	//		auto* nextAnimationState = layerState->GetNextAnimationState();
	//		auto* prevAnimationState = layerState->GetPreviousAnimationState();

	//		if (nextAnimationState && nextAnimationState->stateData->type == StateType::BLEND_TREE) {
	//			nodeEditor.Link(GetLinkId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), GetOutputPinId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0), GetInputPinId(nodeId, 0));
	//			if (prevAnimationState && nextAnimationState != prevAnimationState)
	//				nodeEditor.Flow(GetLinkId(nextAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), NodeEd::FlowDirection::Backward);
	//		}

	//		if (prevAnimationState && prevAnimationState->stateData->type == StateType::BLEND_TREE) {
	//			nodeEditor.Link(GetLinkId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0), GetOutputPinId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0), GetInputPinId(nodeId, 0));
	//			if (nextAnimationState && nextAnimationState != prevAnimationState)
	//				nodeEditor.Flow(GetLinkId(prevAnimationState->stateData->rootBlendNodeOrClipIndex, 0, nodeId, 0));
	//		}
	//	}
	//}
}
