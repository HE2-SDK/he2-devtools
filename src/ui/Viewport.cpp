#ifdef DEVTOOLS_TARGET_SDK_rangers
#include "Viewport.h"
#include <ui/common/viewers/Basic.h>
#include <ui/common/Textures.h>

using namespace hh::gfx;

bool Viewport::isHovered = false;

void Viewport::Render() {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    if (auto* renderingEngine = static_cast<RenderingEngineRangers*>(static_cast<RenderManager*>(RenderManager::GetInstance())->GetNeedleResourceDevice()))
#elif DEVTOOLS_TARGET_SDK_miller
    if (auto* renderingEngine = static_cast<RenderingEngineMiller*>(static_cast<RenderManager*>(RenderManager::GetInstance())->GetNeedleResourceDevice()))
#endif
    {
        auto& rt = renderingEngine->supportFX.renderTargetTextureView;
        ImGui::Begin("Viewport");

        const ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        const ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        const ImVec2 windowPos = ImGui::GetWindowPos();

        const ImVec2 min = { contentMin.x + windowPos.x, contentMin.y + windowPos.y };
        const ImVec2 max = { contentMax.x + windowPos.x, contentMax.y + windowPos.y };

        if (isHovered = (ImGui::IsMouseHoveringRect(min, max) && ImGui::IsMouseDown(ImGuiMouseButton_Right))) {
            bool moveMouse = false;
            float mouseX = 0;
            float mouseY = 0;
            if (auto* mouse = reinterpret_cast<hh::hid::MouseWin32*>(hh::hid::DeviceManagerWin32::GetInstance()->mouse)) {
                if (moveMouse) {
                    mouse->flags.bits = 6;
                    mouse->state.deltaX = mouseX - mouse->absoluteX;
                    mouse->state.deltaY = mouseY - mouse->absoluteY;
                    mouse->deltaX = mouseX - mouse->absoluteX;
                    mouse->deltaY = mouseY - mouse->absoluteY;
                    mouse->prevX = mouse->absoluteX;
                    mouse->prevY = mouse->absoluteY;
                    mouse->absoluteX = mouseX;
                    mouse->absoluteY = mouseY;
                    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                        mouse->state.buttonState.set(hh::hid::Mouse::MouseState::ButtonStateFlag::LEFT);
                    else
                        mouse->state.buttonState.reset(hh::hid::Mouse::MouseState::ButtonStateFlag::LEFT);
                    mouse->Update(1);
                }
                else
                    mouse->flags.bits = 0;
            }
        }

        ImGui::Image(GetTextureIDFromMIRAGETexture(rt), ImGui::GetContentRegionAvail());
        ImGui::End();
    }
}
#endif
