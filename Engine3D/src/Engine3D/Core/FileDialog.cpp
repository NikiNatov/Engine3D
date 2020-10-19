#include "pch.h"
#include "FileDialog.h"

namespace E3D
{
	std::string FileDialog::OpenFile(const COMDLG_FILTERSPEC* fileTypes, UINT fileTypeCount)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;
			/*COMDLG_FILTERSPEC rgSpec[] =
			{
				{ L"JPEG", L"*.jpg;*.jpeg" },
				{ L"PNG", L"*.png" },
				{ L"HDR", L"*.hdr" },
				{ L"TGA", L"*.tga" },
				{ L"Images", L"*.jpg;*.png;*.tga;*bmp;*.hdr" }
			};*/
			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			pFileOpen->SetFileTypes(fileTypeCount, fileTypes);
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

		return std::string();
	}

	std::string FileDialog::SaveFile(const COMDLG_FILTERSPEC* fileTypes, UINT fileTypeCount)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileSaveDialog* pFileSave;

			hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
				IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

			pFileSave->SetFileTypes(fileTypeCount, fileTypes);
			if (SUCCEEDED(hr))
			{
				hr = pFileSave->Show(NULL);

				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileSave->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

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
				pFileSave->Release();
			}
		}

		CoUninitialize();

		return std::string();
	}

	std::string FileDialog::OpenModelFile()
	{
		COMDLG_FILTERSPEC rgSpec[] =
		{
			{ L"Models", L"*.fbx;*.obj"},
			{ L"FBX", L"*.fbx" },
			{ L"OBJ", L"*.obj" }
		};

		return OpenFile(rgSpec, 3);
	}

	std::string FileDialog::OpenTextureFile()
	{
		COMDLG_FILTERSPEC rgSpec[] =
		{
			{ L"Images", L"*.jpg;*.png;*.tga;*bmp;*.hdr" },
			{ L"JPEG", L"*.jpg;*.jpeg" },
			{ L"PNG", L"*.png" },
			{ L"HDR", L"*.hdr" },
			{ L"TGA", L"*.tga" },
			{ L"BMP", L"*.bmp" }
		};

		return OpenFile(rgSpec, 6);
	}
}