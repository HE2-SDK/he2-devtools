target_precompile_headers(${DEVTOOLS_TARGET}
    PRIVATE
        Pch.h
)
target_sources(${DEVTOOLS_TARGET}
    PRIVATE
        devtools.rc
        DllMain.cpp
        Mod.cpp
        ImGuiFileDialog.cpp
        debug-rendering/DebugRenderable.cpp
        debug-rendering/DebugRenderer.cpp
        debug-rendering/DebugRenderingSystem.cpp
        debug-rendering/renderables/Colliders.cpp
        debug-rendering/renderables/GOCVisualDebugDraws.cpp
        debug-rendering/renderables/OcclusionCapsules.cpp
        debug-rendering/renderables/Paths.cpp
        debug-rendering/renderables/Bones.cpp
        debug-rendering/renderables/PhysicalAnimation.cpp
        debug-rendering/renderables/Light.cpp
        debug-rendering/visuals/AabbTree.cpp
        debug-rendering/visuals/Geometry.cpp
        debug-rendering/visuals/KdTree.cpp
        debug-rendering/visuals/Math.cpp
        api/API.cpp
        api/DebugCamera.cpp
        api/ObjectWorld.cpp
        api/GameManager.cpp
        api/RenderManager.cpp
        modules/PhotoMode.cpp
        resources/ManagedMemoryRegistry.cpp
        resources/ReloadManager.cpp
        reflection/RflDiff.cpp
        ui/common/NodeEditorAutoLayout.cpp
        ui/common/editors/Basic.cpp
        ui/common/editors/Reflection.cpp
        ui/common/editors/GraphicsContext.cpp
        ui/common/editors/Effects.cpp
        ui/common/editors/${DEVTOOLS_TARGET_SDK}/LevelInfo.cpp
        ui/common/editors/Pointcloud.cpp
        ui/common/editors/Resources.cpp
        ui/common/editors/HsmBase.cpp
        ui/common/viewers/Basic.cpp
        ui/common/viewers/RflDiff.cpp
        ui/common/inputs/Basic.cpp
        ui/common/overlays/Tag.cpp
        ui/common/previewer/Previewer.cpp
        ui/common/icons.cpp
        ui/common/StandaloneWindow.cpp
        ui/common/stb_image.cpp
        ui/common/Textures.cpp
        ui/common/Theme.cpp
        ui/common/Translations.cpp
        ui/Context.cpp
        ui/input/Input.cpp
        ui/input/${DEVTOOLS_TARGET_SDK}/Input.cpp
        ui/Action.cpp
        ui/Component.cpp
        ui/Desktop.cpp
        ui/GlobalSettings.cpp
        ui/game-services/GameServiceInspector.cpp
        ui/operation-modes/OperationModeBehavior.cpp
        ui/operation-modes/OperationMode.cpp
        ui/resources/editors/ResReflectionEditor.cpp
        ui/resources/ResourceBrowser.cpp
        ui/SettingsManager.cpp
        ui/Shortcuts.cpp
        ui/ToolBar.cpp
        ui/Viewport.cpp
        ui/tools/RflComparer.cpp
        ui/tools/MemoryViewer.cpp
        utilities/BoundingBoxes.cpp
        utilities/math/EulerTransform.cpp
        utilities/math/Frustum.cpp
        utilities/math/MathUtils.cpp
        utilities/ObjectDataUtils.cpp
        utilities/GameObjectUtils.cpp
        utilities/ResourceTypes.cpp
        utilities/CompatibleObject.cpp
        utilities/SurfRideBugFix.cpp
    PRIVATE FILE_SET HEADERS FILES
        Pch.h
        ImGuiFileDialog.h
        ImGuiFileDialogConfig.h
        debug-rendering/DebugRenderable.h
        debug-rendering/DebugRenderer.h
        debug-rendering/DebugRenderingSystem.h
        debug-rendering/renderables/Colliders.h
        debug-rendering/renderables/GOCVisualDebugDraws.h
        debug-rendering/renderables/OcclusionCapsules.h
        debug-rendering/renderables/Paths.h
        debug-rendering/renderables/Bones.h
        debug-rendering/renderables/PhysicalAnimation.h
        debug-rendering/renderables/Light.h
        debug-rendering/visuals/AabbTree.h
        debug-rendering/visuals/Geometry.h
        debug-rendering/visuals/KdTree.h
        debug-rendering/visuals/Math.h
        api/API.h
        api/APIContext.h
        api/Common.h
        api/DebugCamera.h
        api/ObjectWorld.h
        api/GameManager.h
        api/RenderManager.h
        modules/PhotoMode.h
        resources/managed-memory/ManagedCArray.h
        resources/ManagedMemoryRegistry.h
        resources/ReloadManager.h
        reflection/RflDiff.h
        ui/GlobalSettings.h
        ui/common/NodeEditorAutoLayout.h
        ui/common/editors/Basic.h
        ui/common/editors/Camera.h
        ui/common/editors/Reflection.h
        ui/common/editors/GraphicsContext.h
        ui/common/editors/Effects.h
        ui/common/editors/LevelInfo.h
        ui/common/editors/Needle.h
        ui/common/editors/Pointcloud.h
        ui/common/editors/Resources.h
        ui/common/editors/HsmBase.h
        ui/common/viewers/Basic.h
        ui/common/viewers/RflDiff.h
        ui/common/inputs/Basic.h
        ui/common/inputs/Needle.h
        ui/common/overlays/Tag.h
        ui/common/previewer/Previewer.h
        ui/common/icons.h
        ui/common/NodeEditor.h
        ui/common/StandaloneWindow.h
        ui/common/stb_image.h
        ui/common/Textures.h
        ui/common/Theme.h
        ui/common/Translations.h
        ui/common/TreeView.h
        ui/Context.h
        ui/input/Input.h
        ui/Action.h
        ui/Component.h
        ui/Desktop.h
        ui/fonts/FiraCode.h
        ui/fonts/Inter.h
        ui/game-services/GameServiceInspector.h
        ui/operation-modes/behaviors/Clipboard.h
        ui/operation-modes/behaviors/DebugCommentsVisual.h
        ui/operation-modes/behaviors/Gizmo.h
        ui/operation-modes/behaviors/MousePicking.h
        ui/operation-modes/behaviors/ObjectLocationVisual3D.h
        ui/operation-modes/behaviors/Selection.h
        ui/operation-modes/behaviors/SelectionAabb.h
        ui/operation-modes/behaviors/SelectionMousePicking.h
        ui/operation-modes/behaviors/SelectionTransformation.h
        ui/operation-modes/behaviors/SelectionVisual.h
        ui/operation-modes/behaviors/Zoom.h
        ui/operation-modes/OperationModeHost.h
        ui/operation-modes/OperationModeBehavior.h
        ui/operation-modes/OperationMode.h
        ui/resources/editors/ResReflectionEditor.h
        ui/resources/ResourceBrowser.h
        ui/SettingsManager.h
        ui/Shortcuts.h
        ui/ToolBar.h
        ui/Viewport.h
        ui/tools/RflComparer.h
        ui/tools/MemoryViewer.h
        utilities/BoundingBoxes.h
        utilities/Helpers.h
        utilities/math/EulerTransform.h
        utilities/math/MathUtils.h
        utilities/math/Frustum.h
        utilities/math/Ray.h
        utilities/ObjectDataUtils.h
        utilities/GameObjectUtils.h
        utilities/ResourceTypes.h
        utilities/CompatibleObject.h
        utilities/NameHash.h
        utilities/SurfRideBugFix.h
)


