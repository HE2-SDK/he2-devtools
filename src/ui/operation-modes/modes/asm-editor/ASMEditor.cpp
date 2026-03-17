#include "ASMEditor.h"
#include <ui/common/editors/Basic.h>
#include <optional>

namespace ui::operation_modes::modes::asm_editor
{
	using namespace hh::anim;

	void ASMEditor::RenderScene()
	{
		nodeEditor->BeginContext();
		nodeEditor->Begin();

		RenderNodes();
		RenderTransitions();
		RenderFlow();

		if (nodeEditor->ShowNodeContextMenu(ctxNodeId)) {
			ax::NodeEditor::Suspend();
			ImGui::OpenPopup("Node Context Menu");
			ax::NodeEditor::Resume();
		}

		ax::NodeEditor::Suspend();
		if (ImGui::BeginPopup("Node Context Menu")) {
			auto& state = asmInterface.GetState(ctxNodeId.id);

			if (ImGui::MenuItem("Change to"))
				GetContext().gocAnimator->ChangeState(state.name);
			if (ImGui::MenuItem("Change to without transition"))
				GetContext().gocAnimator->ChangeStateWithoutTransition(state.name);
			ImGui::EndPopup();
		}
		ax::NodeEditor::Resume();

		nodeEditor->End();
		nodeEditor->EndContext();
	}

	void ASMEditor::SetGOCAnimator(GOCAnimator* gocAnimator)
	{
		GetContext().gocAnimator = gocAnimator;
		//nodeEditor = new (GetAllocator()) NodeEditor{ GetAllocator(	), *gocAnimator->asmResourceManager->animatorResource, gocAnimator };
	}


	void ASMEditor::RenderNodes()
	{
		for (unsigned short i = 0; i < asmInterface.asmData.stateCount; i++)
			nodeEditor->State(i);
	}

	void ASMEditor::RenderTransitions()
	{
		for (unsigned short i = 0; i < asmInterface.asmData.stateCount; i++) {
			auto& state = asmInterface.asmData.states[i];

			if (state.stateEndTransition.transitionInfo.targetStateIndex != -1)
				nodeEditor->StateDefaultTransition(i, state.stateEndTransition.transitionInfo.targetStateIndex);

			for (unsigned short j = 0; j < state.eventCount; j++)
				nodeEditor->StateEventTransition(i, asmInterface.asmData.events[state.eventOffset + j].transition.transitionInfo.targetStateIndex, j);

			if (state.transitionArrayIndex != -1) {
				if (nodeEditor->IsStateSelected(i)) {
					auto& transitionArray = asmInterface.asmData.transitionArrays[state.transitionArrayIndex];
					auto* transitions = &asmInterface.asmData.transitions[transitionArray.offset];

					for (int j = 0; j < transitionArray.size; j++)
						nodeEditor->StateTransition(i, transitions[j].transitionInfo.targetStateIndex);
				}
			}
		}
	}

	void ASMEditor::RenderFlow()
	{
		nodeEditor->StateActiveTransitionFlow();
	}
}
