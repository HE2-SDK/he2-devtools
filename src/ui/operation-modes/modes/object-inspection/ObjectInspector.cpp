#include "ObjectInspector.h"
#include "Behaviors.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/editors/Basic.h>
#include <ui/common/editors/Reflection.h>
#include <ui/common/inputs/Basic.h>
#include <ui/common/icons.h>
#include <ui/tools/MemoryViewer.h>
#include <utilities/math/EulerTransform.h>
#include <utilities/math/MathUtils.h>
#include <utilities/GameObjectUtils.h>
#include <imgui_internal.h>

#ifdef DEVTOOLS_TARGET_SDK_wars
#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/GOCCollider.h"
#include "component-inspectors/GOCSprite.h"
#include "component-inspectors/GOCPlayerParameter.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	hh::physics::GOCMeshCollider,
	hh::ui::GOCSprite,
	app::player::GOCPlayerParameter
> InspectableComponents;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/GOCCollider.h"
#include "component-inspectors/GOCSprite.h"
#include "component-inspectors/GOCUIComposition.h"
#include "component-inspectors/GOCEffect.h"
#include "component-inspectors/GOCAnimator.h"
#include "component-inspectors/GOCActivator.h"
#include "component-inspectors/GOCVisual.h"
#include "component-inspectors/GOCSound.h"
#include "component-inspectors/GOCParticleLocator.h"
#include "component-inspectors/PathComponent.h"
#include "component-inspectors/GOCEvent.h"
#include "component-inspectors/GOCPlayerBlackboard.h"
#include "component-inspectors/GOCPlayerKinematicParams.h"
#include "component-inspectors/GOCCamera.h"
#include "component-inspectors/GOCPlayerParameter.h"
#include "component-inspectors/GOCPlayerHsm.h"
#include "component-inspectors/GOCPlayerEffect.h"
#include "component-inspectors/GOCPlayerVisual.h"
#include "component-inspectors/GOCPlayerCollider.h"
#include "component-inspectors/GOCColliderQuery.h"
#include "component-inspectors/GOCPhysicalAnimation.h"
#include "component-inspectors/GOCPointLight.h"
#include "component-inspectors/GOCVibration.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::game::GOCActivator,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	hh::physics::GOCMeshCollider,
	hh::ui::GOCSprite,
	hh::ui::GOCUIComposition,
	hh::eff::GOCEffect,
	hh::gfx::GOCVisual,
	hh::gfx::GOCVisualTransformed,
	hh::gfx::GOCVisualModel,
	hh::snd::GOCSound,
	hh::animeff::GOCParticleLocator,
	hh::anim::GOCAnimator,
	hh::path::PathComponent,
	app_cmn::camera::GOCCamera,
	app::game::GOCEventCollision,
	app::player::GOCPlayerKinematicParams,
	app::player::GOCPlayerBlackboard,
	app::player::GOCPlayerParameter,
	app::player::GOCPlayerHsm,
	app::player::GOCPlayerCollider,
	app::player::GOCPlayerEffect,
	app::player::GOCPlayerVisual,
	app::physics::GOCMoveSphereColliderQuery,
	hh::pba::GOCPhysicalAnimationBullet,
	hh::gfx::GOCPointLight,
	hh::game::GOCVibration
> InspectableComponents;
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
#include "component-inspectors/GOCTransform.h"
#include "component-inspectors/GOCCollider.h"
#include "component-inspectors/GOCSprite.h"
#include "component-inspectors/GOCUIComposition.h"
#include "component-inspectors/GOCEffect.h"
#include "component-inspectors/GOCAnimation.h"
#include "component-inspectors/GOCAnimator.h"
#include "component-inspectors/GOCVisual.h"
#include "component-inspectors/GOCPhysicalAnimation.h"
#include "component-inspectors/GOCSound.h"
#include "component-inspectors/GOCParticleLocator.h"
#include "component-inspectors/GOCPlayerParameter.h"
#include "component-inspectors/GOCPlayerHsm.h"
#include "component-inspectors/GOCPlayerKinematicParams.h"

