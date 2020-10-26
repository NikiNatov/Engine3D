#pragma once

namespace E3D
{
	class FileDialog
	{
	public:
		FileDialog(const FileDialog& other) = delete;
		FileDialog& operator=(const FileDialog& other) = delete;

		static std::string OpenFile(const char* filters);
		static std::string SaveFile(const char* filters);

		static std::string OpenModelFile();
		static std::string OpenTextureFile();
	private:
		FileDialog() = default;
	};
}