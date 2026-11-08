#include "RflDiff.h"
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/accessors/binary.h>
#include <ui/common/viewers/Basic.h>
#include <sstream>
#include <regex>

class RenderRflDiffChange {
public:
	inline static RflDiffChange* change;
	inline static int64_t pathIdx;

	static void process_primitive_data_(const auto& obj) {
		if (pathIdx >= 0)
			ImGui::Text("An error occurred.");

		Viewer("Old value", obj);
		Viewer("New value", *static_cast<const std::decay_t<decltype(obj)>*>(change->value));
	}

	static void process_primitive_data(const auto& obj) {
		process_primitive_data_(*obj.value);
	}

	static void process_primitive(const auto& obj) {
		obj.visit([&](const auto& data) { process_primitive_data(data); });
	}

	static void process_array(const auto& arr) {
		auto item = arr[change->path[pathIdx].arrayIdx];

		pathIdx--;
		process_type(item);
		pathIdx++;
	}

	static void process_enum(const auto& obj) {
		obj.refl.visit([&](const auto& r) { process_primitive_data_(*(const typename std::decay_t<decltype(r)>::repr*)obj.value); });
	}

	static void process_bitfield(const auto& obj) {
		obj.visit([&](const auto& val) { process_primitive_data(val); });
	}

	static void process_pointer(const auto& obj) {
		if (obj != nullptr)
			process_type(*obj);
	}

	static void process_carray(const auto& obj) {
		auto item = obj[change->path[pathIdx].arrayIdx];

		pathIdx--;
		process_type(item);
		pathIdx++;
	}

	static void process_union(const auto& obj) {
		obj.visit([&](const auto& field) { process_type(field); });
	}

	static void process_field(const auto& obj, const auto& refl) {
		if (strcmp(refl.get_name(), change->path[pathIdx].propertyName))
			return;

		pathIdx--;
		process_type(obj);
		pathIdx++;
	}

	static void process_fields(const auto& obj) {
		auto base = obj.get_base();

		if (base.has_value())
			process_fields(base.value());

		obj.visit_fields([&](const auto& field, const auto& fieldRefl) {
			process_field(field, fieldRefl);
		});
	}

	static void process_struct(const auto& obj) {
		process_fields(obj);
	}

	static void process_type(const auto& obj) {
		obj.visit([&](const auto& v) {
			if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) process_primitive(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::ENUM) process_enum(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) process_bitfield(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::ARRAY) process_array(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::TARRAY) process_array(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::CARRAY) process_carray(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::POINTER) process_pointer(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::UNION) process_union(v);
			else if constexpr (decltype(v.refl)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) process_struct(v);
			else static_assert(false, "invalid type kind");
		});
	}

	static void process_root(const auto& obj) {
		process_type(obj);
	}
};


void Viewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass) {
	for (auto& change : diff.changes) {
		char pathBuf[500];
		size_t idx{};

		for (size_t i = change.path.size(); i > 0; i--) {
			auto& pathEl = change.path[i - 1];
			if (pathEl.type == RflDiffChange::Index::Type::INDEX) {
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "%zd", pathEl.arrayIdx);
			}
			else {
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "%s", pathEl.propertyName);
			}
			
			if (i > 1)
				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ".");
		}
		
		if (change.type == RflDiffChange::Type::UPDATE) {
			if (ImGui::TreeNode(pathBuf, "UPDATED: %s", pathBuf)) {
				RenderRflDiffChange::change = &change;
				RenderRflDiffChange::pathIdx = change.path.size() - 1;
				ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::RootType refl{ rflClass };
				ucsl::reflection::accessors::binary<he2sdk::ucsl::GameInterface>::ValueAccessor<decltype(refl)> acc{ obj, refl };
				RenderRflDiffChange::process_root(acc);
				ImGui::TreePop();
			}
		}
		else
			ImGui::Text("%s: %s", change.type == RflDiffChange::Type::ADD ? "ADDED" : "REMOVED", pathBuf);
	}
}

