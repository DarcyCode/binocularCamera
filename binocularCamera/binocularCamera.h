
// binocularCamera.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CbinocularCameraApp:
// �йش����ʵ�֣������ binocularCamera.cpp
//

class CbinocularCameraApp : public CWinApp
{
public:
	CbinocularCameraApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CbinocularCameraApp theApp;