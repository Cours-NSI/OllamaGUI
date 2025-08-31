project "OllamaAPI"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files
   {
       "Source/**.h",
       "Source/**.cpp",
       "Source/**.hpp"
   }

   includedirs
   {
      "Source",
      "Source/Core",
      
      "../vendor/imgui",
      "../vendor/cpr/include",
      "../vendor/nlohmann-json/include",
      "../vendor/base64/include",

      "%{IncludeDir.spdlog}",
   }

   links
   {
      "ImGui",
      "curl",
      "cpr",
   }

   --Visual Studio filters
   vpaths
   {
      ["Core/OllamaAPI"] = { "Source/Core/OllamaAPI/**.h", "Source/Core/OllamaAPI/**.cpp"},
      ["Utils"] = { "Source/Utils/**.h", "Source/Utils/**.cpp"},
      ["Core/Log"] = { "Source/Core/Log/**.h", "Source/Core/Log/**.cpp"}
   }

   targetdir (".../../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "OL_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

    filter "configurations:Debug"
      defines { "OL_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "OL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "OL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      