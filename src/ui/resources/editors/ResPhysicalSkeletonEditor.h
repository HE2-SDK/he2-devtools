#pragma once
#include <ui/common/StandaloneWindow.h>
#include <ui/common/editors/Basic.h>
#include <resources/managed-memory/ManagedCArray.h>

using namespace ucsl::resources::pba::v1;

class ResPhysicalSkeletonEditor : public StandaloneWindow {
	hh::fnd::Reference<hh::pba::ResPhysicalSkeleton> resource;
	hh::fnd::Reference<hh::pba::GOCPhysicalAnimation> gocPhys;

	bool Editor(Node& value, unsigned int idx, SoftBody& parent);
	bool Editor(Link& value, unsigned int idx, SoftBody& parent);
	bool Editor(Limit& value);
	bool Editor(SoftBody& value, unsigned int idx, PbaData& parent);
	bool Editor(Constraint& value, unsigned int idx, PbaData& parent);
	bool Editor(RigidBody& value, unsigned int idx, PbaData& parent);
	bool Editor(PbaData& value);
public:
	ResPhysicalSkeletonEditor(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc = nullptr);
	virtual void RenderContents();
	static ResPhysicalSkeletonEditor* Create(csl::fnd::IAllocator* allocator, hh::pba::ResPhysicalSkeleton* resource, hh::pba::GOCPhysicalAnimation* goc = nullptr);
};
