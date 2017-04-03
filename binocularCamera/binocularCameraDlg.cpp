
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
	, m_nMinDisp(0)
	, m_nMaxDisp(0)
	, m_nSADWinSiz(0)
	, m_nTextThres(0)
	, m_nDisp12MaxDiff(0)
	, m_nPreFiltCap(0)
	, m_nUniqRatio(0)
	, m_nSpeckRange(0)
	, m_nSpeckWinSiz(0)
	, m_bFullDP(FALSE)
	, m_nDelayTime(5)
	, m_bSaveFrame(FALSE)
	, m_nViewWidth(200)
	, m_nViewHeight(200)
	, m_nViewDepth(200)
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
	DDX_Text(pDX, IDC_EDIT_minDisp, m_nMinDisp);
	DDV_MinMaxInt(pDX, m_nMinDisp, -64, 16);
	DDX_Text(pDX, IDC_EDIT_maxDisp, m_nMaxDisp);
	DDV_MinMaxInt(pDX, m_nMaxDisp, 0, 240);
	DDX_Text(pDX, IDC_EDIT_SADWinSiz, m_nSADWinSiz);
	DDV_MinMaxInt(pDX, m_nSADWinSiz, 0, 21);
	DDX_Text(pDX, IDC_EDIT_textThres, m_nTextThres);
	DDX_Text(pDX, IDC_EDIT_disp12MaxDiff, m_nDisp12MaxDiff);
	DDV_MinMaxInt(pDX, m_nDisp12MaxDiff, -1, 16);
	DDX_Text(pDX, IDC_EDIT_preFiltCap, m_nPreFiltCap);
	DDV_MinMaxInt(pDX, m_nPreFiltCap, 0, 100);
	DDX_Text(pDX, IDC_EDIT_uniqRatio, m_nUniqRatio);
	DDV_MinMaxInt(pDX, m_nUniqRatio, 0, 50);
	DDX_Text(pDX, IDC_EDIT_speckRange, m_nSpeckRange);
	DDV_MinMaxInt(pDX, m_nSpeckRange, 0, 64);
	DDX_Text(pDX, IDC_EDIT_speckWinSiz, m_nSpeckWinSiz);
	DDV_MinMaxInt(pDX, m_nSpeckWinSiz, 0, 200);
	DDX_Control(pDX, IDC_SPIN_minDisp, m_spinMinDisp);
	DDX_Control(pDX, IDC_SPIN_maxDisp, m_spinMaxDisp);
	DDX_Control(pDX, IDC_SPIN_SADWinSiz, m_spinSADWinSiz);
	DDX_Control(pDX, IDC_SPIN_textThres, m_spinTextThres);
	DDX_Control(pDX, IDC_SPIN_disp12MaxDiff, m_spinDisp12MaxDiff);
	DDX_Control(pDX, IDC_SPIN_preFiltCap, m_spinPreFiltCap);
	DDX_Control(pDX, IDC_SPIN_uniqRatio, m_spinUniqRatio);
	DDX_Control(pDX, IDC_SPIN_speckRange, m_spinSpeckRange);
	DDX_Control(pDX, IDC_SPIN_speckWinSiz, m_spinSpeckWinSiz);
	DDX_Check(pDX, IDC_CHK_fullDP, m_bFullDP);
	DDX_Text(pDX, IDC_EDIT_DelayTime, m_nDelayTime);
	DDX_Check(pDX, IDC_CHK_SaveAsVideo, m_bSaveFrame);
	DDX_Text(pDX, IDC_EDIT_viewWidth, m_nViewWidth);
	DDX_Text(pDX, IDC_EDIT_viewHeight, m_nViewHeight);
	DDX_Text(pDX, IDC_EDIT_viewDepth, m_nViewDepth);
	DDV_MinMaxInt(pDX, m_nViewWidth, 60, 600);
	DDV_MinMaxInt(pDX, m_nViewHeight, 40, 400);
	DDV_MinMaxInt(pDX, m_nViewDepth, 60, 600);
	DDX_Control(pDX, IDC_SPIN_viewWidth, m_spinViewWidth);
	DDX_Control(pDX, IDC_SPIN_viewHeight, m_spinViewHeight);
	DDX_Control(pDX, IDC_SPIN_viewDepth, m_spinViewDepth);
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
	ON_BN_CLICKED(IDC_BN_StereoDefParam, &CbinocularCameraDlg::OnBnClickedBnStereodefparam)
	ON_BN_CLICKED(IDC_BN_CompDisp, &CbinocularCameraDlg::OnBnClickedBnCompdisp)
	ON_BN_CLICKED(IDC_BN_StopDispComp, &CbinocularCameraDlg::OnBnClickedBnStopdispcomp)
	ON_BN_CLICKED(IDC_RAD_BM, &CbinocularCameraDlg::OnBnClickedRadBm)
	ON_BN_CLICKED(IDC_RAD_SGBM, &CbinocularCameraDlg::OnBnClickedRadSgbm)
	ON_BN_CLICKED(IDC_BTN_DefaultViewField, &CbinocularCameraDlg::OnBnClickedBtnDefaultviewfield)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_maxDisp, &CbinocularCameraDlg::OnDeltaposSpinmaxdisp)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SADWinSiz, &CbinocularCameraDlg::OnDeltaposSpinSadwinsiz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_speckRange, &CbinocularCameraDlg::OnDeltaposSpinspeckrange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_viewWidth, &CbinocularCameraDlg::OnDeltaposSpinviewwidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_viewHeight, &CbinocularCameraDlg::OnDeltaposSpinviewheight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_viewDepth, &CbinocularCameraDlg::OnDeltaposSpinviewdepth)
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
	// ��ȡ������·��
	m_workDir = F_InitWorkDir();
	m_stereoCalibrator.setWorkDir(CStringA(m_workDir).GetBuffer(0));

	// Ĭ��ͼ������Ϊ��ʾԭʼͼ��
