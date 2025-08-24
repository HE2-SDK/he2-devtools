#pragma once
#include <ui/operation-modes/OperationMode.h>
#include <ui/common/StandaloneWindow.h>
#include <utilities/CompatibleObject.h>
#include "Action.h"
#include "Shortcuts.h"

class Desktop : public CompatibleObject, OperationModeHost {
    struct ShortcutBinding {
        ShortcutId shortcutId;
        ActionId actionId;
    };

    hh::fnd::Reference<hh::gfnd::ResTexture> iconTexture{};
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windows{ GetAllocator() };
    csl::ut::MoveArray<hh::fnd::Reference<StandaloneWindow>> windowsThatOpened{ GetAllocator() };
    csl::ut::MoveArray<ShortcutBinding> boundShortcuts{ GetAllocator() };
    void AddStandaloneWindow(StandaloneWindow* window);
    void RemoveStandaloneWindow(StandaloneWindow* window);
public:
    static bool selectionColliderFilters[32][32];
    hh::fnd::Reference<OperationModeBase> operationMode{};
    static Desktop* instance;

    ID3D11ShaderResourceView* iconView;
    Desktop(csl::fnd::IAllocator* allocator);
    void Render();
    void RenderSceneWindow();
    void RenderOverlayWindow();
    void OpenStandaloneWindow(StandaloneWindow* window);
    
    // TODO: Refactor this!
    void SwitchToOperationMode(hh::fnd::Reference<OperationModeBase> mode) {
        if (operationMode != nullptr)
            operationMode->DeinitBehaviors();

        operationMode = mode;

        operationMode->InitBehaviors();
    }
    template<typename T>
    inline void SwitchToOperationMode() {
        SwitchToOperationMode(new (GetAllocator()) T{ GetAllocator(), *this });
    }
    template<typename T>
    inline void SwitchToOperationMode(const hh::fnd::ResourceTypeInfo* typeInfo) {
        SwitchToOperationMode(new (GetAllocator()) T{ GetAllocator(), *this, typeInfo });
    }
    void Dispatch(const ActionBase& action);

    template<typename T, typename = std::enable_if_t<std::is_same_v<typename T::PayloadType, EmptyActionPayload>>> void BindShortcut(ShortcutId shortcutId) {
        boundShortcuts.push_back({ shortcutId, T::id });
    }

    void UnbindShortcut(ShortcutId shortcutId);
    void HandleShortcuts();

    // Inherited via OperationModeHost
    void RenderPanel(PanelBase& panel) override;
    bool BeginSceneWindow() override;
    void EndSceneWindow() override;
    bool BeginOverlayWindow() override;
    void EndOverlayWindow() override;
    bool BeginMenuWindow() override;
    void EndMenuWindow() override;
    bool IsMouseOverSceneWindow() override;
};
