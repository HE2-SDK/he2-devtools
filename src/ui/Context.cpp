#include "fonts/Inter.h"
#include "common/Theme.h"
#include "Context.h"
#include "input/Input.h"
#include "Desktop.h"
#include "SettingsManager.h"
#include "operation-modes/modes/object-inspection/ObjectInspection.h"
#include <debug-rendering/DebugRenderingSystem.h>
#include <resources/ReloadManager.h>
#include <resources/ManagedMemoryRegistry.h>
#include "Context.h"
#include <modules/PhotoMode.h>
#include <ui/common/icons.h>
#include <api/API.h>

static ID3D11Device* device;
static ID3D11DeviceContext* deviceContext;
static ID3D11RenderTargetView* renderTargetView;
static ID3D11Texture2D* backBuffer;
static ImFont* font;

bool Context::visible = false;
bool Context::passThroughMouse = false;
bool Context::inited = false;
bool Context::imguiInited = false;
bool Context::enableViewports = false;
float Context::fontSize = 14.0f;
bool Context::reinitImGuiNextFrame = false;
bool Context::rebuildFontsNextFrame = false;
bool Context::enableApi = false;
char Context::apiHost[] = "127.0.0.1";
unsigned short Context::apiPort = 7007;
csl::fnd::IAllocator* Context::allocator = nullptr;
csl::ut::MoveArray<hh::fnd::Reference<Module>> Context::modules{};

using namespace hh::game;
using namespace hh::needle;

#ifdef DEVTOOLS_TARGET_SDK_wars
constexpr size_t appResetAddr = 0x145400300;
constexpr size_t appShutdownAddr = 0x145401AE0;
constexpr size_t wndProcAddr = 0x1406EC680;
constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x14082FD50;
constexpr size_t displaySwapDevicePresentAddr = 0x1463FA880;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
constexpr size_t appResetAddr = 0x14FEF16E0;
constexpr size_t appShutdownAddr = 0x14FEE88A0;
constexpr size_t wndProcAddr = 0x140D68F80;
constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x1410FB090;
constexpr size_t displaySwapDevicePresentAddr = 0x1410FAEE0;
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
constexpr size_t appResetAddr = 0x145E64E30;
constexpr size_t appShutdownAddr = 0x14F2BF7C0;
constexpr size_t wndProcAddr = 0x140A56B60;
constexpr size_t displaySwapDeviceResizeBuffersAddr = 0x140F09030;
constexpr size_t displaySwapDevicePresentAddr = 0x140F08E60;
#endif

HOOK(uint64_t, __fastcall, GameApplication_Reset, appResetAddr, hh::game::GameApplication* self) {
	auto res = originalGameApplication_Reset(self);
	Context::init();
	return res;
}

