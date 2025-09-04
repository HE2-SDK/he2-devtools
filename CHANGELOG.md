# v0.2.4
* Fixed bone debug rendering.

# Sonic Frontiers
* Added UIMapParam Editor tool.


# v0.2.3
* Added ResGismoConfig editor.
* Added ResGismoDesign editor.
* Added ResLuaData editor.
* Added ResMirageLight editor.
* Added ResPhysicalSkeleton editor.
* Added ResVibration editor.
* Added ResPointCloud editor.
* Added GOCPhysicalAnimation inspector.
* Added GOCPointLight inspector.
* Added GOCVibration inspector.
* Added debug rendering for skeletons.
* Added debug rendering for physical skeletons.
* Added debug rendering for point lights.
* Fixed various memory corruption issues in the DvScene editor.
* Improved GOCTransform inspector.
* Improved GOCActivator inspector.
* Fixed GOCVisualDebugDraw visuals not rendering

## Sonic Forces
* Added experimental ResScene editor.


# v0.2.2
* Level Editor: Added context menu item to enable/disable layers.


# v0.2.1
* Fix resource export dialog not showing if resource browser is closed.


# v0.2.0
* Added DvScene editor, contributed by the amazing Ashrindy! Huge thanks for this incredible contribution!
* Added 2 new themes, "Contrast ImGui" and "Modern Contrast ImGui", also contributed by Ashrindy.
* Added experimental pcmodel editor.
* Added DiEventManager inspector, contributed by Ashrindy.
* Added EventPlayer inspector, contributed by Ashrindy.
* Added MovieManager inspector, contributed by Ashrindy.
* Added "load resource" option in resource browser.
* Added support for ingame filesystems in the file dialog.
* Added generic name tag overlay framework for operation modes, and made it work with the level editor.
* Added transform tool pivot setting (AABB center / local origin).
* Angles are now edited in degrees instead of radians.
* Fixed a bug in resource reloading.
* Fixed array tool preview objects being visible while the array tool is closed.
* Fixed a crash when adding items to arrays loaded from disk (regression).
* Fixed a crash when closing the array tool while the preview objects are visible in the tree view.

## Sonic Frontiers & Shadow Generations
* Added LevelManager inspector (allows viewing/loading/unloading levels).

## Sonic Forces
* Removed duplicate "Component Configuration" header in object properties.


# v0.1.70
* Added array tool.
* Added live update of set paths that are not grind rails.
* Added rendering of path normals and tangents.
* Added ability to add/edit event tracks in SurfRide editor.
* Made it possible to close operation mode panels. They can be re-opened from the new `Window` menu.
* Copy/Paste now remembers the tree structure of the copied objects.
* Fixed a crash when deselecting an `ObjectWorld` chunk.

## Sonic Frontiers & Shadow Generations
* Added `GOCPlayerHsm` inspector.

## Sonic Forces
* Fixed a crash on startup.


# v0.1.69
* Fixed a crash when not installed in default Steam library location.
* Fixed broken SWIF export.
* Fixed some wrongly tagged icons.

## Shadow Generations
* Fixed resource browser showing too many resources.


# v0.1.68
* Added SvCol editor.
* Added animation editing to SurfRide editor.
* Added adding/removal of casts in SurfRide editor (still kind of broken in Forces, may need manual reload of the UI before it takes effect).
* Added crop library in SurfRide editor.
* Added support for various flags and settings in SurfRide editor element inspector.
* Added multiple view modes to resource browser.
* Added icons to object inspector.
* Added experimental triggers tab to ASM editor.
* Fixed SurfRide export.
* Fixed SurfRide editor crashing when the chosen GOCSprite unloads (except in Forces).
* Fixed bug where critical debug visuals were no longer rendering when debug rendering is turned off.
* Fixed a bug where "managed" memory was unaligned.
* Improved ObjectWorld API endpoint.
* Improved icon system.
* Improved timeline.
* Further improved reload queue.

## Sonic Frontiers
* Added static & dynamic sectors in StageData inspector.

## Shadow Generations
* Ported SurfRide editor.


# v0.1.67
* Added API query endpoint for objects.

## Sonic Forces
* Fixed color channel order issue that occurred since UCSL integration.


# v0.1.66
* Added API.