if(${DEVTOOLS_TARGET_SDK} STREQUAL "wars")
    set(DEVTOOLS_FEATURE_OBJECT_INSPECTION ON)
    set(DEVTOOLS_FEATURE_LEVEL_EDITOR ON)
    set(DEVTOOLS_FEATURE_FXCOL_EDITOR ON)
    set(DEVTOOLS_FEATURE_SVCOL_EDITOR ON)
    set(DEVTOOLS_FEATURE_PCMODEL_EDITOR OFF)
    set(DEVTOOLS_FEATURE_PCRT_EDITOR OFF)
    set(DEVTOOLS_FEATURE_SURFRIDE_EDITOR ON)
    set(DEVTOOLS_FEATURE_ASM_EDITOR OFF)
    set(DEVTOOLS_FEATURE_DVSCENE_EDITOR OFF)
    set(DEVTOOLS_FEATURE_SCENE_EDITOR ON)
    set(DEVTOOLS_FEATURE_LEGACY_NEEDLEFX_TESTER OFF)
    set(DEVTOOLS_FEATURE_NEEDLEFX_TESTER ON)
    set(DEVTOOLS_FEATURE_RENDERING_ENGINE_INSPECTORS OFF)
    set(DEVTOOLS_FEATURE_UI_MAP_PARAM_EDITOR OFF)

    set(DEVTOOLS_FEATURE_COMMON_INPUT_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_COMMON_EDITOR_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_COMMON_VIEWER_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_GAME_OBJECT_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_COMPONENT_INSPECTORS
        GOCTransform
        GOCCollider
        GOCSprite
        GOCPlayerParameter
    )
    set(DEVTOOLS_FEATURE_RESOURCE_INSPECTORS
        ResObjectWorldEditor
        ResReflectionEditor
        ResFxColFile2Editor
    )
    set(DEVTOOLS_FEATURE_CORE_SERVICE_INSPECTORS
        MemoryInspector
    )
    set(DEVTOOLS_FEATURE_GAME_SERVICE_INSPECTORS
        FxParamManager
        FxColManager
        ObjectWorld
        StageInfo
        CameraManager
    )
