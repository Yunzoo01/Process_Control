//handler.cpp

#include "std.h"

vector<WBPROCESS> g_processs;

extern HWND g_hListView;

int g_selectid;

BOOL OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ui_GetControlHandle(hDlg);
	ui_ListViewCreateHeader(hDlg);

	return 0;
}

BOOL OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDCANCEL:		EndDialog(hDlg, IDCANCEL);	return TRUE;
	case IDC_BUTTON1:	OnInsert(hDlg);				return TRUE;
	case IDC_BUTTON2:	OnUpdate(hDlg);				return TRUE;
	case IDC_BUTTON3:	OnCheck(hDlg);				return TRUE;
	case IDC_BUTTON4:	OnClose(hDlg);				return TRUE;
	}
	return TRUE;
}

BOOL OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ui_SelectListView(hDlg, wParam, lParam);

	return TRUE;
}

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("unknown");

	// 프로세스의   핸들   얻기	
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, processID);
	// process 이름   가져오기
	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD    cbNeeded;
		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName));
		}
		else
			return;
	}
	else
		return;

	//print
	WBPROCESS pinfo;
	_tcscpy_s(pinfo.name, _countof(pinfo.name), szProcessName);
	pinfo.pid = processID;

	g_processs.push_back(pinfo);

	CloseHandle(hProcess);
}

void WbEnumProcess(HWND hDlg)
{
	// process list 가져오기(id값)
	DWORD aProcess[1024], cbNeeded, cProcesses;
	unsigned int i;

	// 배열    수, 리턴되는   바이트   수 // 배열에   id값들이   들어간다. 
	if (!EnumProcesses(aProcess, sizeof(aProcess), &cbNeeded))
		return;

	// 얼마나   많은   프로세스들이   리턴되었나   계산 
	cProcesses = cbNeeded / sizeof(DWORD);
	// process 이름   출력
	for (i = 0; i < cProcesses; i++)
		PrintProcessNameAndID(aProcess[i]);

	TCHAR buf[20];
	wsprintf(buf, TEXT("프로세스 개수 : %d개"), cProcesses);
	SetDlgItemText(hDlg, IDC_STATIC1, buf);
}

void WbPrintProcess()
{
	ListView_DeleteAllItems(g_hListView);

	for (int i = 0; i < (int)g_processs.size(); i++)
	{
		WBPROCESS pro = g_processs[i];

		LVITEM LI;
		LI.mask = LVIF_TEXT;

		LI.iItem = i;   
		LI.iSubItem = 0;

		LI.pszText = pro.name; 
		SendMessage(g_hListView, LVM_INSERTITEM, 0, (LPARAM)&LI);
		
		LI.iSubItem = 1;
		TCHAR temp[20];
		wsprintf(temp, TEXT("%d"), pro.pid);
		LI.pszText = temp;			
		SendMessage(g_hListView, LVM_SETITEM, 0, (LPARAM)&LI);
	}
}

void OnInsert(HWND hDlg)
{
	WbEnumProcess(hDlg);

	WbPrintProcess();
}

void OnUpdate(HWND hDlg)
{
	TCHAR name[20];
	GetDlgItemText(hDlg, IDC_EDIT1, name, _countof(name));

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;

	BOOL b = CreateProcess(0, name, 0, 0, 0, 0, 0, 0, &si, &pi);
	if (b)
	{
		WaitForInputIdle(pi.hProcess, INFINITE);
	}

	WbPrintProcess();
}

void OnCheck(HWND hDlg)
{
	ui_SelectPrint(hDlg);
}

void OnClose(HWND hDlg)
{
	int pid = GetDlgItemInt(hDlg, IDC_EDIT2, 0, 0);
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

	TerminateProcess(h, 0);
}

