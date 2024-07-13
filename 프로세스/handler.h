//handler.h
#pragma once

BOOL OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
BOOL OnNotify(HWND hDlg, WPARAM wParam, LPARAM lParam);

void PrintProcessNameAndID(DWORD processID);
void WbEnumProcess(HWND hDlg);
void WbPrintProcess();

void OnInsert(HWND hDlg);
void OnUpdate(HWND hDlg);
void OnCheck(HWND hDlg);
void OnClose(HWND hDlg);
