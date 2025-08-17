-- WalnutExternal.lua

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["glm"] = "../vendor/glm"
IncludeDir["spdlog"] = "../vendor/spdlog/include"
IncludeDir["base64"] = "../vendor/base64/include"
IncludeDir["cpr"] = "../vendor/cpr/include"
IncludeDir["nlohmann_json"] = "../vendor/nlohmann_json/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

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

-- Configuration-specific linking for CPR
filter "configurations:Debug"
    libdirs { "../vendor/cpr/lib/debug" }
    links { "cpr" }

filter "configurations:Release"
    libdirs { "../vendor/cpr/lib/release" }
    links { "cpr" }

filter {}