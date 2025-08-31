#include "OllamaChat.h"
#include "Utils/Macros.h"

OllamaChat::OllamaChat(const std::string& filePath, const std::string& name)
	:m_ChatName(name), m_path(filePath)
{
	std::string path(filePath);
	path += base64::to_base64(name) + ".chat";

	std::ifstream file(path);

	OL_ASSERT(file, "Chat file {0}(name: {1}) doesn't exit", path, name);

	std::string content((std::istreambuf_iterator<char>(file)),
						std::istreambuf_iterator<char>());
	content = base64::from_base64(content);

	std::string line;
	ParsingState state = ParsingState::NONE;
	ParsingState previousState = ParsingState::NONE;
	
	std::vector<ChatItem> tempHistory;
	Role currentRole;
	std::string currentMessage;
	std::istringstream stream(content);
	FileInfo fileInfo;

	while (std::getline(stream, line))
	{
		// Retrieving chat model
		if (line.find("<Model>") != std::string::npos && state == ParsingState::NONE)
		{
			fileInfo.hasModel = true;
			state = ParsingState::CHAT_MODEL;
			continue;
		}
		if (line.find("</Model>") != std::string::npos && state == ParsingState::CHAT_MODEL)
		{
			state = ParsingState::NONE;
			continue;
		}

		// Chat history
		if (line.find("<Chat History>") != std::string::npos && state == ParsingState::NONE)
		{
			fileInfo.hasHistory = true;
			state = ParsingState::IN_HISTORY;
			continue;
		}
		if (line.find("</Chat History>") != std::string::npos && state == ParsingState::IN_HISTORY)
			break;
		
		if (line.find("<Chat Item>") != std::string::npos && state == ParsingState::IN_HISTORY)
		{
			state = ParsingState::IN_CHATITEM;
			continue;
		}
		if (line.find("</Chat Item>") != std::string::npos && state == ParsingState::IN_CHATITEM)
		{
			state = ParsingState::IN_HISTORY;

			tempHistory.push_back({ currentRole, currentMessage });
			currentRole = Role::NONE;
			currentMessage = "";

			continue;
		}

		if (line.find("<Role>") != std::string::npos && state == ParsingState::IN_CHATITEM)
		{
			state = ParsingState::IN_CHATROLE;
			continue;
		}
		if (line.find("</Role>") != std::string::npos && state == ParsingState::IN_CHATROLE)
		{
			state = ParsingState::IN_CHATITEM;
			continue;
		}

		if (line.find("<Message>") != std::string::npos && state == ParsingState::IN_CHATITEM)
		{
			state = ParsingState::IN_CHATMESSAGE;
			continue;
		}
		if (line.find("</Message>") != std::string::npos && state == ParsingState::IN_CHATMESSAGE)
		{
			state = ParsingState::IN_CHATITEM;
			continue;
		}

		if (state == ParsingState::CHAT_MODEL)
			m_model = line;

		if (state == ParsingState::IN_CHATROLE)
			currentRole = role_from_string(line);
		if (state == ParsingState::IN_CHATMESSAGE)
			currentMessage += (previousState == ParsingState::IN_CHATMESSAGE)
			? "\n" + line
			: line;
		previousState = state;
	}

	OL_VERIFY(fileInfo.hasModel && fileInfo.hasHistory, "Chat file ({0}) is corrupted and should be removed", path)

	m_ChatHistory = tempHistory;
}

OllamaChat::~OllamaChat()
{
	std::string content = "<Model>\n" + m_model + "\n</Model>\n";
	content += "<Chat History>\n";

	for (ChatItem item : m_ChatHistory)
	{
		content += "<Chat Item>\n";
		content += "<Role>\n" + OllamaChat::role_to_string(item.role) + "\n</Role>\n";
		content += "<Message>\n" + item.message + "\n</Message>\n";
		content += "</Chat Item>\n";
	}

	content += "</Chat History>";

	// Encode for obscution
	m_ChatName = base64::to_base64(m_ChatName);
	content = base64::to_base64(content);

	OL_INFO_TAG("Ollama Chat Helper", "Chat path, name : {0}, {1}", m_path, m_ChatName);
	std::ofstream file(m_path + m_ChatName + ".chat");
	file << content;
}

const std::vector<OllamaChat::ChatItem>& OllamaChat::GetChatHistory() const
{
	return m_ChatHistory;
}

const std::string OllamaChat::GetModel() const
{
	return m_model;
}

const std::string OllamaChat::GetName() const
{
	return m_ChatName;
}

void OllamaChat::AddChatItem(OllamaChat::Role role, std::string message)
{
	m_ChatHistory.push_back({ role, message });
}

void OllamaChat::ClearChat()
{
	m_ChatHistory = std::vector<ChatItem>();
}
