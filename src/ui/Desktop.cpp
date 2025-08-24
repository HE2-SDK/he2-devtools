#include "common/Icons.h"
#include "Desktop.h"
#include "ToolBar.h"
#include "Viewport.h"
#include "SettingsManager.h"
#include "resources/ResourceBrowser.h"
#include "operation-modes/modes/object-inspection/ObjectInspection.h"
#include "operation-modes/modes/level-editor/LevelEditor.h"
#include "operation-modes/modes/surfride-editor/SurfRideEditor.h"
#include <debug-rendering/DebugRenderer.h>
#include <utilities/math/MathUtils.h>
#include <utilities/CompatibleObject.h>
#include "common/editors/Basic.h"
#include "common/inputs/Basic.h"
#include <imtimeline.h>
#include <ui/common/editors/Needle.h>

using namespace hh::fnd;
using namespace hh::game;
using namespace hh::physics;

Desktop* Desktop::instance{};
bool Desktop::selectionColliderFilters[32][32]{ true };

#ifdef DEVTOOLS_TARGET_SDK_miller
hh::fnd::Reference<app::player::PlayerReplayService> replayService{};
const char* replayCommandNames[] = {
	"TOGGLE_PLAYBACK",
	"TOGGLE_RECORDING",
	"TOGGLE_PLAYBACK_2",
	"TOGGLE_PLAYBACK_SPEED",
	"TOGGLE_REPLAY_CAMERA",
	"LOAD",
	"SAVE",
};
#endif

ImTimeline::ImTimelineContext* tlCtx = ImTimeline::CreateContext();

Desktop::Desktop(csl::fnd::IAllocator* allocator) : CompatibleObject{ allocator } {
	Translations::Init(allocator);
}

namespace app::game {
	class GrindService : hh::game::GameService {
		GAMESERVICE_CLASS_DECLARATION(GrindService)
	};
}
void Desktop::Render() {
	// These overlay windows need to be created in reverse order, because passing ImGuiWindowFlags_NoBringToFrontOnFocus
	// to ImGui::Begin will also insert the window at the front of the display list instead of append it to the back,
	// so the last window will be the first one to be rendered.

	ToolBar::Render();
	// Pushing a style color here because ImGui::DockSpaceOverViewport draws a background "rectangle with hole" over the
	// part of the screen that's not covered by the central node, and since our windows have transparent background, we
	// don't want this as it would apply the background alpha twice.
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::PopStyleColor();
	ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
	ImGuizmo::BeginFrame();
	RenderOverlayWindow();
	RenderSceneWindow();
	//Viewport::Render();

	devtools::debug_rendering::DebugRenderer::instance->RenderImGuiVisuals();
	operationMode->Render();
	ResourceBrowser::RenderGlobalDialogs();

	csl::ut::MoveArray<StandaloneWindow*> windowsThatWantToClose{ hh::fnd::MemoryRouter::GetTempAllocator() };

	for (auto& window : windows)
		if (!window->Render())
			windowsThatWantToClose.push_back(window);

	for (auto& window : windowsThatOpened)
		AddStandaloneWindow(window);

	windowsThatOpened.clear();

	for (auto* window : windowsThatWantToClose)
		RemoveStandaloneWindow(window);

	SettingsManager::Render();

	HandleShortcuts();

//#ifdef DEVTOOLS_TARGET_SDK_miller
//	auto* gameManager = hh::game::GameManager::GetInstance();
//	static app::player::MsgReplayCommand::Command command{};
//
//	if (ImGui::Button("Create replay service")) {
//		replayService = gameManager->CreateService<app::player::PlayerReplayService>(nullptr);
//		gameManager->RegisterService(replayService);
//	}
//
//	if (ImGui::Button("Create replay recorder")) {
//		if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
//			auto* player = (app::player::Player*)static_cast<hh::fnd::Messenger*>(levelInfo->GetPlayerObject(0));
//			auto* rec = player->UNSAFE_CreateComponent<app::player::GOCPlayerReplayRecorder>();
//			player->UNSAFE_AddComponent(rec);
//		}
//	}
//
//	ComboEnum("replay command", command, replayCommandNames);
//	ImGui::SameLine();
//
//	if (ImGui::Button("Send")) {
//		if (auto* levelInfo = gameManager->GetService<app::level::LevelInfo>()) {
//			auto hPlayer = levelInfo->GetPlayerObject(0);
//			app::player::MsgReplayCommand msg{ command };
//
//			hPlayer->SendMessageImm(msg);
//		}
//	}
//#endif

	//if (auto* s = GameManager::GetInstance()->GetService<app::game::GrindService>()) {
	//	ImGui::Text("%d", *reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
	//	ImGui::Checkbox("grind", reinterpret_cast<bool*>(reinterpret_cast<size_t>(s) + 0x1d0));
	//}
	//static char packfileName[300];
	//ImGui::InputText("Packfile", packfileName, sizeof(packfileName));
	//if (ImGui::Button("Put packfile in objectworld lol"))
	//	if (auto* objectWorld = GameManager::GetInstance()->GetService<hh::game::ObjectWorld>())
	//		objectWorld->packFile = ResourceManager::GetInstance()->GetResource<hh::fnd::Packfile>(packfileName);
}

