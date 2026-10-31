#include "Reflection.h"
#include <imgui_internal.h>
#include <ucsl/rfl/ranges.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/accessors/binary.h>
#include <ucsl-reflection/algorithms/copy.h>
#include <ucsl-reflection/accessors/binary.h>
#include <ui/common/Translations.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/tools/MemoryViewer.h>
#include <ui/Desktop.h>

using namespace hh::fnd;
using namespace ucsl::reflection;
using namespace ucsl::rfl::ranges;

struct RflDragDropData {
	const he2sdk::ucsl::GameInterface::RflSystem::RflClass* rflClass;
	const void* obj;
};

class ReflectionEditorBuilder {
	static inline const char* currentMemberName{};

	template<typename T> static float rfl_step(const range_value_t<T>& rangeValue) { return std::fmaxf(rflMinFloatStep, static_cast<float>(rangeValue)); }
	template<> float rfl_step<RangeVector2>(const range_value_t<RangeVector2>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }
	template<> float rfl_step<RangeVector3>(const range_value_t<RangeVector3>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }
	template<> float rfl_step<RangeVector4>(const range_value_t<RangeVector4>& rangeValue) { return rfl_step<RangeFloat>(rangeValue.x); }


	template<typename T, bool allowSliders = true>
	static bool InputRflScalar(T& obj, const range_t<T>* range) {
		if (range) {
			auto rangeMin = static_cast<T>(range->min);
			auto rangeMax = static_cast<T>(range->max);
			auto rangeStep = range->step;

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

public:
	static inline bool defaultOpen{};

	template<typename F>
	static bool NameScope(const char* memberName, F f) {
		const char* savedMemberName = currentMemberName;
		currentMemberName = memberName;
		bool edited = f();
		currentMemberName = savedMemberName;
		return edited;
	}

	static bool PrimitiveData(auto& obj) {
		if constexpr (std::is_arithmetic_v<typename decltype(obj.refl)::repr> && !std::is_same_v<typename decltype(obj.refl)::repr, bool>) return InputRflScalar<typename decltype(obj.refl)::repr>(*obj.value, obj.refl.range);
		else if constexpr (std::is_same_v<typename decltype(obj.refl)::repr, ucsl::math::Vector2> || std::is_same_v<typename decltype(obj.refl)::repr, ucsl::math::Vector3> || std::is_same_v<typename decltype(obj.refl)::repr, ucsl::math::Vector4>) return InputRflScalar<typename decltype(obj.refl)::repr, false>(*obj.value, obj.refl.range);
		else if constexpr (std::is_same_v<typename decltype(obj.refl)::repr, const char*>) { ImGui::Text("%s: %s", currentMemberName, *obj.value); return false; }
		else return Editor(currentMemberName, *obj.value);
	}

	static bool Enum(auto& obj) {
		bool edited{};
		const char* previewValue = "<INVALID VALUE>";
		long long currentValue = static_cast<long long>(obj);

		for (auto& option : obj.refl.get_options())
			if (currentValue == option.GetIndex())
				previewValue = option.GetEnglishName();

		if (ImGui::BeginCombo(currentMemberName, previewValue)) {
			for (auto option : obj.refl.get_options()) {
				long long index = option.GetIndex();
				bool is_selected = currentValue == index;

				if (ImGui::Selectable(option.GetEnglishName(), is_selected)) {
					obj = index;
					edited = true;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		return edited;
	}

	static bool Bitfield(auto& obj) {
		return obj.visit([&](auto& underlying) {
			typename decltype(underlying.refl)::repr bitfield{ underlying };
			bool edited{};

			if (ImGui::TreeNode(currentMemberName)) {
				obj.refl.visit_components([&](const auto& component) {
					size_t mask{ (1 << component.get_width()) - 1 };
					size_t value{ (bitfield >> component.get_offset()) & mask };

					bool ed = NameScope(component.get_name(), [&]() {
						auto componentType = component.get_type();

						ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(componentType)> acc{ &value, componentType };

						return acc.visit([&](auto& v) {
							if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return v.visit([&](auto& pd) { return PrimitiveData(pd); });
							else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::ENUM) return Enum(v);
							else return false;
						});
					});

					if (ed) {
						typename decltype(underlying.refl)::repr mask{ (1 << component.get_width()) - 1 };
						typename decltype(underlying.refl)::repr value_in_underlying = *reinterpret_cast<typename decltype(underlying.refl)::repr*>(&value);

						bitfield = (bitfield & ~(mask << component.get_offset())) | ((value_in_underlying & mask) << component.get_offset());
						edited = true;
					}
				});
				ImGui::TreePop();
			}

			return edited;
		});
	}

	static bool Array(auto& arr, auto f) {
		bool edited{};
		if (ImGui::TreeNode(currentMemberName, "%s[0..]", currentMemberName)) {
			for (int i = 0; i < arr.size(); i++) {
				ImGui::PushID(i);
				if (ImGui::Button("x"))
					arr.remove(i);
				ImGui::SameLine();

				char indexedName[200];
				snprintf(indexedName, sizeof(indexedName), "%s[%d]", currentMemberName, i);

				edited |= NameScope(indexedName, [&]() { auto item = arr[i]; return f(item, i); });

				ImGui::PopID();
			}

			if (ImGui::Button("Add item")) {
				if constexpr (decltype(arr.refl)::kind == ucsl::reflection::providers::TypeKind::ARRAY)
					if (arr.size() == arr.capacity() && arr.get_allocator() == nullptr)
						arr.change_allocator(hh::fnd::MemoryRouter::GetModuleAllocator());
				arr.emplace_back();
			}

			ImGui::TreePop();
		}
		return edited;
	}

	static bool CArray(accessors::CArrayAccessor auto& obj, auto f) {
		bool edited{};
		if (ImGui::TreeNode(currentMemberName, "%s[0..%zd]", currentMemberName, obj.size() - 1)) {
			for (size_t i = 0; i < obj.size(); i++) {
				char name[200] = "";
				snprintf(name, 200, "%s[%zd]", currentMemberName, i);

				edited |= NameScope(name, [&]() {
					ImGui::PushID(static_cast<int>(i));
					auto item = obj[i];
					bool itemEdited = f(item, i);
					ImGui::PopID();
					return itemEdited;
				});
			}

			ImGui::TreePop();
		}
		return edited;
	}

	static bool Field(auto& obj, const auto& refl, auto f) {
		return NameScope(refl.get_name(), [&]() { return f(obj); });
	}

	static bool Structure(auto& obj, auto f) {
		ImGui::PushID(obj.value);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(currentMemberName, defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };
		const auto* rflClass = (const he2sdk::ucsl::GameInterface::RflSystem::RflClass*)obj.refl.rflClass;

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj.value };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && dndData.obj != obj.value) {
					ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::Structure refl{ rflClass };
					ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::StructureAccessor<decltype(refl)> origAcc{ dndData.obj, refl };
					ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>::process_struct(obj, origAcc);
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (ImGui::Selectable("Reset to default values")) {
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::destroy(obj.refl, obj.value);
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::construct(obj.refl, hh::fnd::MemoryRouter::GetModuleAllocator(), obj.value);
				edited = true;
			}
			if (ImGui::Selectable("Open in memory viewer"))
				new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), obj.value, rflClass->GetSize() };
			ImGui::EndPopup();
		}
		
		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			edited = f(obj);
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}

