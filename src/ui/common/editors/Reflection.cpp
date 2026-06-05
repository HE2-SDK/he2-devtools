#include "Reflection.h"
#include <imgui_internal.h>
#include <ucsl/rfl/ranges.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/traversals/types.h>
#include <ucsl-reflection/traversals/traversal.h>
#include <ucsl-reflection/algorithms/copy.h>
#include <ui/common/Translations.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/tools/MemoryViewer.h>
#include <ui/Desktop.h>

using namespace hh::fnd;
using namespace ucsl::reflection;
using namespace ucsl::reflection::traversals;
using namespace ucsl::rfl::ranges;

struct RflDragDropData {
	const he2sdk::ucsl::GameInterface::RflSystem::RflClass* rflClass;
	opaque_obj& obj;
};

class RenderStaticReflectionEditor {
public:
	constexpr static size_t arity = 1;
	static inline const char* currentMemberName{};
	static inline bool defaultOpen{};
	static inline const RflClass* currentRflClass{};
	static inline const ImGuiTextFilter* filter{};
	typedef bool result_type;


	template<typename T> static float rfl_step(const range_value_t<T>& rangeValue) { return std::fmaxf(rflMinFloatStep, static_cast<float>(rangeValue)); }
	template<> float rfl_step<RangeVector2>(const range_value_t<RangeVector2>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }
	template<> float rfl_step<RangeVector3>(const range_value_t<RangeVector3>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }
	template<> float rfl_step<RangeVector4>(const range_value_t<RangeVector4>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }

	template<typename F>
	static bool NameScope(const char* memberName, F f) {
		const char* savedMemberName = currentMemberName;
		currentMemberName = memberName;
		bool edited = f();
		currentMemberName = savedMemberName;
		return edited;
	}

	static bool FilterActive() { return filter != nullptr && filter->IsActive(); }
	static bool Matches(const RflClass* rflClass) { for (auto& member : rflClass->GetMembers()) if (Matches(member)) return true; return false; }
	static bool Matches(const RflClassMember& member) { auto* memberClass = static_cast<const RflClass*>(member.GetClass()); return !FilterActive() || filter->PassFilter(member.GetName()) || memberClass != nullptr && Matches(memberClass); }

	template<typename T, bool allowSliders = true>
	static bool InputRflScalar(T& obj, const PrimitiveInfo<T>& info) {
		if (info.range) {
			auto rangeMin = static_cast<T>(info.range->min);
			auto rangeMax = static_cast<T>(info.range->max);
			auto rangeStep = info.range->step;

			if constexpr (allowSliders)
				if (rangeMax - rangeMin < rflSliderCutOff)
					return SliderScalar(currentMemberName, obj, &rangeMin, &rangeMax);

			return DragScalar(currentMemberName, obj, rfl_step<range_t<T>>(rangeStep), &rangeMin, &rangeMax);
		}
		else
			return DragScalar(currentMemberName, obj);
	}

	static const char* GetRflMemberName(const RflClassMember* member) {
		const char* caption = member->GetCaption();
		const char* name = member->GetName();

		if (caption == nullptr)
			return name;

		const char* translation = Translations::GetTranslation(caption);

		return translation == nullptr ? name : translation;
	}

	template<typename T>
	static bool visit_primitive(T& obj, const PrimitiveInfo<T>& info) { return Editor(currentMemberName, obj); }
	static bool visit_primitive(int8_t& obj, const PrimitiveInfo<int8_t>& info) { return InputRflScalar<int8_t>(obj, info); }
	static bool visit_primitive(uint8_t& obj, const PrimitiveInfo<uint8_t>& info) { return InputRflScalar<uint8_t>(obj, info); }
	static bool visit_primitive(int16_t& obj, const PrimitiveInfo<int16_t>& info) { return InputRflScalar<int16_t>(obj, info); }
	static bool visit_primitive(uint16_t& obj, const PrimitiveInfo<uint16_t>& info) { return InputRflScalar<uint16_t>(obj, info); }
	static bool visit_primitive(int32_t& obj, const PrimitiveInfo<int32_t>& info) { return InputRflScalar<int32_t>(obj, info); }
	static bool visit_primitive(uint32_t& obj, const PrimitiveInfo<uint32_t>& info) { return InputRflScalar<uint32_t>(obj, info); }
	static bool visit_primitive(int64_t& obj, const PrimitiveInfo<int64_t>& info) { return InputRflScalar<int64_t>(obj, info); }
	static bool visit_primitive(uint64_t& obj, const PrimitiveInfo<uint64_t>& info) { return InputRflScalar<uint64_t>(obj, info); }
	static bool visit_primitive(float& obj, const PrimitiveInfo<float>& info) { return InputRflScalar<float>(obj, info); }
	static bool visit_primitive(ucsl::math::Vector2& obj, const PrimitiveInfo<ucsl::math::Vector2>& info) { return InputRflScalar<ucsl::math::Vector2, false>(obj, info); }
	static bool visit_primitive(ucsl::math::Vector3& obj, const PrimitiveInfo<ucsl::math::Vector3>& info) { return InputRflScalar<ucsl::math::Vector3, false>(obj, info); }
	static bool visit_primitive(ucsl::math::Vector4& obj, const PrimitiveInfo<ucsl::math::Vector4>& info) { return InputRflScalar<ucsl::math::Vector4, false>(obj, info); }
	static bool visit_primitive(const char*& obj, const PrimitiveInfo<const char*>& info) { ImGui::Text("%s: %s", currentMemberName, *obj); return false; }

