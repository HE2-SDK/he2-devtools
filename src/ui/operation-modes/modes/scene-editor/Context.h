#pragma once
#include <utilities/CompatibleObject.h>

namespace ui::operation_modes::modes::scene_editor {
	struct Selection {
		enum class Type {
			SCENE_NODE
		};

		struct SceneNode {
			ucsl::resources::scene::v106::SceneNode* node;
			ucsl::resources::scene::v106::SceneGraphNode* graphNode;
		};

		Type type;
		ucsl::resources::scene::v106::SceneData* sceneData;
		union {
			SceneNode sceneNode;
		};

		inline bool operator==(const Selection& other) const
		{
			if (type != other.type)
				return false;

			if (type == Type::SCENE_NODE)
				return sceneNode.graphNode == other.sceneNode.graphNode;

			return false;
		}

		inline Selection() {}
		inline Selection(ucsl::resources::scene::v106::SceneData* sceneData, ucsl::resources::scene::v106::SceneGraphNode* graphNode) 
			: type{ Type::SCENE_NODE }, sceneData { sceneData } 
		{
			sceneNode.graphNode = graphNode;
			sceneNode.node = &sceneData->sceneNodes[graphNode->index];
		}
	};

	class Context : public CompatibleObject {
	public:
		using CompatibleObject::CompatibleObject;

		hh::scene::SceneControl* sceneCtrl{};

		Context(csl::fnd::IAllocator* allocator);
	};
}
