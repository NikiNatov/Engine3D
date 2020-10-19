#pragma once

#include <Windows.h>
#include <shobjidl.h>

namespace E3D
{
	class FileDialog
	{
	public:
		FileDialog(const FileDialog& other) = delete;
		FileDialog& operator=(const FileDialog& other) = delete;

		static std::string OpenFile(const COMDLG_FILTERSPEC* fileTypes, UINT fileTypeCount);
		static std::string SaveFile(const COMDLG_FILTERSPEC* fileTypes, UINT fileTypeCount);

		static std::string OpenModelFile();
		static std::string OpenTextureFile();
	private:
		FileDialog() = default;
	};
}