	template<bool change_allocator, typename F, typename A, typename C, typename D>
	static bool _visit_array(A& arr, C c, D d, F f) {
		bool edited{};
		if (ImGui::TreeNode(currentMemberName, "%s[0..]", currentMemberName)) {
			for (int i = 0; i < arr.size(); i++) {
				ImGui::PushID(i);
				if (ImGui::Button("x"))
					arr.remove(i);
				ImGui::SameLine();

				char indexedName[200];
				snprintf(indexedName, sizeof(indexedName), "%s[%d]", currentMemberName, i);

				edited |= NameScope(indexedName, [&]() { return f(arr[i]); });

				ImGui::PopID();
			}

			if (ImGui::Button("Add item")) {
				if constexpr (change_allocator)
					if (arr.size() == arr.capacity() && arr.get_allocator() == nullptr)
						arr.change_allocator(hh::fnd::MemoryRouter::GetModuleAllocator());
				arr.emplace_back();
			}

			ImGui::TreePop();
		}
		return edited;
	}

	template<typename F, typename A, typename C, typename D> static bool visit_array(A& arr, const ArrayInfo& info, C c, D d, F f) { return _visit_array<true>(arr, c, d, f); }
	template<typename F, typename A, typename C, typename D> static bool visit_tarray(A& arr, const ArrayInfo& info, C c, D d, F f) { return _visit_array<false>(arr, c, d, f); }

