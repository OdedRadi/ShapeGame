
// ShapeGameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShapeGame.h"
#include "ShapeGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CShapeGameDlg dialog







CShapeGameDlg::CShapeGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShapeGameDlg::IDD, pParent), currShape(NULL), mode(PLAY), solution(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShapeGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_S, game_zone_v);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_UNDO, m_undo);
	DDX_Control(pDX, IDC_REDO, m_redo);
}

BEGIN_MESSAGE_MAP(CShapeGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_SAVE, &CShapeGameDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CShapeGameDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_PLAY, &CShapeGameDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_CREATE, &CShapeGameDlg::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_UNDO, &CShapeGameDlg::OnBnClickedUndo)
	ON_BN_CLICKED(IDC_REDO, &CShapeGameDlg::OnBnClickedRedo)
END_MESSAGE_MAP()


// CShapeGameDlg message handlers

BOOL CShapeGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	/* ------ Init creation shapes (for create mode) ------ */
	CPoint p(300,310);
	p.x=120;
	p.y=445;
	
	create.Add(new MSquare(p));
	p.x+=150;
	create.Add(new MParall(p));
	p.x+=150;
	create.Add(new MCircle(p));
	p.x+=150;
	create.Add(new MTriangle(p));
	/* ---------------------------------------------------- */

	/* ------ Init bank positions for creation (for create mode) ------ */
	bank_point[0].x=100;
	bank_point[0].y=445;
	for(int i=1; i<MAX_SHAPES; i++)
	{
		bank_point[i].x=bank_point[i-1].x+100;
		bank_point[i].y=445;
	}
	/* ---------------------------------------------------------------- */

	CheckRadioButton(IDC_PLAY,IDC_CREATE,IDC_PLAY);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShapeGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShapeGameDlg::OnPaint()
{
	CPaintDC dc(this);
	
	for(int i=0; i<figure.GetSize(); i++)
		figure[i]->Draw(&dc);

	if(mode==PLAY)
	{
		for(int i=0; i<bank.GetSize(); i++)
			bank[i]->Draw(&dc);
		//for(int i=0; i<solution.GetSize(); i++)
			//solution[i]->Draw(&dc);

		m_save.EnableWindow(false);

		if(undo.GetSize())
			m_undo.EnableWindow(true);
		else
			m_undo.EnableWindow(false);

		if(redo.GetSize())
			m_redo.EnableWindow(true);
		else
			m_redo.EnableWindow(false);
	}

	else
	{
		for(int i=0; i<create.GetSize(); i++)
			create[i]->Draw(&dc);
		dc.Rectangle(50,50,100,100);

		m_save.EnableWindow(true);
		m_undo.EnableWindow(false);
		m_redo.EnableWindow(false);
	}

	

	CDialogEx::OnPaint();
	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShapeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CShapeGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i=0;

	if(mode==PLAY)
	{
		for(i=0; i<bank.GetSize(); i++)
		{
			if(bank[i]->Contains(point))
			{
				currShape=bank[i];

				if(bank[i]->getCenter()==figure[i]->getCenter())
					solution--;

				break;
			}
		}

		MSquare *sqr;
		if(sqr = dynamic_cast<MSquare*>(currShape))
		{
			Shape *newShape = new MSquare(currShape->getCenter());
			newShape->setColor(currShape->getColor());
			Command *newCommand = new Command(newShape, i);
			undo.Add(newCommand);
			redo.RemoveAll();
		}
			
		MTriangle *trl;
		if(trl = dynamic_cast<MTriangle*>(currShape))
		{
			Shape *newShape = new MTriangle(currShape->getCenter());
			newShape->setColor(currShape->getColor());
			Command *newCommand = new Command(newShape, i);
			undo.Add(newCommand);
			redo.RemoveAll();
		}

		MCircle *crl;
		if(crl = dynamic_cast<MCircle*>(currShape))
		{
			Shape *newShape = new MCircle(currShape->getCenter());
			newShape->setColor(currShape->getColor());
			Command *newCommand = new Command(newShape, i);
			undo.Add(newCommand);
			redo.RemoveAll();
		}

		MParall *prl;
		if(prl = dynamic_cast<MParall*>(currShape))
		{
			Shape *newShape = new MParall(currShape->getCenter());
			newShape->setColor(currShape->getColor());
			Command *newCommand = new Command(newShape, i);
			undo.Add(newCommand);
			redo.RemoveAll();
		}
	}

	else if (mode==CREATE)
	{
		for(int i=0; i < 4; i++)
		{
			if(create[i]->Contains(point) && figure.GetSize()<MAX_SHAPES)
			{
				currShape=create[i];

				MSquare *sqr;
				if(sqr = dynamic_cast<MSquare*>(currShape))
				{
					figure.Add(new MSquare(create[i]->getCenter()));
					bank.Add(new MSquare(bank_point[figure.GetSize()-1]));
					bank[bank.GetSize()-1]->setColor(RGB(rand()%256, rand()%256, rand()%256));
					currShape=figure[figure.GetSize()-1];
				}
			
				MTriangle *trl;
				if(trl = dynamic_cast<MTriangle*>(currShape))
				{
					figure.Add(new MTriangle(create[i]->getCenter()));
					bank.Add(new MTriangle(bank_point[figure.GetSize()-1]));
					bank[bank.GetSize()-1]->setColor(RGB(rand()%256, rand()%256, rand()%256));
					currShape=figure[figure.GetSize()-1];
				}

				MCircle *crl;
				if(crl = dynamic_cast<MCircle*>(currShape))
				{
					figure.Add(new MCircle(create[i]->getCenter()));
					bank.Add(new MCircle(bank_point[figure.GetSize()-1]));
					bank[bank.GetSize()-1]->setColor(RGB(rand()%256, rand()%256, rand()%256));
					currShape=figure[figure.GetSize()-1];
				}

				MParall *prl;
				if(prl = dynamic_cast<MParall*>(currShape))
				{
					figure.Add(new MParall(create[i]->getCenter()));
					bank.Add(new MParall(bank_point[figure.GetSize()-1]));
					bank[bank.GetSize()-1]->setColor(RGB(rand()%256, rand()%256, rand()%256));
					currShape=figure[figure.GetSize()-1];
				}
				break;
			}
		}

		for(int i=0; i<figure.GetSize(); i++)
			if(figure[i]->Contains(point))
			{
				CPoint tmp;

				currShape=figure[i];
				bankCurrShape=bank[i];

				tmp=bank_point[i];
				for (int j=i; j<figure.GetSize()-1; j++)
					bank_point[j]=bank_point[j+1];
				bank_point[figure.GetSize()-1]=tmp;

				if(bankCurrShape->getCenter() == currShape->getCenter())
					solution--;

				bank.RemoveAt(i,1);
				figure.RemoveAt(i, 1);
			}
	}
		
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CShapeGameDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(currShape)
	{
		CClientDC dc(this);

		dc.SetROP2(R2_NOTXORPEN);

		currShape->Draw(&dc);
		currShape->setCenter(point);
		currShape->Draw(&dc);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CShapeGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int flag=0;

	if(mode==PLAY && currShape !=NULL)
	{
		for(int i=0; i<figure.GetSize(); i++)
		{
			if(figure[i]->Contains(point))
			{
				MSquare *sqr;
				if(sqr = dynamic_cast<MSquare*>(currShape))
					if (sqr = dynamic_cast<MSquare*>(figure[i])) 
						flag=1;

				MCircle *crl; 
				if(crl = dynamic_cast<MCircle*>(currShape))
					if (crl = dynamic_cast<MCircle*>(figure[i]))
						flag=1;

				MTriangle *trl;
				if(trl = dynamic_cast<MTriangle*>(currShape))
					if(trl = dynamic_cast<MTriangle*>(figure[i]))
						flag=1;

				MParall *prl;
				if(prl = dynamic_cast<MParall*>(currShape))
					if(prl = dynamic_cast<MParall*>(figure[i]))
						flag=1;
			}
			if(flag)
			{
				currShape->setCenter(figure[i]->getCenter());
				solution++;
				break;
			}
		}

		currShape=NULL;
		Invalidate();
		if(solution==figure.GetSize() && figure.GetSize() != 0)
			int msgboxID = MessageBoxW(L"You completed the figure!", L"Victory!", MB_ICONWARNING | MB_OK | MB_DEFBUTTON1 | MB_TASKMODAL);
	}

	else if(mode==CREATE && currShape != NULL)
	{
		int x, y;
		x=currShape->getCenter().x;
		y=currShape->getCenter().y;

		if((x<=50 || x >=100 || y<=50 || y>=100))
		{
			figure.Add(currShape);
			bank.Add(bankCurrShape);
		}

		else
		{
			delete bankCurrShape;
			delete currShape;
		}

		bankCurrShape=NULL;
		currShape=NULL;
		Invalidate();
	}		

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CShapeGameDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(false,_T("Chart Files (*.$$)|*.$$|"),NULL,NULL,_T("Chart Files (*.$$)|*.$$|"));
	
	auto result = dlg.DoModal(); 
	if(result != IDOK)
		MessageBox(L"Invalid file", L"Shapes Game");
    
	OPENFILENAME & ofn = dlg.GetOFN( );

	try
	{
		CFile file(ofn.lpstrFile, CFile::modeCreate);
	}
	catch(int e)
	{};
	
	CFile file(ofn.lpstrFile, CFile::modeWrite);	
	CArchive ar (&file, CArchive::store);
	
	figure.Serialize(ar);
	bank.Serialize(ar);
	
	ar.Close();
	file.Close();
}


void CShapeGameDlg::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,_T("Chart Files (*.$$)|*.$$|"),NULL,NULL,_T("Chart Files (*.$$)|*.$$|"));

	auto result = dlg.DoModal();
    if(result != IDOK)
		MessageBox(L"Invalid file", L"Shapes Game");
	if(result == IDOK)
		solution=0;
	OPENFILENAME& ofn = dlg.GetOFN( );

	CFile file(ofn.lpstrFile, CFile::modeRead);
	CArchive ar (&file, CArchive::load);

	figure.Serialize(ar);
	bank.Serialize(ar);

	ar.Close();
	file.Close();
	Invalidate();
}

