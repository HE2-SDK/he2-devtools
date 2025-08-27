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
							Editor("unk1", effect.unk1);
							Editor("unk2", effect.unk2);
							Editor("unk3", effect.unk3);
							Editor("unk4", effect.unk4);
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
							Editor("unk1", sound.unk1);
							Editor("unk2", sound.unk2);
							Editor("unk3", sound.unk3);
							Editor("unk4", sound.unk4);

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
