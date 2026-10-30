#pragma once
#include <ui/common/StandaloneWindow.h>
#include <imbrowser.h>
#include <ImGuiFileDialogFileSystemOverride.h>

class ResourceBrowser : public StandaloneWindow
{
    csl::ut::MoveArray<hh::fnd::Reference<hh::fnd::Packfile>> currentPath{ GetAllocator() };
    hh::fnd::Reference<hh::fnd::DynamicResourceContainer> selectedResourceContainer{};
    hh::fnd::Reference<hh::fnd::ManagedResource> selectedResource{};
    ImBrowser::ImBrowserContext* browser{};
    ImBrowser::ViewType currentView{};
#ifndef DEVTOOLS_TARGET_SDK_wars
    IGFD::FileDialog loadNewDialog{ new FileSystemHE2{} };
    hh::fnd::Reference<hh::fnd::ResourceLoader> resourceLoader{ new (GetAllocator()) hh::fnd::ResourceLoader{ GetAllocator() } };
#endif

public:
    class PropSet {
    public:
        struct PropVal {
	        const char* key{};
	        char value[128]{};
        };

        csl::ut::MoveArray<PropVal> props;

        inline PropSet(csl::fnd::IAllocator* allocator) : props{ allocator } {}
        template<typename... Args>
        void AddProp(const char* key, const char* fmt, Args... args) {
            props.emplace_back();
            auto& prop = props[props.size() - 1];

            prop.key = key;
            snprintf(prop.value, 128, fmt, args...);
        }
    };

    ResourceBrowser(csl::fnd::IAllocator* allocator);
    ~ResourceBrowser();

    virtual void PreRender() override;
    virtual void RenderContents() override;
    void RenderMainArea();
    void RenderResource(hh::fnd::ManagedResource* resource);
    ImBrowser::Image GetImage(hh::fnd::ManagedResource* resource);
    void GetProps(hh::fnd::ManagedResource* resource, PropSet& props);
    void RenderContainerContents(const hh::fnd::ResourceContainer * container);
    static void ExportResource(const wchar_t* filePath, hh::fnd::ManagedResource* resource);
    static void ShowLoadResourceDialog(hh::fnd::ManagedResource* resource);
    static void ShowExportResourceDialog(hh::fnd::ManagedResource* resource);
    static void ShowExportResourceDialog(hh::fnd::ManagedResource** resources, size_t count);
    void RenderLoadNewResourceDialog();
    static void RenderLoadDialog();
    static void RenderExportDialog();
    void RenderDialogs();
    static void RenderGlobalDialogs();
};
