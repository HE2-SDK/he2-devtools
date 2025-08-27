#include "GOCAnimator.h"
#include <ui/common/viewers/Basic.h>
#include <ui/Desktop.h>
#include <ui/common/StandaloneOperationModeHost.h>
#include <ui/operation-modes/modes/asm-editor/BlendTreeEditor.h>
#include <ui/operation-modes/modes/asm-editor/ASMEditor.h>
#include "GOCAnimation.h"

void RenderBlendNode(hh::anim::BlendNodeBase* node) {
	const char* type;

	switch (node->type) {
	case hh::anim::BlendNodeType::LERP:
		type = "LERP";
		break;
	case hh::anim::BlendNodeType::ADDITIVE:
		type = "ADDITIVE";
		break;
	case hh::anim::BlendNodeType::CLIP:
		type = "CLIP";
		break;
	case hh::anim::BlendNodeType::OVERRIDE:
		type = "OVERRIDE";
		break;
	case hh::anim::BlendNodeType::LAYER:
		type = "LAYER";
		break;
	case hh::anim::BlendNodeType::MULTIPLY:
		type = "MULTIPLY";
		break;
	case hh::anim::BlendNodeType::BLEND_SPACE:
		type = "BLENDSPACE";
		break;
	case hh::anim::BlendNodeType::TWO_POINT_LERP:
		type = "TWO POINT LERP";
		break;
	default:
		type = "UNKNOWN";
		break;
	}

	if (ImGui::TreeNodeEx(node, ImGuiTreeNodeFlags_DefaultOpen, "%x - type %s - flags %x", node, type, node->flags)) {
		for (auto* child : node->children)
			RenderBlendNode(child);
		ImGui::TreePop();
	}
}

//void RenderLayerState(hh::anim::AnimationStateMachine::LayerStateBase& layerState) {
//	if (layerState.GetRuntimeTypeInfo() == reinterpret_cast<void*>(0x1416FD3C8ull)) {
//		auto& layerStateTransition = static_cast<hh::anim::AnimationStateMachine::LayerStateTransition&>(layerState);
//
//
//	}
//}

void RenderComponentInspector(hh::anim::GOCAnimator& component)
{
	RenderComponentInspector(static_cast<hh::anim::GOCAnimationSingle&>(component));

	bool nope{ false };
	if (ImGui::Button("ASM Editor")) {
		auto* host = new (Desktop::instance->GetAllocator()) StandaloneOperationModeHost<ui::operation_modes::modes::asm_editor::ASMEditor>{ Desktop::instance->GetAllocator() };
		host->operationMode.SetGOCAnimator(&component);
	}

	const char* select{ "Select one" };
	if (ImGui::BeginCombo("Current state", select)) {
		auto& stateIds = component.asmResourceManager->animatorResource->stateIdsByName;
		for (auto it = stateIds.begin(); it != stateIds.end(); it++) {
			if (ImGui::Selectable(it.key(), &nope)) {
				component.ChangeState(it.key());
			}
		}
		ImGui::EndCombo();
	}
	//if (ImGui::BeginCombo("Variables", select)) {
	ImGui::SeparatorText("Variables");
	auto& variableIds = component.asmResourceManager->animatorResource->variableIdsByName;

	for (auto it = variableIds.begin(); it != variableIds.end(); it++) {
		ImGui::Text("%d: %s", *it, it.key());
	}
	//}

	if (component.animationStateMachine->layerBlendTree) {
		ImGui::SeparatorText("Layer Blend Tree");
		if (ImGui::Button("Layer Blend Tree Editor"))
			new (Desktop::instance->GetAllocator()) ui::operation_modes::modes::asm_editor::BlendTreeEditor{ Desktop::instance->GetAllocator(), &component, component.animationStateMachine->layerBlendTree, component.asmResourceManager->animatorResource->binaryData->blendTreeRootNodeId };
		RenderBlendNode(component.animationStateMachine->layerBlendTree);
	}

	ImGui::SeparatorText("Layers");
	for (auto& layer : component.animationStateMachine->layers) {
		if (ImGui::TreeNodeEx(&layer, ImGuiTreeNodeFlags_None, "%d - %s", layer.layerId, component.asmResourceManager->animatorResource->binaryData->layers[layer.layerId].name)) {
			Viewer("Layer ID", layer.layerId);
			Viewer("Unk2", layer.unk2);
			Viewer("Unk3", layer.unk3);
			Viewer("Next sequence number", layer.nextSequenceNumber);
			Viewer("Unk5", layer.unk5);
			Viewer("Speed", layer.speed);
			Viewer("Transition ID", layer.transitionId);
			RenderBlendNode(layer.blendNode);

			if (layer.layerState != nullptr && ImGui::TreeNode("Layer state")) {

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
}