//	m_ProcMethod = SHOW_ORIGINAL_FRAME;

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

//	m_CBNResolution.SetCurSel(1);

	// ʹһϵ�а�ťʧЧ
	GetDlgItem( IDC_BN1RunCam )->EnableWindow( FALSE );			// ��������ͷʧЧ
	GetDlgItem( IDC_BN2StopCam )->EnableWindow( FALSE );		// ֹͣ����ͷʧЧ
	GetDlgItem(IDC_BN_StopDispComp)->EnableWindow(FALSE);		// ֹͣ�Ӳ����ʧЧ
	GetDlgItem(IDC_EDIT_disp12MaxDiff)->EnableWindow(FALSE);	// �����Ӳ�ͼ����ʧЧ

	// ���ñ궨��ز���
	CheckRadioButton(IDC_RAD_CalibFromCam, IDC_RAD_CalibFromImg, IDC_RAD_CalibFromCam);
	CheckRadioButton(IDC_RAD_Load1CamCalibResult, IDC_RAD_StereoCalib, IDC_RAD_Calib1CamFirst);

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
	// ��ʼ��˫Ŀƥ������ؼ�����
	CheckRadioButton(IDC_RAD_BOUGUET, IDC_RAD_HARTLEY, IDC_RAD_BOUGUET);				// Ĭ��ʹ��BOUGUET����˫ĿУ��
	CheckRadioButton(IDC_RAD_SGBM, IDC_RAD_BM, IDC_RAD_BM);								// Ĭ��ʹ�� BM �㷨�����Ӳ�
	CheckRadioButton(IDC_RAD_DispFromImg, IDC_RAD_DispFromCam, IDC_RAD_DispFromCam);	// Ĭ�ϴ��������ȡͼ��
	
	CheckRadioButton(IDC_RAD_ColorDisp, IDC_RAD_SideView, IDC_RAD_ColorDisp);			// Ĭ����ʾ��ɫ�Ӳ�ͼ
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_StereoRectify);								// Ĭ��ִ��˫ĿУ��
	m_pCheck->SetCheck(1);
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ProjTo3D);			// Ĭ��ִ��˫Ŀ���
	m_pCheck->SetCheck(1);
	m_pCheck = NULL;

	m_nViewWidth = 200;
	m_nViewHeight = 200;
	m_nViewDepth = 200;
	m_nMaxDisp = 0;
	m_nSADWinSiz =0;
	m_nPreFiltCap =0;
	m_nSpeckRange = 0;
	m_nDelayTime = 5;
	m_spinMinDisp.SetRange(-64, 16);
	m_spinMaxDisp.SetRange(0, 240);
	m_spinSADWinSiz.SetRange(0, 21);
	m_spinTextThres.SetRange(0, 50);
	m_spinDisp12MaxDiff.SetRange(-1, 36);
	m_spinPreFiltCap.SetRange(0, 100);
	m_spinUniqRatio.SetRange(0, 50);
	m_spinSpeckRange.SetRange(0, 64);
	m_spinSpeckWinSiz.SetRange(0, 200);
	m_spinViewWidth.SetRange(60, 600);
	m_spinViewHeight.SetRange(40, 400);
	m_spinViewDepth.SetRange(60, 600);

	UpdateData( FALSE );

	// ��ʼ��ͼ����ʾ�ؼ���ͼ��
	CRect rect;
	GetDlgItem(IDC_PicLfCam) ->GetClientRect( &rect );
	m_lfImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicRiCam) ->GetClientRect( &rect );
	m_riImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicSynImg) ->GetClientRect( &rect );
	m_disparity = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	
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
			F_ShowImage( m_disparity, m_disparity, IDC_PicSynImg );	// �ػ�ͼƬ����
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

		GetDlgItem( IDC_BN_CompDisp )->EnableWindow( TRUE );
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


