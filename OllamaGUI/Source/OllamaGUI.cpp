#include <filesystem>
#include <fstream>

#include "base64.hpp"

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "OllamaAPI/API.h"
#include "Utils/OllamaUtils.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

class OllamaGUI : public Walnut::Layer
{
public:
	OllamaClient client = OllamaClient("http://localhost:11434/api");
	std::vector<OllamaChat> chatList;
	std::vector<std::filesystem::directory_entry> chatFiles;
	bool wasClientBusy = false;

    OllamaGUI()
	{
		std::string path("./chats");
		std::string ext(".chat");

		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);

		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			if (p.path().extension() == ext)
			{
				chatFiles.push_back(p);
			}
		}
	}

	~OllamaGUI()
	{

	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("ChatList");

		int numWidgets = 1;
		int numRows = 5;
		
		float spcx = ImGui::GetStyle().ItemSpacing.x;

		float maxWidth = ImGui::GetContentRegionAvail().x - spcx;

		float mrw = maxWidth / numWidgets - spcx;

		for (const auto& chat : chatFiles)
		{
			if (ImGui::Button(base64::from_base64(chat.path().stem().string()).c_str(), ImVec2(mrw, 45)))
			{
				chatList.push_back(OllamaChat("./chats/", base64::from_base64(chat.path().stem().string())));
			}
		}
		
		for (OllamaChat& chat : chatList)
		{
			ImGui::Begin(chat.GetName().c_str());

			if (client.busy)
			{
				std::vector<OllamaChat::ChatItem> tempHistory = chat.GetChatHistory();
				tempHistory.push_back({ OllamaChat::Role::ASSISTANT, client.streamingBuffer });
				
				OllamaUtils::RenderChatWindow(chat.GetModel(), tempHistory);
				
				wasClientBusy = true;
			}
			else
				OllamaUtils::RenderChatWindow(chat.GetModel(), chat.GetChatHistory());

			static char buffer[1024 * 4] = "";
			if (ImGui::InputTextMultiline("##Chat Input", buffer, IM_ARRAYSIZE(buffer),
				ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5),
				ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (buffer[0] != '\0')
				{
					chat.AddChatItem(OllamaChat::Role::USER, buffer);
					client.GenerateAnswer(chat);

					buffer[0] = '\0';
					ImGui::SetKeyboardFocusHere(-1);
				}
			}

			if (!client.busy && wasClientBusy)
			{
				chat.AddChatItem(OllamaChat::Role::ASSISTANT, client.streamingBuffer);
				wasClientBusy = false;
			}

			ImGui::End();

			ImGui::ShowDemoWindow();

		}

		ImGui::End();
		
		UI_DrawAboutModal();
		UI_DrawCreateChat();
	}

	void UI_DrawAboutModal()
	{
		if (!m_AboutModalOpen)
			return;

		ImGui::OpenPopup("About");
		m_AboutModalOpen = ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_AboutModalOpen)
		{
			auto image = Walnut::Application::Get().GetApplicationIcon();
			ImGui::Image(image->GetDescriptorSet(), { 48, 48 });

			ImGui::SameLine();
			Walnut::UI::ShiftCursorX(20.0f);

			ImGui::BeginGroup();
			ImGui::Text("Walnut application framework");
			ImGui::Text("by Studio Cherno.");
			ImGui::EndGroup();

			if (Walnut::UI::ButtonCentered("Close"))
			{
				m_AboutModalOpen = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void UI_DrawCreateChat()
	{
		if (!m_CreateChatOpen)
			return;

		ImGui::OpenPopup("Create Chat");
		m_CreateChatOpen = ImGui::BeginPopupModal("Create Chat", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (m_CreateChatOpen)
		{
			static char buffer[1024 * 4] = "";
			static int selectedModel = 0;
			std::vector<std::string> models = client.GetModelsList();

			if (ImGui::InputTextWithHint("##Chat Name", "Chat Name", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_CreateChatOpen = false;
				ImGui::CloseCurrentPopup();
				std::ofstream file("./chats/" + base64::to_base64(buffer) + ".chat");
				chatFiles.push_back(std::filesystem::directory_entry("./chats/" + base64::to_base64(buffer) + ".chat"));
				std::string content = "<Model>\n";
				content += models[selectedModel];
				content += "\n</Model>\n<Chat History>\n</Chat History>";
				file << base64::to_base64(content);
				buffer[0] = '\0';
			}

			if (ImGui::Button("Select Model.."))
				ImGui::OpenPopup("model_selection_popup");
			ImGui::SameLine();
			ImGui::Text(models[selectedModel].c_str());

			if (ImGui::BeginPopup("model_selection_popup"))
			{	
				ImGui::Text("Model");
				ImGui::Separator();
				for (int i = 0; i < models.size(); i++)
					if (ImGui::Selectable(models[i].c_str()))
						selectedModel = i;
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			if (ImGui::Button("Cancel"))
			{
				m_CreateChatOpen = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			bool createButton = Walnut::UI::ButtonLeft("Create");
			if (buffer[0] != '\0' && createButton)
			{
				m_CreateChatOpen = false;
				ImGui::CloseCurrentPopup();
				std::ofstream file("./chats/" + base64::to_base64(buffer) + ".chat");
				chatFiles.push_back(std::filesystem::directory_entry("./chats/" + base64::to_base64(buffer) + ".chat"));
				std::string content = "<Model>\n";			
				content += models[selectedModel];
				content += "\n</Model>\n<Chat History>\n</Chat History>";
				file << base64::to_base64(content);
				buffer[0] = '\0';
			}
			ImGui::EndPopup();
		}
	}

	void UI_DrawAddModel()
	{
	}

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}

	void ShowCreateChat()
	{
		m_CreateChatOpen = true;
	}
private:
	bool m_AboutModalOpen = false;
	bool m_CreateChatOpen = false;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "OllamaGUI";
	spec.CustomTitlebar = true;

	OllamaCore::Log logger;
	logger.Init();

	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<OllamaGUI> appLayer = std::make_shared<OllamaGUI>();

	appLayer->client.ShowModelsList();
	
	app->PushLayer(appLayer);
	app->SetMenubarCallback([app, appLayer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}

			if (ImGui::MenuItem("New Chat"))
			{
				appLayer->ShowCreateChat();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				appLayer->ShowAboutModal();
			}
			ImGui::EndMenu();
		}
	});

	return app;
}