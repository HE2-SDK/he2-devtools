#include "SceneEditor.h"
#include "SceneSettings.h"
#include "Timeline.h"
#include "SceneNodeGraph.h"
#include "SceneNodeInspector.h"

#include "Behaviors.h"

namespace ui::operation_modes::modes::scene_editor
{
	SceneEditor::SceneEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host) : OperationMode{ allocator, host }
	{
        AddPanel<SceneSettings>();
        AddPanel<Timeline>();
        AddPanel<SceneNodeGraph>();
        AddPanel<SceneNodeInspector>();
		AddBehavior<SelectionBehavior>();
		AddBehavior<SelectionAabbBehavior>();
		AddBehavior<SelectionTransformationBehavior>();
		AddBehavior<GizmoBehavior>();
	}

	SceneEditor::~SceneEditor(){
	}
}