void CbinocularCameraDlg::OnBnClickedBnStereodefparam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_SGBM, IDC_RAD_BM);
	if (m_nID_RAD == IDC_RAD_BM)
	{
		m_nMinDisp = 0;	
		m_nMaxDisp = 64;
		m_nSADWinSiz = 19;
		m_nTextThres = 10;
		m_nDisp12MaxDiff = -1;
		m_nPreFiltCap = 31;
		m_nUniqRatio = 25;
		m_nSpeckRange = 32;
		m_nSpeckWinSiz = 100;
	} 
	else if(m_nID_RAD == IDC_RAD_SGBM)
	{
		m_nMinDisp = 0;	
		m_nMaxDisp = 64;
		m_nSADWinSiz = 7;
		m_nDisp12MaxDiff = -1;
		m_nPreFiltCap = 63;
		m_nUniqRatio = 25;
		m_nSpeckRange = 32;
		m_nSpeckWinSiz = 100;
		m_pCheck = (CButton*)GetDlgItem(IDC_CHK_fullDP);
		m_pCheck->SetCheck(0);
	}
	UpdateData(FALSE);
}


void CbinocularCameraDlg::OnBnClickedBnCompdisp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��������е�����ƥ��ѡ������
	OptionStereoMatch optMatch;
	if (!DoParseOptionsOfStereoMatch( optMatch )) 
		return;

	// ��ͣ��ʾ����ͷ����Ĳ���
	KillTimer(1);			

	GetDlgItem(IDC_BN_StopDispComp)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_BM)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAD_SGBM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BN2StopCam)->EnableWindow(FALSE);
	GetDlgItem(IDC_BN_CompDisp)->EnableWindow(FALSE);
	GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(FALSE);