HOOK(uint64_t, __fastcall, GameApplication_Shutdown, appShutdownAddr, hh::game::GameApplication* self) {
	Context::deinit();
	return originalGameApplication_Shutdown(self);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HOOK(LRESULT, __fastcall, WndProcHook, wndProcAddr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY) {
		// Deinitialize ImGui before the window is destroyed.
		// If we don't do this, the game will crash when closing the window, because ImGui pops out
		// all the windows into new viewports with invalid handles.
		Context::deinit_imgui();
	}
	if (msg == WM_KEYDOWN && wParam == VK_F8) {
		Context::visible = !Context::visible;
		return true;
	}
	if (msg == WM_KEYDOWN && wParam == VK_F7 && Context::visible) {
		Context::passThroughMouse = !Context::passThroughMouse;
		return true;
	}
	if (Context::imguiInited && Context::visible) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		if (ShouldCaptureMouseInputs() && msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
			return true;

		if (ShouldCaptureKeyboardInputs() && msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
			return true;
	}

	return originalWndProcHook(hWnd, msg, wParam, lParam);
}

static void createBackBuffer(IDXGISwapChain* This)
{
	if (device)
	{
		// TODO: fix scaling.
		This->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();
	}
}

HOOK(bool, __fastcall, DisplaySwapDevice_ResizeBuffers, displaySwapDeviceResizeBuffersAddr, hh::needle::ImplDX11::DisplaySwapDeviceDX11* self, unsigned int* width, unsigned int* height)
{
	auto result = originalDisplaySwapDevice_ResizeBuffers(self, width, height);

	createBackBuffer(self->swapChain);

	if (Context::imguiInited) {
		bool shouldEnableViewports = Context::should_enable_viewports_considering_fullscreen(self);
		bool viewportsEnabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;

		if (shouldEnableViewports != viewportsEnabled)
			Context::reinit_imgui();
	}

	return result;
}

HOOK(bool, __fastcall, DisplaySwapDevice_Present, displaySwapDevicePresentAddr, hh::needle::ImplDX11::DisplaySwapDeviceDX11* self, unsigned int flags)
{
	for (auto& module : Context::modules)
		module->Update();

	if (Context::imguiInited && Context::visible) {
		ShowCursor(true);
		if (!renderTargetView) {
			createBackBuffer(self->swapChain);
		}
		Context::update();
	}

	return originalDisplaySwapDevice_Present(self, flags);
}

//HOOK(bool, __fastcall, CreateRenderingDeviceDX11, 0x1410EC330, hh::needle::RenderingDevice** renderingDevice, hh::needle::RenderingDeviceContext** renderingDeviceContext, void* deviceCreationSetting, void** displaySwapDevice, unsigned int creationFlags)
//{
//	return originalCreateRenderingDeviceDX11(renderingDevice, renderingDeviceContext, deviceCreationSetting, displaySwapDevice, D3D11_CREATE_DEVICE_DEBUG);
//}

//#ifdef DEVTOOLS_TARGET_SDK_rangers
//HOOK(bool, __fastcall, GOCCamera_PushController, 0x14D39B880, app_cmn::camera::GOCCamera* self, hh::fnd::Handle<hh::fnd::Messenger>& cameraFrame, unsigned int controllerId, unsigned int unkParam1, app_cmn::camera::CameraInterpolator* interpolator)
//{
//	if (self->owner->objectClass == app::player::Sonic::GetClass() || self->owner->objectClass == app::player::Amy::GetClass() || self->owner->objectClass == app::player::Knuckles::GetClass() || self->owner->objectClass == app::player::Tails::GetClass()) {
//		auto* player = static_cast<app::player::Player*>(self->owner);
//		hh::fnd::Handle<hh::fnd::Messenger> cameraFrame2 = hh::game::GameManager::GetInstance()->GetService<app::camera::CameraService>()->GetCameraFrame(player->setupInfo.playerId * 2);
//		return originalGOCCamera_PushController(self, cameraFrame2, controllerId, unkParam1, interpolator);
//	}
//	else
//		return originalGOCCamera_PushController(self, cameraFrame, controllerId, unkParam1, interpolator);
//}
//#endif

void Context::install_hooks()
{
	INSTALL_HOOK(GameApplication_Reset);
	INSTALL_HOOK(GameApplication_Shutdown);
	INSTALL_HOOK(WndProcHook);
	INSTALL_HOOK(DisplaySwapDevice_ResizeBuffers);
	INSTALL_HOOK(DisplaySwapDevice_Present);
	InstallInputHooks();
	//INSTALL_HOOK(RenderingEngineRangers_SetupMainRenderUnit);
//#ifdef DEVTOOLS_TARGET_SDK_rangers
//	INSTALL_HOOK(GOCCamera_PushController);
//#endif
	//INSTALL_HOOK(CreateRenderingDeviceDX11);
	devtools::debug_rendering::renderables::GOCVisualDebugDraws::InstallHooks();
}

void Context::init() {
	if (inited)
		return;

	init_modules();
	init_imgui();

	inited = true;
}

void Context::deinit()
{
	if (!inited)
		return;

	inited = false;

	deinit_imgui();
	deinit_modules();
}

void Context::init_modules()
{
	auto* moduleAllocator = hh::fnd::MemoryRouter::GetModuleAllocator();
	//auto* allocator = moduleAllocator;

#ifdef DEVTOOLS_TARGET_SDK_wars
	//static hh::fnd::ThreadSafePooledAllocator devtoolsAllocator{ "devtools" };
	//devtoolsAllocator.Setup(moduleAllocator, 100 * 1024 * 1024, nullptr, 0, true);
	//devtoolsAllocator.SetName("devtools");
	//auto* allocator = &devtoolsAllocator;
	allocator = moduleAllocator;
#endif
#ifdef DEVTOOLS_TARGET_SDK_rangers
	static hh::fnd::ThreadSafeTlsfHeapAllocator devtoolsAllocator{ "devtools" };
	devtoolsAllocator.Setup(moduleAllocator, { 100 * 1024 * 1024, true });
	allocator = &devtoolsAllocator;
#endif
#ifdef DEVTOOLS_TARGET_SDK_miller
	allocator = moduleAllocator;
#endif

#ifndef DEVTOOLS_TARGET_SDK_miller
	ReloadManager::instance = new (allocator) ReloadManager(allocator);
#endif
#ifdef DEVTOOLS_TARGET_SDK_wars
	RESOLVE_STATIC_VARIABLE(hh::game::DebugCameraManager::instance) = hh::game::DebugCameraManager::Create();
#endif
	devtools::debug_rendering::DebugRenderingSystem::Init(allocator);
	Desktop::instance = new (allocator) Desktop{ allocator };
	Desktop::instance->SwitchToOperationMode<ui::operation_modes::modes::object_inspection::ObjectInspection>();
	resources::ManagedMemoryRegistry::Init(allocator);
	modules = { allocator };
	modules.push_back(new (allocator) PhotoMode{ allocator });
}

void Context::deinit_modules()
{
	modules.clear();
	resources::ManagedMemoryRegistry::Deinit();
	Desktop::instance->~Desktop();
	Desktop::instance->GetAllocator()->Free(Desktop::instance);
	devtools::debug_rendering::DebugRenderingSystem::Deinit();
#ifndef DEVTOOLS_TARGET_SDK_miller
	ReloadManager::instance->GetAllocator()->Free(ReloadManager::instance);
#endif
}

void Context::init_imgui()
{
	if (imguiInited)
		return;

	auto* renderManager = static_cast<hh::gfx::RenderManager*>(hh::gfx::RenderManager::GetInstance());
	auto* renderingEngine = renderManager->GetNeedleResourceDevice();

	device = renderingEngine->GetNativeDevice();

	device->GetImmediateContext(&deviceContext);

	HWND hwnd = reinterpret_cast<HWND>(hh::fw::Application::GetInstance()->frameworkEnvironment->window->hWnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();

	SettingsManager::Init();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	if (enableViewports && !renderingEngine->GetSupportFX()->swapDevice->GetFullScreenState())
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	rebuild_fonts();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	imguiInited = true;
}

void Context::deinit_imgui()
{
	if (!imguiInited)
		return;

	imguiInited = false;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void Context::reinit_imgui()
{
	deinit_imgui();
	init_imgui();
}

void Context::rebuild_fonts()
{
	ImGuiIO& io = ImGui::GetIO();

	//static ImWchar ranges[] = { 0x1, 0xffff, 0 };
	ImFontConfig fontConfig{};
	io.Fonts->Clear();
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoHinting;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_NoAutoHint;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bitmap;
	//fontConfig.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;
	font = io.Fonts->AddFontFromMemoryCompressedTTF((void*)inter_compressed_data, inter_compressed_size, fontSize, &fontConfig);// , ranges);
	RegisterIconGlyphs(font);
	io.Fonts->Build();
	AddIconTextures();

	if (imguiInited)
		ImGui_ImplDX11_InvalidateDeviceObjects();
}

void Context::set_api_config(bool enable, const char* host, unsigned short port)
{
	if (enable != enableApi || strcmp(apiHost, host) || apiPort != port) {
		for (size_t i = 0; i < modules.size(); i++) {
			if (modules[i]->GetId() == devtools::api::API::id)
				modules.remove(i);
		}

		if (enable)
			modules.push_back(new (allocator) devtools::api::API{ allocator, host, port });
	}
	enableApi = enable;
	strcpy_s(apiHost, host);
	apiPort = port;
}

void Context::set_font_size(float size)
{
	if (fontSize == size)
		return;

	fontSize = size;
	
	if (!imguiInited)
		return;

	rebuildFontsNextFrame = true;
}

void Context::set_enable_viewports(bool enable)
{
	if (enableViewports == enable)
		return;

	enableViewports = enable;

	if (!imguiInited)
		return;

	reinitImGuiNextFrame = true;
}

bool Context::should_enable_viewports_considering_fullscreen(hh::needle::DisplaySwapDevice* swapDevice)
{
	return enableViewports && !swapDevice->GetFullScreenState();
}

void Context::update()
{
	if (reinitImGuiNextFrame) {
		reinit_imgui();
		reinitImGuiNextFrame = false;
	}

	if (rebuildFontsNextFrame) {
		rebuild_fonts();
		rebuildFontsNextFrame = false;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//ImGui::PushFont(firaCode);
#ifdef DEVTOOLS_ENABLE_IMGUI_DEMO_WINDOW
	ImGui::ShowDemoWindow();
#endif
#ifdef DEVTOOLS_ENABLE_IMPLOT_DEMO_WINDOW
	ImPlot::ShowDemoWindow();
#endif
	Desktop::instance->Render();

	ImGui::Render();
	//ImGui::PopFont();

	deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
