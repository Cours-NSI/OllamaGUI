#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "imgui.h"

#include "OllamaChat.h"

namespace OllamaUtils {
	struct OllamaParsingResult
	{
		std::string thinking;
		std::string answer;
	};

	enum ParsingSection
	{
		NONE = -1,
		THINKING = 0,
		ANSWER = 1
	};

    inline cpr::WriteCallback CreateStreamingCallback(std::string& outputBuffer, bool& isStreaming)
    {
        return cpr::WriteCallback(
            std::function<bool(const std::string_view&, intptr_t)>(
                [&outputBuffer, &isStreaming](const std::string_view& data, intptr_t) -> bool {
                    std::string temp(data);
                    std::istringstream stream(temp);
                    std::string line;

                    while (std::getline(stream, line)) {
                        if (line.empty()) continue;

                        try {
                            auto json = nlohmann::json::parse(line);

                            if (json.contains("message") && json["message"].contains("content")) {
                                outputBuffer += json["message"]["content"].get<std::string>();
                            }

                            if (json.contains("done") && json["done"].get<bool>()) {
                                isStreaming = false;
                                return false; // stop streaming
                            }

                        }
                        catch (const std::exception& e) {
                            outputBuffer += "\n[Error parsing chunk]\n";
                            std::cerr << "JSON parse error: " << e.what() << std::endl;
                        }
                    }

                    return true; // keep streaming until "done": true
                }
            ),
            0
        );
    }


    inline OllamaParsingResult ParseModelAnswer(const std::string& answer)
    {
        std::stringstream ss[2];
        ParsingSection section = ParsingSection::NONE;

        std::string line;
        std::string temp(answer);
        std::istringstream stream(temp);

        while (std::getline(stream, line))
        {
            if (line.empty()) continue;

            if (line.find("<think>") != std::string::npos || line.find("</think>") != std::string::npos)
            {
                section = (ParsingSection)((int)section + 1);
                continue;
            }

            ss[(int)section] << line << "\n";
        }

        return { ss[0].str(), ss[1].str() };
    }

    inline void RenderChatWindow(const std::string& chatModel, const std::vector<OllamaChat::ChatItem>& chatHistory)
    {
        std::string model = chatModel;
        const std::vector<OllamaChat::ChatItem>& history = chatHistory;

        // Container for the chat messages
        ImGui::BeginChild("ChatBox", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - (ImGui::GetTextLineHeight() * 5) - 10), true, ImGuiWindowFlags_HorizontalScrollbar);

        for (int i = 0; i < history.size(); ++i) {
            const auto& item = history[i];
            ImGui::PushID(i);

            OllamaParsingResult message = { std::string(), item.message };

            if (item.role == OllamaChat::Role::ASSISTANT)
                message = ParseModelAnswer(item.message);

            float totalHeight = ImGui::CalcTextSize(item.message.c_str()).y + ImGui::GetTextLineHeightWithSpacing() * 2 + 5; // Add extra margin for separator

            ImVec4 bgColor = (item.role == OllamaChat::Role::USER)
                ? ImVec4(0.2f, 0.5f, 0.9f, 1.0f)  // blue-ish for user
                : ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // gray for assistant
            ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);

            ImGui::BeginChild(("MessageBox" + std::to_string(i)).c_str(), ImVec2(0, totalHeight), true);

            const std::string label = (item.role == OllamaChat::Role::USER)
                ? "User"
                : model;
            ImGui::Text(label.c_str());
            ImGui::Separator();

            if (!message.thinking.empty())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(3, 252, 111, 255));
                ImGui::TextWrapped(message.thinking.c_str());
                ImGui::PopStyleColor();
            }

            if (!message.answer.empty())
                ImGui::TextWrapped(message.answer.c_str());

            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopID();
        }

        ImGui::EndChild();
    }
}
