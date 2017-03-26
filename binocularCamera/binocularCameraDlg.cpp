
// binocularCameraDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "binocularCamera.h"
#include "binocularCameraDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbinocularCameraDlg �Ի���




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


// CbinocularCameraDlg ��Ϣ�������

BOOL CbinocularCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// ��ȡ����ͷ��Ŀ
	m_nCamCount = CCameraDS::CameraCount();
	if( m_nCamCount < 1 )
	{
		AfxMessageBox(_T("���������1������ͷ��"));
		//return -1;
	}

	// ����Ͽ�CamList���������ͷ���Ƶ��ַ���
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
			AfxMessageBox(_T("���ܻ�ȡ����ͷ������"));
	}
	camstr.ReleaseBuffer();
	if (m_nCamCount <= 1)
		m_ComboBoxCameraR.EnableWindow(FALSE);

	// ʹһϵ�а�ťʧЧ
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( FALSE );			// ��������ͷʧЧ
	GetDlgItem( IDC_BN2StopCam )->EnableWindow( FALSE );		// ֹͣ����ͷʧЧ
	// ��ʼ��ͼ����ʾ�ؼ���ͼ��
	CRect rect;
	GetDlgItem(IDC_PicLfCam) ->GetClientRect( &rect );
	m_lfImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicRiCam) ->GetClientRect( &rect );
	m_riImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	
	CheckRadioButton(IDC_RAD_CalibFromCam, IDC_RAD_CalibFromImg, IDC_RAD_CalibFromCam);
	CheckRadioButton(IDC_RAD_Load1CamCalibResult, IDC_RAD_StereoCalib, IDC_RAD_Calib1CamFirst);
	CheckRadioButton(IDC_RAD_BOUGUET, IDC_RAD_HARTLEY, IDC_RAD_BOUGUET);				// Ĭ��ʹ��BOUGUET����˫ĿУ��

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
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CbinocularCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CDialog::UpdateWindow();				// ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		if (!m_lfImage.empty())
		{
			F_ShowImage( m_lfImage, m_lfImage, IDC_PicLfCam );		// �ػ�ͼƬ����
			F_ShowImage( m_riImage, m_riImage, IDC_PicRiCam );		// �ػ�ͼƬ����
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CbinocularCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CbinocularCameraDlg::OnCbnSelchangeComboCameral()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ȷ����������ͷ��Ӧ���豸��ţ���Ӧ�����˵�ѡ�������ţ�
	m_lfCamID = m_ComboBoxCameraL.GetCurSel();

	// ʹ��������ͷ��ť��Ч
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( TRUE );

	return;
}


void CbinocularCameraDlg::OnCbnSelchangeComboCamerar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ȷ����������ͷ��Ӧ���豸��ţ���Ӧ�����˵�ѡ�������ţ�
	m_riCamID = m_ComboBoxCameraR.GetCurSel();

	// ʹ��������ͷ��ť��Ч
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( TRUE );

	return;
}


void CbinocularCameraDlg::OnCbnSelchangeCbnResolution()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nImageWidth * m_nImageHeight * m_nImageChannels == 0)
	{
		AfxMessageBox(_T("��ѡ������ͷ����ķֱ��ʣ�"));
		return;
	}
	if (m_nCamCount > 0)
	{
		//�򿪵�һ������ͷ
		//if( ! lfCam.OpenCamera(m_riCamID, false, m_nImageWidth, m_nImageHeight) ) //����������ѡ�񴰿ڣ��ô����ƶ�ͼ���͸�
		if ( !lfCam.open(m_lfCamID) )
		{
			AfxMessageBox(_T("��������ͷʧ��."));
			return;
		}
		lfCam.set(CV_CAP_PROP_FRAME_WIDTH,  m_nImageWidth);
		lfCam.set(CV_CAP_PROP_FRAME_HEIGHT, m_nImageHeight);
	}
	if (m_nCamCount > 1)
	{
		if (m_lfCamID == m_riCamID)
		{
			AfxMessageBox(_T("��������ͷ���豸��Ų�����ͬ��"));
			return;
		}
		//�򿪵ڶ�������ͷ
		//if( ! riCam.OpenCamera(m_lfCamID, false, m_nImageWidth, m_nImageHeight) ) 
		if ( !riCam.open(m_riCamID) )
		{
			AfxMessageBox(_T("��������ͷʧ��."));
			return;
		}
		riCam.set(CV_CAP_PROP_FRAME_WIDTH,  m_nImageWidth);
		riCam.set(CV_CAP_PROP_FRAME_HEIGHT, m_nImageHeight);

		// GetDlgItem( IDC_BN_CompDisp )->EnableWindow( TRUE );
	}

	// ʹ���ְ�ť��Ч
	GetDlgItem( IDC_BN2StopCam )->EnableWindow( TRUE );
	GetDlgItem( IDC_BN2StereoCalib)->EnableWindow( TRUE );
	// ʹ���ְ�ťʧЧ
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( FALSE );
	GetDlgItem( IDC_COMBO_CAMERAL )->EnableWindow( FALSE );
	GetDlgItem( IDC_COMBO_CAMERAR )->EnableWindow( FALSE );
	GetDlgItem( IDC_CBN_Resolution )->EnableWindow( FALSE );

	// ��������ͷ����ʾʵʱ����
	DoShowOrigFrame();

	return;
}

