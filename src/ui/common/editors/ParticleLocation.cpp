#include "ParticleLocation.h"
#ifndef DEVTOOLS_TARGET_SDK_wars
#include <resources/ManagedMemoryRegistry.h>

bool Editor(const char* name, hh::fnd::ManagedResource* resource, ucsl::resources::effdb::v100::Particle& data);
bool Editor(const char* name, hh::fnd::ManagedResource* resource, const char*& data);
bool Editor(const char* name, hh::fnd::ManagedResource* resource, ucsl::resources::effdb::v100::Binding& data);

#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

using namespace hh::animeff;
using namespace ucsl::resources::effdb::v100;

bool Editor(const char* name, hh::fnd::ManagedResource* resource, Particle& data) {
	bool changed{};

	if (ImGui::TreeNode(name)) {
		changed |= Editor("Attached To Bone", data.attachedToBone);
		changed |= CheckboxFlags("Ignore Relative Position", data.flags, Particle::Flag::IGNORE_RELATIVE_POSITION);
		changed |= CheckboxFlags("Use Position", data.flags, Particle::Flag::USE_POSITION);
		changed |= CheckboxFlags("Use Rotation", data.flags, Particle::Flag::USE_ROTATION);
		changed |= CheckboxFlags("Use Scale", data.flags, Particle::Flag::USE_SCALE);
		changed |= InputText("Particle Name", data.particleName, resource);
		changed |= InputText("Bone Name", data.boneName, resource);
		changed |= Editor("Position", data.position);
		changed |= Editor("Rotation", data.rotation);
		changed |= Editor("Scale", data.scale);

		ImGui::TreePop();
	}

	return changed;
}

bool Editor(const char* name, hh::fnd::ManagedResource* resource, const char*& data) {
	bool changed{};

	changed |= InputText(name, data, resource);

	return changed;
}

void InitParticle(hh::fnd::ManagedResource* resource, Particle& item) {
	item = {};
	item.boneName = "BoneName";
	item.particleName = "ParticleName";
	item.rotation = { 0, 0, 0, 1 };
	item.scale = { 1, 1, 1 };
}

void DeinitParticle(hh::fnd::ManagedResource* resource, Particle& item) { }

void InitSoundName(hh::fnd::ManagedResource* resource, const char*& item) {
	item = {};
	item = "SoundName";
}

void DeinitSoundName(hh::fnd::ManagedResource* resource, const char*& item) {}

bool Editor(const char* name, hh::fnd::ManagedResource* resource, Binding& data) {
	bool changed{};

	if (ImGui::TreeNode(name)) {
		changed |= InputText("Clip Name", data.clipName, resource);

		resources::ManagedCArray<Particle, unsigned int> particles{ resource, data.particles, data.particleCount };
		changed |= Editor("Particles", resource, particles, InitParticle, DeinitParticle);

		resources::ManagedCArray<const char*, unsigned int> soundNames{ resource, data.soundNames, data.soundNameCount };
		changed |= Editor("Sound Names", resource, soundNames, InitSoundName, DeinitSoundName);

		ImGui::TreePop();
	}

	return changed;
}

void InitBinding(hh::fnd::ManagedResource* resource, Binding& item) {
	item = {};
	item.clipName = "ClipName";
}

void DeinitBinding(hh::fnd::ManagedResource* resource, Binding& item) {}

bool Editor(EffdbData& data, ResParticleLocation* resource) {
	bool changed{};

	resources::ManagedCArray<Binding, unsigned int> bindings{ resource, data.bindings, data.bindingCount };
	changed |= Editor("Bindings", resource, bindings, InitBinding, DeinitBinding);

	return changed;
}
#endif
