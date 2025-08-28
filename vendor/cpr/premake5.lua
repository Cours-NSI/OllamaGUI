-- curl.lua
project "curl"
    kind "StaticLib"
    language "C"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir   ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "lib/curl/**.h",
        "lib/curl/**.c",
        "include/curl/**.h",
    }

    includedirs
    {
        "include",
        "include/curl",
        "lib/curl"
    }

    defines {
        "CURL_STATICLIB",
        "BUILDING_LIBCURL",
    }

    filter "system:windows"
        systemversion "latest"
        defines { "WIN32", "USE_WINDOWS_SSPI", "USE_SCHANNEL" }
        buildoptions { "/utf-8" }

        links {
            "ws2_32.lib",
            "wldap32.lib",
            "crypt32.lib",
            "secur32.lib"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "Off"

-- cpr.lua
project "cpr"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir   ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/cpr/**.cpp",
        "include/cpr/**.h"
    }

    includedirs
    {
        "include",
        "include/cpr",
        "include/curl"
    }

    links { 
        "curl" 
    }

    defines {
        "CPR_STATICLIB",
        "CURL_STATICLIB"
    }

    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        runtime "Release"
        optimize "On"
        symbols "Off"