	static bool Structure(auto& obj, const auto& orig, auto f) {
		ImGui::PushID(obj.value);
		bool edited{};
		bool isOpen{ ImGui::TreeNodeEx(currentMemberName, defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };
		const auto* rflClass = (const he2sdk::ucsl::GameInterface::RflSystem::RflClass*)obj.refl.rflClass;

		if (ImGui::BeginDragDropSource()) {
			RflDragDropData dndData{ rflClass, obj.value };
			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);

				if (dndData.rflClass == rflClass && dndData.obj != obj.value) {
					ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::Structure refl{ rflClass };
					ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::StructureAccessor<decltype(refl)> origAcc{ dndData.obj, refl };
					ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>::process_struct(obj, origAcc);
					edited = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem("RFL Operations")) {
			if (orig.has_value() && ImGui::Selectable("Reset to cached values")) {
				ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>::process_struct(obj, orig.value());
				edited = true;
			}
			if (ImGui::Selectable("Reset to default values")) {
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::destroy(obj.refl, obj.value);
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::construct(obj.refl, hh::fnd::MemoryRouter::GetModuleAllocator(), obj.value);
				edited = true;
			}
			if (ImGui::Selectable("Open in memory viewer"))
				new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), obj.value, rflClass->GetSize() };
			ImGui::EndPopup();
		}

