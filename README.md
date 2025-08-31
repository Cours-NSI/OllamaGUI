# OllamaGUI

A graphical user interface application built with C++ to run Large Language Models (LLMs) with Ollama.

## Dependencies

This project uses the following dependencies (located in the `vendor/` folder):

- [imgui](vendor/imgui): Dear ImGui (UI library)
- [GLFW](vendor/GLFW): Window/context/input management
- [yaml-cpp](vendor/yaml-cpp): YAML parser/emitter
- [glm](vendor/glm): Math library for graphics
- [spdlog](vendor/spdlog): Fast logging library
- [base64](vendor/base64): Base64 encoding/decoding
- [cpr](vendor/cpr): C++ Requests: HTTP client library
- [nlohmann-json](vendor/nlohmann-json): JSON for Modern C++
- [stb_image](vendor/stb_image): Image loading library
- [Walnut](Walnut/Source/Walnut/): Desktop Application framework made using `Dear ImGui`

## Prerequisities
To start developping with this project, make sure you have the following softwares installed :
- [Visual Studio](https://visualstudio.microsoft.com)
- [Ollama](https://ollama.com/download)

## Getting Started

To get started with this project, run the following commands:
```bash
git clone --recursive https://github.com/SAMSAM-55/OllamaGUI.git
cd OllamaGUI/scripts
.\Setup-Project.bat
```

## Planned Features

Planned features before beta v0.1:
- Auto docking for `Chats List` and all chats windows
- Locked docking layout for release builds

## Currently working on
*Note: this section may not always be up to date; the last updated date is always included*

**(28/08/2025)**
\
Currently working on auto/forced docking for ImGui windows. 

## License

See [LICENSE.md](LICENSE.md) for licensing information.

## Contributing

Feel free to open issues or pull requests!