endif()

if(${DEVTOOLS_TARGET_SDK} STREQUAL "rangers")
    set(DEVTOOLS_FEATURE_OBJECT_INSPECTION ON)
    set(DEVTOOLS_FEATURE_LEVEL_EDITOR ON)
    set(DEVTOOLS_FEATURE_FXCOL_EDITOR ON)
    set(DEVTOOLS_FEATURE_SVCOL_EDITOR ON)
    set(DEVTOOLS_FEATURE_PCMODEL_EDITOR ON)
    set(DEVTOOLS_FEATURE_PCRT_EDITOR ON)
    set(DEVTOOLS_FEATURE_SURFRIDE_EDITOR ON)
    set(DEVTOOLS_FEATURE_ASM_EDITOR ON)
    set(DEVTOOLS_FEATURE_DVSCENE_EDITOR ON)
    set(DEVTOOLS_FEATURE_SCENE_EDITOR OFF)
    set(DEVTOOLS_FEATURE_LEGACY_NEEDLEFX_TESTER ON)
    set(DEVTOOLS_FEATURE_NEEDLEFX_TESTER ON)
    set(DEVTOOLS_FEATURE_RENDERING_ENGINE_INSPECTORS ON)
    set(DEVTOOLS_FEATURE_UI_MAP_PARAM_EDITOR ON)

    set(DEVTOOLS_FEATURE_COMMON_INPUT_INSPECTORS
        Needle
    )
    set(DEVTOOLS_FEATURE_COMMON_EDITOR_INSPECTORS
        TimeService
        Camera
        Needle
    )
    set(DEVTOOLS_FEATURE_COMMON_VIEWER_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_GAME_OBJECT_INSPECTORS
        CameraFrame
        ObjCameraVolume
        ObjGismo
        BossGiant
    )
    set(DEVTOOLS_FEATURE_COMPONENT_INSPECTORS
        GOCTransform
        GOCCollider
        GOCSprite
        GOCUIComposition
        GOCEffect
        GOCPlayerParameter
        GOCPlayerHsm
        GOCPlayerVisual
        GOCPlayerEffect
        GOCEvent
        GOCCamera
        GOCAnimation
        GOCAnimator
        GOCVisual
        GOCPhysicalAnimation
        GOCSound
        GOCParticleLocator
        PathComponent
        GOCActivator
        GOCPlayerBlackboard
        GOCPlayerKinematicParams
        GOCColliderQuery
        GOCPlayerCollider
        GOCPointLight
        GOCVibration
    )
    set(DEVTOOLS_FEATURE_RESOURCE_INSPECTORS
        ResObjectWorldEditor
        ResReflectionEditor
        ResMaterialEditor
        ResFxColFile2Editor
        ResEffectEditor
        ResPhysicalSkeletonEditor
        ResGismoConfigDesignEditor
        ResGismoConfigPlanEditor
        ResMirageLightEditor
        ResVibrationEditor
        ResModelEditor
    )
    set(DEVTOOLS_FEATURE_CORE_SERVICE_INSPECTORS
        MemoryInspector
        GameUpdaterInspector
        GraphicsContextInspector
        RenderManagerInspector
        RenderingEngineInspector
        GameModeInspector
    )
    set(DEVTOOLS_FEATURE_GAME_SERVICE_INSPECTORS
        FxParamManager
        FxColManager
        ObjectWorld
        StageInfo
        CameraManager
        LevelInfo
        CameraService
        GameModeResourceManager
        TerrainManager
        SoundDirector
        PathManager
        TimeService
        RespawnTimeManager
        MonologueService
        LevelManager
        DiEventManager
        EventPlayer
        MovieManager
        SoundManagerCri
    )
