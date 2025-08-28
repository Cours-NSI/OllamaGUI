project "OllamaGUI"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",
      "../vendor/spdlog/include",

      "../vendor/cpr/include",
      "../vendor/nlohmann-json/include",
      "../vendor/base64/include",

      "../Walnut/Source",
      "../Walnut/Platform/GUI",

      "../OllamaAPI/Source",
      "../OllamaAPI/Source/Core",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

    links
    {
        "Walnut",
        "OllamaAPI",
         "ImGui",
         "cpr",
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   -- Visual Studio filters
	vpaths
	{
      ["Core"] = { "Source/**.h", "Source/**.cpp"}
	}

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

   filter "configurations:Debug"
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"


   filter "configurations:Release"
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"