		if (isOpen) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			edited = f(obj);
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}
		ImGui::PopID();
		return edited;
	}
};

class RenderStaticReflectionEditor {
public:
	static bool process_primitive_data(auto& obj) { return ReflectionEditorBuilder::PrimitiveData(obj); }
	static bool process_primitive(auto& obj) { return obj.visit([&](auto& data) { return process_primitive_data(data); }); }
	static bool process_enum(auto& obj) { return ReflectionEditorBuilder::Enum(obj); }
	static bool process_bitfield(auto& obj) { return ReflectionEditorBuilder::Bitfield(obj); }
	static bool process_array(auto& obj) { return ReflectionEditorBuilder::Array(obj, [&](auto& item, size_t i) { return process_type(item); }); }
	static bool process_carray(auto& obj) { return ReflectionEditorBuilder::CArray(obj, [&](auto& item, size_t i) { return process_type(item); }); }
	static bool process_pointer(auto& obj) {
		if (obj == nullptr)
			return false;
		else {
			auto target = *obj;

			return process_type(target);
		}
	}
	static bool process_field(auto& obj, const auto& refl) { return ReflectionEditorBuilder::Field(obj, refl, [&](auto& val) { return process_type(val); }); }
	static bool process_union(auto& obj) { return obj.visit([&](auto& field) { return process_type(field); }); }
	static bool process_fields(auto& obj) {
		bool edited{};
		auto base = obj.get_base();

		if (base.has_value())
			edited |= process_fields(base.value());

		obj.visit_fields([&](auto& field, const auto& fieldRefl) {
			edited |= process_field(field, fieldRefl);
		});

		return edited;
	}
	static bool process_struct(auto& obj) { return ReflectionEditorBuilder::Structure(obj, [&](auto& val) { return process_fields(val); }); }
	static bool process_type(auto& obj) {
		return obj.visit([&](auto& v) {
			if constexpr (decltype(v.refl)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ENUM) return process_enum(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::BITFIELD) return process_bitfield(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ARRAY) return process_array(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::TARRAY) return process_array(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::CARRAY) return process_carray(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::POINTER) return process_pointer(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::UNION) return process_union(v);
			else if constexpr (decltype(v.refl)::kind == providers::TypeKind::STRUCTURE) return process_struct(v);
			else static_assert(false, "invalid type kind");
		});
	}

public:
	static bool process_root(const char* label, accessors::ValueAccessor auto& obj, bool defaultOpen) {
		ReflectionEditorBuilder::defaultOpen = defaultOpen;
		return ReflectionEditorBuilder::NameScope(label, [&]() { return process_type(obj); });
	}
};

