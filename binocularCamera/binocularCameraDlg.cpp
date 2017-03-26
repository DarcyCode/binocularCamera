
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
	, m_nCornerSize_X(9)
	, m_nCornerSize_Y(6)
	, m_nSquareSize(26.0f)
	, m_nBoards(10)
	, m_dAlpha(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbinocularCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERAR, m_ComboBoxCameraR);
	DDX_Control(pDX, IDC_COMBO_CAMERAL, m_ComboBoxCameraL);
	DDX_Control(pDX, IDC_CBN_Resolution, m_CBNResolution);
	DDX_Text(pDX, IDC_BoardWidth, m_nCornerSize_X);
	DDV_MinMaxInt(pDX, m_nCornerSize_X, 0, 50);
	DDX_Text(pDX, IDC_BoardHeight, m_nCornerSize_Y);
	DDV_MinMaxInt(pDX, m_nCornerSize_Y, 0, 50);
	DDX_Text(pDX, IDC_SquareSize, m_nSquareSize);
	DDX_Text(pDX, IDC_nBoards, m_nBoards);
	DDV_MinMaxInt(pDX, m_nBoards, 0, 100);
	DDX_Text(pDX, IDC_Edit_alpha_rectify, m_dAlpha);
	DDV_MinMaxDouble(pDX, m_dAlpha, -1, 1);
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
	ON_BN_CLICKED(IDC_BN2StereoCalib, &CbinocularCameraDlg::OnBnClickedBn2stereocalib)
	ON_BN_CLICKED(IDC_BN_ExitCameraCalib, &CbinocularCameraDlg::OnBnClickedBnExitcameracalib)
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

	// 使一系列按钮失效
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( FALSE );			// 启动摄像头失效
	GetDlgItem( IDC_BN2StopCam )->EnableWindow( FALSE );		// 停止摄像头失效
	// 初始化图像显示控件的图像
	CRect rect;
	GetDlgItem(IDC_PicLfCam) ->GetClientRect( &rect );
	m_lfImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicRiCam) ->GetClientRect( &rect );
	m_riImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	
	CheckRadioButton(IDC_RAD_CalibFromCam, IDC_RAD_CalibFromImg, IDC_RAD_CalibFromCam);
	CheckRadioButton(IDC_RAD_Load1CamCalibResult, IDC_RAD_StereoCalib, IDC_RAD_Calib1CamFirst);
	CheckRadioButton(IDC_RAD_BOUGUET, IDC_RAD_HARTLEY, IDC_RAD_BOUGUET);				// 默认使用BOUGUET方法双目校正

	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ShowCalibResult);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK1FPP);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK1FPP2);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK2UIG);
	m_pCheck->SetCheck(0);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK2UIG2);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK3FAR);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK3FAR2);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK4ZTD);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK4SFL);
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK5FI);
	m_pCheck->SetCheck(0);
	m_pCheck = NULL;
	
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
	GetDlgItem( IDC_BN2StereoCalib)->EnableWindow( TRUE );
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


