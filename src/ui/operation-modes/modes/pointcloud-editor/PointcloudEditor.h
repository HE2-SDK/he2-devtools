#pragma once
#include <debug-rendering/DebugRenderable.h>
#include <ui/operation-modes/OperationMode.h>
#include "Context.h"

namespace ui::operation_modes::modes::pointcloud_editor {
    class PointcloudEditor : public OperationMode<Context> {
    public:
        PointcloudEditor(csl::fnd::IAllocator* allocator, OperationModeHost& host, const hh::fnd::ResourceTypeInfo* typeInfo);

        virtual void ProcessAction(const ActionBase& action) override;
        virtual void RenderScene() override;
    };
}