## Sonic Frontiers
* Added extremely experimental CEMT editor.


# v0.1.65
* Added toggle to disable photo mode in Camera settings.

## Sonic Frontiers
* Updated for Sonic Frontiers v1.42


# v0.1.64
* Added path rendering.
* Added reload queue.
* Refactored debug rendering.
* Fixed bug where reloading a gedit with new object types would crash.
* Fixed a bug where inspecting the PVO of a GOCVisual's material instance would crash the game if it didn't exist.

## Sonic Frontiers & Sonic Forces
* Added options to create new timeline items in the SWIF editor.

## Shadow Generations & Sonic Frontiers
* Added Photo Mode (press L3 to activate).
* Added occlusion capsule rendering.

## Shadow Generations
* Updated for v1.10.0.1


# v0.1.63
* Fixed font scaling in ASM editor.

## Shadow Generations
* Upgraded to game version 1.10.0.0.


# v0.1.62
* Added accessibility options: font size, gizmo line and handle scale.


# v0.1.61
* Fixed various issues and bugs causing corrupt files and crashes in the new `rip`-based file export.
* Fixed a bug where alt tabbing with multi-viewport mode disabled in windowed mode would reset ImGui.

## Sonic Forces / Sonic Frontiers
* Fixed accidentally trying to export REF v2-1.00 instead of v1.
* Re-enabled multi viewport mode.
* Re-enabled resource reloading.

## Sonic Forces
* Fixed accidentally trying to export gedit v3 instead of v2.

## Shadow Generations
* Ported `RenderManager` inspector.


# v0.1.60
* Added a Memory Viewer that lets you view raw memory areas.
  You can open it by right clicking objects, components, gameservices or RFL data.


# v0.1.59
* Added toggles for position and rotation inheritance in `GOCTransform` inspector.

## Shadow Generations
* Ported `Update configuration` panel in object inspection mode.
* Moving an object now enables `GOCTransform`'s `PRE_ANIM` update flag.
  This makes it possible to move static objects that turn `GOCTransform` updates off
  because they don't expect to be moving.


# v0.1.58
* Added panel that allows direct editing of debug camera parameters.
* Selection box now also works for box, cylinder and capsule colliders.


# v0.1.57
* Fixed a bug in object placement introduced in the previous version.


# v0.1.56
* Integrated DevTools with `rip`.
* Fixed a bug where selecting parented objects didn't work properly in the level editor.

## Sonic Forces / Sonic Frontiers
* ASM editor: You can now assign a blend node to a state.
* ASM editor: Fixed a bug where assigning clips to states would not set the state to be a clip state.
* Added RFL HedgeSet template export. Thanks to Ashrindy for this contribution!


# v0.1.55
* Added RFL HedgeSet template export. Thanks to Ashrindy for this contribution!


# v0.1.54
## Shadow Generations
* Update for Shadow Generations v1.1.0.1.


# v0.1.53
## Shadow Generations
* Ported "teleport player".
* Added `GOCPlayerKinematicParams` inspector.


# v0.1.52
## Shadow Generations
* Ported material editor.


# v0.1.51
## Shadow Generations
* ASM editor: You can now assign a blend node to a state.
* ASM editor: Fixed a bug where assigning clips to states would not set the state to be a clip state.


# v0.1.50
## Shadow Generations
* Ported ASM editor.
* Ported StageInfo inspector.
* Ported LevelInfo inspector.
* Fixed an issue with generated HSON templates.


# v0.1.49
## Shadow Generations
* Ported NeedleFXSceneData Testing Tool.
* Ported FxParamManager inspector.


# v0.1.48
## Shadow Generations
* First experimental Shadow Generations release.


# v0.1.47
## Sonic Frontiers
* Added `PathManager` inspector.
* Added `PathComponent` inspector (to be extended later).


# v0.1.46
## Sonic Frontiers
* Re-enabled the adding and removing of component configurations in the level editor.

## Sonic Forces
* Fixed a crash in the SWIF editor.
* Fixed a crash relating to hash maps.


