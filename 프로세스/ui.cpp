//ui.cpp

#include "std.h"

HWND g_hListView;

void ui_GetControlHandle(HWND hDlg)
{
	g_hListView = GetDlgItem(hDlg, IDC_LIST1);
}

void ui_ListViewCreateHeader(HWND hDlg)
{
	// 헤더를 추가한다.
	LVCOLUMN COL;

	COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt = LVCFMT_LEFT;
	COL.cx = 100;
	COL.pszText = (LPTSTR)TEXT("name");				// 첫 번째 헤더
	COL.iSubItem = 0;
	SendMessage(g_hListView, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

	COL.cx = 150;
	COL.pszText = (LPTSTR)TEXT("pid");			// 두 번째 헤더
	COL.iSubItem = 1;
	SendMessage(g_hListView, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

	ListView_SetExtendedListViewStyle(g_hListView, LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
}

void ui_SelectListView(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR hdr;
	LPNMLISTVIEW nlv;
	hdr = (LPNMHDR)lParam;
	nlv = (LPNMLISTVIEW)lParam;

	if (hdr->hwndFrom == g_hListView)
	{
		switch (hdr->code)
		{
		case LVN_ITEMCHANGED:
			if (nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
			{
				LVITEM LI;
				LI.iItem = nlv->iItem;
				LI.iSubItem = 0;
				ListView_GetItem(g_hListView, &LI);

				TCHAR temp[20];
				ListView_GetItemText(g_hListView, nlv->iItem, 1, temp, _countof(temp));
				SetDlgItemText(hDlg, IDC_EDIT2, temp);
			}
		}
	}
}

void ui_SelectPrint(HWND hDlg)
{
	int pid = GetDlgItemInt(hDlg, IDC_EDIT2, 0, 0);
	HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);    

	DWORD code;
	GetExitCodeProcess(h, &code);
	if (STILL_ACTIVE == code)
		SetDlgItemText(hDlg, IDC_EDIT3, TEXT("실행중"));
	else
		SetDlgItemText(hDlg, IDC_EDIT3, TEXT("종료"));
}