typedef std::tuple<
	hh::game::GOCTransform,
	hh::physics::GOCSphereCollider,
	hh::physics::GOCBoxCollider,
	hh::physics::GOCCapsuleCollider,
	hh::physics::GOCCylinderCollider,
	hh::physics::GOCMeshCollider,
	hh::ui::GOCSprite,
	hh::ui::GOCUIComposition,
	hh::eff::GOCEffect,
	hh::gfx::GOCVisual,
	hh::gfx::GOCVisualTransformed,
	hh::gfx::GOCVisualModel,
	hh::snd::GOCSound,
	hh::animeff::GOCParticleLocator,
	app::player::GOCPlayerParameter,
	app::player::GOCPlayerHsm,
	app::player::GOCPlayerKinematicParams,
	hh::anim::GOCAnimationSimple,
	hh::anim::GOCAnimator,
	hh::pba::GOCPhysicalAnimationBullet
> InspectableComponents;
#endif

#ifdef DEVTOOLS_TARGET_SDK_wars
typedef std::tuple<> InspectableGameObjects;
#endif

#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "gameobject-inspectors/CameraFrame.h"
#include "gameobject-inspectors/ObjCameraVolume.h"
#include "gameobject-inspectors/ObjGismo.h"
#include "gameobject-inspectors/BossGiant.h"

typedef std::tuple<
	app_cmn::camera::CameraFrame,
	app::ObjCameraVolume,
	app::ObjGismo,
	app::BossGiant
> InspectableGameObjects;
#endif

#ifdef DEVTOOLS_TARGET_SDK_miller
typedef std::tuple<
> InspectableGameObjects;
#endif

namespace app {
	class GOCEnemy : public hh::game::GOComponent {
	public:
		GOCOMPONENT_CLASS_DECLARATION(GOCEnemy);
	};
}

namespace ui::operation_modes::modes::object_inspection {

	using namespace hh::fnd;
	using namespace hh::game;


	template<typename T>
	bool RenderComponentInspectorT(hh::game::GOComponent& component) {
		bool result{ component.pStaticClass == T::GetClass() };

		if (result) {
			ImGui::PushID(&component);
			RenderComponentInspector(static_cast<T&>(component));
			ImGui::PopID();
		}

		return result;
	}

	template<typename = InspectableComponents>
	class ComponentIterator;
	template<typename... T>
	class ComponentIterator<std::tuple<T...>> {
	public:
		static void Render(hh::game::GOComponent& component) {
			(RenderComponentInspectorT<T>(component) || ...) || (ImGui::Text("Inspector for this component not yet implemented"), true);
		}
	};


	template<typename T>
	bool RenderGameObjectInspectorT(hh::game::GameObject& gameObject) {
		bool result{ gameObject.objectClass == T::GetClass() };

		if (result) {
			ImGui::PushID(&gameObject);
			RenderGameObjectInspector(static_cast<T&>(gameObject));
			ImGui::PopID();
		}

		return result;
	}

	template<typename = InspectableGameObjects>
	class GameObjectIterator;
	template<typename... T>
	class GameObjectIterator<std::tuple<T...>> {
	public:
		static void Render(hh::game::GameObject& gameObject) {
			(RenderGameObjectInspectorT<T>(gameObject) || ...);
		}
	};