# v0.1.45
## Sonic Frontiers
* ASM editor: Clips, Variables and Blend Spaces can now be reassigned by dragging pins. Can't create new ones yet.
* ASM editor: Flag nodes added.
* ASM editor: Refactorings.
* Rendering Engine Inspector: Added SceneContextManager.
* Rendering Engine Inspector: Added SCLocalLight light count.
* Rendering Engine Inspector: Added Dynamic Resolution Controller render ratio.


# v0.1.44
## Sonic Frontiers
* General improvements to ASM node editor.
* Added ASM nodes for BlendSpace, Clip, Blend Mask.
* Added ResAnimator/ASM export.
* Added menu item in ASM editor to toggle between detailed and compact version of BlendSpaceNodes.
* Added menu item in ASM editor to manually trigger an auto-layout.


# v0.1.43
* Added WIP ResAnimator editor (no export atm).
* Added FxCol export.
* Added very barebones GOCPlayerCollider inspector.
* Added GOCMoveSphereCollisionQuery and GOCCollisionQuery inspector.
* Fixed resources not being cleaned up properly on exit.


# v0.1.42
* SWIF user data can now be edited properly.
* Adding and deleting FxCol shapes updates bounding boxes.
* Placing a new object now selects it in all operation modes.


# v0.1.41
* You can now place objects from the context menu.
* Added ability to create bounding volumes and collision shapes in the FxCol editor.
* Added a search box to the ResReflection editor in order to find RFL classes more quickly.
* Fixed a bug with float type color editors.

## Sonic Frontiers
* Added MonologueService inspector.


# v0.1.40
* Added toggles to transform the material color, illumination color and display flag.


# v0.1.39
* Added option to set default directory for file dialogs.
* Fixed a bug in the shortcut settings serialization.
* Fixed a bug in the experimental "Add Image Cast" option.


# v0.1.38
* Added model instance material parameters editor to GOCVisualModel.
* Added extremely experimental "Add Image Cast" option to SurfRide editor tree view context menu. Don't use unless you know exactly what you are doing and you have backups.


# v0.1.37
* Fixed a bug in the selection tool introduced in `v0.1.36`.
* Fixed a bug with multiselect in the SWIF editor introduced in `v0.1.34`.

## Sonic Forces
* Ported the SWIF editor (minus a small issue that prevents playing the animations).


# v0.1.36
* Added texture / crop editor to the SWIF editor.
* Added editors for the Blur and Reflect effects in the SWIF editor.
* Added full transform mode in transform gizmo. You can now press T to enter this mode, and use Shift-G, Shift-R and Shift-S
  to invert the behavior of G, R and S, i.e. analogous to the X, Y and Z shortcuts.
* Added `SceneParamContainer` editor tree to the `RenderingEngine` panel.
* Added view of render targets to the `RenderingEngine` panel.
* Added list of render jobs to the `RenderingEngine` panel.
* Slightly cleaned up the `RenderingEngine` panel.
* Properly named some fields in the Camera parameter panels.
* Identified various fields in the RenderingEngine and RenderManager panels previously marked as unknown.
* Fixed a use after free error regarding intrusive pointers in arrays.
* Fixed a bug where adding or removing an array item did not mark the array as "edited".


# v0.1.35
* Fixed Copy/Paste being broken after OperationMode refactor.
* Added Cut.
* Added setting to disable multiple viewports
* Also made it possible for standalone windows to be dragged out of the main viewport.
* Fixed a crash when closing the game related to multiple viewports.
* Moved to /W4 warning level and fixed most warnings.


# v0.1.34
* Added SWIF file export.
* Added pivot type selection to the SWIF editor (selection box will be fixed soon).
* Added crop orientation and mirroring options to the SWIF editor.
* Added text data editor to the SWIF editor.
* Added an option to export a HSON template for all loaded objects in the Tools menu.
* Refactored I/O system.
* Upgraded to C++20.


# v0.1.33
* Fixed a bug/crash in the gedit export code introduced by v0.1.30.


# v0.1.32
* Multi-Viewport mode is now only enabled when the game is running in windowed mode.
* Level editor now no longer uses object IDs to identify objects, but instead uses pointers to the objects themselves. This avoids issues with faulty gedit files generated by other tools.


# v0.1.31
* Fixed a bug where the level editor did not check if a placement layer is selected when using Shift+Click to place objects.


