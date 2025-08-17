# OllamaGUI

A graphical user interface application built with C++ to run Large Language Models (LLMs) with Ollama.

## Dependencies

Your project uses the following dependencies (located in the `vendor/` folder):

- [imgui](vendor/imgui): Dear ImGui (UI library)
- [GLFW](vendor/GLFW): Window/context/input management
- [yaml-cpp](vendor/yaml-cpp): YAML parser/emitter
- [glm](vendor/glm): Math library for graphics
- [spdlog](vendor/spdlog): Fast logging library
- [base64](vendor/base64): Base64 encoding/decoding
- [cpr](vendor/cpr): C++ Requests: HTTP client library
- [nlohmann-json](vendor/nlohmann-json): JSON for Modern C++
- [stb_image](vendor/stb_image): Image loading library
- [Walnut](Walnut/Source/Walnut/): Desktop Apllication framework made using imgui

## Getting Started

To get started with this project, first make sure you have [Visual Studio 2022](https://visualstudio.microsoft.com/) installed. Then ruun the following commands:
```bash
git clone --recursive htpps://github.com/SAMSAM5-5/OllamaGUI.git
cd OllamaGUI/scripts
.\Setup-OllamaGUI.bat
```

Finally, when you build, you will have to copy the .dll from `vendor/cpr/lib` ( from `Debug` or `Release` sub-folder depending on your configuration) in order to get rid of the error : "cpr.dll" can't be found.

## License

See [LICENSE.md](LICENSE.md) for licensing information.

## Contributing

Feel free to open issues or pull requests!
