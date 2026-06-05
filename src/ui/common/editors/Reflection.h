#pragma once

struct ImGuiTextFilter;

inline float rflMinFloatStep{ 0.01f };
inline unsigned int rflSliderCutOff{ 3000 };

bool ReflectionEditor(const char* label, void* reflectionData, const hh::fnd::RflClass* rflClass, bool defaultOpen = false, const ImGuiTextFilter* filter = nullptr);

template<typename T, typename = decltype(T::rflClass)>
static bool Editor(const char* label, T& reflectionData, bool defaultOpen = false) {
    return ReflectionEditor(label, &reflectionData, &RESOLVE_STATIC_VARIABLE(T::rflClass), defaultOpen);
}

bool ResettableReflectionEditor(const char* label, void* reflectionData, void* originalReflectionData, const hh::fnd::RflClass* rflClass, const ImGuiTextFilter* filter = nullptr);

template<typename T, typename = decltype(T::rflClass)>
static bool Editor(const char* label, T& reflectionData, T& originalReflectionData) {
    return ResettableReflectionEditor(label, &reflectionData, originalReflectionData, &RESOLVE_STATIC_VARIABLE(T::rflClass));
}
