#pragma once
#include <ui/common/StandaloneWindow.h>

class ResModelEditor : public StandaloneWindow {
	hh::needle::PBRModelInstance* modelInstance;
	hh::needle::RenderTextureHandle* renderTexture;
	unsigned int resolution[2]{ 1024, 1024 };
	hh::gfnd::ViewportData viewportData;
	csl::ut::VariableString name;

	void UpdateViewportData();
	void SetViewMatrix(const csl::math::Matrix34& mat);
	void UpdateViewMatrix();
	void SetCameraPositionAndTarget(const csl::math::Vector3& position, const csl::math::Vector3& target);
	void SetCameraPosition(const csl::math::Vector3& position);
	void SetCameraTarget(const csl::math::Vector3& target);
	void SetCameraFOV(const float fov);
public:
	virtual ~ResModelEditor();

	ResModelEditor(csl::fnd::IAllocator* allocator, hh::gfx::ResModel* resource);
	virtual void RenderContents();
	static ResModelEditor* Create(csl::fnd::IAllocator* allocator, hh::gfx::ResModel* resource);
};