# v0.1.30
* Added support for ImGui DockSpaces, making it possible to dock windows to the sides of the screen.
* Added support for ImGui Multi-Viewport mode, allowing you to move windows outside of the main viewport and/or to other monitors.
* Cleaner shutdown of the DevTools when the game is closed.
* Cleaner handling of window resizing.
* Implemented semi-automatic RflClass generation through templates.
* Added a "managed resource memory" mechanism, making it possible to allocate memory for resources that is automatically freed when the resource is unloaded.
* Added RflClass implementations for ResSurfRideProject datastructures, preparing for implementation of the export module.


# v0.1.29
* Added beginnings of a ResFxColFile2 editor.
* Added rebindable shortcuts.
* Added right click context menu which currently contains the "Teleport Sonic" option.
* Added render limit and draw distance settings for the debug boxes.
* Added debug comment rendering to the object inspection mode.
* Added ability to place objects by holding Shift while clicking in the level editor.
* Added ability to toggle place mode using Caps Lock in the level editor.
* Added ability to select dead objects using drag select in the level editor.
* Changed SurfRide editor so you now have to select a scene to edit first.
* Further improved communication between the level editor and the game.
* Improved mouse handling so that windows don't capture the mouse input when moving over them while dragging.
* Improved build script so it is no longer necessary to supply `-A x64` when building.
* Made project buildable using Visual Studio 2022's CMake integration.
* Fixed a crash when adding items to an array loaded from disk in the level editor.
* Fixed crash when Sonic is selected while changing maps.
* Fixed loading screen hanging when an object is selected.
* Fixed a bug where capsules and cylinders were rendered with their radii and heights swapped.
* Reinstated live grind rail editing.
* Refactored operation modes so that editors can be added more easily.


# v0.1.28
* Fixed TYPE_ARRAY editors on the same level unfolding together.
* Fixed a crash when clicking outside the UI very early in the game's boot sequence.
* Fixed RFL editor default defaultOpen being true.

## Sonic Frontiers
* Added TimeService inspector.
* Added RespawnTimeManager inspector.
* Made controller priority in CameraFrame inspector editable.

## Sonic Forces
* Fixed crash when exporting gedit.
* Added support for TYPE_OLD_ARRAY in the serializer.
* Added workaround to show TYPE_OLD_ARRAY arrays of unsigned ints as object ids in RFL editor.


# v0.1.27
* Fixed ObjectId and GameObject selection inputs always being set to the last item in the list.


# v0.1.26
* Fixed a previously masked problem in the selection tool which made it impossible to select or place objects in the previous version.
* Fixed a problem in the build scripts.


# v0.1.25
* Removed "Sonic's Birthday" HEMS event easter egg.
* Added workaround to not crash when objects have invalid NULL IDs.
* Added marker for objects with invalid NULL IDs in the ObjectData tree.
* Added "Free Memory" tab in memory inspector.
* Added support for TYPE_OLD_ARRAY in the RFL editor.
* Added checks for NULL or duplicate ID's when generating a new Object ID.

## Sonic Forces
* Added NeedleFXSceneData Testing Tool.
* Added Memory Inspector.
* Added pausing.


# v0.1.24
* HOTFIX: Fixed a regression where the game would crash if SQUID spots you. Also debug rendering didn't work.
* Made it possible to use the time and weather NeedleFXSceneData options in NeedleFXSceneData Testing Tool V2 without also setting the base NeedleFXParameters.
* Fixed a bug in the RFL copy code.
* Fixed a bug in the mouse hook.

