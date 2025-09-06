# HE2 DevTools

This is an in-engine modding toolkit for the Sonic Team Engine Hedgehog Engine 2.
Releases can be found on [GitHub](https://github.com/HE2-SDK/he2-devtools/releases)
or on [the project's GameBanana page](https://gamebanana.com/tools/15218).
A HMM update server is also available.

![The level editor](/.github/editor.png)

## Setting up the development environment

### Using Visual Studio 2022's CMake integration

You will need to have the following prerequisites installed:

- Visual Studio 2022

Check out the project and make sure to also check out its submodules:

```sh
git clone --recurse-submodules https://github.com/HE2-SDK/he2-devtools.git
```

Simply open the project folder in Visual Studio 2022 (File > Open > CMake...) and it should automatically detect the CMakeLists.txt file.

The following presets are available:

- x64 Debug: Debug build.
- x64 Debug ImGui Demo: Debug build with ImGui demo window enabled (for editing themes etc.).
- x64 Release: Release build.
- x64 RelWithDebInfo: Release build with debug information.

### Using CMake with the Visual Studio 2022 generator

You will need to have the following prerequisites installed:

- Visual Studio 2022
- CMake 3.28 or higher

Check out the project and make sure to also check out its submodules:

```sh
git clone --recurse-submodules https://github.com/HE2-SDK/he2-devtools.git
```

Now let CMake do its thing:

```sh
cmake -B build
```

If you have your target game installed in a non-standard location, you can specify that location
with the `DEVTOOLS_GAME_FOLDER` variable:

```sh
cmake -B build -DDEVTOOLS_GAME_FOLDER="C:\ShadowFrontiers"
```

Once CMake is finished, navigate to the build directory and open `devtools.sln` with VS2022.
You should have a fully working environment available.

Building the INSTALL project will install the mod into HedgeModManager's `Mods` directory.

## Screenshots

DevTools has a builtin level editor complete with rendering of debug visualizations such as volumes and colliders:

![The level editor](/.github/editor.png)
![Debug comments in the level editor](/.github/editor2.png)

As well as debug information left in the game's binary:

![Rendering of debug visuals](/.github/debugv.jpg)

DevTools is able to manipulate datastructures and resources inside the game's memory through reflection:

![RFL editing](/.github/rfl.jpg)

DevTools has a resource browser where you can see the game's currently loaded resources, edit them and hot reload them while the game is running:

![Resource browser](/.github/resource-browser.jpg)

DevTools can edit NeedleFXSceneData graphics parameters in realtime, giving you instant feedback when modding postprocessing and lighting parameters:

![NeedleFxSceneData editing](/.github/lighting.jpg)

DevTools can edit materials and scene parameters in realtime:

![Material and scene param editing](/.github/materials-and-scene-params.png)

DevTools can edit SurfRide (SWIF) projects in realtime:

![Material and scene param editing](/.github/surfride-editor.png)

DevTools provides inspectors for a variety of game systems, such as the memory allocation system, the rendering engine, GraphicsContext and GameUpdater:

![The memory inspector](/.github/memory-inspector.png)
![The rendering engine inspector](/.github/rendering-engine-inspector.png)
![The GameUpdater inspector](/.github/game-updater-inspector.png)
