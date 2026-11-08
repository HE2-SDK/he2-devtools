#pragma once

#include <ui/common/inputs/Basic.h>

template<typename T, std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
static void Viewer(const char* label, const T& obj) {
	if constexpr (std::numeric_limits<T>::is_integer) {
		if constexpr (std::is_same_v<uint64_t, T> || std::is_same_v<int64_t, T>)
			ImGui::Text("%s: %zd", label, obj);
		else
			ImGui::Text("%s: %d", label, obj);
	}
	else
		ImGui::Text("%s: %f", label, obj);
}

template<typename T, int Rows, int Options, int MaxRows>
static void Viewer(const char* label, const Eigen::Matrix<T, Rows, 1, Options, MaxRows>& vec) {
	if constexpr (Rows == 4)
		IM_ASSERT(ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")));

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.x());
	ImGui::TableNextColumn();
	ImGui::Text("%f", vec.y());
	ImGui::TableNextColumn();
	if constexpr (Rows >= 3)
		ImGui::Text("%f", vec.z());
	if constexpr (Rows == 4) {
		ImGui::TableNextColumn();
		ImGui::Text("%f", vec.w());
	}
	else {
		if (ImGui::GetStateStorage()->GetBool(ImGui::GetID("WithW")))
			ImGui::TableNextColumn();
	}
	ImGui::TableNextColumn();
	ImGui::Text(label);
}

void Viewer(const char* label, bool obj);
void Viewer(const char* label, const char* str);
void Viewer(const char* label, const csl::ut::String& str);
void Viewer(const char* label, const ucsl::objectids::ObjectIdV1& obj);
void Viewer(const char* label, const ucsl::objectids::ObjectIdV2& obj);
void Viewer(const char* label, void* const& obj);
void Viewer(const char* label, const Eigen::Quaternionf& quat);
void Viewer(const char* label, const hh::fnd::WorldPosition& worldPos);
void Viewer(const char* label, const csl::math::Transform& transform);
void Viewer(const char* label, hh::game::GameObject* const& gameObject);
void Viewer(const char* label, const hh::fnd::HFrame& frame);

template<ucsl::colors::ChannelOrder order>
void Viewer(const char* label, const ucsl::colors::Color8<order>& color) {
	ImGui::Text("%s: R: %d, G: %d, B: %d, A: %d", label, color.r, color.g, color.b, color.a);
}

template<ucsl::colors::ChannelOrder order>
void Viewer(const char* label, const ucsl::colors::Colorf<order>& color) {
	ImGui::Text("%s: R: %f, G: %f, B: %f, A: %f", label, color.r, color.g, color.b, color.a);
}

bool BeginVectorViewerTable(const char* id, bool withWAxis);
void EndVectorViewerTable();

template<typename T, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
static void Viewer(const char* label, const Eigen::Matrix<T, Rows, Cols, Options, MaxRows, MaxCols>& mat) {
	auto cols = reinterpret_cast<const Eigen::Matrix<T, Rows, 1, Options, MaxRows>(&)[Cols]>(mat);

	if (BeginVectorViewerTable(label, true)) {
		for (int i = 0; i < Cols; i++) {
			Viewer(label, cols[i]);
		}

		EndVectorViewerTable();
	}
}

template<typename T, int Dim, int Mode>
static void Viewer(const char* label, const Eigen::Transform<T, Dim, Mode>& mat) {
	Viewer(label, mat.matrix());
}

template<typename T, size_t Len>
static void Viewer(const char* label, const T(&arr)[Len]) {
	char name[200];
	snprintf(name, sizeof(name), "%s[0..%zd]", label, Len);

	if (ImGui::TreeNode(name)) {
		for (size_t i = 0; i < Len; i++) {
			snprintf(name, sizeof(name), "%s[%zd]", label, i);
			Viewer(name, arr[i]);
		}

		ImGui::TreePop();
	}
}