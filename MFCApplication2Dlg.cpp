
// MFCApplication2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include <gdiplus.h>
#include <iostream>
#pragma comment (lib, "GdiPlus.lib")
#pragma comment (lib, "Gdi32.lib")

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

#include <chrono>
#include <mutex>

class WatermarkElapsedTimer {

public:
	WatermarkElapsedTimer() {
		startMillSec_ = 0;
		running_ = false;
	}

	inline void start() {
		running_ = true;
		if (0 == startMillSec_) {
			std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp
				= std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
			startMillSec_ = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
		}
	}

	inline void restart() {
		running_ = true;
		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp
			= std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		startMillSec_ = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
	}

	inline long long elapsed() const {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp
			= std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		long long curMillSec = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
		return curMillSec - startMillSec_;
	}
private:
	long long startMillSec_;
public:
	bool running_;
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 对话框



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。


int   GetEncoderClsid(const   WCHAR*   format, CLSID*   pClsid)
{
	UINT     num = 0;                     //   number   of   image   encoders
	UINT     size = 0;                   //   size   of   the   image   encoder   array   in   bytes

	Gdiplus::ImageCodecInfo*   pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return   -1;     //   Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return   -1;     //   Failure

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return   j;     //   Success
		}
	}

	free(pImageCodecInfo);
	return   -1;     //   Failure
}

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		static int count = 1;
		if (count-- > 0) {
			
			AllocConsole();

			freopen("CONOUT$", "w+t", stdout);// 申请写

			freopen("CONIN$", "r+t", stdin);  // 申请读

			WatermarkElapsedTimer el;
			el.start();

			CPaintDC dc(this);
			CRect rect;
			GetClientRect(&rect);
			int nWidth = 4960;
			int nHeight = 7015;

			Gdiplus::Image img(L"D:\\aa.png");


			int nMaxWidth = 0.5 * nWidth + 0.87 * nHeight;
			int nMaxHeight = 0.5 * nWidth + 0.87 * nHeight;

			Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromImage(&img);

			graphics->SetPageUnit(Gdiplus::UnitPixel);
			graphics->TranslateTransform(nWidth / 2, nHeight / 2, Gdiplus::MatrixOrderPrepend);
			graphics->RotateTransform(-30);

			Gdiplus::SolidBrush blackBrush(0x66111111);

			//Gdiplus::GraphicsPath graphicePath;
			Gdiplus::StringFormat stringformat;
			stringformat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

			Gdiplus::FontFamily pFontFamil(L"Tahoma");

			
			int nCopyHeight = 954;
			int nCopyWidth = 4889;
			Gdiplus::GraphicsPath graphicePath;
			for (int h = -0.5 * nMaxHeight; h < 0.5 * nMaxHeight; h += nCopyHeight)
			{
				
				for (int w = -0.5 * nMaxWidth; w < 0.5 * nMaxWidth; w += nCopyWidth)
				{
					graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(260), Gdiplus::Rect(w, h, nCopyWidth, nCopyHeight), &stringformat);
				}
				//graphics->FillPath(&blackBrush, &graphicePath);
			}

			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-1000, -2000, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  -160, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-100, -120, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  -80, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-100, -40, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  0, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-100, 20, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  300, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-100, 120, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  160, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(-100, 200, 400, 100), &stringformat);
			//graphicePath.AddString(L"1234567890", -1, &pFontFamil, Gdiplus::FontStyleBold, Gdiplus::REAL(30), Gdiplus::Rect(100,  2400, 400, 100), &stringformat);

			graphics->FillPath(&blackBrush, &graphicePath);

			graphics->ResetTransform();
			
			CLSID encoderClsid;
			GetEncoderClsid(L"image/png", &encoderClsid);
			img.Save(L"D:\\33334.png", &encoderClsid);

			std::cout << "elapsed=" << el.elapsed() << std::endl;

			FreeConsole();

		}

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

