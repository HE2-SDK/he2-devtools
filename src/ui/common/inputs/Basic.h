#pragma once
#include <imgui_internal.h>

inline float defaultFloatStep{ 0.01f };

template<typename T, typename = void> class imgui_datatype;
template<> class imgui_datatype<uint8_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_U8; static constexpr int components = 1; };
template<> class imgui_datatype<int8_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_S8; static constexpr int components = 1; };
template<> class imgui_datatype<char> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_S8; static constexpr int components = 1; };
template<> class imgui_datatype<uint16_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_U16; static constexpr int components = 1; };
template<> class imgui_datatype<int16_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_S16; static constexpr int components = 1; };
template<> class imgui_datatype<uint32_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_U32; static constexpr int components = 1; };
template<> class imgui_datatype<int32_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_S32; static constexpr int components = 1; };
template<> class imgui_datatype<uint64_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_U64; static constexpr int components = 1; };
template<> class imgui_datatype<int64_t> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_S64; static constexpr int components = 1; };
template<> class imgui_datatype<float> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Float; static constexpr int components = 1; };
template<> class imgui_datatype<double> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Double; static constexpr int components = 1; };
//template<> class imgui_datatype<csl::math::Vector2> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Float; static constexpr int components = 2; };
//template<> class imgui_datatype<csl::math::Vector3> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Float; static constexpr int components = 3; };
//template<> class imgui_datatype<csl::math::Vector4> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Float; static constexpr int components = 4; };
//template<> class imgui_datatype<csl::math::Position> { public: static constexpr ImGuiDataType dtype = ImGuiDataType_Float; static constexpr int components = 3; };
//template<typename T> class imgui_datatype<T, std::enable_if_t<std::is_base_of_v<Eigen::Quaternion<typename T::Scalar>, T>>> { public: static constexpr ImGuiDataType dtype = imgui_datatype<typename T::Scalar>::dtype; static constexpr int components = 4; };
template<typename T> class imgui_datatype<T, std::enable_if_t<std::is_base_of_v<Eigen::Matrix<typename T::Scalar, T::RowsAtCompileTime, 1, T::Options, T::MaxRowsAtCompileTime, 1>, T>>> { public: static constexpr ImGuiDataType dtype = imgui_datatype<typename T::Scalar>::dtype; static constexpr int components = T::RowsAtCompileTime; };
//template<typename T> class imgui_datatype<T, std::enable_if_t<std::is_base_of_v<Eigen::AlignedVector3<typename T::Scalar>, T>>> { public: static constexpr ImGuiDataType dtype = imgui_datatype<typename T::Scalar>::dtype; static constexpr int components = T::RowsAtCompileTime; };
template<> class imgui_datatype<csl::ut::Color8> { public: static constexpr ImGuiDataType dtype = imgui_datatype<uint8_t>::dtype; static constexpr int components = 4; };
template<> class imgui_datatype<csl::ut::Colorf> { public: static constexpr ImGuiDataType dtype = imgui_datatype<float>::dtype; static constexpr int components = 4; };
template<> class imgui_datatype<hh::needle::uint_vector4> { public: static constexpr ImGuiDataType dtype = imgui_datatype<uint32_t>::dtype; static constexpr int components = 4; };
template<> class imgui_datatype<hh::needle::float_vector4> { public: static constexpr ImGuiDataType dtype = imgui_datatype<float>::dtype; static constexpr int components = 4; };

template<typename T> float default_step() {
	if constexpr (std::numeric_limits<T>::is_integer)
		return 1.0f;
	else
		return defaultFloatStep;
}

//template<typename T>
//bool InputScalar(const char* label, T& obj, float v_speed = default_step<T>(), const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) {
//	return ImGui::InputScalarN(label, imgui_datatype<T>::dtype, &obj, imgui_datatype<T>::components, v_speed, p_min, p_max, format, flags);
//}
//
//template<typename T>
//bool InputScalar(const char* label, T& obj, const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) {
//	return InputScalar(label, obj, default_step<T>(), p_min, p_max, format, flags);
//}

template<typename T>
bool DragScalar(const char* label, T& obj, float v_speed, const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) {
	return ImGui::DragScalarN(label, imgui_datatype<T>::dtype, &obj, imgui_datatype<T>::components, v_speed, p_min, p_max, format, flags);
}

template<typename T>
bool DragScalar(const char* label, T& obj, const T* p_min = nullptr, const T* p_max = nullptr, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) {
	return DragScalar(label, obj, default_step<T>(), p_min, p_max, format, flags);
}

template<typename T>
bool SliderScalar(const char* label, T& obj, const T* v_min, const T* v_max, const char* format = nullptr, ImGuiSliderFlags flags = ImGuiSliderFlags_None) {
	return ImGui::SliderScalarN(label, imgui_datatype<T>::dtype, &obj, imgui_datatype<T>::components, v_min, v_max, format, flags);
}

template<size_t Len>
bool InputText(const char* label, char (&str)[Len], ImGuiInputTextFlags flags = ImGuiInputTextFlags_None) {
    return ImGui::InputText(label, str, Len, flags);
}

bool InputText(const char* label, csl::ut::VariableString& str, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);
bool InputText(const char* label, csl::ut::String& str, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);
bool InputText(const char* label, const char*& str, hh::fnd::ManagedResource* resource, ImGuiInputTextFlags flags = ImGuiInputTextFlags_None);

template<size_t Len>
bool InputDirectory(const char* label, char(&str)[Len]) {
	bool edited{};
	ImGui::PushID(label);
	edited |= InputText(label, str);
	ImGui::SameLine();
	if (ImGui::Button("Browse...")) {
		IGFD::FileDialogConfig config;
		config.path = str;
		config.flags = ImGuiFileDialogFlags_Modal;
		ImGuiFileDialog::Instance()->OpenDialog("DirectoryChoiceDialog", "Choose a directory", nullptr, config);
	}
	if (ImGuiFileDialog::Instance()->Display("DirectoryChoiceDialog"))
	{
		if (ImGuiFileDialog::Instance()->IsOk()) {
			strcpy_s(str, ImGuiFileDialog::Instance()->GetCurrentPath().c_str());
			edited = true;
		}
		ImGuiFileDialog::Instance()->Close();
	}
	ImGui::PopID();
	return edited;
}

template<typename T>
bool CheckboxFlags(const char* label, T& v, T value) {
	if constexpr (sizeof(T) < sizeof(unsigned int)) {
		unsigned int v2 = v;
		bool edited = ImGui::CheckboxFlags(label, &v2, value);
		v = static_cast<T>(v2);
		return edited;
	}
	else {
		return ImGui::CheckboxFlags(label, &v, value);
	}
}

template<typename T, typename U>
bool CheckboxFlags(const char* label, csl::ut::Bitset<T, U>& v, T value) {
	return CheckboxFlags<U>(label, v.bits, static_cast<U>(1) << static_cast<U>(value));
}

template<typename T, size_t count>
bool ComboEnum(const char* label, T& v, const char* (&names)[count]) {
	int x = static_cast<std::underlying_type_t<T>>(v);
	bool edited = ImGui::Combo(label, &x, names, count);
	v = static_cast<T>(x);
	return edited;
}
