#pragma once
#include <utilities/CompatibleObject.h>

class Previewer : public CompatibleObject {
public:
	struct Description {
		unsigned int resolution[2]{ 1024, 1024 };
		hh::needle::PBRModelInstance* modelInstance;
		const char* name = "Previewer";
		float fov{ 1.0476f };
		bool setToAabb{ false };
	};

	csl::ut::MoveArray<hh::needle::PBRModelInstance*> models;
	hh::needle::RenderTextureHandle* renderTexture;
	hh::gfnd::ViewportData viewportData;
	csl::ut::VariableString name;

	virtual ~Previewer();
	Previewer(csl::fnd::IAllocator* allocator);

	void Setup(const Description& desc);

	void UpdateViewportData();
	void SetViewMatrix(const csl::math::Matrix34& mat);
	void UpdateViewMatrix();

	void SetCameraToAABB(const csl::geom::Aabb& aabb);
	void SetCameraToAABB(hh::needle::PBRModelInstance* modelAabb);
	void SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target);
	void SetCameraPosition(const csl::math::Vector3& position);
	void SetCameraTarget(const csl::math::Vector3& target);
	void SetCameraFOV(const float fov);

	hh::needle::Texture* GetTexture() const;
	ImTextureID GetTextureID() const;

	void AddModel(hh::needle::PBRModelInstance* modelInstance, bool setToAabb = false);
	void AddModel(hh::gfx::ResModel* resModel, bool setToAabb = false);
};
