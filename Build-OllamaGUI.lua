-- premake5.lua
workspace "OllamaGUI"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "OllamaGUI"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Build-OllamaGUI-External.lua"
include "OllamaGUI/Build-OllamaGUI-App.lua"