void CShapeGameDlg::OnBnClickedPlay()
{
	// TODO: Add your control notification handler code here
	mode = PLAY;
	Invalidate();
}


void CShapeGameDlg::OnBnClickedCreate()
{
	// TODO: Add your control notification handler code here
	mode = CREATE;

	for(int i=0; i<undo.GetSize(); i++)	
		delete undo[i];

	for(int i=0; i<redo.GetSize(); i++)
		delete redo[i];

	undo.RemoveAll();
	redo.RemoveAll();
	Invalidate();
}


void CShapeGameDlg::OnBnClickedUndo()
{
	// TODO: Add your control notification handler code here
	Command *lastCommand = undo[undo.GetSize()-1];
	undo.RemoveAt(undo.GetSize()-1, 1);

	Shape *restoredShape = lastCommand->getShape();
	Shape *cancledShape = bank[lastCommand->getIndex()];

	bank[lastCommand->getIndex()] = restoredShape;

	Command *redoCommand = new Command(cancledShape, lastCommand->getIndex());
	redo.Add(redoCommand);
	
	if(restoredShape->getCenter()==figure[lastCommand->getIndex()]->getCenter())
					solution++;
	if(cancledShape->getCenter()==figure[lastCommand->getIndex()]->getCenter())
					solution--;

	Invalidate();

	if(solution==figure.GetSize() && figure.GetSize() != 0)
		 int msgboxID = MessageBoxW(L"You completed the figure!", L"Victory!", MB_ICONWARNING | MB_OK | MB_DEFBUTTON1 | MB_TASKMODAL);
}


void CShapeGameDlg::OnBnClickedRedo()
{
	// TODO: Add your control notification handler code here
	Command *lastCommand = redo[redo.GetSize()-1];
	redo.RemoveAt(redo.GetSize()-1, 1);

	Shape *restoredShape = lastCommand->getShape();
	Shape *cancledShape = bank[lastCommand->getIndex()];

	bank[lastCommand->getIndex()] = restoredShape;

	Command *undoCommand = new Command(cancledShape, lastCommand->getIndex());
	undo.Add(undoCommand);
	
	if(restoredShape->getCenter()==figure[lastCommand->getIndex()]->getCenter())
					solution++;
	if(cancledShape->getCenter()==figure[lastCommand->getIndex()]->getCenter())
					solution--;

	Invalidate();

	if(solution==figure.GetSize() && figure.GetSize() != 0)
		 int msgboxID = MessageBoxW(L"You completed the figure!", L"Victory!", MB_ICONWARNING | MB_OK | MB_DEFBUTTON1 | MB_TASKMODAL);
}