void Desktop::RenderSceneWindow()
{
	auto* ivp = ImGui::GetMainViewport();

	ImGui::SetNextWindowSize(ivp->Size);
	ImGui::SetNextWindowPos(ivp->Pos);
	ImGui::SetNextWindowViewport(ivp->ID);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);
}

void Desktop::RenderOverlayWindow()
{
	auto* ivp = ImGui::GetMainViewport();

	ImGui::SetNextWindowSize(ivp->Size);
	ImGui::SetNextWindowPos(ivp->Pos);
	ImGui::SetNextWindowViewport(ivp->ID);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
	ImGui::PushStyleColor(ImGuiCol_Border, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

	ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);
}

void Desktop::AddStandaloneWindow(StandaloneWindow* window) {
	this->windows.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::RemoveStandaloneWindow(StandaloneWindow* window) {
	this->windows.remove(this->windows.find(window));
}

void Desktop::OpenStandaloneWindow(StandaloneWindow* window) {
	this->windowsThatOpened.push_back(hh::fnd::Reference<StandaloneWindow>{ window });
}

void Desktop::Dispatch(const ActionBase& action)
{
	operationMode->ProcessAction(action);
}

void Desktop::UnbindShortcut(ShortcutId shortcutId)
{
	for (auto i = 0; i < boundShortcuts.size(); i++)
		if (boundShortcuts[i].shortcutId == shortcutId) {
			boundShortcuts.remove(i);
			break;
		}
}

void Desktop::HandleShortcuts()
{
	if (ImGui::GetIO().WantCaptureKeyboard)
		return;

	for (auto& boundShortcut : boundShortcuts) {
		auto binding = GetShortcutBinding(boundShortcut.shortcutId);

		if (ImGui::IsKeyChordPressed(binding))
			Dispatch(ActionBase{ boundShortcut.actionId });
	}
}

void Desktop::RenderPanel(PanelBase& panel)
{
	const ImGuiWindowFlags windowFlags = 0;

	auto traits = panel.GetPanelTraits();

	ImGui::SetNextWindowPos(traits.position, ImGuiCond_FirstUseEver, traits.pivot);
	ImGui::SetNextWindowSize(traits.size, ImGuiCond_FirstUseEver);

	if (ImGui::Begin(traits.title, NULL, windowFlags))
		panel.RenderPanel();

	ImGui::End();
}

bool Desktop::BeginSceneWindow()
{
	return ImGui::Begin("Scene");
}

void Desktop::EndSceneWindow()
{
	ImGui::End();
}

bool Desktop::BeginOverlayWindow()
{
	return ImGui::Begin("Overlay");
}

void Desktop::EndOverlayWindow()
{
	ImGui::End();
}

bool Desktop::BeginMenuWindow()
{
	return ImGui::Begin("Main menu");
}

void Desktop::EndMenuWindow()
{
	ImGui::End();
}

bool Desktop::IsMouseOverSceneWindow()
{
	return !ImGui::GetIO().WantCaptureMouse && !ImGui::IsAnyItemHovered();
}
