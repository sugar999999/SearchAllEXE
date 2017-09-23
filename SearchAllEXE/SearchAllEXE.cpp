// SearchAllEXE.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#define MAX_ARGC 3

using namespace std;

int main(int argc, char* argv[])
{
	CHAR filter[MAX_PATH] = {};
	BOOL isDebug = FALSE;
	
	if (argc == 1)
	{
		cout << "no paramater, search all exe file..." << endl;
		
	}
	else if (argc > MAX_ARGC)
	{
		cout << "paramater invalid.." << endl;
		return 1;
	}
	else
	{
		for (int i = argc - 1; i > 0; i--)
		{
			if (_stricmp(argv[i], "-d") == 0)
			{
				if (isDebug)
				{
					cout << "parameter invalid.." << endl;
					return 1;
				}
				isDebug = TRUE;
			}
			else
			{
				if (strnlen(filter, _countof(filter)) > 0)
				{
					cout << "paramater invalid.." << endl;
					return 1;
				}
				strncpy_s(filter, _countof(filter), argv[i], _TRUNCATE); 
				_strupr_s(filter, _countof(filter));
			}
		}
	}

	searchEXE("C:\\", filter, isDebug);
	
	cout << "FINISH!!!" << endl << "Please enter any key.. " << endl;
	
	getchar();

	return 0;
}



int searchEXE(LPCSTR path, LPSTR filter, BOOL isDebug)
{

	DWORD ret = 0;

	HANDLE hFind = NULL;
	WIN32_FIND_DATAA fData = {};
	CHAR searchPath[MAX_PATH] = {};

	// sanity check
	if (path == NULL)
	{
		ret = 1;
		goto cleanup;
	}

	sprintf_s(searchPath, "%s*", path);

	hFind = FindFirstFileA(searchPath, &fData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		if (isDebug)
			cout << "[ERROR] FindFirstFileA is failed: " << searchPath << " (code:" << GetLastError() << ")" << endl;
		ret = 1;
		goto cleanup;
	}

	do
	{
		if (_stricmp(fData.cFileName, ".") == 0 || _stricmp(fData.cFileName, "..") == 0)
		{
			continue;
		}
		
		LPSTR lpExt = NULL;

		lpExt = strrchr(fData.cFileName, '.');
		
		if (lpExt == NULL)
		{
			CHAR szNextSearchPath[MAX_PATH] = {};
			sprintf_s(szNextSearchPath, "%s%s\\", path, fData.cFileName);
			searchEXE(szNextSearchPath, filter, isDebug);
			continue;
		} 
		else if (_stricmp(lpExt, ".exe") == 0)
		{
			CHAR cFilePathName[MAX_PATH] = {};
			CHAR cFilePathUpr[MAX_PATH] = {};

			sprintf_s(cFilePathName, "%s%s", path, fData.cFileName);
			sprintf_s(cFilePathUpr, "%s%s", path, fData.cFileName);
			_strupr_s(cFilePathUpr, _countof(cFilePathUpr));

			if (filter == NULL ||
				(filter != NULL && strstr(cFilePathUpr, filter) != NULL))
					cout << cFilePathName << endl;
		}

		
	} while (FindNextFileA(hFind, &fData));
	
cleanup:

	FindClose(hFind);
	return ret;
}