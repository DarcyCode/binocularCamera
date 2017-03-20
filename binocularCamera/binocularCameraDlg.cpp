
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

	// ��ʼ��ͼ����ʾ�ؼ���ͼ��
	CRect rect;
	GetDlgItem(IDC_PicLfCam) ->GetClientRect( &rect );
	m_lfImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
	GetDlgItem(IDC_PicRiCam) ->GetClientRect( &rect );
	m_riImage = Mat::zeros(rect.Height(), rect.Width(), CV_8UC3);
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
	//GetDlgItem( IDC_BN2StereoCalib)->EnableWindow( TRUE );
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
