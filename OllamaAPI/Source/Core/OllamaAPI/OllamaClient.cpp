#include "OllamaClient.h"

OllamaClient::OllamaClient(const std::string& host_url)
	: m_apiUrl(host_url), m_modelsList({}), busy(false)
{
	std::string final_url = m_apiUrl + "/tags";
	cpr::Response response = cpr::Get(cpr::Url{ final_url });

	if (response.status_code != 200)
	{
		std::cout << "[Ollama Client Helper] Error: status " << response.status_code << std::endl;
		return;
	}

	try
	{
		nlohmann::json data = nlohmann::json::parse(response.text);
		for (const auto& model : data["models"])
		{
			if (model.contains("model"))
				m_modelsList.push_back(model["model"]);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "[Ollama Client Helper] JSON error: " << e.what() << std::endl;
	}
}

OllamaClient::~OllamaClient()
{
}

void OllamaClient::ShowModelsList() const
{
	std::cout << "[Ollama Client Helper] Locally installed models:" << std::endl;
	for (const std::string& m : m_modelsList)
	{
		std::cout << " - " << m << std::endl;
	}
}

void OllamaClient::GenerateAnswer(const OllamaChat& chat) {
	streamingBuffer.clear();

	nlohmann::json messages = nlohmann::json::array();

	const std::vector<OllamaChat::ChatItem>& history = chat.GetChatHistory();
	for (const OllamaChat::ChatItem& item : history)
	{
		messages.push_back({
			{"role", OllamaChat::role_to_string(item.role)},
			{"content", item.message}
		});

	}

	nlohmann::json payload = {
		{"model", chat.GetModel()},
		{"messages", messages}
	};

	std::cout << "[Ollama Client Helper] Chat completion payload :\n" << payload << std::endl;

	auto callback = OllamaUtils::CreateStreamingCallback(streamingBuffer, busy);

	busy = true;

	std::thread([this, payload, callback]() {
		cpr::Post(
			cpr::Url{ m_apiUrl + "/chat" },
			cpr::Header{ {"Content-Type", "application/json"} },
			cpr::Body{ payload.dump() },
			callback
		);
	}).detach(); // run without blocking UI

}

std::vector<std::string> OllamaClient::GetModelsList() const
{
	return m_modelsList;
}
