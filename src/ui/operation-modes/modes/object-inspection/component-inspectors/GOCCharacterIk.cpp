#include "GOCCamera.h"
#include <ui/common/editors/Camera.h>
#include <ui/common/editors/Basic.h>

using namespace hh::anim;
using namespace hh::anim::iksolver;

bool Editor(const char* label, SolverBase::Unk1& unk1) {
	bool edited{};

	edited |= Editor("unk1", unk1.unk1);
	edited |= Editor("unk2", unk1.unk2);

	return edited;
}

void RenderSolverFor(SolverBase& solver, SolverBase::SolverType type) {
	char buf[50];
	snprintf(buf, sizeof(buf), "Solver type %d", type);

	ImGui::PushID(static_cast<unsigned int>(type));
	if (ImGui::TreeNode(buf)) {
		float unk18{};
		csl::math::Vector3 vec3;
		SolverBase::Unk1 unk1;

		if (solver.GetFloatParameter(type, unk18, 0))
			if (Editor("unk18 0", unk18))
				solver.SetFloatParameter(type, unk18, 0);
		if (solver.GetFloatParameter(type, unk18, 1))
			if (Editor("unk18 1", unk18))
				solver.SetFloatParameter(type, unk18, 1);
		if (solver.GetFloatParameter(type, unk18, 2))
			if (Editor("unk18 2", unk18))
				solver.SetFloatParameter(type, unk18, 2);
		if (solver.GetFloatParameter(type, unk18, 3))
			if (Editor("unk18 3", unk18))
				solver.SetFloatParameter(type, unk18, 3);
		if (solver.GetFloatParameter(type, unk18, 4))
			if (Editor("unk18 4", unk18))
				solver.SetFloatParameter(type, unk18, 4);
		if (solver.GetFloatParameter(type, unk18, 5))
			if (Editor("unk18 5", unk18))
				solver.SetFloatParameter(type, unk18, 5);

		if (solver.GetVectorParameter(type, vec3, 0))
			if (Editor("vec3 0", vec3))
				solver.SetVectorParameter(type, vec3, 0);
		if (solver.GetVectorParameter(type, vec3, 1))
			if (Editor("vec3 1", vec3))
				solver.SetVectorParameter(type, vec3, 1);
		if (solver.GetVectorParameter(type, vec3, 2))
			if (Editor("vec3 2", vec3))
				solver.SetVectorParameter(type, vec3, 2);
		if (solver.GetVectorParameter(type, vec3, 3))
			if (Editor("vec3 3", vec3))
				solver.SetVectorParameter(type, vec3, 3);
		if (solver.GetVectorParameter(type, vec3, 4))
			if (Editor("vec3 4", vec3))
				solver.SetVectorParameter(type, vec3, 4);
		if (solver.GetVectorParameter(type, vec3, 5))
			if (Editor("vec3 5", vec3))
				solver.SetVectorParameter(type, vec3, 5);

		if (solver.GetUnk1Parameter(type, unk1, 0))
			if (Editor("unk1 0", unk1))
				solver.SetUnk1Parameter(type, unk1, 0);
		if (solver.GetUnk1Parameter(type, unk1, 1))
			if (Editor("unk1 1", unk1))
				solver.SetUnk1Parameter(type, unk1, 1);
		if (solver.GetUnk1Parameter(type, unk1, 2))
			if (Editor("unk1 2", unk1))
				solver.SetUnk1Parameter(type, unk1, 2);
		if (solver.GetUnk1Parameter(type, unk1, 3))
			if (Editor("unk1 3", unk1))
				solver.SetUnk1Parameter(type, unk1, 3);
		if (solver.GetUnk1Parameter(type, unk1, 4))
			if (Editor("unk1 4", unk1))
				solver.SetUnk1Parameter(type, unk1, 4);
		if (solver.GetUnk1Parameter(type, unk1, 5))
			if (Editor("unk1 5", unk1))
				solver.SetUnk1Parameter(type, unk1, 5);

		ImGui::TreePop();
	}
	ImGui::PopID();
}

void RenderSolver(SolverBase& solver) {
	RenderSolverFor(solver, SolverBase::SolverType::FOOT);
	RenderSolverFor(solver, SolverBase::SolverType::HAND);
	RenderSolverFor(solver, SolverBase::SolverType::LOOKAT);
}

void RenderComponentInspector(GOCCharacterIk& component)
{
	size_t idx{};
	for (auto& solver : component.ikSolvers) {
		ImGui::PushID(idx);
		RenderSolver(*solver);
		ImGui::PopID();
		idx++;
	}
}
