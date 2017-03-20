
// binocularCameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "binocularCamera.h"
#include "binocularCameraDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbinocularCameraDlg 对话框




CbinocularCameraDlg::CbinocularCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CbinocularCameraDlg::IDD, pParent)
	, m_lfCamID(0)
	, m_riCamID(0)
	, m_nCamCount(0)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
	, m_nImageChannels(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbinocularCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERAR, m_ComboBoxCameraR);
	DDX_Control(pDX, IDC_COMBO_CAMERAL, m_ComboBoxCameraL);
	DDX_Control(pDX, IDC_CBN_Resolution, m_CBNResolution);
}

BEGIN_MESSAGE_MAP(CbinocularCameraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERAL, &CbinocularCameraDlg::OnCbnSelchangeComboCameral)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERAR, &CbinocularCameraDlg::OnCbnSelchangeComboCamerar)
	ON_CBN_SELCHANGE(IDC_CBN_Resolution, &CbinocularCameraDlg::OnCbnSelchangeCbnResolution)
	ON_BN_CLICKED(IDC_BN1RunCam, &CbinocularCameraDlg::OnBnClickedBn1runcam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BN2StopCam, &CbinocularCameraDlg::OnBnClickedBn2stopcam)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CbinocularCameraDlg 消息处理程序

BOOL CbinocularCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 获取摄像头数目
	m_nCamCount = CCameraDS::CameraCount();
	if( m_nCamCount < 1 )
	{
		AfxMessageBox(_T("请插入至少1个摄像头！"));
		//return -1;
	}

	// 在组合框CamList中添加摄像头名称的字符串
	char camera_name[1024];
	char istr[1024];
	CString camstr;
	for(int i=0; i < m_nCamCount; i++)
	{  
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name) );

		sprintf_s(istr, "#%d ", i);
		strcat_s( istr, camera_name );  
		camstr = istr;
		if(retval >0)
		{
			m_ComboBoxCameraL.AddString(camstr);
			m_ComboBoxCameraR.AddString(camstr);
		}
		else
			AfxMessageBox(_T("不能获取摄像头的名称"));
	}
	camstr.ReleaseBuffer();
	if (m_nCamCount <= 1)
		m_ComboBoxCameraR.EnableWindow(FALSE);

	// 初始化图像显示控件的图像
	CRect rect;
	GetDlgItem(IDC_PicLfCam) ->GetClientRect( &rect );
	m_lfImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicRiCam) ->GetClientRect( &rect );
	m_riImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbinocularCameraDlg::OnPaint()
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
		CDialogEx::OnPaint();
		CDialog::UpdateWindow();				// 更新windows窗口，如果无这步调用，图片显示还会出现问题
		if (!m_lfImage.empty())
		{
			F_ShowImage( m_lfImage, m_lfImage, IDC_PicLfCam );		// 重绘图片函数
			F_ShowImage( m_riImage, m_riImage, IDC_PicRiCam );		// 重绘图片函数
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbinocularCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CbinocularCameraDlg::OnCbnSelchangeComboCameral()
{
	// TODO: 在此添加控件通知处理程序代码
	// 确定左右摄像头对应的设备序号（对应下拉菜单选择项的序号）
	m_lfCamID = m_ComboBoxCameraL.GetCurSel();

	// 使启动摄像头按钮生效
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( TRUE );

	return;
}


void CbinocularCameraDlg::OnCbnSelchangeComboCamerar()
{
	// TODO: 在此添加控件通知处理程序代码
	// 确定左右摄像头对应的设备序号（对应下拉菜单选择项的序号）
	m_riCamID = m_ComboBoxCameraR.GetCurSel();

	// 使启动摄像头按钮生效
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( TRUE );

	return;
}


void CbinocularCameraDlg::OnCbnSelchangeCbnResolution()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_CBNResolution.GetCurSel();
	switch (index)
	{
	case 0:
		m_nImageWidth = 2048;
		m_nImageHeight = 1536;
		break;
	case 1:
		m_nImageWidth = 640;
		m_nImageHeight = 480;
		break;
	case 2:
		m_nImageWidth = 352;
		m_nImageHeight = 288;
		break;
	case 3:
		m_nImageWidth = 320;
		m_nImageHeight = 240;
		break;
	default:
		m_nImageWidth = 640;
		m_nImageHeight = 480;
	}

	m_nImageChannels = 3;
}


