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
      ["Utils"] = { "Source/Utils/**.h", "Source/Utils/**.cpp"}
   }

   targetdir (".../../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "OLLAMA_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

    filter "configurations:Debug"
      defines { "OLLAMA_DEBUG" }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      defines { "OLLAMA_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "OLLAMA_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      