endif()

if(${DEVTOOLS_TARGET_SDK} STREQUAL "miller")
    set(DEVTOOLS_FEATURE_OBJECT_INSPECTION ON)
    set(DEVTOOLS_FEATURE_LEVEL_EDITOR ON)
    set(DEVTOOLS_FEATURE_FXCOL_EDITOR OFF)
    set(DEVTOOLS_FEATURE_SVCOL_EDITOR ON)
    set(DEVTOOLS_FEATURE_PCMODEL_EDITOR OFF)
    set(DEVTOOLS_FEATURE_PCRT_EDITOR OFF)
    set(DEVTOOLS_FEATURE_SURFRIDE_EDITOR ON)
    set(DEVTOOLS_FEATURE_ASM_EDITOR ON)
    set(DEVTOOLS_FEATURE_DVSCENE_EDITOR ON)
    set(DEVTOOLS_FEATURE_SCENE_EDITOR OFF)
    set(DEVTOOLS_FEATURE_LEGACY_NEEDLEFX_TESTER OFF)
    set(DEVTOOLS_FEATURE_NEEDLEFX_TESTER ON)
    set(DEVTOOLS_FEATURE_RENDERING_ENGINE_INSPECTORS OFF)
    set(DEVTOOLS_FEATURE_UI_MAP_PARAM_EDITOR OFF)

    set(DEVTOOLS_FEATURE_COMMON_INPUT_INSPECTORS
        Needle
    )
    set(DEVTOOLS_FEATURE_COMMON_EDITOR_INSPECTORS
        Needle
    )
    set(DEVTOOLS_FEATURE_COMMON_VIEWER_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_GAME_OBJECT_INSPECTORS
    )
    set(DEVTOOLS_FEATURE_COMPONENT_INSPECTORS
        GOCTransform
        GOCCollider
        GOCSprite
        GOCUIComposition
        GOCEffect
        GOCPlayerParameter
        GOCPlayerHsm
        GOCAnimation
        GOCAnimator
        GOCVisual
        GOCPhysicalAnimation
        GOCSound
        GOCParticleLocator
        GOCPlayerKinematicParams
    )
    set(DEVTOOLS_FEATURE_RESOURCE_INSPECTORS
        ResObjectWorldEditor
        ResReflectionEditor
        ResMaterialEditor
        ResPhysicalSkeletonEditor
    )
    set(DEVTOOLS_FEATURE_CORE_SERVICE_INSPECTORS
        GraphicsContextInspector
        RenderManagerInspector
    )
    set(DEVTOOLS_FEATURE_GAME_SERVICE_INSPECTORS
        FxParamManager
        ObjectWorld
        StageInfo
        LevelInfo
    )
endif()

function(inspector_collection BASE_PATH ITEM_CLASS)
    foreach(ITEM ${DEVTOOLS_FEATURE_${ITEM_CLASS}_INSPECTORS})
        if(${ITEM} IN_LIST DEVTOOLS_FEATURE_${ITEM_CLASS}_CUSTOMIZED_INSPECTORS)
            target_sources(${DEVTOOLS_TARGET}
                PRIVATE
                    ${BASE_PATH}/${DEVTOOLS_TARGET_SDK}/${ITEM}.cpp
                PRIVATE FILE_SET HEADERS FILES
                    ${BASE_PATH}/${ITEM}.h
            )
        else()
            target_sources(${DEVTOOLS_TARGET}
                PRIVATE
                    ${BASE_PATH}/${ITEM}.cpp
                PRIVATE FILE_SET HEADERS FILES
                    ${BASE_PATH}/${ITEM}.h
            )
        endif()

    endforeach()
endfunction()

