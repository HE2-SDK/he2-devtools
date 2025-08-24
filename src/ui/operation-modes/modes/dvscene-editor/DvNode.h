#pragma once
#include "Context.h"
#include <dieventlib/dvnode.h>

namespace ui::operation_modes::modes::dvscene_editor {
    struct DvNode {
	private:
		DvNode LoopThroughParents(hh::dv::DvNodeBase* node) {
			if(node->nodeType == hh::dv::DvNodeBase::NodeType::PATH)
				return DvNode(ctx->GetFileNode(node), *ctx);
			else
				return LoopThroughParents(node->parent);
		}

		void Delete(dv::DvNode& node) {
			auto parent = ctx->GetFileNode(this->node->parent);
			for (auto& x : node.childNodes)
				Delete(x);
			node.childNodes.clear();
			int y = 0;
			for (auto& x : parent->childNodes) {
				if (memcmp(&x.guid, &node.guid, 16) == 0)
				{
					parent->childNodes.erase(std::next(parent->childNodes.begin(), y));
					return;
				}
				y++;
			}
		}

		void Delete(hh::dv::DvNodeBase* node){
			if (node->nodeType == hh::dv::DvNodeBase::NodeType::ELEMENT) {
				reinterpret_cast<hh::dv::DvNodeElement*>(node)->element->RemoveCallback();
				ctx->DeallocateNode(node);
			}
			if(auto* y = node->parent){
				y->childrenElements0.remove(y->childrenElements0.find(node));
				y->childrenElements1.remove(y->childrenElements1.find(node));
				y->childrenElements2.remove(y->childrenElements2.find(node));
				y->childrenPath.remove(y->childrenPath.find(node));
				y->childrenCamera.remove(y->childrenCamera.find(node));
				y->childrenMotion.remove(y->childrenMotion.find(node));
				if (node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER || node->nodeType == hh::dv::DvNodeBase::NodeType::MODEL)
					hh::game::GameObjectSystem::GetInstance()->GetGameObjectByHandle(reinterpret_cast<hh::dv::DvNodeBaseAnimationModel*>(node)->nodeCharacterObj->dvStandardChar)->Kill();
				if(node->childrenElements0.begin())
					for(auto* x : node->childrenElements0)
						Delete(x);
				if (node->childrenElements1.begin())
					for (auto* x : node->childrenElements1)
						Delete(x);
				if (node->childrenElements2.begin())
					for (auto* x : node->childrenElements2)
						Delete(x);
				if (node->childrenPath.begin())
					for (auto* x : node->childrenPath)
						Delete(x);
				if (node->childrenCamera.begin())
					for (auto* x : node->childrenCamera)
						Delete(x);
				if (node->childrenMotion.begin())
					for (auto* x : node->childrenMotion)
						Delete(x);
				node->Free();
			}
		}
	public:
		Context* ctx;
        hh::dv::DvNodeBase* node;
		dv::DvNode* fileNode;
		dv::Resource* fileResource;

		DvNode();
		DvNode(dv::DvNode* node, Context& ctx);

		bool operator==(const DvNode& other) const;
		const char* GetName() const;
		bool CanTransform() const;
		Eigen::Affine3f GetTransform() const;
		void SetTransform(const Eigen::Affine3f& transform);
		DvNode GetParent();
		void DeleteNode();
		void UpdateRuntimeNode();
	};
}