//	GetDlgItem(IDC_CBN2MethodList)->EnableWindow(FALSE);

	//////////////////////////////////////////////////////////////////////////
	// ������ʼ��
	CStringA xmlPath;			// �����������ļ�
	vector<CStringA> imgFiles1; //������ͼ�ļ�·������
	vector<CStringA> imgFiles2;	
	int i = 0, j = 0;
	const char* img1_filename = NULL;
	const char* img2_filename = NULL;
	const char* xml_filename = NULL;

	cv::Mat img1, img2, img1p, img2p, disp, disp8u, pointCloud;
	LPCTSTR errMsg;

	try 
	{
		//////////////////////////////////////////////////////////////////////////
		// ѡ��/Ԥ��ͼ������� or ����ͼƬ��
		if (optMatch.readLocalImage)
		{
			// ѡ������ͼ
			imgFiles1 = DoSelectFiles(
				_T("*.bmp"), 
				OFN_ENABLESIZING |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
				_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
				_T("ѡ������ͼͼ��"),
				m_workDir
				);
			// ѡ������ͼ
			imgFiles2 = DoSelectFiles(
				_T("*.bmp"), 
				OFN_ENABLESIZING |OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY,
				_T("image files (*.bmp; *.png; *.jpg) |*.bmp; *.png; *.jpg; *.jpeg| All Files (*.*) |*.*||"),
				_T("ѡ������ͼͼ��"),
				m_workDir
				);

			if( imgFiles1.empty() || imgFiles2.empty() )	
			{
				errMsg = _T("û��ѡ����Ч��ͼ���ļ�!");
				throw errMsg;
			}

			img1_filename = imgFiles1[0];
			img2_filename = imgFiles2[0];
			img1 = imread(img1_filename);
			img2 = imread(img2_filename);
		} 
		else
		{
			lfCam >> img1;
			riCam >> img2;
		}

		//////////////////////////////////////////////////////////////////////////
		// ��ȡ˫ĿУ���任����
		if( optMatch.useStereoRectify )
		{
			// ������ͷ��������ļ�
			vector<CStringA> xmlFiles;
			xmlFiles = DoSelectFiles(
				_T("*.xml"), 
				OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
				_T("XML/YML file (*.xml; *.yml) |*.xml; *.yml | All Files (*.*) |*.*||"), 
				_T("ѡ������ͷ��������ļ�"),
				m_workDir
				);
			if( xmlFiles.empty() )	
			{
				errMsg = _T("û��ѡ����Ч������ͷ��������ļ�!");			
				throw errMsg;
			}
			xmlPath = xmlFiles[0];			// ��ȡxml�ļ�·��
			xml_filename = xmlPath;

			// ��������ͷ�������
			switch (m_stereoMatcher.init(img1.cols, img1.rows, xml_filename))
			{
			case 0:
			case -99:
				errMsg = _T("��ȡ����ͷ��������ļ�ʧ�ܣ�������ѡ��!");			
				throw errMsg;
			case -1:
				errMsg = _T("���������ͼ��ߴ��뵱ǰ���õ�ͼ��ߴ粻һ�£�������ѡ��!");			
				throw errMsg;
			case -2:
				if (optMatch.generatePointCloud)
				{
					optMatch.generatePointCloud = false;
					errMsg = _T("�����ļ���Ӧ��У���������� BOUGUET �������޷�������ά����!");			
					throw errMsg;
				}
				break;
			}

		}	
		else
			m_stereoMatcher.init(img1.cols, img1.rows, "");

		//////////////////////////////////////////////////////////////////////////
		// ��ʼ����ͼ���Ӳ�
		m_stereoMatcher.m_BM.state->disp12MaxDiff = m_nDisp12MaxDiff;
		m_stereoMatcher.m_SGBM.disp12MaxDiff = m_nDisp12MaxDiff;

		int frameCount = 0;	//ͼ�����
		PointCloudAnalyzer pointCloudAnalyzer;
		clock_t tShowObjInfo = clock(), tStart, tPerFrame;
		CStringA strPerFrame;
		MSG msg;
		while(true)
		{
			tStart = clock();

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
			if (optMatch.readLocalImage)
			{
				if (frameCount >= MIN(imgFiles1.size(), imgFiles2.size()))
					break;
				img1_filename = imgFiles1[frameCount];
				img2_filename = imgFiles2[frameCount];
				img1 = imread(img1_filename, 1);
				img2 = imread(img2_filename, 1);
				if(img1.empty() || img2.empty()) break;
				frameCount++;
			} 
			else
			{
				lfCam >> img1;
				riCam >> img2;
				if(img1.empty()) break;
			}

			/*
			if (m_ProcMethod)
			{
				DoFrameProc(img1, img1);
				DoFrameProc(img2, img2);
			}
*/ 
			// ִ��˫Ŀƥ��
			if (optMatch.stereoMethod == STEREO_BM)
			{
				DoUpdateStateBM();
				m_stereoMatcher.bmMatch(img1, img2, disp, img1p, img2p);
			} 
			else if (optMatch.stereoMethod == STEREO_SGBM)
			{
				DoUpdateStateSGBM(img1.channels());
				m_stereoMatcher.sgbmMatch(img1, img2, disp, img1p, img2p);
			}

			// �����Ⱦ�����������ߣ��Աȶ� �ж�׼ ���
			for( j = 0; j < img1p.rows; j += 32 )		
			{
				line( img1p, cvPoint(0,j),	cvPoint(img1p.cols,j), CV_RGB(0,255,0)); 
				line( img2p, cvPoint(0,j),	cvPoint(img2p.cols,j), CV_RGB(0,255,0)); 
			}

			// ���������ͷ��������壿
			if ( optMatch.generatePointCloud )
			{
				m_stereoMatcher.getPointClouds(disp, pointCloud);

				vector<PointCloudAnalyzer::ObjectInfo> objectInfos;
				pointCloudAnalyzer.detectNearObject(img1p, pointCloud, objectInfos);

				if (!objectInfos.empty() && (clock()-tShowObjInfo) > 500)
				{
					tShowObjInfo = clock();
					double fl = m_stereoMatcher.m_FL;
					m_ObjectDistance = objectInfos[0].distance; m_ObjectDistance = (int)(m_ObjectDistance * 10000) / 10000.;
					m_ObjectHeight = objectInfos[0].boundRect.height * objectInfos[0].distance / fl; m_ObjectHeight = (int)(m_ObjectHeight * 10000) / 10000.;
					m_ObjectWidth = objectInfos[0].boundRect.width * objectInfos[0].distance / fl; m_ObjectWidth = (int)(m_ObjectWidth * 10000) / 10000.;
					m_ObjectDisparity = disp.at<short>(objectInfos[0].nearest) / 16.;
					UpdateData(FALSE);
				}
			}

			tPerFrame = clock() - tStart;
			strPerFrame.Format("��ÿ֡ %d ms��", tPerFrame);

			// �л��Ӳ�ͼ/�ӳ�ͼ
			UpdateData(TRUE);
			m_stereoMatcher.setViewField(m_nViewWidth, m_nViewHeight, m_nViewDepth);
			m_nID_RAD = GetCheckedRadioButton(IDC_RAD_ColorDisp, IDC_RAD_SideView);
			switch ( m_nID_RAD )
			{
			case IDC_RAD_ColorDisp:
				m_stereoMatcher.getDisparityImage(disp, disp8u, true);
				GetDlgItem(IDC_STATIC_DispView)->SetWindowText(_T("��ɫ�Ӳ�ͼ") + CStringW(strPerFrame));
				break;
			case IDC_RAD_GrayDisp:
				m_stereoMatcher.getDisparityImage(disp, disp8u, false);
				GetDlgItem(IDC_STATIC_DispView)->SetWindowText(_T("�Ҷ��Ӳ�ͼ") + CStringW(strPerFrame));
				break;
			case IDC_RAD_TopDownView:
				if ( optMatch.generatePointCloud )
				{
					m_stereoMatcher.getTopDownView(pointCloud, disp8u, img1);
					GetDlgItem(IDC_STATIC_DispView)->SetWindowText(_T("��������ͼ") + CStringW(strPerFrame));
				}
				else
				{
					CheckRadioButton(IDC_RAD_ColorDisp, IDC_RAD_SideView, IDC_RAD_ColorDisp);			// Ĭ����ʾ��ɫ�Ӳ�ͼ
					m_stereoMatcher.getDisparityImage(disp, disp8u, true);
				}
				break;
			case IDC_RAD_SideView:
				if ( optMatch.generatePointCloud )
				{
					m_stereoMatcher.getSideView(pointCloud, disp8u, img1);
					GetDlgItem(IDC_STATIC_DispView)->SetWindowText(_T("��������ͼ") + CStringW(strPerFrame));
				}
				else
				{
					CheckRadioButton(IDC_RAD_ColorDisp, IDC_RAD_SideView, IDC_RAD_ColorDisp);			// Ĭ����ʾ��ɫ�Ӳ�ͼ
					m_stereoMatcher.getDisparityImage(disp, disp8u, true);
				}
				break;
			default:
				m_stereoMatcher.getDisparityImage(disp, disp8u, true);
			}            
			UpdateData(FALSE);

			// ������MFC������ʾ
			F_ShowImage( img1p, m_lfImage, IDC_PicLfCam );
			F_ShowImage( img2p, m_riImage, IDC_PicRiCam );
			F_ShowImage( disp8u, m_disparity, IDC_PicSynImg );	

			// ���滭����Ӳ�ͼ��
			if( optMatch.saveResults )
				F_Saveframe( img1, img2, disp);

			if( optMatch.delayEachFrame )
				Sleep(m_nDelayTime * 1000);
		}

		// �������һ֡����ͼ��ĵ�������
		if ( optMatch.generatePointCloud )
		{
			CStringA filePath(m_workDir);
			filePath.AppendFormat("PointsClouds.txt");
			m_stereoMatcher.savePointClouds(pointCloud, filePath.GetBuffer(0));
			filePath.ReleaseBuffer();
		}

		// ��ͼ����������
		m_disparity = Scalar(0);
		F_ShowImage( m_disparity, m_disparity, IDC_PicSynImg );

		// �ͷ��ڴ�
		xmlPath.ReleaseBuffer();
	}
	catch (LPCTSTR ErrMsg)
	{
		AfxMessageBox(ErrMsg);
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

	GetDlgItem(IDC_RAD_BM)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_SGBM)->EnableWindow(TRUE);
	GetDlgItem(IDC_BN2StopCam)->EnableWindow(TRUE);
	GetDlgItem(IDC_BN_CompDisp)->EnableWindow(TRUE);
	GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(TRUE);
