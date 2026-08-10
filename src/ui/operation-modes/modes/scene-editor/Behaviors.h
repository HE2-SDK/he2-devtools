#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/operation-modes/behaviors/ForwardDeclarations.h>
#include <utilities/math/MathUtils.h>
#include "Context.h"

namespace ui::operation_modes::modes::scene_editor {
	using namespace hh::game;

	template<> struct SelectionBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using ObjectType = Selection;
	};

	template<> struct SelectionAabbBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		bool CalculateAabb(const csl::ut::MoveArray<Selection>& objects, csl::geom::Aabb& aabb) {
			return false;
		}
	};

	template<> struct SelectionTransformationBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool Projective = false;
		bool HasTransform(Selection obj) { return obj.type == Selection::Type::SCENE_NODE; }
		bool IsRoot(Selection obj) { return true; }
		Selection GetParent(Selection obj) { return {}; }
		Eigen::Affine3f GetSelectionSpaceTransform(Selection obj) const { 
			auto* node = obj.sceneNode.node;
			return TransformToAffine3f({ .position = node->position, .rotation = EulerToQuat(node->rotation), .scale = node->scale});
		}
		void SetSelectionSpaceTransform(Selection obj, const Eigen::Affine3f& transform) {
			csl::math::Transform transformX = Affine3fToTransform(transform);
			auto* node = obj.sceneNode.node;
			node->position = transformX.position;
			Eigen::Matrix3f absoluteRotation;
			Eigen::Matrix3f absoluteScaling;
			transform.computeRotationScaling(&absoluteRotation, &absoluteScaling);
			node->rotation = MatrixToEuler(absoluteRotation);
			node->scale = transformX.scale;
		}
	};

	template<> struct GizmoBehaviorTraits<Context> : BehaviorTraitsImpl<Context> {
		using BehaviorTraitsImpl::BehaviorTraitsImpl;
		static constexpr bool allowTranslate = true;
		static constexpr bool allowRotate = true;
		static constexpr bool allowScale = true;
	};
}

#include <ui/operation-modes/behaviors/Selection.h>
#include <ui/operation-modes/behaviors/SelectionAabb.h>
#include <ui/operation-modes/behaviors/SelectionTransformation.h>
#include <ui/operation-modes/behaviors/Gizmo.h>