	void ObjectInspector::RenderPanel() {
		auto& selection = GetBehavior<SelectionBehavior<Context>>()->GetSelection();

		if (selection.size() == 0) {
			ImGui::Text("Select an object in the left pane.");
		}
		else if (selection.size() > 1) {
			ImGui::Text("Multiple objects selected");
		}
		else {
			auto focusedObject = selection[0];
			ImGui::BeginGroup();
			ImGui::Text("Object name: %s", focusedObject->name.c_str());
			ImGui::Text("Layer: %d", focusedObject->layer);
			ImGui::Text("Class: %s", focusedObject->objectClass ? focusedObject->objectClass->GetName() : "<none>");
			ImGui::EndGroup();
			if (ImGui::BeginPopupContextItem("GameObject Operations")) {
				if (ImGui::Selectable("Open in memory viewer"))
					new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), focusedObject, focusedObject->objectClass->GetObjectSize() };
				ImGui::EndPopup();
			}
			ImGui::Separator();
			if (ImGui::BeginTabBar("Inspector types")) {
				if (ImGui::BeginTabItem("Properties")) {
					if (ImGui::BeginChild("Content")) {
						ImGui::SeparatorText("Components");
						for (auto* component : focusedObject->components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s %s (%s) - %x", GetIcon(*component), component->pStaticClass->pName, component->pStaticClass->category, component->nameHash);

							bool isComponentSectionOpen = ImGui::CollapsingHeader(title);

							if (ImGui::BeginPopupContextItem("Component Operations")) {
								if (ImGui::Selectable("Open in memory viewer"))
									new (Desktop::instance->GetAllocator()) MemoryViewer{ Desktop::instance->GetAllocator(), component, component->pStaticClass->size };
								ImGui::EndPopup();
							}

							if (isComponentSectionOpen)
								ComponentIterator<>::Render(*component);

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					GameObjectIterator<>::Render(*focusedObject);
					ImGui::EndTabItem();
				}
				if (auto* status = focusedObject->GetWorldObjectStatus()) {
					if (ImGui::BeginTabItem("WorldObjectStatus")) {
						CheckboxFlags("Enabled", status->flags, WorldObjectStatus::Flag::ENABLED);
						CheckboxFlags("Is alive", status->flags, WorldObjectStatus::Flag::IS_ALIVE);
						CheckboxFlags("Shutdown", status->flags, WorldObjectStatus::Flag::SHUTDOWN);
						CheckboxFlags("No restart", status->flags, WorldObjectStatus::Flag::NO_RESTART);

						if (ImGui::Button("Shutdown"))
							status->Shutdown();
						if (ImGui::Button("Restart"))
							status->Restart();

#ifndef DEVTOOLS_TARGET_SDK_wars
						Editor("Spawn priority", status->spawnPriority);
#endif

						unsigned short state0 = status->GetObjectState(0);
						if (Editor("State 0", state0))
							status->SetObjectState(0, state0);

						unsigned short state1 = status->GetObjectState(1);
						if (Editor("State 1", state1))
							status->SetObjectState(1, state1);

						ImGui::EndTabItem();
					}
				}
#ifndef DEVTOOLS_TARGET_SDK_wars
				if (ImGui::BeginTabItem("Update configuration")) {
					if (ImGui::BeginChild("Content")) {
						CheckboxFlags("Asleep", focusedObject->statusFlags, GameObject::StatusFlags::ASLEEP);
						ImGui::Text("GameObject requested update phases:");
						ImGui::BeginGroup();
						CheckboxFlags("PreAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::PRE_ANIM);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::PRE_ANIM);
						ImGui::SameLine();
						CheckboxFlags("PostAnim", focusedObject->forcedUpdateFlags, UpdatingPhase::POST_ANIM);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::POST_ANIM);
						ImGui::SameLine();
						CheckboxFlags("Final", focusedObject->forcedUpdateFlags, UpdatingPhase::FINAL);
						if (ImGui::IsItemEdited())
							focusedObject->UNSAFE_SetUpdateFlags(UpdatingPhase::FINAL);
						ImGui::EndGroup();
						ImGui::Text("Real update phases due to components:");
						ImGui::BeginDisabled();
						CheckboxFlags("PreAnim", focusedObject->updateFlags, UpdatingPhase::PRE_ANIM);
						ImGui::SameLine();
						CheckboxFlags("PostAnim", focusedObject->updateFlags, UpdatingPhase::POST_ANIM);
						ImGui::SameLine();
						CheckboxFlags("Final", focusedObject->updateFlags, UpdatingPhase::FINAL);
						ImGui::EndDisabled();
						ImGui::SeparatorText("Component update configuration");
						for (auto* component : focusedObject->components) {
							ImGui::PushID(component);

							char title[200];
							snprintf(title, 200, "%s (%s)", component->pStaticClass->pName, component->pStaticClass->category);

							if (ImGui::CollapsingHeader(title)) {
								ImGui::Text("Update phases");
								//ImGui::BeginDisabled();
								ImGui::BeginGroup();
								CheckboxFlags("PreAnim", component->updateFlags, UpdatingPhase::PRE_ANIM);
								if (ImGui::IsItemDeactivatedAfterEdit())
									ApplyComponentUpdateFlag(*component, UpdatingPhase::PRE_ANIM);
								ImGui::SameLine();
								CheckboxFlags("PostAnim", component->updateFlags, UpdatingPhase::POST_ANIM);
								if (ImGui::IsItemDeactivatedAfterEdit())
									ApplyComponentUpdateFlag(*component, UpdatingPhase::POST_ANIM);
								ImGui::SameLine();
								CheckboxFlags("Final", component->updateFlags, UpdatingPhase::FINAL);
								if (ImGui::IsItemDeactivatedAfterEdit())
									ApplyComponentUpdateFlag(*component, UpdatingPhase::FINAL);
								ImGui::EndGroup();
								//ImGui::EndDisabled();
								ImGui::Text("Update priorities:");
								ImGui::InputScalarN("PreAnim, PostAnim, Final", ImGuiDataType_U8, component->updatePriorityByPhase, 3);
							}

							ImGui::PopID();
						}
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
#endif

				ImGui::EndTabBar();
			}
		}
	}

	PanelTraits ObjectInspector::GetPanelTraits() const
	{
		return { "Object inspector", ImVec2(ImGui::GetMainViewport()->WorkSize.x, 100), ImVec2(600, ImGui::GetMainViewport()->WorkSize.y - 140), ImVec2(1, 0) };
	}

	const char* ObjectInspector::GetIcon(hh::game::GOComponent& component)
	{
		if (component.pStaticClass == hh::game::GOCTransform::GetClass())
			return GetIconGlyph(IconId::COMPONENT_TRANSFORM);
		else if (component.pStaticClass == hh::gfx::GOCVisual::GetClass())
			return GetIconGlyph(IconId::COMPONENT_GEOMETRY);
		else if (component.pStaticClass == hh::gfx::GOCVisualTransformed::GetClass())
			return GetIconGlyph(IconId::COMPONENT_GEOMETRY);
#ifdef DEVTOOLS_TARGET_SDK_rangers
		else if (component.pStaticClass == hh::gfx::GOCVisualModel::GetClass())
			return GetIconGlyph(IconId::COMPONENT_GEOMETRY);
		else if (component.pStaticClass == hh::game::GOCInput::GetClass())
			return GetIconGlyph(IconId::COMPONENT_INPUT);
		else if (component.pStaticClass == app_cmn::camera::GOCCamera::GetClass())
			return GetIconGlyph(IconId::COMPONENT_CAMERA);
#endif
		else if (component.pStaticClass == app::player::GOCPlayerParameter::GetClass())
			return GetIconGlyph(IconId::COMPONENT_PARAMETER);
		else if (component.pStaticClass == hh::path::PathComponent::GetClass())
			return GetIconGlyph(IconId::COMPONENT_PATH);
		else if (component.pStaticClass == hh::game::GOCTransform::GetClass())
			return GetIconGlyph(IconId::COMPONENT_STATE_MACHINE);
#ifndef DEVTOOLS_TARGET_SDK_wars
		else if (component.pStaticClass == app::player::GOCPlayerHsm::GetClass())
			return GetIconGlyph(IconId::COMPONENT_STATE_MACHINE);
#endif
		//else if (component.pStaticClass == app_cmn::fsm::GOCHsm2::GetClass())
		//	return GetIconGlyph(IconId::COMPONENT_STATE_MACHINE);
		//else if (component.pStaticClass == app_cmn::fsm::GOCTinyFsm2::GetClass())
		//	return GetIconGlyph(IconId::COMPONENT_STATE_MACHINE);
		//else if (component.pStaticClass == hh::physics::GOCCollider::GetClass())
		//	return GetIconGlyph(IconId::COMPONENT_COLLISION);
		else if (component.pStaticClass == hh::physics::GOCSphereCollider::GetClass())
			return GetIconGlyph(IconId::COMPONENT_COLLISION);
		else if (component.pStaticClass == hh::physics::GOCBoxCollider::GetClass())
			return GetIconGlyph(IconId::COMPONENT_COLLISION);
		else if (component.pStaticClass == hh::physics::GOCCapsuleCollider::GetClass())
			return GetIconGlyph(IconId::COMPONENT_COLLISION);
		else if (component.pStaticClass == hh::physics::GOCCylinderCollider::GetClass())
			return GetIconGlyph(IconId::COMPONENT_COLLISION);
		else if (component.pStaticClass == hh::ui::GOCSprite::GetClass())
			return GetIconGlyph(IconId::COMPONENT_UI);
#ifndef DEVTOOLS_TARGET_SDK_wars
		else if (component.pStaticClass == hh::snd::GOCSound::GetClass())
			return GetIconGlyph(IconId::COMPONENT_SOUND);
		else if (component.pStaticClass == hh::anim::GOCAnimator::GetClass())
			return GetIconGlyph(IconId::COMPONENT_ANIMATION);
		else if (component.pStaticClass == app::GOCEnemy::GetClass())
			return GetIconGlyph(IconId::COMPONENT_ENEMY);
#endif
		else return GetIconGlyph(IconId::COMPONENT_OBJECT);
	}
}