//	GetDlgItem(IDC_CBN2MethodList)->EnableWindow(TRUE);
	GetDlgItem(IDC_BN_StopDispComp)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_DispView)->SetWindowText(_T("�Ӳ�ͼ"));

	// ������ʾʵʱ����
	DoShowOrigFrame();
	return;
}

void CbinocularCameraDlg::F_Saveframe(Mat& lfImg, Mat&riImg, Mat& lfDisp)
{
	static int nSavedFrames = 0;

	CStringA lfImgName(m_workDir), riImgName(m_workDir), lfDispName(m_workDir), xmlName(m_workDir);
	lfImgName.AppendFormat("Imgs\\SaveFrame\\left_%02d.png", nSavedFrames);
	riImgName.AppendFormat("Imgs\\SaveFrame\\right_%02d.png", nSavedFrames);
	lfDispName.AppendFormat("Imgs\\SaveFrame\\disp_%02d.png", nSavedFrames);
	xmlName.AppendFormat("Imgs\\SaveFrame\\disp_%02d.txt", nSavedFrames);
	nSavedFrames++;

	try
	{
		imwrite( lfImgName.GetBuffer(0), lfImg);
		imwrite( riImgName.GetBuffer(0), riImg);
		imwrite( lfDispName.GetBuffer(0), lfDisp);

		FILE* fp;
		fopen_s(&fp, xmlName.GetBuffer(0), "wt");
		fprintf(fp, "%d\n", lfDisp.rows);
		fprintf(fp, "%d\n", lfDisp.cols);
		for(int y = 0; y < lfDisp.rows; y++)
		{
			for(int x = 0; x < lfDisp.cols; x++)
			{
				short disp = lfDisp.at<short>(y, x);	
				fprintf(fp, "%d\n", disp);
			}
		}

		fclose(fp);
	}
	catch (...)
	{
	}

	lfDispName.ReleaseBuffer();
	lfImgName.ReleaseBuffer();
	riImgName.ReleaseBuffer();
	xmlName.ReleaseBuffer();
}