# v0.1.23
* Added rudimentary copy+paste of objects in the level editor (select a placement layer first!).
* Added align/distribute option of objects in the level editor when multiple are selected.
* Added GOCVisual inspector.
* Added GOCActivator inspector.
* Added SoundDirector inspector.
* Added allocation map tab to the memory inspector.
* Added RenderManager inspector.
* Added RenderingEngine inspector.
* Added a GameObject input for handles to GameObjects.
* Added generalized non-RFL array inputs.
* Added default unfold of spawner data and component data in the level editor.
* Added a button to spawn/despawn/shutdown/restart objects in the ObjectWorld inspector.
* Added an Asleep toggle in the object inspector.
* Added Sonic Forces theme.
* Selection AABB rendering now also accounts for selected dead objects when in level editor mode.
* Resources can now be dragged into text inputs to copy their name.
* ObjectData objects can now be dragged into ObjectId inputs to copy their ID.
* ObjectId inputs now have a Select button that selects the object in the level editor if it is open.
* Slightly improved the material editor. It now live updates the material for some parameters (not all yet).
* Added support for alternative projection types in the ViewportData viewer.
* Multiple select now shows number of objects selected.
* Made debug box rendering size configurable.
* Fixed a bug where absolute transforms were not properly updated when moving objects that have children in the level editor.
* Fixed a regression where list editors in the RFL editor were corrupting memory and causing crashes.
* Fixed a regression in vector/matrix inputs in the RFL editor where they were editing the wrong address in memory.
* Fixed a regression in fixed size arrays in the RFL editor where they would open simultaneously.
* Fixed a bug in RFL drag and drop processing.
* Fixed a bug in quaternion inputs.
* Fixed a bug with edit detection in ObjectId editors.
* Fixed keyboard input being passed to the game when an input field is in focus.
* Fixed the camera glitching out when first manipulating it.
* Fixed a bug in gedit serialization where fields that should always be zero were not always zero (this bug was masked and probably did not affect your files).
* Fixed a bug with object ID's in the level editor.
* Fixed a major (known) memory leak in the level editor when creating new objects.
* Fixed a crash when viewing unloaded texture resources in the ResourceManager.
* Small regression: updating debug camera sensitivity now requires a restart. Will be fixed in a future version. However, this also fixes the issue where sonic sidesteps or the map opens when you open DevTools.
* Custom ComponentData editor has been temporarily disabled but will be back in next version.

# v0.1.21
* Added a build for Sonic Forces. This build is not yet full featured.
* Improved free camera handling and keyboard/mouse capture.

# v0.1.20
* Fixed a bug where the ResReflectionEditor would always export the contents of the window you opened first if multiple editors were open.

# v0.1.19
* Added selection of SWIF elements.
* Added manipulation of SWIF elements.

# v0.1.18
* Fixed a crash when opening the blend tree of the secondary camera frame.

