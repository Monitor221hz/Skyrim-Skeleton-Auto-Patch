-- info to change
local name = "commonlibsse-ng-template"
local author = "monitor221hz"
local description = "SKSE64 plugin template using CommonLibSSE-NG"
local version = "0.0.0"
local license = "GPL-3.0"

-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse-ng")

-- set project
set_project(name)
set_version(version)
set_license(license)

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")

-- idk if this even works still but its here just in case
add_rules("plugin.vsxmake.autoupdate")

-- include custom rules
includes("xmake-rules.lua")


-- targets
target(name)
    -- add dependencies to target
    add_deps("commonlibsse-ng")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = name,
        author = author,
        description = description
    })

    -- copy a folder named ini to output
    add_rules("template.copy_resources", {
        files = {
            {src = "ini/Settings.ini", dest = string.format("SKSE/Plugins/%s/Settings.ini", name)}
        }
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