void CbinocularCameraDlg::OnBnClickedBn2stereocalib()
{
	// TODO: 在此添加控件通知处理程序代码

	//确认摄像机定标的相关选项
	OptionCameraCalib optCalib;
	if (!DoParseOptionsOfCameraCalib(optCalib))
		return;

	KillTimer(1);
	GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(FALSE);
//	GetDlgItem(IDC_CBN2MethodList)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BN_CompDisp)->EnableWindow(FALSE);

	// 棋盘检测参数初始化
	int nFoundBoard = 0, nFrame = 0;
	vector<CStringA> img0Files, img1Files;
	const char* img0_file = NULL, *img1_file = NULL;
	cv::Mat frame0, frame1, img0, img1;
	cv::Size imageSize;

	StereoCalib::CornerDatas cornerDatas;
	StereoCalib::StereoParams stereoParams;
	StereoCalib::RemapMatrixs remapMatrixs;

	try
	{
		////////////////////////////////////////////////////////////////////////
		// 读入或检测棋盘角点
		// *** 从本地文件读入角点坐标数据
		CStringA filePath(m_workDir);
		filePath.AppendFormat("CornerData.yml");
		const char* cornerFile = filePath.GetBuffer(0);
		if (optCalib.loadConerDatas)		
		{
			if ( m_stereoCalibrator.loadCornerData(cornerFile, cornerDatas) )
			{
				AfxMessageBox(_T("已读入角点坐标数据"));
			}
			else
			{
				LPCTSTR errMsg = _T("角点坐标数据读入失败！\n  --确认当前文件夹中包含 CornerData.yml 文件！ \n 或者\n  --执行棋盘角点检测。");
				throw errMsg;
			}
		} 
		// *** 从摄像头或本地图片检测棋盘角点
		else		
		{
			// 若选择从本地图片读入定标图像，首先弹出对话框选择图片文件
			if(optCalib.readLocalImage)
			{
				img0Files = DoSelectFiles(
					_T("*.bmp"), 
					OFN_ENABLESIZING   |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
					_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
					_T("选择左视图文件"),
					_T("Imgs\\Left"));

				if (optCalib.doStereoCalib)
				{
					img1Files = DoSelectFiles(
						_T("*.bmp"), 
						OFN_ENABLESIZING   |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
						_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
						_T("选择右视图文件"),
						_T("Imgs\\Right"));
				}

				if( img0Files.empty() )	// 判断是否获得图片
				{
					LPCTSTR errMsg = _T("没有选择到有效的图像文件，请重新选择!");
					throw errMsg;
				}

				img0_file = img0Files[ 0 ];
				img0 = cvLoadImage(img0_file);
				imageSize = img0.size();

				if (false == img1Files.empty())
				{
					optCalib.numberBoards = MIN(optCalib.numberBoards, MIN(img0Files.size(), img1Files.size()));
				}
				else
				{
					optCalib.doStereoCalib = false;
					optCalib.numberBoards = MIN(optCalib.numberBoards, img0Files.size());
				}
			} 
			else
			{
				lfCam >> img0;
				imageSize = img0.size();
			}

			////////////////////////////////////////////////////////////////////////
			// 初始化角点数据集
			m_stereoCalibrator.initCornerData(optCalib.numberBoards, imageSize, optCalib.cornerSize, optCalib.squareSize, cornerDatas);

			// 启用退出摄像机定标按钮
			GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(TRUE);

			// 开始检测角点
			MSG msg;
			while(nFoundBoard < optCalib.numberBoards)
			{
				// MFC 窗口消息处理
				if(::PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE)) 
				{ 
					if(msg.message == WM_QUIT) 
					{ 
						LPCTSTR errMsg = _T("手动退出双目标定!");
						throw errMsg; 
					}	

					//// 需要 #include "Strsafe.h"
					//TCHAR info[50];
					//StringCbPrintf( info, sizeof(info), _T("消息序号 = %d\n"), msg.message );
					//OutputDebugString(info);

					::TranslateMessage(&msg); 
					::DispatchMessage(&msg);
				}

				// 载入图像
				if(optCalib.readLocalImage)	// 从本地图片
				{
					img0_file = img0Files[ nFoundBoard ];
					frame0 = cvLoadImage(img0_file);
					if (optCalib.doStereoCalib)
					{
						img1_file = img1Files[ nFoundBoard ];
						frame1 = cvLoadImage(img1_file);
					}
				} 
				else	// 从摄像头
				{
					lfCam >> frame0;
					if (optCalib.doStereoCalib) 
						riCam >> frame1;
				}				
				if ( frame0.empty())	break;
				if (optCalib.doStereoCalib)
				{
					if ( frame1.empty())	break;
				}

				// 复制图像
				img0 = frame0.clone();
				if (optCalib.doStereoCalib) img1 = frame1.clone();

				// 检测角点
				if ( m_stereoCalibrator.detectCorners(img0, img1, cornerDatas, nFoundBoard) )
				{
					if (nFrame++ > optCalib.numberFrameSkip)
					{
						//保存视图
						if (optCalib.readLocalImage == false)
						{
							CStringA imgName( m_workDir );
							imgName.AppendFormat("Imgs\\left%03d.bmp",nFoundBoard);
							string imgname = imgName.GetBuffer(0);
							imwrite(imgname, frame0);

							if (optCalib.doStereoCalib) 
							{
								imgName = m_workDir;
								imgName.AppendFormat("Imgs\\right%03d.bmp",nFoundBoard);
								imgname = imgName.GetBuffer(0);
								imwrite(imgname, frame1);
							}
							imgName.ReleaseBuffer();
						}

						nFoundBoard++; 
						nFrame = 0;
					}

					if (nFoundBoard > 0 && nFrame < 5)
					{
						//使图像反色，表明同时找到完整的棋盘角点
						bitwise_not(img0, img0);
						if (optCalib.doStereoCalib) bitwise_not(img1, img1);
					}
				} // --End "if ( m_stereoCalibrator.detectCorners )"

				// 显示检测到的角点
				F_ShowImage( img0, m_lfImage, IDC_PicLfCam );
				if (optCalib.doStereoCalib) F_ShowImage( img1, m_riImage, IDC_PicRiCam ); 
			} // --End "while(nFoundBoard < optCalib.numberBoards) "

			if (nFoundBoard < 4)
			{
				LPCTSTR errMsg = _T("检测成功的棋盘图像数小于4个，退出双目标定!");
				throw errMsg;
			}
			else if (nFoundBoard != optCalib.numberBoards)
			{
				optCalib.numberBoards = nFoundBoard;
				m_stereoCalibrator.resizeCornerData(optCalib.numberBoards, cornerDatas);
			}

			// 保存角点坐标数据
			m_stereoCalibrator.saveCornerData(cornerFile, cornerDatas);

			AfxMessageBox(_T("棋盘角点检测结束"));
		} // --End "if (optCalib.loadConerDatas)"
		// 已获取角点坐标数据

		// 禁用退出摄像机定标按钮
		GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);

		////////////////////////////////////////////////////////////////////////
		// 是否读入已标定好的摄像头内参？
		if (optCalib.calibOrder == CALIB_LOAD_CAMERA_PARAMS)
		{		
			filePath = m_workDir;
			filePath.AppendFormat("cameraParams_left.yml");
			if (0 == m_stereoCalibrator.loadCameraParams(filePath.GetBuffer(0), stereoParams.cameraParams1))
			{
				LPCTSTR errMsg = _T("读入摄像头内参失败，找不到 cameraParams_left.yml 文件!");
				throw errMsg;
			}

			if (optCalib.doStereoCalib) 
			{
				filePath = m_workDir;
				filePath.AppendFormat("cameraParams_right.yml");
				if (0 == m_stereoCalibrator.loadCameraParams(filePath.GetBuffer(0), stereoParams.cameraParams2))
				{
					LPCTSTR errMsg = _T("读入摄像头内参失败，找不到 cameraParams_right.yml 文件!");
					throw errMsg;
				}
			}

			// 显示已读入已标定好的摄像头内参
			AfxMessageBox(_T("已读入标定好的摄像头内参"));
		}	

		// 进行摄像头标定
		if (optCalib.doStereoCalib)	// 双目标定和校正
		{
			stereoParams.cameraParams1.flags = optCalib.flagCameraCalib;
			stereoParams.cameraParams2.flags = optCalib.flagCameraCalib;
			stereoParams.flags = optCalib.flagStereoCalib;
			stereoParams.alpha = optCalib.alpha;

			bool needCalibSingleCamera = (CALIB_SINGLE_CAMERA_FIRST == optCalib.calibOrder);
			m_stereoCalibrator.calibrateStereoCamera(cornerDatas, stereoParams, needCalibSingleCamera);

			// 计算标定误差
			double avgErr = 0;
			m_stereoCalibrator.getStereoCalibrateError(cornerDatas, stereoParams, avgErr);
			char info[50];
			sprintf_s( info, "标定误差 = %7.4g", avgErr );
			CString ss;
			ss = info;
			AfxMessageBox(ss);

			// 执行双目校正
			m_stereoCalibrator.rectifyStereoCamera(cornerDatas, stereoParams, remapMatrixs, optCalib.rectifyMethod);

			AfxMessageBox(_T("已完成双目校正"));

			// 保存摄像头定标参数	
			filePath = m_workDir;
			filePath.AppendFormat("calib_paras.xml");
			m_stereoCalibrator.saveCalibrationDatas(filePath.GetBuffer(0), optCalib.rectifyMethod, cornerDatas, stereoParams, remapMatrixs);

			AfxMessageBox(_T("已保存定标参数"));

		} 
		else // 单目标定
		{
			StereoCalib::CameraParams cameraParams;
			cameraParams.flags = optCalib.flagCameraCalib;

			// 执行单目定标
			m_stereoCalibrator.calibrateSingleCamera(cornerDatas, cameraParams);

			// 保存定标参数
			filePath = m_workDir;
			filePath.AppendFormat("cameraParams.yml");
			m_stereoCalibrator.saveCameraParams(cameraParams, filePath.GetBuffer(0));

			// 计算标定误差
			double avgErr = 0;
			m_stereoCalibrator.getCameraCalibrateError(cornerDatas.objectPoints, cornerDatas.imagePoints1, cameraParams, avgErr);
			char info[50];
			sprintf_s( info, "已保存定标参数，标定误差 = %7.4g", avgErr );
			CString ss;
			ss = info;
			AfxMessageBox(ss);

			// 执行单目校正
			m_stereoCalibrator.rectifySingleCamera(cameraParams, remapMatrixs);

		}

		//////////////////////////////////////////////////////////////////////////
		// 显示标定效果
		{
			// 启用退出摄像机定标按钮
			GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(TRUE);

			MSG msg;
			while (true)
			{
				// MFC 窗口消息处理
				if(::PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE)) 
				{ 
					if(msg.message == WM_QUIT) 
					{ 
						break; 
					}	
					::TranslateMessage(&msg); 
					::DispatchMessage(&msg);
				}

				// 载入图像
				lfCam >> frame0;
				if (optCalib.doStereoCalib) 
					riCam >> frame1;			
				if ( frame0.empty())	break;
				if (optCalib.doStereoCalib)
				{
					if ( frame1.empty())	break;
				}

				// 复制图像
				img0 = frame0.clone();
				if (optCalib.doStereoCalib) 
					img1 = frame1.clone();

				// 校正图像
				UpdateData(FALSE);
				m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ShowCalibResult);
				bool showCalibrated = m_pCheck->GetCheck() > 0;
				if ( showCalibrated )
					m_stereoCalibrator.remapImage(img0, img1, img0, img1, remapMatrixs);

				// 显示校正效果
				if (optCalib.doStereoCalib) 
				{
					// 画出等距的若干条横线，以比对 行对准 情况
					for( int j = 0; j < img0.rows; j += 32 )		
					{
						line( img0, cvPoint(0,j),	cvPoint(img0.cols,j), CV_RGB(0,255,0)); 
						line( img1, cvPoint(0,j),	cvPoint(img1.cols,j), CV_RGB(0,255,0)); 
					}

					F_ShowImage(img0, m_lfImage, IDC_PicLfCam);
					F_ShowImage(img1, m_riImage, IDC_PicRiCam);
				}
				else
				{
					m_stereoCalibrator.showText(img0, "Distort");
					F_ShowImage(img0, m_lfImage, IDC_PicLfCam);
				}
			}
		}

		filePath.ReleaseBuffer();
	}
	//////////////////////////////////////////////////////////////////////////
	// 错误信息处理
	catch (LPCTSTR errMsg)
	{
		AfxMessageBox(errMsg);
	}
	catch (cv::Exception& e)
	{
		char err[2048];
		sprintf_s(err, "发生错误: %s", e.what());
		CStringW errInfo(err);
		AfxMessageBox(errInfo);
	}
	catch (...)
	{
		AfxMessageBox(_T("发生未知错误！"));
	}

	// 禁用退出摄像机定标按钮
	GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////	
	if(lfCam.isOpened() || riCam.isOpened())
	{
		// 定标结束，恢复双目定标按钮
		GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(TRUE);
//		GetDlgItem(IDC_CBN2MethodList)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BN_CompDisp)->EnableWindow(TRUE);

		// 恢复正常显示
		DoShowOrigFrame();		
	}

	return;
}

