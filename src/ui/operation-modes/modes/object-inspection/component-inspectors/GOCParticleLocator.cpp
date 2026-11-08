#include "GOCParticleLocator.h"
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Effects.h>
#include <ui/common/viewers/Basic.h>

using namespace hh::animeff;

void RenderComponentInspector(GOCParticleLocator& component) {
	Editor("Effect trigger type index", component.effectTriggerTypeIndex);
	Editor("Sound trigger type index", component.soundTriggerTypeIndex);
	Editor("GOCVisualModel name hash", component.gocVisualModelNameHash);
	Editor("GOCAnimator name hash", component.gocAnimatorNameHash);
	
	Viewer("GOCAnimator found", component.gocAnimator != nullptr);
	Viewer("GOCEffect found", component.gocEffect != nullptr);
	Viewer("GOCVisualModel found", component.gocVisualModel != nullptr);

	if (ImGui::TreeNode("Effects")) {
		size_t boneIdx{};
		for (auto& effects : component.boundEffectsByBoneIndex) {
			if (effects) {
				ImGui::PushID(boneIdx);
				if (ImGui::TreeNodeEx("Bone", ImGuiTreeNodeFlags_DefaultOpen, "Bone %d", boneIdx)) {
					size_t effectIdx{};
					for (auto& effect : effects->boundItems) {
						ImGui::PushID(effectIdx);
						if (ImGui::TreeNodeEx("Effect", ImGuiTreeNodeFlags_DefaultOpen)) {
							Editor("State Sequence Number", effect.stateSequenceNr);
							Editor("ID", effect.id);
							Editor("Looping", effect.looping);
							if (effect.emitter != nullptr)
								Viewer("Emitter", effect.emitter->emitterName);
							Editor("Handle", effect.item);
							ImGui::TreePop();
						}
						ImGui::PopID();

						effectIdx++;
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}

			boneIdx++;
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sounds")) {
		size_t boneIdx{};
		for (auto& sounds : component.boundSoundsByBoneIndex) {
			if (sounds) {
				ImGui::PushID(boneIdx);
				if (ImGui::TreeNodeEx("Bone", ImGuiTreeNodeFlags_DefaultOpen, "Bone %d", boneIdx)) {
					size_t soundIdx{};
					for (auto& sound : sounds->boundItems) {
						ImGui::PushID(soundIdx);
						if (ImGui::TreeNodeEx("Sound", ImGuiTreeNodeFlags_DefaultOpen)) {
							Editor("State Sequence Number", sound.stateSequenceNr);
							Editor("ID", sound.id);
							Editor("Looping", sound.looping);
							if (sound.emitter != nullptr)
								Viewer("Emitter", sound.emitter->emitterName);

							if (sound.item.IsPlay())
								Viewer("Name", sound.item.GetCueName());
							ImGui::TreePop();
						}
						ImGui::PopID();

						soundIdx++;
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}

			boneIdx++;
		}
		ImGui::TreePop();
	}
}
