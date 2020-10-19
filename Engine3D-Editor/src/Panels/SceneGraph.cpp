#include "SceneGraph.h"

#include "Engine3D\Scene\Components.h"
#include "Engine3D\Renderer\Model.h"

#include "Engine3D\Renderer\MeshFactory.h"

#include <ImGui\imgui.h>

#include "Engine3D\ResourceManager\ModelManager.h"
#include "Engine3D\Core\FileDialog.h"

namespace E3D
{
	SceneGraph::SceneGraph(const Ref<Scene>& scene)
	{
		SetScene(scene);
	}

	void SceneGraph::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
	}

	void SceneGraph::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// Temporary //////////////////////////////////////////////////////////////////////
		if (ImGui::Button("Create Entity"))
		{
			Entity newEntity = m_Scene->CreateEntity();
			newEntity.AddComponent<MeshComponent>(MeshFactory::CreateCube(1.0f, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader"))));
		}
		ImGui::SameLine();
		if (ImGui::Button("Create From Model"))
		{
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"Models", L"*.fbx;*.obj"},
				{ L"FBX", L"*.fbx" },
				{ L"OBJ", L"*.obj" }
			};
			const std::string& path = FileDialog::SaveFile(rgSpec, 3);

			if (!path.empty())
			{
				auto& newModel = ModelManager::LoadModel(path);
				m_Scene->CreateFromModel(newModel, newModel->GetName());
			}
		}
		///////////////////////////////////////////////////////////////////////////////////

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity(entityID, m_Scene.get());

			auto& parent = entity.GetComponent<SceneNodeComponent>().Parent;

			if (!parent)
				DrawNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::End();
	}

	void SceneGraph::DrawNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;
		auto& nodeComponent = entity.GetComponent<SceneNodeComponent>();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);

		bool isOpen;
		if (entity.HasComponent<MeshComponent>())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f });
			isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
			ImGui::PopStyleColor();
		}
		else
			isOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());

		if (ImGui::IsItemClicked())
			m_SelectedEntity = entity;

		ImGui::PushID((void*)(uint64_t)(uint32_t)entity);
		if (ImGui::BeginPopupContextItem("Options Menu", 1))
		{
			m_SelectedEntity = entity;
			if (ImGui::Selectable("Add Child"))
			{
				Entity newEntity = m_Scene->CreateEntity();
				newEntity.AddComponent<MeshComponent>(MeshFactory::CreateCube(1.0f, CreateRef<Material>(ShaderLibrary::Get("StaticModelShader"))));
				entity.AddChild(newEntity);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();

		if (isOpen)
		{
			auto currentChild = nodeComponent.FirstChild;

			while (currentChild)
			{
				DrawNode(currentChild);
				currentChild = currentChild.GetComponent<SceneNodeComponent>().NextSibling;
			}

			ImGui::TreePop();
		}

	}

	std::string SceneGraph::LoadModelFromFile()
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;
			COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"FBX", L"*.fbx" },
				{ L"OBJ", L"*.obj" },
			};
			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			pFileOpen->SetFileTypes(2, rgSpec);
			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							char cstrPath[260 * 2]{ 0 };
							int ret = wcstombs(cstrPath, pszFilePath, sizeof(cstrPath));
							CoUninitialize();

							return std::string(cstrPath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
		}
		CoUninitialize();
		//char filename[MAX_PATH]{ 0 };
		////BasicFileOpen(filename);
		//OPENFILENAMEA ofn;
		//ZeroMemory(&filename, sizeof(filename));
		//ZeroMemory(&ofn, sizeof(ofn));
		//ofn.lStructSize = sizeof(ofn);
		//ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		//ofn.lpstrFilter = "FBX Files\0*.fbx\0";
		//ofn.lpstrFile = filename;
		//ofn.nMaxFile = MAX_PATH;
		//ofn.lpstrTitle = "Select a File";
		//ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		//if (!GetOpenFileNameA(&ofn))
		//	E3D_LOG_ERROR("Failed to load the file!");

		return std::string();
	}

}