	template<typename T, typename O>
	static bool visit_enum(T& obj, const EnumInfo<O>& info) {
		bool edited{};
		const char* previewValue = "<INVALID VALUE>";
		int currentValue = static_cast<int>(obj);

		for (auto& option : info.options)
			if (currentValue == option.GetIndex())
				previewValue = option.GetEnglishName();

		if (ImGui::BeginCombo(currentMemberName, previewValue)) {
			for (auto option : info.options) {
				int index = option.GetIndex();
				bool is_selected = currentValue == index;

				if (ImGui::Selectable(option.GetEnglishName(), is_selected)) {
					obj = static_cast<T>(index);
					edited = true;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return edited;
	}

	template<typename T, typename O>
	static bool visit_flags(T& obj, const FlagsInfo<O>& info) {
		if (!info.flags)
			return visit_primitive(obj, PrimitiveInfo<T>{});

		bool edited{};
		int currentValue = static_cast<int>(obj);

		if (ImGui::TreeNode(currentMemberName)) {
			for (auto& flag : *info.flags)
				edited |= ImGui::CheckboxFlags(flag.GetEnglishName(), &currentValue, static_cast<size_t>(1) << flag.GetIndex());
			ImGui::TreePop();
		}

		if (edited)
			obj = static_cast<T>(currentValue);

		return edited;
	}

	template<typename F, typename A, typename S>
	static bool visit_pointer(opaque_obj*& obj, const PointerInfo<A, S>& info, F f) {
		return obj == nullptr ? false : f(*obj);
	}

	template<typename F>
	static bool visit_carray(opaque_obj* obj, const CArrayInfo& info, F f) {
		bool edited{};
		if (ImGui::TreeNode(currentMemberName, "%s[0..%zd]", currentMemberName, info.size - 1)) {
			for (size_t i = 0; i < info.size; i++) {
				char name[200] = "";
				snprintf(name, 200, "%s[%zd]", currentMemberName, i);

				edited |= NameScope(name, [&]() {
					ImGui::PushID(static_cast<int>(i));
					bool itemEdited = f(*reinterpret_cast<opaque_obj*>(reinterpret_cast<size_t>(obj) + info.stride * i));
					ImGui::PopID();
					return itemEdited;
				});
			}

			ImGui::TreePop();
		}
		return edited;
	}

	template<typename F>
	static bool visit_type(opaque_obj& obj, const TypeInfo& info, F f) {
		return f(obj);
	}

	template<typename F>
	static bool visit_field(opaque_obj& obj, const FieldInfo& info, F f) {
		if (currentRflClass != nullptr)
			for (auto& member : currentRflClass->GetMembers())
				if (!strcmp(member.GetName(), info.name) && !Matches(member))
					return false;
		return NameScope(info.name, [&]() { return f(obj); });
	}

	template<typename F>
	static bool visit_base_struct(opaque_obj& obj, const StructureInfo& info, F f) {
		return f(obj);
	}

	template<typename F>
	static bool visit_struct(opaque_obj& obj, const StructureInfo& info, F f) {
		ImGui::PushID(&obj);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(currentMemberName, RenderStaticReflectionEditor::defaultOpen || FilterActive() ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };
		const auto* rflClass = static_cast<const RflClass*>(info.rflClass);

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && &dndData.obj != &obj) {
					ucsl::reflection::traversals::traversal<ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>>{}(obj, dndData.obj, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (ImGui::Selectable("Reset to default values")) {
				const hh::fnd::RflTypeInfo* typeInfo = hh::fnd::RflTypeInfoRegistry::GetInstance()->GetTypeInfo(rflClass->GetName());
				typeInfo->CleanupLoadedObject(&obj);
				typeInfo->ConstructObject(&obj, hh::fnd::MemoryRouter::GetModuleAllocator());
				edited = true;
			}
			if (ImGui::Selectable("Open in memory viewer"))
				new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), &obj, rflClass->GetSize() };
			ImGui::EndPopup();
		}
		
		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			auto* savedRflClass = currentRflClass; currentRflClass = rflClass;
			edited = f(obj);
			currentRflClass = savedRflClass;
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}

	template<typename F>
	static bool visit_root(opaque_obj& obj, const RootInfo& info, F f) {
		return f(obj);
	}
};

class RenderResettableReflectionEditor {
public:
	constexpr static size_t arity = 2;
	typedef bool result_type;

	template<typename T>
	static bool visit_primitive(T& obj, T& orig, const PrimitiveInfo<T>& info) { return RenderStaticReflectionEditor::visit_primitive(obj, info); }

	template<bool change_allocator, typename F, typename A, typename C, typename D>
	static bool _visit_array(A& arr, A& orig, C c, D d, F f) {
		size_t idx{};
		opaque_obj* emptyElement{};

		bool edited = RenderStaticReflectionEditor::_visit_array<change_allocator>(arr, c, d, [&](opaque_obj& obj) {
			if (idx >= orig.size() && emptyElement == nullptr)
				emptyElement = c();

			bool edited = f(obj, idx < orig.size() ? orig[idx] : *emptyElement);

			idx++;

			return edited;
		});

		if (emptyElement)
			d(emptyElement);

		return edited;
	}

	template<typename F, typename A, typename C, typename D>
	static bool visit_array(A& arr, A& orig, const ArrayInfo& info, const ArrayInfo& infoOrig, C c, D d, F f) {
		return _visit_array<true>(arr, orig, c, d, f);
	}

	template<typename F, typename A, typename C, typename D>
	static bool visit_tarray(A & arr, A & orig, const ArrayInfo & info, const ArrayInfo & infoOrig, C c, D d, F f) {
		return _visit_array<false>(arr, orig, c, d, f);
	}

	template<typename T, typename O>
	static bool visit_enum(T& obj, T& orig, const EnumInfo<O>& info) {
		return RenderStaticReflectionEditor::visit_enum(obj, info);
	}

	template<typename T, typename O>
	static bool visit_flags(T& obj, T& orig, const FlagsInfo<O>& info) {
		return RenderStaticReflectionEditor::visit_flags(obj, info);
	}

