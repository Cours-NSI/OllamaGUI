-- OllamaGUIExternal.lua

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["glm"] = "../vendor/glm"
IncludeDir["spdlog"] = "../vendor/spdlog/include"
IncludeDir["base64"] = "../vendor/base64/include"
IncludeDir["cpr"] = "../vendor/cpr/include"
IncludeDir["nlohmann-json"] = "../vendor/nlohmann-json/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["cpr"] = "../vendor/cpr/lib/%{cfg.buildcfg}"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["cpr"] = "%{LibraryDir.cpr}/cpr.lib"

group "Dependencies"
    include "vendor/imgui"
    include "vendor/glfw"
    include "vendor/yaml-cpp"
group ""

group "Core"
    include "Walnut/Build-Walnut.lua"

    -- Optional modules
    if os.isfile("Walnut-Modules/Walnut-Networking/Build-Walnut-Networking.lua") then
        include "Walnut-Modules/Walnut-Networking/Build-Walnut-Networking.lua"
    end
group ""