void CbinocularCameraDlg::DoUpdateStateBM(void)
{
	UpdateData(TRUE);
	m_stereoMatcher.m_BM.state->preFilterCap = m_nPreFiltCap;
	m_stereoMatcher.m_BM.state->SADWindowSize = m_nSADWinSiz;
	m_stereoMatcher.m_BM.state->minDisparity = m_nMinDisp;
	m_stereoMatcher.m_BM.state->numberOfDisparities = m_nMaxDisp;
	m_stereoMatcher.m_BM.state->textureThreshold = m_nTextThres;
	m_stereoMatcher.m_BM.state->uniquenessRatio = m_nUniqRatio;
	m_stereoMatcher.m_BM.state->speckleWindowSize = m_nSpeckWinSiz;
	m_stereoMatcher.m_BM.state->speckleRange = m_nSpeckRange;
}

void CbinocularCameraDlg::DoUpdateStateSGBM(int imgChannels)
{
	UpdateData(TRUE);
	m_stereoMatcher.m_SGBM.preFilterCap = m_nPreFiltCap;
	m_stereoMatcher.m_SGBM.SADWindowSize = m_nSADWinSiz;
	m_stereoMatcher.m_SGBM.P1 =  8 * imgChannels * m_nSADWinSiz * m_nSADWinSiz;
	m_stereoMatcher.m_SGBM.P2 = 32 * imgChannels * m_nSADWinSiz * m_nSADWinSiz;
	m_stereoMatcher.m_SGBM.minDisparity = m_nMinDisp;
	m_stereoMatcher.m_SGBM.numberOfDisparities = m_nMaxDisp;
	m_stereoMatcher.m_SGBM.uniquenessRatio = m_nUniqRatio;
	m_stereoMatcher.m_SGBM.speckleWindowSize = m_nSpeckWinSiz;
	m_stereoMatcher.m_SGBM.speckleRange = m_nSpeckRange;
	m_stereoMatcher.m_SGBM.fullDP = m_bFullDP;
}

