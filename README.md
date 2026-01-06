# CommonLibSSE-NG Plugin Template

This is a basic plugin template using CommonLibSSE-NG.

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
```bat
git clone --recurse-submodules https://github.com/libxse/commonlibsse-ng-template
cd commonlibsse-ng-template
```
Or clone with your IDE and run this command:
```
git submodule update --init --recursive
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Intellisense (VSCode)

After a successful build, add the following line as a property to your `c_cpp_properties.json`:

```json
"compileCommands":[ "${workspaceFolder}/.vscode/compile_commands.json"]
```

Example:
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.26100.0",
            "compilerPath": "cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-msvc-x64", 
            "compileCommands":[ "${workspaceFolder}/.vscode/compile_commands.json"]
        }
    ],
    "version": 4
}
```

Then run the `XMake: Update Intellisense` command to make intellisense start working. You need to do this every time a new file is created and linked.

### Debugger (VSCode)

Copy and paste the following into `launch.json` in the `.vscode` folder.
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(Windows) Attach",
            "type": "cppvsdbg",
            "request": "attach",
        }
    ]
}
```

Press F5 or the Run and Debug button in the sidebar to attach to SkyrimSE.exe while it's running (build mode must be `debug` to hit breakpoints).

### Build Output (Optional)

#### Mod Directory
Set one of or both of the following environment variables:

- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

#### Mod Resources
Copy resources like ini files, Bethesda plugins, etc. with the custom rule `copy_resource`

If 

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```

## Documentation
Please refer to the [Wiki](../../wiki/Home) for more advanced topics.