set(DEVTOOLS_FEATURE_COMMON_EDITOR_CUSTOMIZED_INSPECTORS
    TimeService
)
set(DEVTOOLS_FEATURE_GAME_SERVICE_CUSTOMIZED_INSPECTORS
    FxParamManager
    TimeService
    RespawnTimeManager
    MonologueService
)

inspector_collection(ui/common/inputs COMMON_INPUT)
inspector_collection(ui/common/editors COMMON_EDITOR)
inspector_collection(ui/common/viewers COMMON_VIEWER)
inspector_collection(ui/core-services CORE_SERVICE)
inspector_collection(ui/game-services/game-service-inspectors GAME_SERVICE)
inspector_collection(ui/resources/editors RESOURCE)

if(${DEVTOOLS_FEATURE_LEGACY_NEEDLEFX_TESTER})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/tools/${DEVTOOLS_TARGET_SDK}/NeedleFxSceneDataTester.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/tools/${DEVTOOLS_TARGET_SDK}/NeedleFxSceneDataTester.h
    )
endif()

if(${DEVTOOLS_FEATURE_NEEDLEFX_TESTER})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/tools/${DEVTOOLS_TARGET_SDK}/NeedleFxSceneDataTesterV2.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/tools/${DEVTOOLS_TARGET_SDK}/NeedleFxSceneDataTesterV2.h
    )
endif()

if(${DEVTOOLS_FEATURE_UI_MAP_PARAM_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/tools/${DEVTOOLS_TARGET_SDK}/UIMapParamEditor.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/tools/${DEVTOOLS_TARGET_SDK}/UIMapParamEditor.h
    )
endif()

if(${DEVTOOLS_FEATURE_OBJECT_INSPECTION})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/operation-modes/modes/object-inspection/Context.cpp
            ui/operation-modes/modes/object-inspection/ObjectInspection.cpp
            ui/operation-modes/modes/object-inspection/ObjectInspector.cpp
            ui/operation-modes/modes/object-inspection/ObjectList.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/operation-modes/modes/object-inspection/Context.h
            ui/operation-modes/modes/object-inspection/ObjectInspection.h
            ui/operation-modes/modes/object-inspection/ObjectInspector.h
            ui/operation-modes/modes/object-inspection/ObjectList.h
    )

    set(DEVTOOLS_FEATURE_COMPONENT_CUSTOMIZED_INSPECTORS
        GOCPlayerParameter
        GOCColliderQuery
        GOCPlayerCollider
    )

    inspector_collection(ui/operation-modes/modes/object-inspection/gameobject-inspectors GAME_OBJECT)
    inspector_collection(ui/operation-modes/modes/object-inspection/component-inspectors COMPONENT)
endif()