bool CbinocularCameraDlg::DoParseOptionsOfStereoMatch(OptionStereoMatch& opt)
{
	bool res = UpdateData(TRUE);
	if (!res)
		return false;

	if (m_nMaxDisp==0)
	{
		AfxMessageBox(_T("��������ƥ���㷨����"));
		return false;
	}

	// ȷ�ϼ����Ӳ���㷨
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_BM, IDC_RAD_SGBM);
	opt.stereoMethod = m_nID_RAD == IDC_RAD_BM ? STEREO_BM :
		m_nID_RAD == IDC_RAD_SGBM ? STEREO_SGBM : STEREO_BM;
	// ȷ��������ͼ����Դ������ͷ or ����ͼƬ��
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_DispFromCam, IDC_RAD_DispFromImg);
	opt.readLocalImage = m_nID_RAD == IDC_RAD_DispFromImg;
	// ȷ���Ƿ�ִ��˫ĿУ��
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_StereoRectify);
	opt.useStereoRectify = m_pCheck->GetCheck() > 0;
	// ȷ���Ƿ񱣴�ƥ����
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_SaveAsVideo);
	opt.saveResults = m_pCheck->GetCheck() > 0;
	// ȷ���Ƿ���ʱ��ʾƥ��Ч��
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_DelayProc);
	opt.delayEachFrame = m_pCheck->GetCheck() > 0;
	if (opt.readLocalImage)
	{
		opt.delayEachFrame = true;
		m_pCheck->SetCheck(1);
	}
	// ȷ���Ƿ�������ά����
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_ProjTo3D);
	opt.generatePointCloud = m_pCheck->GetCheck() > 0;

	return true;
}


void CbinocularCameraDlg::OnBnClickedBnStopdispcomp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �����˳���Ϣ
	::PostMessage(this->m_hWnd, WM_QUIT, 0, 0);
	GetDlgItem(IDC_BN_StopDispComp)->EnableWindow(FALSE);

	if (lfCam.isOpened() || riCam.isOpened())
	{
		// ���ò��ְ�ť
		GetDlgItem(IDC_BN2StopCam)->EnableWindow(TRUE);
		GetDlgItem(IDC_BN2StereoCalib)->EnableWindow(TRUE);
		GetDlgItem(IDC_BN_CompDisp)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_BM)->EnableWindow(TRUE);
		GetDlgItem(IDC_RAD_SGBM)->EnableWindow(TRUE);
	}

	return;
}


void CbinocularCameraDlg::OnBnClickedRadBm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DoClearParamsOfStereoMatch();
	m_spinPreFiltCap.SetRange(0, 31);
	GetDlgItem(IDC_EDIT_minDisp)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_maxDisp)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SADWinSiz)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_textThres)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_disp12MaxDiff)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_preFiltCap)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_uniqRatio)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_speckRange)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_speckWinSiz)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHK_fullDP)->EnableWindow(FALSE);
}


void CbinocularCameraDlg::OnBnClickedRadSgbm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DoClearParamsOfStereoMatch();
	m_spinPreFiltCap.SetRange(0, 100);
	GetDlgItem(IDC_EDIT_minDisp)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_maxDisp)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SADWinSiz)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_textThres)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_disp12MaxDiff)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_preFiltCap)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_uniqRatio)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_speckRange)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_speckWinSiz)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHK_fullDP)->EnableWindow(TRUE);
}

void CbinocularCameraDlg::DoClearParamsOfStereoMatch(void)
{
	m_nMinDisp = 0;	
	m_nMaxDisp = 0;
	m_nSADWinSiz = 0;
	m_nTextThres = 0;
	m_nDisp12MaxDiff = -1;
	m_nPreFiltCap = 0;
	m_nUniqRatio = 0;
	m_nSpeckRange = 0;
	m_nSpeckWinSiz = 0;
	m_pCheck = (CButton*)GetDlgItem(IDC_CHK_fullDP);
	m_pCheck->SetCheck(0);
	UpdateData(FALSE);
	return;
}


void CbinocularCameraDlg::OnBnClickedBtnDefaultviewfield()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nViewWidth = 200;
	m_nViewHeight = 200;
	m_nViewDepth = 200;
	UpdateData(FALSE);
}


void CbinocularCameraDlg::OnDeltaposSpinmaxdisp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	UpdateData(TRUE);

	m_nMaxDisp += (int)pNMUpDown->iDelta * 16;
	if(m_nMaxDisp<16) 
		m_nMaxDisp = 16;
	if(m_nMaxDisp>240)
		m_nMaxDisp = 240;

	UpdateData(FALSE);
}


