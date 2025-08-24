#include "DvSceneEditor.h"
#include "Tree.h"
#include "SceneSettings.h"
#include "node-inspector/NodeInspector.h"
#include "timeline/Timeline.h"
#include "PageEditor.h"

#include "Behaviors.h"
#include <ui/common/editors/Reflection.h>

namespace ui::operation_modes::modes::dvscene_editor
{
	DvSceneEditor::DvSceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		AddPanel<Tree>();
        AddPanel<SceneSettings>();
        AddPanel<NodeInspector>();
        AddPanel<Timeline>();
        AddPanel<PageEditor>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<GizmoBehavior>();
		AddBehavior<DeleteBehavior>();
		auto* gameManager = hh::game::GameManager::GetInstance();
		gameManager->AddListener(this);
	}

	DvSceneEditor::~DvSceneEditor(){
		auto* gameManager = hh::game::GameManager::GetInstance();
		gameManager->RemoveListener(this);
	}

	void DvSceneEditor::GameObjectRemovedCallback(hh::game::GameManager* gameManager, hh::game::GameObject* gameObject)
	{
		auto& ctx = GetContext();
		if (gameObject->objectClass == hh::dv::DvSceneControl::GetClass())
			if (gameObject == ctx.goDVSC) {
				ctx.DeallocateNode(ctx.goDVSC->nodeTree->mainNode);
				ctx.addedNodes.clear();
				delete ctx.parsedScene;
				ctx.parsedScene = nullptr;
				ctx.dvPages.clear();
				ctx.goDVSC = nullptr;
				ctx.evtScene = nullptr;
				GetBehavior<SelectionBehavior<Context>>()->DeselectAll();
			}
	}
}
