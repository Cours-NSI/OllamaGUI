#include "OllamaClient.h"
#include "Utils/Macros.h"

OllamaClient::OllamaClient(const std::string& host_url)
	: m_apiUrl(host_url), m_modelsList({}), busy(false)
{
	std::string final_url = m_apiUrl + "/tags";
	cpr::Response response = cpr::Get(cpr::Url{ final_url });

	OL_HTTP_ASSERT(response, "HTTP request to {0} wasn't succesful and responded with code: {1}. Installing Ollama (https://ollama.com/download) or launching it if you have already installed it", final_url, response.status_code);

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
		OL_ERROR_TAG("JSON Helper", "An error occurred during response parsing, execption : {0}", e.what());
	}
}

OllamaClient::~OllamaClient()
{
}

void OllamaClient::ShowModelsList() const
{
	std::string message = "Locally installed models:\n";
	for (const std::string& m : m_modelsList)
		message += "	- " + m + "\n";
	OL_INFO_TAG("Ollama Client Helper", message);
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

	OL_INFO_TAG("Ollama Client Helper", "Chat completion payload :\n{0}", payload.dump(2));

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