class RenderResettableReflectionEditor {
public:
	static bool process_primitive_data(auto& obj) { return ReflectionEditorBuilder::PrimitiveData(obj); }
	static bool process_primitive(auto& obj) { return obj.visit([&](auto& data) { return process_primitive_data(data); }); }
	static bool process_enum(auto& obj) { return ReflectionEditorBuilder::Enum(obj); }
	static bool process_bitfield(auto& obj) { return ReflectionEditorBuilder::Bitfield(obj); }
	static bool process_array(auto& obj, const auto& orig) { return ReflectionEditorBuilder::Array(obj, [&](auto& item, size_t i) { return process_type(item, orig.has_value() && i < orig.value().size() ? std::make_optional(orig.value()[i]) : std::optional<std::decay_t<decltype(orig.value()[i])>>{}); }); }
	static bool process_carray(auto& obj, const auto& orig) { return ReflectionEditorBuilder::CArray(obj, [&](auto& item, size_t i) { return process_type(item, orig.has_value() && i < orig.value().size() ? std::make_optional(orig.value()[i]) : std::optional<std::decay_t<decltype(orig.value()[i])>>{}); }); }
	static bool process_pointer(auto& obj, const auto& orig) {
		if (obj == nullptr)
			return false;
		else {
			auto target = *obj;

			return process_type(target, orig.has_value() ? std::make_optional(*orig.value()) : std::optional<std::decay_t<decltype(*orig.value())>>{});
		}
	}
	static bool process_field(auto& obj, const auto& orig, const auto& refl) { return ReflectionEditorBuilder::Field(obj, refl, [&](auto& val) { return process_type(val, orig); }); }
	static bool process_union(auto& obj, const auto& orig) {
		return obj.visit([&](auto& field) {
			if (orig.has_value()) return orig.value().visit([&](const auto& origField) { return process_type(field, std::make_optional(origField)); });
			else return process_type(field, std::optional<std::decay_t<decltype(field)>>{});
		});
	}
	static bool process_fields(auto& obj, const auto& orig) {
		bool edited{};
		auto base = obj.get_base();

		if (base.has_value())
			edited |= process_fields(base.value(), orig.has_value() ? orig.value().get_base().value() : std::optional<std::decay_t<decltype(orig.value().get_base().value())>>{});

		obj.refl.template visit_fields<size_t>(obj, [&](const auto& fieldRefl) {
			auto field = obj[fieldRefl];

			edited |= process_field(field, orig.has_value() ? orig.value()[fieldRefl] : std::optional<std::decay_t<decltype(orig.value()[fieldRefl])>>{}, fieldRefl);
		});

		return edited;
	}
	static bool process_struct(auto& obj, const auto& orig) { return ReflectionEditorBuilder::Structure(obj, orig, [&](auto& val) { return process_fields(val, orig); }); }
	static bool process_type(auto& obj, const auto& orig) {
		return obj.visit([&](auto& v) {
			if (orig.has_value()) {
				orig.value().visit([&](const auto& origV) {
					if constexpr (!std::is_same_v<std::decay_t<decltype(v.refl)>, std::decay_t<decltype(origV.refl)>>) return false;
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(v);
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ENUM) return process_enum(v);
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::BITFIELD) return process_bitfield(v);
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ARRAY) return process_array(v, std::make_optional(origV));
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::TARRAY) return process_array(v, std::make_optional(origV));
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::CARRAY) return process_carray(v, std::make_optional(origV));
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::POINTER) return process_pointer(v, std::make_optional(origV));
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::UNION) return process_union(v, std::make_optional(origV));
					else if constexpr (decltype(v.refl)::kind == providers::TypeKind::STRUCTURE) return process_struct(v, std::make_optional(origV));
					else static_assert(false, "invalid type kind");
				});
			}
			else {
				if constexpr (decltype(v.refl)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(v);
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ENUM) return process_enum(v);
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::BITFIELD) return process_bitfield(v);
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::ARRAY) return process_array(v, std::optional<std::decay_t<decltype(v)>>{});
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::TARRAY) return process_array(v, std::optional<std::decay_t<decltype(v)>>{});
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::CARRAY) return process_carray(v, std::optional<std::decay_t<decltype(v)>>{});
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::POINTER) return process_pointer(v, std::optional<std::decay_t<decltype(v)>>{});
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::UNION) return process_union(v, std::optional<std::decay_t<decltype(v)>>{});
				else if constexpr (decltype(v.refl)::kind == providers::TypeKind::STRUCTURE) return process_struct(v, std::optional<std::decay_t<decltype(v)>>{});
				else static_assert(false, "invalid type kind");
			}
		});
	}

