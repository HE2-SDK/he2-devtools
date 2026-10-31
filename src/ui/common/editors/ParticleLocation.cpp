#include "ParticleLocation.h"
#ifndef DEVTOOLS_TARGET_SDK_wars
#include <resources/ManagedMemoryRegistry.h>

bool Editor(const char* name, hh::fnd::ManagedResource* resource, ucsl::resources::effdb::v100::Emitter& data);
bool Editor(const char* name, hh::fnd::ManagedResource* resource, const char*& data);
bool Editor(const char* name, hh::fnd::ManagedResource* resource, ucsl::resources::effdb::v100::EmitterSet& data);

#include <ui/common/inputs/Basic.h>
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>

using namespace hh::animeff;
using namespace ucsl::resources::effdb::v100;

bool Editor(const char* name, hh::fnd::ManagedResource* resource, Emitter& data) {
	bool changed{};

	if (ImGui::TreeNode(name)) {
		bool isAttached = data.GetIsAttachedFlag();
		bool disableBoneScale = data.GetDisableBoneScaleFlag();
		bool usePosition = data.GetUsePositionFlag();
		bool useRotation = data.GetUseRotationFlag();
		bool useScale = data.GetUseScaleFlag();

		Emitter::TransType transType = data.GetTransType();

		if (Editor("Attached To Bone", isAttached)) {
			data.SetIsAttachedFlag(isAttached);
			changed = true;
		}

		if (Editor("Disable bone scale", disableBoneScale)) {
			data.SetDisableBoneScaleFlag(disableBoneScale);
			changed = true;
		}

		if (Editor("Use Position", usePosition)) {
			data.SetUsePositionFlag(usePosition);
			changed = true;
		}

		if (Editor("Use Rotation", useRotation)) {
			data.SetUseRotationFlag(useRotation);
			changed = true;
		}

		if (Editor("Use Scale", useScale)) {
			data.SetUseScaleFlag(useScale);
			changed = true;
		}

		changed |= InputText("Emitter Name", data.emitterName, resource);
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

void InitEmitter(hh::fnd::ManagedResource* resource, Emitter& item) {
	item = {};
	item.boneName = "BoneName";
	item.emitterName = "EmitterName";
	item.rotation = { 0, 0, 0, 1 };
	item.scale = { 1, 1, 1 };
}

void DeinitEmitter(hh::fnd::ManagedResource* resource, Emitter& item) { }

void InitSoundName(hh::fnd::ManagedResource* resource, const char*& item) {
	item = {};
	item = "SoundName";
}

void DeinitSoundName(hh::fnd::ManagedResource* resource, const char*& item) {}

bool Editor(const char* name, hh::fnd::ManagedResource* resource, EmitterSet& data) {
	bool changed{};

	if (ImGui::TreeNode(name)) {
		changed |= InputText("Clip Name", data.clipName, resource);

		resources::ManagedCArray<Emitter, unsigned int> emitters{ resource, data.emitters, data.emitterCount };
		changed |= Editor("Emitters", resource, emitters, InitEmitter, DeinitEmitter);

		resources::ManagedCArray<const char*, unsigned int> soundNames{ resource, data.soundNames, data.soundNameCount };
		changed |= Editor("Sound Names", resource, soundNames, InitSoundName, DeinitSoundName);

		ImGui::TreePop();
	}

	return changed;
}

void InitEmitterSet(hh::fnd::ManagedResource* resource, EmitterSet& item) {
	item = {};
	item.clipName = "ClipName";
}

void DeinitEmitterSet(hh::fnd::ManagedResource* resource, EmitterSet& item) {}

bool Editor(EffdbData& data, ResParticleLocation* resource) {
	bool changed{};

	resources::ManagedCArray<EmitterSet, unsigned int> emitterSets{ resource, data.emitterSets, data.emitterSetCount };
	changed |= Editor("Emitter sets", resource, emitterSets, InitEmitterSet, DeinitEmitterSet);

	return changed;
}
#endif