void CbinocularCameraDlg::DoShowOrigFrame(void)
{
	// m_CBNMethodList.SetCurSel(0);
	// m_ProcMethod = SHOW_ORIGINAL_FRAME;
	SetTimer(1, 50, NULL);	// 50ms ��ʱ��ʾ
}


void CbinocularCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// ���㽫ͼƬ���ŵ� Image ��������ı�������
	double wRatio = des.cols / (double)src.cols;
	double hRatio = des.rows / (double)src.rows;
	double srcWH = src.cols / (double)src.rows;
	double desWH = des.cols / (double)des.rows;
	double scale = srcWH > desWH ? wRatio : hRatio;

	// ���ź�ͼƬ�Ŀ�͸�
	int nw = (int)( src.cols * scale );
	int nh = (int)( src.rows * scale );

	// Ϊ�˽����ź��ͼƬ���� des �����в�λ�������ͼƬ�� des ���Ͻǵ���������ֵ
	int tlx = (int)((des.cols - nw) / 2);
	int tly = (int)((des.rows - nh) / 2);

	// ���� des �� ROI ������������ͼƬ img
	Mat desRoi = des(Rect(tlx, tly, nw, nh));

	// ���src�ǵ�ͨ��ͼ����ת��Ϊ��ͨ��ͼ��
	if (src.channels() == 1)
	{
		Mat src_c;
		cvtColor(src, src_c, CV_GRAY2BGR);
		// ��ͼƬ src_t �������ţ������뵽 des ��
		resize(src_c, desRoi, desRoi.size());
	}
	else
	{
		// ��ͼƬ src �������ţ������뵽 des ��
		resize( src, desRoi, desRoi.size() );
	}

	CDC* pDC = GetDlgItem( ID ) ->GetDC();		// �����ʾ�ؼ��� DC
	HDC hDC = pDC ->GetSafeHdc();				// ��ȡ HDC(�豸���) �����л�ͼ����
	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );	// ��ȡ�ؼ��ߴ�λ��
	CvvImage cimg;
	IplImage cpy = des;
	cimg.CopyOf( &cpy );						// ����ͼƬ
	cimg.DrawToHDC( hDC, &rect );				// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
}


void CbinocularCameraDlg::OnBnClickedBn2stopcam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (lfCam.isOpened())
	{
		GetDlgItem( IDC_BN2StopCam )->EnableWindow( FALSE );
		KillTimer(1);

		// ��ͼ����������
		m_lfImage = Scalar(0);
		F_ShowImage( m_lfImage, m_lfImage, IDC_PicLfCam );
		lfCam.release();
		if (riCam.isOpened())
		{
			m_riImage = Scalar(0);
			F_ShowImage( m_riImage, m_riImage, IDC_PicRiCam );
			riCam.release();
		}

		// ʹ��������ͷ��ť������ͷѡ���б���Ч
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (lfCam.isOpened() || riCam.isOpened())
	{
		AfxMessageBox(_T("���ȹر�����ͷ��"));
		return;
	}

	CDialogEx::OnClose();
}


void CbinocularCameraDlg::OnBnClickedBn2stereocalib()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//ȷ���������������ѡ��
	OptionCameraCalib optCalib;
	if (!DoParseOptionsOfCameraCalib(optCalib))
		return;

	KillTimer(1);
	GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(FALSE);
