#include <filesystem>
#include <fstream>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "OllamaAPI/OllamaClient.h"
#include "OllamaAPI/OllamaUtils.hpp"
#include "OllamaAPI/OllamaChat.h"

class OllamaGUI : public Walnut::Layer
{
public:
	OllamaClient client = OllamaClient("http://localhost:11434/api");
	std::vector<OllamaChat> chatList;
	std::vector<std::filesystem::directory_entry> chatFiles;
	bool wasClientBusy = false;
	bool debugDone = false;

    OllamaGUI()
	{ 
		std::cout << "Hello from layer" << std::endl;

		std::string path("./chats");
		std::string ext(".chat");
		for (auto& p : std::filesystem::recursive_directory_iterator(path))
		{
			if (p.path().extension() == ext)
			{
				std::cout << p.path().stem().string() << '\n';
				chatFiles.push_back(p);
			}
		}
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
		
		ImGui::End();

		ImGui::Begin("Test Chat");

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

			if (ImGui::InputTextMultiline("ChatInput", buffer, IM_ARRAYSIZE(buffer),
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

			// Debug
			if (debugDone)
				continue;

			/* Chat related debug code goes here */
			const std::vector<OllamaChat::ChatItem>& history = chat.GetChatHistory();
			int messageCount = history.size();
			for (int i = messageCount - 1; i >= 0; i--)
			{
				std::cout << "[Ollama Client Helper] Chat message :\n" << history[i].message << std::endl;
			}

			debugDone = true;
		}

		ImGui::End();

		ImGui::ShowDemoWindow();
		
		UI_DrawAboutModal();
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

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}
private:
	bool m_AboutModalOpen = false;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "OllamaGUI";
	spec.CustomTitlebar = true;

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