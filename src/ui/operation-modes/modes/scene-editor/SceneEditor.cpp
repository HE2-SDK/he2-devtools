#include "SceneEditor.h"
//#include "Tree.h"
#include "SceneSettings.h"
//#include "node-inspector/NodeInspector.h"
#include "Timeline.h"

//#include "Behaviors.h"

namespace ui::operation_modes::modes::scene_editor
{
	SceneEditor::SceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
		//AddPanel<Tree>();
        AddPanel<SceneSettings>();
        //AddPanel<NodeInspector>();
        AddPanel<Timeline>();
		//AddBehavior<SelectionBehavior>();
		//AddBehavior<SelectionAabbBehavior>();
		//AddBehavior<SelectionTransformationBehavior>();
		//AddBehavior<GizmoBehavior>();
		//AddBehavior<DeleteBehavior>();
	}

	SceneEditor::~SceneEditor(){
	}
}