public:
	static bool process_root(const char* label, accessors::ValueAccessor auto& obj, accessors::ValueAccessor auto& orig, bool defaultOpen) {
		ReflectionEditorBuilder::defaultOpen = defaultOpen;
		return ReflectionEditorBuilder::NameScope(label, [&]() { return process_type(obj, std::make_optional(orig)); });
	}
};

//class RenderResettableReflectionEditor {
//public:
//	static bool process_primitive(auto& obj) { return RenderStaticReflectionEditor::process_primitive(obj); }
//
//	template<bool change_allocator, typename F, typename A, typename C, typename D>
//	static bool _process_array(A& arr, A& orig, C c, D d, F f) {
//		size_t idx{};
//		opaque_obj* emptyElement{};
//
//		bool edited = RenderStaticReflectionEditor::_process_array<change_allocator>(arr, c, d, [&](opaque_obj& obj) {
//			if (idx >= orig.size() && emptyElement == nullptr)
//				emptyElement = c();
//
//			bool edited = f(obj, idx < orig.size() ? orig[idx] : *emptyElement);
//
//			idx++;
//
//			return edited;
//		});
//
//		if (emptyElement)
//			d(emptyElement);
//
//		return edited;
//	}
//
//	template<typename F, typename A, typename C, typename D>
//	static bool process_array(A& arr, A& orig, const ArrayInfo& info, const ArrayInfo& infoOrig, C c, D d, F f) {
//		return _process_array<true>(arr, orig, c, d, f);
//	}
//
//	template<typename F, typename A, typename C, typename D>
//	static bool process_tarray(A & arr, A & orig, const ArrayInfo & info, const ArrayInfo & infoOrig, C c, D d, F f) {
//		return _process_array<false>(arr, orig, c, d, f);
//	}
//
//	template<typename T, typename O>
//	static bool process_enum(T& obj, T& orig, const EnumInfo<O>& info) {
//		return RenderStaticReflectionEditor::process_enum(obj, info);
//	}
//
//	template<typename T, typename O>
//	static bool process_flags(T& obj, T& orig, const FlagsInfo<O>& info) {
//		return RenderStaticReflectionEditor::process_flags(obj, info);
//	}
//
//	template<typename F, typename A1, typename S1, typename A2, typename S2>
//	static bool process_pointer(opaque_obj*& obj, opaque_obj*& orig, const PointerInfo<A1, S1>& info, const PointerInfo<A2, S2>& infoOrig, F f) {
//		return RenderStaticReflectionEditor::process_pointer(obj, info, [&](opaque_obj& obj) { return f(obj, *orig); });
//	}
//
//	template<typename F>
//	static bool process_carray(opaque_obj* obj, opaque_obj* orig, const CArrayInfo& info, const CArrayInfo& infoOrig, F f) {
//		return RenderStaticReflectionEditor::process_carray(obj, info, [&](opaque_obj& item) { return f(item, *reinterpret_cast<opaque_obj*>(reinterpret_cast<size_t>(orig) + infoOrig.stride * (&item - obj) / info.stride)); });
//	}
//
//	template<typename F>
//	static bool process_type(opaque_obj& obj, opaque_obj& orig, const TypeInfo& info, const TypeInfo& infoOrig, F f) {
//		return RenderStaticReflectionEditor::process_type(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
//	}
//
//	template<typename F>
//	static bool process_field(opaque_obj& obj, opaque_obj& orig, const FieldInfo& info, F f) {
//		return RenderStaticReflectionEditor::process_field(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
//	}
//
//	template<typename F>
//	static bool process_base_struct(opaque_obj& obj, opaque_obj& orig, const StructureInfo& info, F f) {
//		return RenderStaticReflectionEditor::process_base_struct(obj, info, [&](opaque_obj& obj) { return f(obj, orig); });
//	}
//
//	template<typename F>
//	static bool process_struct(opaque_obj& obj, opaque_obj& orig, const StructureInfo& info, F f) {
//		ImGui::PushID(&obj);
//		bool edited{};
//		bool isOpen{ ImGui::TreeNodeEx(RenderStaticReflectionEditor::currentMemberName, RenderStaticReflectionEditor::defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None) };
//		const auto* rflClass = (const he2sdk::ucsl::GameInterface::RflSystem::RflClass*)info.rflClass;
//
//		if (ImGui::BeginDragDropSource()) {
//			RflDragDropData dndData{ rflClass, obj };
//			ImGui::SetDragDropPayload("RflData", &dndData, sizeof(dndData));
//			ImGui::EndDragDropSource();
//		}
//
//		if (ImGui::BeginDragDropTarget()) {
//			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RflData")) {
//				RflDragDropData& dndData = *static_cast<RflDragDropData*>(payload->Data);
//
//				if (dndData.rflClass == rflClass && &dndData.obj != &obj) {
//					ucsl::reflection::traversals::traversal<ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>>{}(obj, dndData.obj, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
//					edited = true;
//				}
//			}
//			ImGui::EndDragDropTarget();
//		}
//
//		if (ImGui::BeginPopupContextItem("RFL Operations")) {
//			if (ImGui::Selectable("Reset to cached values")) {
//				ucsl::reflection::traversals::traversal<ucsl::reflection::algorithms::Copy<he2sdk::ucsl::GameInterface>>{}(obj, orig, ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::reflect(rflClass));
//				edited = true;
//			}
//
//			if (ImGui::Selectable("Reset to default values")) {
//				const hh::fnd::RflTypeInfo* typeInfo = hh::fnd::RflTypeInfoRegistry::GetInstance()->GetTypeInfo(rflClass->GetName());
//				typeInfo->CleanupLoadedObject(&obj);
//				typeInfo->ConstructObject(&obj, hh::fnd::MemoryRouter::GetModuleAllocator());
//				edited = true;
//			}
//
//			if (ImGui::Selectable("Open in memory viewer"))
//				new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), &obj, rflClass->GetSize() };
//
//			ImGui::EndPopup();
//		}
//		
//		if (isOpen) {
//			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
//			edited |= f(obj, orig);
//			ImGui::PopItemWidth();
//			ImGui::TreePop();
//		}
//		ImGui::PopID();
//		return edited;
//	}
//
//	template<typename F>
//	static bool process_root(opaque_obj& obj, opaque_obj& orig, const RootInfo& info, const RootInfo& infoOrig, F f) {
//		return f(obj, orig);
//	}
//};

bool ReflectionEditor(const char* label, void* reflectionData, const hh::fnd::RflClass* rflClass, bool defaultOpen) {
	ImGui::BeginGroup();
	ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::RootType refl{ rflClass };
	ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> acc{ reflectionData, refl };
	bool edited = RenderStaticReflectionEditor::process_root(label, acc, defaultOpen);
	ImGui::EndGroup();
	return edited;
}

bool ResettableReflectionEditor(const char* label, void* reflectionData, const void* originalReflectionData, const hh::fnd::RflClass* rflClass) {
	ImGui::BeginGroup();
	ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::RootType refl{ rflClass };
	ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> acc{ reflectionData, refl };
	ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> origAcc{ originalReflectionData, refl };
	bool edited = RenderResettableReflectionEditor::process_root(label, acc, origAcc, false);
	ImGui::EndGroup();
	return edited;
}
