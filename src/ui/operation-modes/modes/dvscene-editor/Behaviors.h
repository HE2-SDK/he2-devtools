#pragma once
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include "DvNode.h"
#include <utilities/BoundingBoxes.h>

namespace ui::operation_modes::modes::dvscene_editor {
    template<> struct SelectionBehaviorTraits<Context> {
		using ObjectType = DvNode;
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<DvNode>& objects, csl::geom::Aabb& aabb) 
		{
			aabb = csl::geom::Aabb{ { INFINITY, INFINITY, INFINITY }, { -INFINITY, -INFINITY, -INFINITY } };
			bool edited{ false };
			for (auto& node : objects) {
				if (node.node->nodeType == hh::dv::DvNodeBase::NodeType::CHARACTER ||
					node.node->nodeType == hh::dv::DvNodeBase::NodeType::MODEL)
				{
					auto* modelNode = static_cast<hh::dv::DvNodeBaseAnimationModel*>(node.node);
					if (auto* gocVisual = modelNode->GetDvSceneObject()->dvStandardChar->GetComponent<hh::gfx::GOCVisualModel>()) {
						edited |= true;
						AddAabb(aabb, gocVisual->transformedAabb);
					}
				}
			}
			return edited; 
		}
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(DvNode& obj) { return obj.CanTransform(); }
		bool IsRoot(DvNode obj) { return true; }
		DvNode GetParent(DvNode obj) { return {}; }
		Eigen::Affine3f GetSelectionSpaceTransform(DvNode obj) const { return obj.GetTransform(); }
		void SetSelectionSpaceTransform(DvNode obj, const Eigen::Affine3f& transform) {
			obj.SetTransform(transform);
		}
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = true;
	};
	
	template<> struct DeleteBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		void DeleteObjects(const csl::ut::MoveArray<DvNode>& objects) {
			for(auto& x : objects)
				x.DeleteNode();
		}
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/SelectionVisual.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
#include <ui/operation-modes/behaviors/Delete.h>
