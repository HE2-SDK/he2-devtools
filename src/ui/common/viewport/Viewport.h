#pragma once
#include <utilities/CompatibleObject.h>

class Viewport : public CompatibleObject {
public:
	struct Description {
		unsigned int resolution[2]{ 1024, 1024 };
		hh::needle::PBRModelInstance* modelInstance;
		const char* name{ "Viewport" };
		float fov{ 1.0476f };
		bool setToAabb{ false };
	};

	csl::ut::MoveArray<hh::needle::PBRModelInstance*> models;
	hh::needle::RenderTextureHandle* renderTexture;
	hh::gfnd::ViewportData viewportData{};
	csl::ut::VariableString name;

	virtual ~Viewport();
	Viewport(csl::fnd::IAllocator* allocator);

	virtual void Render();

	void Setup(const Description& desc);

	void UpdateViewportData();
	void SetViewMatrix(const csl::math::Matrix34& mat);
	void UpdateViewMatrix();

	float GetZoomToAABB(const csl::geom::Aabb& aabb) const;
	float GetZoomToAABB(hh::needle::PBRModelInstance* modelAabb) const;
	float GetZoomToAABB() const;

	void SetCameraToAABB(const csl::geom::Aabb& aabb);
	void SetCameraToAABB(hh::needle::PBRModelInstance* modelAabb);
	void SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target);
	void SetCameraPosition(const csl::math::Vector3& position);
	void SetCameraTarget(const csl::math::Vector3& target);
	void SetCameraFOV(const float fov);

	void AddModel(hh::needle::PBRModelInstance* modelInstance, bool setToAabb = false);
	void AddModel(hh::gfx::ResModel* resModel, bool setToAabb = false);

	void RemoveModel(hh::needle::PBRModelInstance* modelInstance);

	void ClearModels();

protected:
	hh::needle::Texture* GetTexture() const;
	ImTextureID GetTextureID() const;
};