void CbinocularCameraDlg::OnBnClickedBn1runcam()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nImageWidth * m_nImageHeight * m_nImageChannels == 0)
	{
		AfxMessageBox(_T("请选择摄像头画面的分辨率！"));
		return;
	}
	if (m_nCamCount > 0)
	{
		//打开第一个摄像头
		//if( ! lfCam.OpenCamera(m_riCamID, false, m_nImageWidth, m_nImageHeight) ) //不弹出属性选择窗口，用代码制定图像宽和高
		if ( !lfCam.open(m_lfCamID) )
		{
			AfxMessageBox(_T("打开左摄像头失败."));
			return;
		}
		lfCam.set(CV_CAP_PROP_FRAME_WIDTH,  m_nImageWidth);
		lfCam.set(CV_CAP_PROP_FRAME_HEIGHT, m_nImageHeight);
	}
	if (m_nCamCount > 1)
	{
		if (m_lfCamID == m_riCamID)
		{
			AfxMessageBox(_T("左右摄像头的设备序号不能相同！"));
			return;
		}
		//打开第二个摄像头
		//if( ! riCam.OpenCamera(m_lfCamID, false, m_nImageWidth, m_nImageHeight) ) 
		if ( !riCam.open(m_riCamID) )
		{
			AfxMessageBox(_T("打开右摄像头失败."));
			return;
		}
		riCam.set(CV_CAP_PROP_FRAME_WIDTH,  m_nImageWidth);
		riCam.set(CV_CAP_PROP_FRAME_HEIGHT, m_nImageHeight);

		// GetDlgItem( IDC_BN_CompDisp )->EnableWindow( TRUE );
	}

	// 使部分按钮生效
	GetDlgItem( IDC_BN2StopCam )->EnableWindow( TRUE );
	//GetDlgItem( IDC_BN2StereoCalib)->EnableWindow( TRUE );
	// 使部分按钮失效
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( FALSE );
	GetDlgItem( IDC_COMBO_CAMERAL )->EnableWindow( FALSE );
	GetDlgItem( IDC_COMBO_CAMERAR )->EnableWindow( FALSE );
	GetDlgItem( IDC_CBN_Resolution )->EnableWindow( FALSE );

	// 启动摄像头后显示实时画面
	DoShowOrigFrame();

	return;
}

void CbinocularCameraDlg::DoShowOrigFrame(void)
{
	// m_CBNMethodList.SetCurSel(0);
	// m_ProcMethod = SHOW_ORIGINAL_FRAME;
	SetTimer(1, 50, NULL);	// 50ms 定时显示
}


void CbinocularCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (lfCam.isOpened())
	{
		Mat lfFrame;
		lfCam >> lfFrame;
	/*
		if (m_ProcMethod != SHOW_ORIGINAL_FRAME)
		{
			DoFrameProc(lfFrame, lfFrame);
		}
	*/
		F_ShowImage(lfFrame, m_lfImage, IDC_PicLfCam);
	}
	if (riCam.isOpened())
	{
		Mat riFrame;
		riCam >> riFrame;
	/*
		if (m_ProcMethod != SHOW_ORIGINAL_FRAME)
		{
			DoFrameProc(riFrame, riFrame);
		}
	*/
		F_ShowImage(riFrame, m_riImage, IDC_PicRiCam);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CbinocularCameraDlg::F_ShowImage(Mat& src, Mat& des, UINT ID)
{
	if (src.empty())
	{
		return;
	}

	des = Scalar::all(0);

	// 计算将图片缩放到 Image 区域所需的比例因子
	double wRatio = des.cols / (double)src.cols;
	double hRatio = des.rows / (double)src.rows;
	double srcWH = src.cols / (double)src.rows;
	double desWH = des.cols / (double)des.rows;
	double scale = srcWH > desWH ? wRatio : hRatio;

	// 缩放后图片的宽和高
	int nw = (int)( src.cols * scale );
	int nh = (int)( src.rows * scale );

	// 为了将缩放后的图片存入 des 的正中部位，需计算图片在 des 左上角的期望坐标值
	int tlx = (int)((des.cols - nw) / 2);
	int tly = (int)((des.rows - nh) / 2);

	// 设置 des 的 ROI 区域，用来存入图片 img
	Mat desRoi = des(Rect(tlx, tly, nw, nh));

	// 如果src是单通道图像，则转换为三通道图像
	if (src.channels() == 1)
	{
		Mat src_c;
		cvtColor(src, src_c, CV_GRAY2BGR);
		// 对图片 src_t 进行缩放，并存入到 des 中
		resize(src_c, desRoi, desRoi.size());
	}
	else
	{
		// 对图片 src 进行缩放，并存入到 des 中
		resize( src, desRoi, desRoi.size() );
	}

	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// 获得显示控件的 DC
	HDC hDC = pDC ->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );	// 获取控件尺寸位置
	CvvImage cimg;
	IplImage cpy = des;
	cimg.CopyOf( &cpy );						// 复制图片
	cimg.DrawToHDC( hDC, &rect );				// 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}


void CbinocularCameraDlg::OnBnClickedBn2stopcam()
{
	// TODO: 在此添加控件通知处理程序代码
	if (lfCam.isOpened())
	{
		GetDlgItem( IDC_BN2StopCam )->EnableWindow( FALSE );
		KillTimer(1);

		// 对图像数据清零
		m_lfImage = Scalar(0);
		F_ShowImage( m_lfImage, m_lfImage, IDC_PicLfCam );
		lfCam.release();
		if (riCam.isOpened())
		{
			m_riImage = Scalar(0);
			F_ShowImage( m_riImage, m_riImage, IDC_PicRiCam );
			riCam.release();
		}

		// 使启动摄像头按钮和摄像头选择列表生效
		GetDlgItem( IDC_BN1RunCam )->EnableWindow( TRUE );
		GetDlgItem( IDC_COMBO_CAMERAL )->EnableWindow( TRUE );
		GetDlgItem( IDC_COMBO_CAMERAR )->EnableWindow( TRUE );
		GetDlgItem( IDC_CBN_Resolution )->EnableWindow( TRUE );
		// GetDlgItem( IDC_BN2StereoCalib)->EnableWindow( FALSE );
		// GetDlgItem( IDC_BN_CompDisp )->EnableWindow( FALSE );
	}
	return;
}


void CbinocularCameraDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (lfCam.isOpened() || riCam.isOpened())
	{
		AfxMessageBox(_T("请先关闭摄像头！"));
		return;
	}

	CDialogEx::OnClose();
}
