#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "base64.hpp"

#include <iostream>
#include <filesystem>

class OllamaChat
{
public:
    enum class Role
    {
        NONE = -1,
        SYSTEM = 0,
        USER = 1,
        ASSISTANT = 2,
        TOOL = 3 // Unused for now
    };

    // Helper function to convert Role to string
    inline static std::string role_to_string(Role role) 
    {
        switch (role) {
        case Role::SYSTEM:    return "system";
        case Role::USER:      return "user";
        case Role::ASSISTANT: return "assistant";
        case Role::TOOL:      return "tool";
        default:              return "unknown";
        }
    }

    inline Role role_from_string(std::string source) 
    {
        if (source == "system")     return Role::SYSTEM;
        if (source == "user")       return Role::USER;
        if (source == "assistant")  return Role::ASSISTANT;
        if (source == "tool")       return Role::TOOL;
        else                        return Role::NONE;
    }

    struct ChatItem
    {
        Role role;
        std::string message;
    };

    enum class ParsingState
    {
        NONE = -1,
        CHAT_MODEL = 0,
        IN_HISTORY = 1,
        IN_CHATITEM = 2,
        IN_CHATROLE = 3,
        IN_CHATMESSAGE = 4
    };

    inline std::string state_to_string(ParsingState state)
    {
        switch (state)
        {
        case OllamaChat::ParsingState::NONE:            return "none";
        case OllamaChat::ParsingState::CHAT_MODEL:      return "model";
        case OllamaChat::ParsingState::IN_HISTORY:      return "in history";
        case OllamaChat::ParsingState::IN_CHATITEM:     return "in chat item";
        case OllamaChat::ParsingState::IN_CHATROLE:     return "in chat role";
        case OllamaChat::ParsingState::IN_CHATMESSAGE:  return "in chat message";
        default:                                        return "unknown";
        }
    }

    OllamaChat(const std::string& filePath, const std::string& name);
    ~OllamaChat();

    const std::vector<OllamaChat::ChatItem>& GetChatHistory() const;
    const std::string GetModel() const;
    const std::string GetName() const;
    void AddChatItem(OllamaChat::Role role, std::string message);
    void ClearChat();

private:
    std::vector<ChatItem> m_ChatHistory;
    std::string m_ChatName;
    std::string m_path;
    std::string m_model;
};