bool CbinocularCameraDlg::DoParseOptionsOfCameraCalib(OptionCameraCalib& opt)
{
	// 摄像头定标变量初始化
	opt.squareSize = 30.0f;
	opt.flagCameraCalib = CV_CALIB_FIX_K3;
	opt.flagStereoCalib = 0;
	opt.numberFrameSkip = 30;
	opt.doStereoCalib = true;

	UINT nCheckIDs[9] = { IDC_CHK1FPP, IDC_CHK2UIG, IDC_CHK3FAR, IDC_CHK4ZTD,
		IDC_CHK1FPP2, IDC_CHK2UIG2, IDC_CHK3FAR2, IDC_CHK4SFL, IDC_CHK5FI };
	int nFlagDefs1[4] = { CV_CALIB_FIX_PRINCIPAL_POINT, CV_CALIB_USE_INTRINSIC_GUESS,
		CV_CALIB_FIX_ASPECT_RATIO, CV_CALIB_ZERO_TANGENT_DIST };
	int nFlagDefs2[5] = { CV_CALIB_FIX_PRINCIPAL_POINT, CV_CALIB_USE_INTRINSIC_GUESS,
		CV_CALIB_FIX_ASPECT_RATIO, CV_CALIB_SAME_FOCAL_LENGTH, CV_CALIB_FIX_INTRINSIC };

	// 读入MFC界面的棋盘参数设定
	bool res = UpdateData(TRUE);
	if (!res)
		return false;

	opt.cornerSize.width = m_nCornerSize_X; 	// 棋盘角点数 (nx, ny)
	opt.cornerSize.height = m_nCornerSize_Y;
	opt.numberBoards = m_nBoards;				// 检测棋盘次数
	opt.squareSize = m_nSquareSize; 			// 棋盘方块大小

	// 确认棋盘角点坐标数据的获取方式
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ReadCornerData);
	opt.loadConerDatas = m_pCheck->GetCheck() > 0;	

	// 确认左右视图的来源（摄像头 or 本地图片）
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_CalibFromCam, IDC_RAD_CalibFromImg);
	opt.readLocalImage = m_nID_RAD == IDC_RAD_CalibFromImg;
	if(opt.readLocalImage)		// 若从本地图片读入，则跳帧数减少为 5 帧
		opt.numberFrameSkip = 5;

	// 确认双目定标的次序
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_Load1CamCalibResult, IDC_RAD_StereoCalib);
	opt.calibOrder = m_nID_RAD == IDC_RAD_Load1CamCalibResult ? CALIB_LOAD_CAMERA_PARAMS : 
		m_nID_RAD == IDC_RAD_Calib1CamFirst ? CALIB_SINGLE_CAMERA_FIRST : 
		m_nID_RAD == IDC_RAD_StereoCalib ? CALIB_STEREO_CAMERAS_DIRECTLY : 	CALIB_SINGLE_CAMERA_FIRST;	
	if (m_nCamCount == 1)	//摄像头只有一个时，不能进行双目定标
	{
		opt.doStereoCalib = false;
		opt.calibOrder = CALIB_SINGLE_CAMERA_FIRST;
		m_pCheck = (CButton*)GetDlgItem(IDC_RAD_Calib1CamFirst);
		m_pCheck->SetCheck(1);
		GetDlgItem(IDC_RAD_StereoCalib)->EnableWindow(FALSE);
	}

	// 确认双目校正算法
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_BOUGUET, IDC_RAD_HARTLEY);
	opt.rectifyMethod = m_nID_RAD == IDC_RAD_BOUGUET ? StereoCalib::RECTIFY_BOUGUET : StereoCalib::RECTIFY_HARTLEY;

	// 确定双目校正缩放系数
	if (m_dAlpha < 0 || m_dAlpha > 1)
		m_dAlpha = -1;
	opt.alpha = m_dAlpha;

	// 读入定标参数的设置
	int i;
	for (i=0; i<4; i++)
	{
		m_pCheck = (CButton*)GetDlgItem(nCheckIDs[i]);
		if(m_pCheck->GetCheck())
			opt.flagCameraCalib |= nFlagDefs1[i];
	}
	for (i=4; i<9; i++)
	{
		m_pCheck = (CButton*)GetDlgItem(nCheckIDs[i]);
		if(m_pCheck->GetCheck())
			opt.flagStereoCalib |= nFlagDefs2[i-4];
	}

	res = UpdateData(FALSE);

	return res;
}

