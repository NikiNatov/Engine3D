#include "pch.h"
#include "FileDialog.h"

#include <Windows.h>
#include <commdlg.h>

#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>

#include "Engine3D\Core\Application.h"

namespace E3D
{
	std::string FileDialog::OpenFile(const char* filters)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filters)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::GetInstance().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filters;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::OpenModelFile()
	{
		const char* filters = "All Models (*.fbx; *.obj)\0*.fbx;*obj\0FBX (*.fbx)\0*.fbx\0OBJ (*.obj)\0.obj";

		return OpenFile(filters);
	}

	std::string FileDialog::OpenTextureFile()
	{
		const char* filters = "Images (*.jpg;*.png;*.tga;*bmp;*.hdr)\0*.jpg;*.png;*.tga;*bmp;*.hdr\0";

		return OpenFile(filters);
	}
}