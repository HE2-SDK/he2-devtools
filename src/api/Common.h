#pragma once
#include <ucsl-reflection/traversals/traversal.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <rip/util/memory.h>
#include <rip/util/object-id-guids.h>
#include <rip/binary/stream.h>
#include <rip/binary/serialization2/json.h>
#include <rip/binary/accessors/binary-stream.h>
#include <rfl.hpp>
#include <rfl/json.hpp>

namespace devtools::api {
	struct ErrorResponse {
		std::string status{};
		std::string message{};
	};

	struct Empty {};

	struct Vector3 {
		float x{}; float y{}; float z{};
	};

	struct Vector4 {
		float x{}; float y{}; float z{}; float w{};
	};

	struct Quaternion {
		float x{}; float y{}; float z{}; float w{};
	};

	struct Transform {
		Vector3 position{};
		Quaternion rotation{};
		Vector3 scale{};
	};

	struct ErrorMessage {
		std::string message{};
	};

	inline Vector3 fromVector3(const csl::math::Vector3& v) {
		return { v.x(), v.y(), v.z() };
	}

	inline csl::math::Vector3 toVector3(const Vector3& v) {
		return { v.x, v.y, v.z };
	}

	inline Vector4 fromVector4(const csl::math::Vector4& v) {
		return { v.x(), v.y(), v.z(), v.w() };
	}

	inline csl::math::Vector4 toVector4(const Vector4& v) {
		return { v.x, v.y, v.z, v.w };
	}

	inline Quaternion fromQuaternion(const csl::math::Quaternion& v) {
		return { v.x(), v.y(), v.z(), v.w() };
	}

	inline csl::math::Quaternion toQuaternion(const Quaternion& v) {
		return { v.x, v.y, v.z, v.w };
	}

	inline Transform fromTransform(const csl::math::Transform& v) {
		return { fromVector3(v.position), fromQuaternion(v.rotation), fromVector3(v.scale) };
	}

	inline csl::math::Transform toTransform(const Transform& v) {
		return { toVector3(v.position), toQuaternion(v.rotation), toVector3(v.scale) };
	}

	template<typename T, typename M>
	inline unsigned int fromHandle(const hh::fnd::Handle<T, M>& handle) {
		return (const unsigned int&)handle;
	}

	template<typename T>
	inline hh::fnd::Handle<T> toHandle(unsigned int handle) {
		hh::fnd::Handle<T> h{};
		(unsigned int&)h = handle;
		return h;
	}

	inline rfl::Object<rfl::Generic> getRflClassSerialization(void* obj, const typename he2sdk::ucsl::GameInterface::RflSystem::RflClass* rflClass) {
		yyjson_mut_doc* doc = yyjson_mut_doc_new(nullptr);

		ucsl::reflection::providers::rflclass<he2sdk::ucsl::GameInterface>::RootType refl{ rflClass };

		rip::binary::mem_istream mis{ obj };
		rip::binary::binary_istream<decltype(mis), size_t> bis{ mis };
		rip::binary::accessors::binary_istream<decltype(bis)>::ValueAccessor<decltype(refl)> acc { bis, refl };

		yyjson_mut_val* json = rip::binary::serializeJson(doc, acc);

		yyjson_mut_doc_set_root(doc, json);

		yyjson_doc* idoc = yyjson_mut_doc_imut_copy(doc, nullptr);
		yyjson_val* iroot = yyjson_doc_get_root(idoc);

		auto result = rfl::json::read<rfl::Object<rfl::Generic>>(iroot);

		yyjson_doc_free(idoc);
		yyjson_mut_doc_free(doc);

		return result.value();
	}
}