//std::regex escapes{ "\\" };
//std::regex quotes{ "\"" };
//
//class RenderRflDiffChangeRfl2HMM {
//public:
//	constexpr static size_t arity = 1;
//	typedef bool result_type;
//	inline static RflDiffChange* change;
//	inline static int64_t pathIdx;
//
//	struct RFL2HMMParamValue {
//		std::string str{};
//
//		RFL2HMMParamValue& operator|=(RFL2HMMParamValue&& other) {
//			str = std::move(other.str);
//		}
//	};
//
//	static RFL2HMMParamValue VisitPrimitive(const char* obj) {
//		RFL2HMMParamValue val{};
//		std::string sval{ obj };
//		std::ostringstream oss{};
//
//		sval = std::regex_replace(sval, escapes, "\\\\");
//		sval = std::regex_replace(sval, quotes, "\\\"");
//
//		oss << "\"" << sval << "\"";
//
//		val.str = std::move(oss.str());
//
//		return std::move(val);
//	}
//
//	static RFL2HMMParamValue VisitPrimitive(csl::ut::VariableString* obj) {
//		return VisitPrimitive(obj->c_str());
//	}
//
//	template<typename T>
//	static RFL2HMMParamValue VisitPrimitive(T* obj) {
//		RFL2HMMParamValue val{};
//		std::ostringstream oss{};
//
//		oss << *obj;
//		val.str = std::move(oss.str());
//
//		return std::move(val);
//	}
//
//	template<typename F, typename C, typename D>
//	static RFL2HMMParamValue VisitArray(RflMoveArrayAccessor& arr, C c, D d, F f) {
//		void* obj = arr[change->path[pathIdx].arrayIdx];
//		pathIdx--;
//		RFL2HMMParamValue&& val = f(obj);
//		pathIdx++;
//		return val;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitEnum(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflClassEnum* enumClass, F f) {
//		return f(obj);
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitFlags(void* obj, hh::fnd::RflClassMember::Type type, const hh::fnd::RflArray<const hh::fnd::RflClassEnumMember>* enumEntries, F f) {
//		return f(obj);
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitClassMember(void* obj, const hh::fnd::RflClassMember* member, F f) {
//		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitArrayClassMember(void* obj, const hh::fnd::RflClassMember* member, size_t size, F f) {
//		if (strcmp(member->GetName(), change->path[pathIdx].propertyName) == 0) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitArrayClassMemberItem(void* obj, size_t idx, F f) {
//		if (idx == change->path[pathIdx].arrayIdx) {
//			pathIdx--;
//			f(obj);
//			pathIdx++;
//		}
//		return false;
//	}
//
//	template<typename F>
//	static RFL2HMMParamValue VisitStruct(void* obj, const hh::fnd::RflClass* rflClass, F f) {
//		f(obj);
//		return false;
//	}
//
//};
//
//
//void Rfl2HmmViewer(const char* label, const RflDiffResult& diff, void* obj, const hh::fnd::RflClass* rflClass)
//{
//	RenderRflDiffChangeRfl2HMM::code = std::string{};
//	for (auto& change : diff.changes) {
//		char pathBuf[500];
//		size_t idx{};
//
//		idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "RFL_SET_PARAM(%s, ", pathEl.arrayIdx);
//		for (size_t i = change.path.size(); i > 0; i--) {
//			auto& pathEl = change.path[i - 1];
//			if (pathEl.type == RflDiffChange::Index::Type::INDEX) {
//				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, "[%zd]", pathEl.arrayIdx);
//			}
//			else {
//				idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ".%s", pathEl.propertyName);
//			}
//		}
//		idx += snprintf(&pathBuf[idx], sizeof(pathBuf) - idx, ", ", pathEl.arrayIdx);
//
//		if (change.type == RflDiffChange::Type::UPDATE) {
//			if (ImGui::TreeNode(pathBuf, "UPDATED: %s", pathBuf)) {
//				RenderRflDiffChange::change = &change;
//				RenderRflDiffChange::pathIdx = change.path.size() - 1;
//				rflops::traversals::rflop<RenderRflDiffChange>::Apply(obj, rflClass);
//				ImGui::TreePop();
//			}
//		}
//	}
//}
