project "OllamaGUI"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp", "src/**.hpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",
      "../vendor/base64/include",
      "../vendor/nlohmann-json/include",
      "../vendor/cpr/include",

      "../Walnut/Source",
      "../Walnut/Platform/GUI",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",
   }

    links
    {
        "Walnut"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   -- Visual Studio filters
	vpaths
	{
		["OllamaAPI"] = { "src/OllamaAPI/**" },
      ["Core"] = { "src/**.h", "src/**.cpp"}
	}

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS" }
      buildoptions { "/utf-8" }

   filter "configurations:Debug"
      libdirs { "../vendor/cpr/lib/Debug" }
      links { "cpr.lib" }
      defines { "WL_DEBUG" }
      runtime "Debug"
      symbols "On"
      postbuildcommands {
         "if not exist \"$(ProjectDir)chats\" mkdir \"$(ProjectDir)chats\"",
         "{COPY} ../vendor/cpr/bin/Debug/cpr.dll %{cfg.targetdir}",
         "{COPY} ../vendor/cpr/bin/Debug/libcurl-d.dll %{cfg.targetdir}",
         "{COPY} ../vendor/cpr/bin/Debug/zlibd1.dll %{cfg.targetdir}"
      }

   filter "configurations:Release"
      libdirs { "../vendor/cpr/lib/Release" }
      links { "cpr.lib" }
      defines { "WL_RELEASE" }
      runtime "Release"
      optimize "On"
      symbols "On"
      postbuildcommands {
         "if not exist \"$(ProjectDir)chats\" mkdir \"$(ProjectDir)chats\"",
         "{COPY} ../vendor/cpr/bin/Release/cpr.dll %{cfg.targetdir}",
         "{COPY} ../vendor/cpr/bin/Release/libcurl.dll %{cfg.targetdir}",
         "{COPY} ../vendor/cpr/bin/Release/zlib1.dll %{cfg.targetdir}"
      }

   filter "configurations:Dist"
      libdirs { "../vendor/cpr/lib/Release" }
      links { "cpr.lib" }
      kind "WindowedApp"
      defines { "WL_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"