void CbinocularCameraDlg::OnDeltaposSpinSadwinsiz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_BM, IDC_RAD_SGBM);
	if (m_nID_RAD == IDC_RAD_BM)
	{
		UpdateData(TRUE);
		m_nSADWinSiz += (int)pNMUpDown->iDelta * 2; 
		if(m_nSADWinSiz % 2 == 0)
			m_nSADWinSiz += 1;
		if(m_nSADWinSiz < 5)
			m_nSADWinSiz = 5;
		if(m_nSADWinSiz > 21)
			m_nSADWinSiz = 21;
		UpdateData(FALSE);
	}
	else if (m_nID_RAD == IDC_RAD_SGBM)
	{
		UpdateData(TRUE);
		m_nSADWinSiz += (int)pNMUpDown->iDelta * 2;
		if(m_nSADWinSiz % 2 == 0)
			m_nSADWinSiz += 1;
		if(m_nSADWinSiz < 1)
			m_nSADWinSiz = 1;
		if(m_nSADWinSiz > 11)
			m_nSADWinSiz = 11;
		UpdateData(FALSE);
	}
}


void CbinocularCameraDlg::OnDeltaposSpinspeckrange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_nID_RAD = GetCheckedRadioButton(IDC_RAD_BM, IDC_RAD_SGBM);
	if (m_nID_RAD == IDC_RAD_BM)
	{
		UpdateData(TRUE);
		m_nSpeckRange += (int)pNMUpDown->iDelta;
		if(m_nSpeckRange < 0)
			m_nSpeckRange = 0;
		if(m_nSpeckRange > 64)
			m_nSpeckRange = 64;
		UpdateData(FALSE);
	}
	else if (m_nID_RAD == IDC_RAD_SGBM)
	{
		UpdateData(TRUE);
		m_nSpeckRange += (int)pNMUpDown->iDelta * 16;
		if(m_nSpeckRange < 0)
			m_nSpeckRange = 0;
		if(m_nSpeckRange > 64)
			m_nSpeckRange = 64;
		UpdateData(FALSE);
	}

}


void CbinocularCameraDlg::OnDeltaposSpinviewwidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	UpdateData(TRUE);

	m_nViewWidth += (int)pNMUpDown->iDelta * 10;
	if(m_nViewWidth<60) 
		m_nViewWidth = 60;
	if(m_nViewWidth>600)
		m_nViewWidth = 600;

	UpdateData(FALSE);
}


void CbinocularCameraDlg::OnDeltaposSpinviewheight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	UpdateData(TRUE);

	m_nViewHeight += (int)pNMUpDown->iDelta * 10;
	if(m_nViewHeight<40) 
		m_nViewHeight = 40;
	if(m_nViewHeight>400)
		m_nViewHeight = 400;

	UpdateData(FALSE);
}


void CbinocularCameraDlg::OnDeltaposSpinviewdepth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	UpdateData(TRUE);

	m_nViewDepth += (int)pNMUpDown->iDelta * 10;
	if(m_nViewDepth<60) 
		m_nViewDepth = 60;
	if(m_nViewDepth>600)
		m_nViewDepth = 600;

	UpdateData(FALSE);
}

CString CbinocularCameraDlg::F_InitWorkDir()
{
	CStringA strPath;
	TCHAR path[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	::GetModuleFileName(AfxGetApp()->m_hInstance, path, _MAX_PATH);
	_tsplitpath(path, drive, dir, fname, ext);

	strPath.Empty();
	strPath += CString(drive);
	strPath += CString(dir); 

	string saveDir = strPath;
	saveDir += "/Imgs";
	F_CheckDir(saveDir, true);
	saveDir += "/SaveFrame";
	F_CheckDir(saveDir, true);

	return CString(strPath);
}

bool CbinocularCameraDlg::F_CheckDir(const string dir, bool creatDir /*= false*/)
{
	bool RetVal = false;

	string fileName = dir;
	fileName += "\\*";

	_finddata_t FileInfo;
	long Handle = _findfirst(fileName.c_str(), &FileInfo);	

	if (Handle == -1L)
	{
		if (creatDir)
		{
			if(_mkdir(dir.c_str())==0)
			{
				RetVal = true;
			}
		}
	}
	else
	{
		RetVal = true;
		_findclose(Handle);
	}

	return RetVal;
}