if(${DEVTOOLS_FEATURE_LEVEL_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/common/editors/ObjectData.cpp
            ui/common/viewers/ObjectData.cpp
            ui/operation-modes/modes/level-editor/Context.cpp
            ui/operation-modes/modes/level-editor/LevelEditor.cpp
            ui/operation-modes/modes/level-editor/ObjectDataInspector.cpp
            ui/operation-modes/modes/level-editor/ObjectLibrary.cpp
            ui/operation-modes/modes/level-editor/SetObjectList.cpp
            ui/operation-modes/modes/level-editor/ArrayTool.cpp
            ui/resources/editors/ResObjectWorldEditor.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/common/editors/ObjectData.h
            ui/common/viewers/ObjectData.h
            ui/operation-modes/modes/level-editor/Context.h
            ui/operation-modes/modes/level-editor/LevelEditor.h
            ui/operation-modes/modes/level-editor/ObjectDataInspector.h
            ui/operation-modes/modes/level-editor/ObjectLibrary.h
            ui/operation-modes/modes/level-editor/SetObjectList.h
            ui/operation-modes/modes/level-editor/ArrayTool.h
            ui/resources/editors/ResObjectWorldEditor.h
    )
endif()

if(${DEVTOOLS_FEATURE_FXCOL_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            debug-rendering/visuals/FxColCollisionShapeData.cpp
            ui/common/editors/FxCol.cpp
            ui/operation-modes/modes/fxcol-editor/Context.cpp
            ui/operation-modes/modes/fxcol-editor/FxColEditor.cpp
            ui/operation-modes/modes/fxcol-editor/ShapeInspector.cpp
            ui/operation-modes/modes/fxcol-editor/ShapeList.cpp
        PRIVATE FILE_SET HEADERS FILES
            debug-rendering/visuals/FxColCollisionShapeData.h
            ui/common/editors/FxCol.h
            ui/operation-modes/modes/fxcol-editor/Context.h
            ui/operation-modes/modes/fxcol-editor/FxColEditor.h
            ui/operation-modes/modes/fxcol-editor/ShapeInspector.h
            ui/operation-modes/modes/fxcol-editor/ShapeList.h
    )
endif()

if(${DEVTOOLS_FEATURE_SVCOL_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            debug-rendering/visuals/SvColShape.cpp
            ui/common/editors/SvCol.cpp
            ui/operation-modes/modes/svcol-editor/Context.cpp
            ui/operation-modes/modes/svcol-editor/SvColEditor.cpp
            ui/operation-modes/modes/svcol-editor/ShapeInspector.cpp
            ui/operation-modes/modes/svcol-editor/ShapeList.cpp
        PRIVATE FILE_SET HEADERS FILES
            debug-rendering/visuals/SvColShape.h
            ui/common/editors/SvCol.h
            ui/operation-modes/modes/svcol-editor/Context.h
            ui/operation-modes/modes/svcol-editor/SvColEditor.h
            ui/operation-modes/modes/svcol-editor/ShapeInspector.h
            ui/operation-modes/modes/svcol-editor/ShapeList.h
    )
endif()

if(${DEVTOOLS_FEATURE_PCMODEL_EDITOR} OR ${DEVTOOLS_FEATURE_PCRT_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/operation-modes/modes/pointcloud-editor/Math.cpp
            ui/operation-modes/modes/pointcloud-editor/Context.cpp
            ui/operation-modes/modes/pointcloud-editor/PointCloudEditor.cpp
            ui/operation-modes/modes/pointcloud-editor/InstanceInspector.cpp
            ui/operation-modes/modes/pointcloud-editor/InstanceList.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/operation-modes/modes/pointcloud-editor/Math.h
            ui/operation-modes/modes/pointcloud-editor/Context.h
            ui/operation-modes/modes/pointcloud-editor/PointCloudEditor.h
            ui/operation-modes/modes/pointcloud-editor/InstanceInspector.h
            ui/operation-modes/modes/pointcloud-editor/InstanceList.h
    )
endif()

if(${DEVTOOLS_FEATURE_SURFRIDE_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/common/editors/SurfRide.cpp
            ui/operation-modes/modes/surfride-editor/Context.cpp
            ui/operation-modes/modes/surfride-editor/Timeline.cpp
            ui/operation-modes/modes/surfride-editor/CropLibrary.cpp
            ui/operation-modes/modes/surfride-editor/ProjectTree.cpp
            ui/operation-modes/modes/surfride-editor/ElementInspector.cpp
            ui/operation-modes/modes/surfride-editor/SurfRideEditor.cpp
            ui/operation-modes/modes/surfride-editor/SurfRideElement.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/References.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/Context.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureElement.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureElementInspector.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureTree.cpp
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureEditor.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/common/editors/SurfRide.h
            ui/operation-modes/modes/surfride-editor/Context.h
            ui/operation-modes/modes/surfride-editor/Timeline.h
            ui/operation-modes/modes/surfride-editor/CropLibrary.h
            ui/operation-modes/modes/surfride-editor/ProjectTree.h
            ui/operation-modes/modes/surfride-editor/ElementInspector.h
            ui/operation-modes/modes/surfride-editor/SurfRideEditor.h
            ui/operation-modes/modes/surfride-editor/SurfRideElement.h
            ui/operation-modes/modes/surfride-editor/texture-editor/Behaviors.h
            ui/operation-modes/modes/surfride-editor/texture-editor/Context.h
            ui/operation-modes/modes/surfride-editor/texture-editor/References.h
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureElement.h
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureElementInspector.h
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureTree.h
            ui/operation-modes/modes/surfride-editor/texture-editor/TextureEditor.h
    )
endif()

if(${DEVTOOLS_FEATURE_ASM_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/operation-modes/modes/asm-editor/Context.cpp
            ui/operation-modes/modes/asm-editor/NodeEditor.cpp
            ui/operation-modes/modes/asm-editor/BlendTreeEditor.cpp
            ui/operation-modes/modes/asm-editor/ASMEditor.cpp
            ui/operation-modes/modes/object-inspection/component-inspectors/GOCAnimator.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/operation-modes/modes/asm-editor/Context.h
            ui/operation-modes/modes/asm-editor/NodeEditor.h
            ui/operation-modes/modes/asm-editor/BlendTreeEditor.h
            ui/operation-modes/modes/asm-editor/ASMEditor.h
            ui/operation-modes/modes/object-inspection/component-inspectors/GOCAnimator.h
    )
endif()

if(${DEVTOOLS_FEATURE_DVSCENE_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/common/editors/DvScene.cpp
            ui/operation-modes/modes/dvscene-editor/Context.cpp
            ui/operation-modes/modes/dvscene-editor/DvSceneEditor.cpp
            ui/operation-modes/modes/dvscene-editor/Tree.cpp
            ui/operation-modes/modes/dvscene-editor/PageEditor.cpp
            ui/operation-modes/modes/dvscene-editor/DvNode.cpp
            ui/operation-modes/modes/dvscene-editor/SceneSettings.cpp
            ui/operation-modes/modes/dvscene-editor/node-inspector/NodeInspector.cpp
            ui/operation-modes/modes/dvscene-editor/timeline/Timeline.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/common/editors/DvScene.h
            ui/operation-modes/modes/dvscene-editor/Context.h
            ui/operation-modes/modes/dvscene-editor/DvSceneEditor.h
            ui/operation-modes/modes/dvscene-editor/Tree.h
            ui/operation-modes/modes/dvscene-editor/PageEditor.h
            ui/operation-modes/modes/dvscene-editor/Behaviors.h
            ui/operation-modes/modes/dvscene-editor/DvNode.h
            ui/operation-modes/modes/dvscene-editor/SceneSettings.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/NodeInspector.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/Nodes.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/Elements.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/Path.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/Character.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/CharacterMotion.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/Model.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/ModelMotion.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/ModelNode.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/nodes/Element.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraParams.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/DrawOff.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/PathOffset.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraShake.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraShakeLoop.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Effect.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/PathInterpolation.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraNearFar.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/UVAnim.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/VisibilityAnim.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/MaterialAnim.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/MultipleAnim.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraOffset.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraHedgehog.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraInGame.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Spotlight.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ControllerVibration.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/VertexAnimationTexture.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/TexturePatternAnim.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/MaterialParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/BloomParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/DOFParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ColorContrast.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraControlParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ShadowResolution.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/SSAOParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/OcclusionCapsuleParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/GodrayParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/AtmosphereGodrayParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ChromaticAberrationFilterParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/VignetteParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Fade.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/BossName.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Sound.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Time.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Sun.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CameraBlurParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/GeneralTrigger.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/DitherParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/QTE.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/FacialAnimation.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/OverrideASM.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Aura.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ChangeTimeScale.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/LipAnimation.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/CrossFade.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Weather.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ShadowMapParam.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/VariablePointLight.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/DensitySectorPoint.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/VAT.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/QTEAccel.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/TheEndCableObject.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/BulletTime.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/TimeStop.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ObjectTimeStop.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/ShadowShape.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Falloff.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/Fog.h
            ui/operation-modes/modes/dvscene-editor/node-inspector/elements/DOF.h
            ui/operation-modes/modes/dvscene-editor/timeline/Timeline.h
            ui/operation-modes/modes/dvscene-editor/timeline/Nodes.h
            ui/operation-modes/modes/dvscene-editor/timeline/Elements.h
        )
endif()

if(${DEVTOOLS_FEATURE_SCENE_EDITOR})
    target_sources(${DEVTOOLS_TARGET}
        PRIVATE
            ui/operation-modes/modes/scene-editor/Context.cpp
            ui/operation-modes/modes/scene-editor/SceneEditor.cpp
            ui/operation-modes/modes/scene-editor/SceneSettings.cpp
            ui/operation-modes/modes/scene-editor/Timeline.cpp
        PRIVATE FILE_SET HEADERS FILES
            ui/operation-modes/modes/scene-editor/Context.h
            ui/operation-modes/modes/scene-editor/SceneEditor.h
            ui/operation-modes/modes/scene-editor/SceneSettings.h
            ui/operation-modes/modes/scene-editor/Timeline.h
        )
endif()