	template<typename F, typename A1, typename S1, typename A2, typename S2>
	static bool visit_pointer(opaque_obj*& obj, opaque_obj*& orig, const PointerInfo<A1, S1>& info, const PointerInfo<A2, S2>& infoOrig, F f) {
		return RenderStaticReflectionEditor::visit_pointer(obj, info, [&](opaque_obj& obj) { return f(obj, *orig); });
	}

	template<typename F>
	static bool visit_carray(opaque_obj* obj, opaque_obj* orig, const CArrayInfo& info, const CArrayInfo& infoOrig, F f) {
		return RenderStaticReflectionEditor::visit_carray(obj, info, [&](opaque_obj& item) { return f(item, *reinterpret_cast<opaque_obj*>(reinterpret_cast<size_t>(orig) + infoOrig.stride * (&item - obj) / info.stride)); });
	}

	template<typename F>
	static bool visit_type(opaque_obj& obj, opaque_obj& orig, const TypeInfo& info, const TypeInfo& infoOrig, F f) {
		return RenderStaticReflectionEditor::visit_type(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool visit_field(opaque_obj& obj, opaque_obj& orig, const FieldInfo& info, F f) {
		return RenderStaticReflectionEditor::visit_field(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool visit_base_struct(opaque_obj& obj, opaque_obj& orig, const StructureInfo& info, F f) {
		return RenderStaticReflectionEditor::visit_base_struct(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
	}

	template<typename F>
	static bool visit_struct(opaque_obj& obj, opaque_obj& orig, const StructureInfo& info, F f) {
		ImGui::PushID(&obj);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(RenderStaticReflectionEditor::currentMemberName, RenderStaticReflectionEditor::defaultOpen || RenderStaticReflectionEditor::FilterActive() ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };
		const auto* rflClass = static_cast<const RflClass*>(info.rflClass);

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && &dndData.obj != &obj) {
					ucsl::reflection::traversals::traversal<ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>>{}(obj, dndData.obj, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (ImGui::Selectable("Reset to cached values")) {
				ucsl::reflection::traversals::traversal<ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>>{}(obj, orig, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
				edited = true;
			}

			if (ImGui::Selectable("Reset to default values")) {
				const hh::fnd::RflTypeInfo* typeInfo = hh::fnd::RflTypeInfoRegistry::GetInstance()->GetTypeInfo(rflClass->GetName());
				typeInfo->CleanupLoadedObject(&obj);
				typeInfo->ConstructObject(&obj, hh::fnd::MemoryRouter::GetModuleAllocator());
				edited = true;
			}

			if (ImGui::Selectable("Open in memory viewer"))
				new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), &obj, rflClass->GetSize() };

			ImGui::EndPopup();
		}
		
		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			auto* savedRflClass = RenderStaticReflectionEditor::currentRflClass; RenderStaticReflectionEditor::currentRflClass = rflClass;
			edited |= f(obj, orig);
			RenderStaticReflectionEditor::currentRflClass = savedRflClass;
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}

	template<typename F>
	static bool visit_root(opaque_obj& obj, opaque_obj& orig, const RootInfo& info, const RootInfo& infoOrig, F f) {
		return f(obj, orig);
	}
};

bool ReflectionEditor(const char* label, void* reflectionData, const hh::fnd::RflClass* rflClass, bool defaultOpen, const ImGuiTextFilter* filter) {
	ImGui::BeginGroup();
	RenderStaticReflectionEditor::defaultOpen = defaultOpen;
	RenderStaticReflectionEditor::filter = filter;
	bool edited = RenderStaticReflectionEditor::NameScope(label, [&]() { return ucsl::reflection::traversals::traversal<RenderStaticReflectionEditor>{}(reflectionData, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass)); });
	RenderStaticReflectionEditor::filter = nullptr;
	ImGui::EndGroup();
	return edited;
}

bool ResettableReflectionEditor(const char* label, void* reflectionData, void* originalReflectionData, const hh::fnd::RflClass* rflClass, const ImGuiTextFilter* filter) {
	ImGui::BeginGroup();
	RenderStaticReflectionEditor::defaultOpen = false;
	RenderStaticReflectionEditor::filter = filter;
	bool edited = RenderStaticReflectionEditor::NameScope(label, [&]() { return ucsl::reflection::traversals::traversal<RenderResettableReflectionEditor>{}(reflectionData, originalReflectionData, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass)); });
	RenderStaticReflectionEditor::filter = nullptr;
	ImGui::EndGroup();
	return edited;
}