# v0.1.17
* Added the beginnings of a SWIF editor. (This can easily hard crash at the moment as it's only half built. You have been warned.)
* Added the beginnings of the GOCPlayerBlackboard inspector.
* Added an inspector for GOCCamera.
* Added an inspector for CameraService.
* Added a details tab to the object inspector, which is currently only used for the CameraFrame object.
* Refactored the reflection part of the codebase.
* Fixed a bug where parts of reflection data were overwritten when manipulating neighbouring comboboxes or flags.
* Added reflective operations, a code framework that makes it easy to add new reflection-centered operations and transformations.
* Added copying of parts of reflection data through drag and drop.
* Added a right-click context menu option to reset reflection data subtrees to default values.
* Added a right-click context menu option to reset reflection data subtrees in ResReflection editors to the values
  that were there when the editor was opened.
* Added a diff view to the ResReflection editor showing you what you changed since it was opened.
* Fixed a bug where the minimum float step in the settings could not be changed.

# v0.1.16
* Improved the GameUpdater inspector. You now have a better overview and can toggle pause participation, timescale and FPS.
* Added a GameMode inspector that currently only shows the Layer Status extension. If you want to change timescale you will
  probably have to do it here because this GameMode extension overwrites the values in the GameUpdater.
* Added update flag inspection and manipulation in object inspection mode.
* Fixed a regression in the range processing of the reflection editor, sometimes allowing you to select unsafe values.

# v0.1.15
* Added live editing of grinding rails.
* Added small magenta boxes visualizing selectable objects without models while in level editor mode.
* Added better communication with the edit mode of ingame objects, e.g. volumes now show their debug visual while being edited
  even if debug rendering is off, cameras show the viewpoint of the camera while selected, etc.
* Added a search box and categories to the Object Library.
* Added a GOCPlayerKinematicParams inspector.
* Added a GOCEventCollision inspector.
* Fixed a crash in GOCAnimator.
* Added drag-and-drop reparenting of GameObjects in object inspection mode.
* Added an experimental RFL diffing tool.
* Extended the data visible in the GOCPlayerParameter inspector.
* Fixed grinding rails disappearing when switching to the editor.
* Fixed a crash while deleting objects.
* Fixed a crash while quitting the game when the level editor is open.
* Added NeedleFXSceneData testing tool V2. This one is experimental and doesn't work properly yet, but uses a more robust approach.

# v0.1.14
* Added hot reload for .terrain-model.
* Added hot reload for .pcmodel.
* Added a ResMaterial editor, which currently does not update or export anything. Only for viewing.
* Fixed a bug where selecting "Place" in the level editor before selecting an object to be placed would crash the game.
* Fixed a bug where opening two standalone windows would crash the game.

# v0.1.13
* Added click-and-drag multi select.
* Added a search box for the world object list in level editor mode. At the moment this is case sensitive, will improve soon.
* The "Gedit" theme was completely revamped. Thanks a lot ik!
* Added an experimental inspector for GOCAnimator. This doesn't do much yet.

# v0.1.12
* Fixed a bug in the GOCTransform inspector introduced by refactoring.

# v0.1.11
* Improved selection tool. DashRings and the like are now selectable.
* Colliders that trigger selection can now be precisely selected using new filter settings.
* Holding down ALT while left clicking will now show a menu with all objects that can be picked at that location.
* Improved GOCVisualDebugDraw rendering.
* Added debug rendering of colliders.
* Added settings for debug rendering.
* Added settings for slider range and precision.
* Added GOCSphereCollider, GOCBoxCollider, GOCCapsuleCollider and GOCCylinderCollider inspectors.
* Fixed selected object drifting in the level editor due to a numerical stability issue.
* Improved performance in level editor.
* Refactored object inspector.
* Core "operation mode" windows like the object list and inspector now remember their position.

# v0.1.10
* Fixed crashes in automatic gedit reloading.

# v0.1.9
* Fixed a bug in the automatic gedit reloading.

# v0.1.8
* Added hot reload for gedit resources.

# v0.1.7
* Collecting debug comments every frame severely degrades performance on some levels.
  Put the feature under a toolbar toggle until I can fix this.

# v0.1.6
* Added StageInfo and LevelInfo service inspectors.
* Added debug comment rendering when in level editor mode.
* Fixed a bug where windows would sometimes take the place of one you just closed.
* Fixed a bug where window docking wasn't working.
* Fixed a bug where the game would crash if you pressed delete while having nothing selected.

# v0.1.5
* Added an experimental "NeedleFxSceneData Testing Tool", which sets FX parameters directly on the rendering engine.
  IMPORTANT: this feature is unstable and currently crashes your game when you switch maps after you used it.
* DevTools now allocates memory in its own subheap, making it easier to debug and preventing it from interfering
  with other game memory.
* Improved the Memory Inspector: new plots have been added, more information has been added and the heap hierarchy is shown.
* Improved the FxParamManager inspector: more information is now visible.
* Improved the reflection editor: now picks up on many more ranges and step sizes in the game's reflection information.
* "Insights" menu has been changed to "Inspectors" menu, the GameService inspector has been moved to this location.
* Fixed a memory leak in csl::ut::VariableString handling.

# v0.1.4
* Fixed an issue where placing objects not already on the map would crash the game.

# v0.1.3
* Added rendering of GOCVisualDebugDraw debug geometry (can be toggled from toolbar).
* Added a memory inspector.
* Vastly improved performance of level editor object list (it can still be improved further).
* Fixed a bug where the level editor's gedit export would crash sometimes due to running out of memory.
* Code cleanup.

# v0.1.2
* Fixed a misidentified function causing a crash when clicking an object in the level editor.

# v0.1.1
* Build for Sonic Frontiers v1.41

# v0.1.0
* Added the beginnings of a gedit editor mode. You can now select "Level editor" from the "Mode" menu and
  drop into a new mode where you edit not the live objects but the underlying gedit resource and your changes
  are persisted and can be exported to a .gedit file (right click a layer in the "Object list" layer view to export).
* Added a physics based mouse picker (you can now select many objects by clicking them).
* Added tree view in addition to existing layer view in object inspection mode.
* Added a gedit editing window to the resource browser.
* Added a GameService inspector for the ObjectWorld service. Be careful with this as manipulating the flags is sure to crash your game.
* Added support for ARRAY and OBJECT_ID types in the RFL editor.
* Improved keyboard controls for the free camera and added mouse controls (you can set sensitivity in the settings).
  Mouse control is not perfect yet and will be improved in a future version.
* Made it possible to drag around the camera view while devtools are open by right clicking and holding the scene.
* Made the path in the resource browser stay fixed.
* Fixed default settings not being loaded if you didn't have a settings file yet.
* Various bugfixes that I forgot.
* Added a new bug: Sonic does a quickstep when devtools are opened.

# v0.0.6
* Added ability to lock the debug camera position. While the camera is locked you regain control over Sonic.

# v0.0.5
* Added English translations for the Japanese descriptions of reflection parameters.
  These can be turned on in the new settings dialog but are currently opt-in because
  they are generated by Google Translate and therefore... not very good.
* Added a settings dialog with some now-persisted config options:
  * Theme choice.
  * Language for reflection descriptions.
  * Enable/disable keyboard and gamepad navigation of the UI.

# v0.0.4
* Added hot reload features to the resource browser. You can now right click a resource in the resource browser and select
  "Load from file" to pick a file on your hard drive to replace the asset with. This has immediate effect in the game for
  many assets. Alternatively, you can choose "Watch folder..." in the resource browser menubar to continuously monitor
  a folder on your hard drive for changes. Any assets you add or update in this folder will be automatically loaded
  by the devtools and replace existing assets with the same name. This feature has currently been tested with textures
  and pxd animations. Other files will probably work as well, but have not been tested yet. Use at your own risk.
* Improved the reflection editor:
  * STRING and FLAGS attributes are now also editable.
  * Improved rendering of arrays. They are now nested and their indices are marked in their names.
* Added 2 small insights windows: GraphicsContext, showing Renderables in use by the engine, and CameraManager,
  showing active camera stacks and their associated view/projection matrices.

# v0.0.3
* Added a resource browser. This will be the place where you can view/edit/load/apply all assets loaded by the game.
  The RFL editor has been moved to this location. It can, however, still be accessed from the GOCPlayerParameter inspector.
  If you open the RFL editor directly from the resource browser, you'll have to select the appropriate schema yourself.
  The devtools will show a list of possibly compatible schemas based on the file size.
  Careful! Editing an RFL with an incompatible schema will corrupt data and may crash your game.
  When you are sent to the editor through a link that knows what the correct schema is (e.g. GOCPlayerParameter),
  the correct schema will be selected for you.
  Icons generously donated by @aymenduturfu on the Hedgehog Engine Modding Server discord!
* Added transform gizmos when an object is selected. These gizmos use the same hotkey system as Blender:
  * G: Move
  * R: Rotate
  * S: Scale
  * Alt+Space: toggle local/world coordinate system
  * X/Y/Z: Restrict transformations along X/Y/Z axis.
  * Shift+X/Y/Z: Restrict transformation along everything but X/Y/Z axis.
* Added a debug camera (detached/free camera). Hover over the toggle in the toolbar to see controls and hotkeys.
* Added object pause, debug pause and frame-by-frame stepping. Hover over the toggles in the toolbar to see an explanation and the associated hotkeys.
* Added 2 new UI themes:
  * Sonic Frontiers, a theme based on the game's main UI and the new main theme for the devtools!
  * Gindows, a theme based on the UI of Sonic Team's in-house development tools!
    This theme was generously donated by @ik_01 on the Hedgehog Engine Modding Server discord!
  
  Currently the theme will revert to the default when the game restarts, I am looking into saving your choice for the next version.
* Added a new insights section. Currently contains only the GameUpdater section, which shows frame times per object layer.
* Slightly improved stability.

# v0.0.2
* Added the beginnings of a GameService inspector
* Added basic inspector for the NeedleFXParameter, FxLODParameter and StageCommonTimeProgressParameter reflections in the service FxParamManager.
  Many of these parameters do not yet have an immediate effect because they need additional update steps to be implemented.
  Others are controlled by other services and will immediately revert.
* Fixed some controls with same name being "linked together" (opening and closing together, value changed together).

# v0.0.1
* Added rudimentary manipulation of the GOCTransform component.
* Fixed an issue where inputs given while the DevTools are closed would build up and be processed all at once when bringing them up.
