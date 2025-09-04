#include "Bones.h"
#include <ui/common/overlays/Tag.h>
#include <utilities/math/MathUtils.h>

namespace devtools::debug_rendering::renderables {
	constexpr unsigned int maxStepCount = 256;

	void Bones::RenderIngameDebugVisuals(hh::gfnd::DrawContext& ctx) {
#ifndef DEVTOOLS_TARGET_SDK_wars
		if (!enabled)
			return;

		auto* gameManager = hh::game::GameManager::GetInstance();
		if (!gameManager)
			return;

		xs.clear();
		ys.clear();
		zs.clear();
		indices.clear();
		size_t totalBoneCount{};

		for (auto* gameObject : gameManager->objects) {
			for (auto* goc : gameObject->components) {
				if (goc->pStaticClass == hh::anim::GOCAnimator::GetClass() || goc->pStaticClass == hh::anim::GOCAnimationSimple::GetClass()) {
					auto* gocAnim = static_cast<hh::anim::GOCAnimationSingle*>(goc);
					auto* pose = static_cast<hh::anim::PosePxd*>(gocAnim->GetPose());

					if (!pose)
						continue;

					totalBoneCount += pose->unk1.transforms.size();

					for (auto& tf : pose->unk1.transforms) {
						auto affine = TransformToAffine3f(gocAnim->GetTransform()) * TransformToAffine3f(tf);
						auto pos = affine * csl::math::Vector3::Zero();
						auto x = affine * (0.15 * csl::math::Vector3::UnitX());
						auto y = affine * (0.15 * csl::math::Vector3::UnitY());
						auto z = affine * (0.15 * csl::math::Vector3::UnitZ());

						xs.push_back({ pos.x(), pos.y(), pos.z(), 0xFF0000FF });
						xs.push_back({ x.x(), x.y(), x.z(), 0xFF0000FF });
						ys.push_back({ pos.x(), pos.y(), pos.z(), 0xFF00FF00 });
						ys.push_back({ y.x(), y.y(), y.z(), 0xFF00FF00 });
						zs.push_back({ pos.x(), pos.y(), pos.z(), 0xFFFF0000 });
						zs.push_back({ z.x(), z.y(), z.z(), 0xFFFF0000 });
					}
				}
			}
		}

		for (unsigned short i = 0; i < totalBoneCount * 2; i++)
			indices.push_back(i);

		ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, &xs[0], &indices[0], totalBoneCount * 2);
		ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, &ys[0], &indices[0], totalBoneCount * 2);
		ctx.DrawPrimitive(hh::gfnd::PrimitiveType::LINE_LIST, &zs[0], &indices[0], totalBoneCount * 2);
#endif
	}

	void Bones::RenderImGuiDebugVisuals(const ImGuiDrawContext& ctx) {
		if (!enabled)
			return;
	}
}