vector<CStringA> CbinocularCameraDlg::DoSelectFiles(LPCTSTR lpszDefExt, DWORD dwFlags, LPCTSTR lpszFilter, LPCWSTR lpstrTitle, LPCWSTR lpstrInitialDir)
{
	vector<CStringA> selectedFiles;
	POSITION filePosition;
	DWORD MAXFILE = 4000;  
	TCHAR* pc = new TCHAR[MAXFILE];  

	CFileDialog dlg( TRUE, lpszDefExt, NULL, dwFlags, lpszFilter, NULL );	

	dlg.m_ofn.nMaxFile = MAXFILE; 
	dlg.m_ofn.lpstrFile = pc;   
	dlg.m_ofn.lpstrFile[0] = NULL; 
	dlg.m_ofn.lpstrTitle = lpstrTitle;
	dlg.m_ofn.lpstrInitialDir = lpstrInitialDir;

	if( dlg.DoModal() == IDOK )
	{
		filePosition = dlg.GetStartPosition();
		while(filePosition != NULL)   
		{   
			CStringA path;
			path = dlg.GetNextPathName(filePosition);
			selectedFiles.push_back( path );  
		}  
	}

	delete []pc;
	return selectedFiles;
}


void CbinocularCameraDlg::OnBnClickedBnExitcameracalib()
{
	// TODO: 在此添加控件通知处理程序代码
	// 发送退出消息
	::PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
	// 禁用停止角点检测按钮
	GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);
}
