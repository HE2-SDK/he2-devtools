#include "GOCSound.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::snd;

void Viewer(const char* label, hh::snd::SoundCueCri::CdataTag& cueTag) {
	Viewer("ID", cueTag.cueId);
	Viewer("Minimum distance", cueTag.minDistance);
	Viewer("Maximum distance", cueTag.maxDistance);
	Viewer("Block count", cueTag.numBlocks);
	Viewer("Is infinite", cueTag.isInfinite);
	Viewer("Is streaming", cueTag.isStreaming);
#ifdef DEVTOOLS_TARGET_SDK_miller
	Viewer("Is 3D position panning", cueTag.is3DPosPanning);
#endif
}

void Viewer(const char* label, hh::snd::HandleTag3d& tag) {
	Viewer("Position", tag.position);
	Viewer("Velocity", tag.velocity);
	Viewer("Unk1", tag.float48);
}

bool Editor(const char* label, hh::snd::SoundHandle& sound) {
	if (!sound.IsPlay()) {
		ImGui::Text("Invalid handle");
		return false;
	}

	bool edited{};
	const char* name = sound.GetCueName();

	if (ImGui::TreeNodeEx(*sound.handle, ImGuiTreeNodeFlags_DefaultOpen, "%s", name)) {
		Viewer("Cue name", name);

		if (ImGui::Button("Stop"))
			sound.StopImm();

		bool paused{ sound.IsPause() };
		if (Editor("Paused", paused))
			sound.Pause(paused, 0.0f);

		ImGui::SeparatorText("Cue tag");
		Viewer("Cue tag", (*sound.handle)->tag->cueData);
		ImGui::SeparatorText("3D tag");
		Viewer("3D tag", (*sound.handle)->tag->tag3d);

		ImGui::TreePop();
	}

	return edited;
}

bool Editor(const char* label, hh::snd::SoundTransInfo& info) {
	bool edited{};
	edited |= Editor("Offset", info.offset);
	edited |= Editor("Unk1", info.unk1);
	edited |= CheckboxFlags("Unk0", info.flags, hh::snd::SoundTransInfo::Flag::UNK0);
	edited |= CheckboxFlags("Use offset", info.flags, hh::snd::SoundTransInfo::Flag::OFFSET);
	Viewer("Frame", info.frame);
	edited |= Editor("Handle", info.handle);
	return edited;
}

bool Editor(const char* label, hh::snd::GOCSound::LiveSoundInfo& sound) {
	bool edited{};
	edited |= Editor("Handle", sound.handle);
	Viewer("Frame", sound.followFrameInfo.frame);
	edited |= Editor("Offset", sound.followFrameInfo.offset);
	return edited;
}

void RenderComponentInspector(GOCSound& component) {
	for (auto& sound : component.sounds)
		Editor("Sound", sound);

	ImGui::SeparatorText("TransInfos");
	for (auto& info : component.soundTransInfos)
		Editor("Transinfo", info);

	ImGui::SeparatorText("Live sounds");
	for (auto& info : component.liveSoundInfos)
		Editor("Live sound", info);
}
