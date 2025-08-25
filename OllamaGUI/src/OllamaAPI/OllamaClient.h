#pragma once

#include "cpr/cpr.h"
#include <iostream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

#include "OllamaUtils.hpp"
#include "OllamaChat.h"

class OllamaClient
{
public:
	std::string streamingBuffer;
	bool busy;

	OllamaClient(const std::string& host_url);
	~OllamaClient();

	void ShowModelsList() const;
	void GenerateAnswer(const OllamaChat& chat);
	std::vector<std::string> GetModelsList() const;

private:
	std::vector<std::string> m_modelsList;
	std::string m_apiUrl;
};