//	GetDlgItem(IDC_CBN2MethodList)->EnableWindow(FALSE);
//	GetDlgItem(IDC_BN_CompDisp)->EnableWindow(FALSE);

	// ���̼�������ʼ��
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
		// ����������̽ǵ�
		// *** �ӱ����ļ�����ǵ���������
		CStringA filePath(m_workDir);
		filePath.AppendFormat("CornerData.yml");
		const char* cornerFile = filePath.GetBuffer(0);
		if (optCalib.loadConerDatas)		
		{
			if ( m_stereoCalibrator.loadCornerData(cornerFile, cornerDatas) )
			{
				AfxMessageBox(_T("�Ѷ���ǵ���������"));
			}
			else
			{
				LPCTSTR errMsg = _T("�ǵ��������ݶ���ʧ�ܣ�\n  --ȷ�ϵ�ǰ�ļ����а��� CornerData.yml �ļ��� \n ����\n  --ִ�����̽ǵ��⡣");
				throw errMsg;
			}
		} 
		// *** ������ͷ�򱾵�ͼƬ������̽ǵ�
		else		
		{
			// ��ѡ��ӱ���ͼƬ���붨��ͼ�����ȵ����Ի���ѡ��ͼƬ�ļ�
			if(optCalib.readLocalImage)
			{
				img0Files = DoSelectFiles(
					_T("*.bmp"), 
					OFN_ENABLESIZING   |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
					_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
					_T("ѡ������ͼ�ļ�"),
					_T("Imgs\\Left"));

				if (optCalib.doStereoCalib)
				{
					img1Files = DoSelectFiles(
						_T("*.bmp"), 
						OFN_ENABLESIZING   |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
						_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
						_T("ѡ������ͼ�ļ�"),
						_T("Imgs\\Right"));
				}

				if( img0Files.empty() )	// �ж��Ƿ���ͼƬ
				{
					LPCTSTR errMsg = _T("û��ѡ����Ч��ͼ���ļ���������ѡ��!");
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
			// ��ʼ���ǵ����ݼ�
			m_stereoCalibrator.initCornerData(optCalib.numberBoards, imageSize, optCalib.cornerSize, optCalib.squareSize, cornerDatas);

			// �����˳���������갴ť
			GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(TRUE);

			// ��ʼ���ǵ�
			MSG msg;
			while(nFoundBoard < optCalib.numberBoards)
			{
				// MFC ������Ϣ����
				if(::PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE)) 
				{ 
					if(msg.message == WM_QUIT) 
					{ 
						LPCTSTR errMsg = _T("�ֶ��˳�˫Ŀ�궨!");
						throw errMsg; 
					}	

					//// ��Ҫ #include "Strsafe.h"
					//TCHAR info[50];
					//StringCbPrintf( info, sizeof(info), _T("��Ϣ��� = %d\n"), msg.message );
					//OutputDebugString(info);

					::TranslateMessage(&msg); 
					::DispatchMessage(&msg);
				}

				// ����ͼ��
				if(optCalib.readLocalImage)	// �ӱ���ͼƬ
				{
					img0_file = img0Files[ nFoundBoard ];
					frame0 = cvLoadImage(img0_file);
					if (optCalib.doStereoCalib)
					{
						img1_file = img1Files[ nFoundBoard ];
						frame1 = cvLoadImage(img1_file);
					}
				} 
				else	// ������ͷ
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

				// ����ͼ��
				img0 = frame0.clone();
				if (optCalib.doStereoCalib) img1 = frame1.clone();

				// ���ǵ�
				if ( m_stereoCalibrator.detectCorners(img0, img1, cornerDatas, nFoundBoard) )
				{
					if (nFrame++ > optCalib.numberFrameSkip)
					{
						//������ͼ
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
						//ʹͼ��ɫ������ͬʱ�ҵ����������̽ǵ�
						bitwise_not(img0, img0);
						if (optCalib.doStereoCalib) bitwise_not(img1, img1);
					}
				} // --End "if ( m_stereoCalibrator.detectCorners )"

				// ��ʾ��⵽�Ľǵ�
				F_ShowImage( img0, m_lfImage, IDC_PicLfCam );
				if (optCalib.doStereoCalib) F_ShowImage( img1, m_riImage, IDC_PicRiCam ); 
			} // --End "while(nFoundBoard < optCalib.numberBoards) "

			if (nFoundBoard < 4)
			{
				LPCTSTR errMsg = _T("���ɹ�������ͼ����С��4�����˳�˫Ŀ�궨!");
				throw errMsg;
			}
			else if (nFoundBoard != optCalib.numberBoards)
			{
				optCalib.numberBoards = nFoundBoard;
				m_stereoCalibrator.resizeCornerData(optCalib.numberBoards, cornerDatas);
			}

			// ����ǵ���������
			m_stereoCalibrator.saveCornerData(cornerFile, cornerDatas);

			AfxMessageBox(_T("���̽ǵ������"));
		} // --End "if (optCalib.loadConerDatas)"
		// �ѻ�ȡ�ǵ���������

		// �����˳���������갴ť
		GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);

		////////////////////////////////////////////////////////////////////////
		// �Ƿ�����ѱ궨�õ�����ͷ�ڲΣ�
		if (optCalib.calibOrder == CALIB_LOAD_CAMERA_PARAMS)
		{		
			filePath = m_workDir;
			filePath.AppendFormat("cameraParams_left.yml");
			if (0 == m_stereoCalibrator.loadCameraParams(filePath.GetBuffer(0), stereoParams.cameraParams1))
			{
				LPCTSTR errMsg = _T("��������ͷ�ڲ�ʧ�ܣ��Ҳ��� cameraParams_left.yml �ļ�!");
				throw errMsg;
			}

			if (optCalib.doStereoCalib) 
			{
				filePath = m_workDir;
				filePath.AppendFormat("cameraParams_right.yml");
				if (0 == m_stereoCalibrator.loadCameraParams(filePath.GetBuffer(0), stereoParams.cameraParams2))
				{
					LPCTSTR errMsg = _T("��������ͷ�ڲ�ʧ�ܣ��Ҳ��� cameraParams_right.yml �ļ�!");
					throw errMsg;
				}
			}

			// ��ʾ�Ѷ����ѱ궨�õ�����ͷ�ڲ�
			AfxMessageBox(_T("�Ѷ���궨�õ�����ͷ�ڲ�"));
		}	

		// ��������ͷ�궨
		if (optCalib.doStereoCalib)	// ˫Ŀ�궨��У��
		{
			stereoParams.cameraParams1.flags = optCalib.flagCameraCalib;
			stereoParams.cameraParams2.flags = optCalib.flagCameraCalib;
			stereoParams.flags = optCalib.flagStereoCalib;
			stereoParams.alpha = optCalib.alpha;

			bool needCalibSingleCamera = (CALIB_SINGLE_CAMERA_FIRST == optCalib.calibOrder);
			m_stereoCalibrator.calibrateStereoCamera(cornerDatas, stereoParams, needCalibSingleCamera);

			// ����궨���
			double avgErr = 0;
			m_stereoCalibrator.getStereoCalibrateError(cornerDatas, stereoParams, avgErr);
			char info[50];
			sprintf_s( info, "�궨��� = %7.4g", avgErr );
			CString ss;
			ss = info;
			AfxMessageBox(ss);

			// ִ��˫ĿУ��
			m_stereoCalibrator.rectifyStereoCamera(cornerDatas, stereoParams, remapMatrixs, optCalib.rectifyMethod);

			AfxMessageBox(_T("�����˫ĿУ��"));

			// ��������ͷ�������	
			filePath = m_workDir;
			filePath.AppendFormat("calib_paras.xml");
			m_stereoCalibrator.saveCalibrationDatas(filePath.GetBuffer(0), optCalib.rectifyMethod, cornerDatas, stereoParams, remapMatrixs);

			AfxMessageBox(_T("�ѱ��涨�����"));

		} 
		else // ��Ŀ�궨
		{
			StereoCalib::CameraParams cameraParams;
			cameraParams.flags = optCalib.flagCameraCalib;

			// ִ�е�Ŀ����
			m_stereoCalibrator.calibrateSingleCamera(cornerDatas, cameraParams);

			// ���涨�����
			filePath = m_workDir;
			filePath.AppendFormat("cameraParams.yml");
			m_stereoCalibrator.saveCameraParams(cameraParams, filePath.GetBuffer(0));

			// ����궨���
			double avgErr = 0;
			m_stereoCalibrator.getCameraCalibrateError(cornerDatas.objectPoints, cornerDatas.imagePoints1, cameraParams, avgErr);
			char info[50];
			sprintf_s( info, "�ѱ��涨��������궨��� = %7.4g", avgErr );
			CString ss;
			ss = info;
			AfxMessageBox(ss);

			// ִ�е�ĿУ��
			m_stereoCalibrator.rectifySingleCamera(cameraParams, remapMatrixs);

		}

		//////////////////////////////////////////////////////////////////////////
		// ��ʾ�궨Ч��
		{
			// �����˳���������갴ť
			GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(TRUE);

			MSG msg;
			while (true)
			{
				// MFC ������Ϣ����
				if(::PeekMessage(&msg, this->m_hWnd, 0, 0, PM_REMOVE)) 
				{ 
					if(msg.message == WM_QUIT) 
					{ 
						break; 
					}	
					::TranslateMessage(&msg); 
					::DispatchMessage(&msg);
				}

				// ����ͼ��
				lfCam >> frame0;
				if (optCalib.doStereoCalib) 
					riCam >> frame1;			
				if ( frame0.empty())	break;
				if (optCalib.doStereoCalib)
				{
					if ( frame1.empty())	break;
				}

				// ����ͼ��
				img0 = frame0.clone();
				if (optCalib.doStereoCalib) 
					img1 = frame1.clone();

				// У��ͼ��
				UpdateData(FALSE);
				m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ShowCalibResult);
				bool showCalibrated = m_pCheck->GetCheck() > 0;
				if ( showCalibrated )
					m_stereoCalibrator.remapImage(img0, img1, img0, img1, remapMatrixs);

				// ��ʾУ��Ч��
				if (optCalib.doStereoCalib) 
				{
					// �����Ⱦ�����������ߣ��Աȶ� �ж�׼ ���
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
	// ������Ϣ����
	catch (LPCTSTR errMsg)
	{
		AfxMessageBox(errMsg);
	}
	catch (cv::Exception& e)
	{
		char err[2048];
		sprintf_s(err, "��������: %s", e.what());
		CStringW errInfo(err);
		AfxMessageBox(errInfo);
	}
	catch (...)
	{
		AfxMessageBox(_T("����δ֪����"));
	}

	// �����˳���������갴ť
	GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////	
	if(lfCam.isOpened() || riCam.isOpened())
	{
		// ����������ָ�˫Ŀ���갴ť
		GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(TRUE);
//		GetDlgItem(IDC_CBN2MethodList)->EnableWindow(TRUE);
//		GetDlgItem(IDC_BN_CompDisp)->EnableWindow(TRUE);

		// �ָ�������ʾ
		DoShowOrigFrame();		
	}

	return;
}

bool CbinocularCameraDlg::DoParseOptionsOfCameraCalib(OptionCameraCalib& opt)
{
	// ����ͷ���������ʼ��
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

	// ����MFC��������̲����趨
	bool res = UpdateData(TRUE);
	if (!res)
		return false;

	opt.cornerSize.width = m_nCornerSize_X; 	// ���̽ǵ��� (nx, ny)
	opt.cornerSize.height = m_nCornerSize_Y;
	opt.numberBoards = m_nBoards;				// ������̴���
	opt.squareSize = m_nSquareSize; 			// ���̷����С

	// ȷ�����̽ǵ��������ݵĻ�ȡ��ʽ
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ReadCornerData);
	opt.loadConerDatas = m_pCheck->GetCheck() > 0;	

	// ȷ��������ͼ����Դ������ͷ or ����ͼƬ��
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_CalibFromCam, IDC_RAD_CalibFromImg);
	opt.readLocalImage = m_nID_RAD == IDC_RAD_CalibFromImg;
	if(opt.readLocalImage)		// ���ӱ���ͼƬ���룬����֡������Ϊ 5 ֡
		opt.numberFrameSkip = 5;

	// ȷ��˫Ŀ����Ĵ���
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_Load1CamCalibResult, IDC_RAD_StereoCalib);
	opt.calibOrder = m_nID_RAD == IDC_RAD_Load1CamCalibResult ? CALIB_LOAD_CAMERA_PARAMS : 
		m_nID_RAD == IDC_RAD_Calib1CamFirst ? CALIB_SINGLE_CAMERA_FIRST : 
		m_nID_RAD == IDC_RAD_StereoCalib ? CALIB_STEREO_CAMERAS_DIRECTLY : 	CALIB_SINGLE_CAMERA_FIRST;	
	if (m_nCamCount == 1)	//����ͷֻ��һ��ʱ�����ܽ���˫Ŀ����
	{
		opt.doStereoCalib = false;
		opt.calibOrder = CALIB_SINGLE_CAMERA_FIRST;
		m_pCheck = (CButton*)GetDlgItem(IDC_RAD_Calib1CamFirst);
		m_pCheck->SetCheck(1);
		GetDlgItem(IDC_RAD_StereoCalib)->EnableWindow(FALSE);
	}

	// ȷ��˫ĿУ���㷨
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_BOUGUET, IDC_RAD_HARTLEY);
	opt.rectifyMethod = m_nID_RAD == IDC_RAD_BOUGUET ? StereoCalib::RECTIFY_BOUGUET : StereoCalib::RECTIFY_HARTLEY;

	// ȷ��˫ĿУ������ϵ��
	if (m_dAlpha < 0 || m_dAlpha > 1)
		m_dAlpha = -1;
	opt.alpha = m_dAlpha;

	// ���붨�����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// �����˳���Ϣ
	::PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
	// ����ֹͣ�ǵ��ⰴť
	GetDlgItem(IDC_BN_ExitCameraCalib)->EnableWindow(FALSE);
}
