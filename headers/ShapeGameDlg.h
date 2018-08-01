
// ShapeGameDlg.h : header file
//

#pragma once
#include"Shape.h"
#include"Command.h"
#define MAX_SHAPES 6
#include "afxwin.h"

// CShapeGameDlg dialog
class CShapeGameDlg : public CDialogEx
{
// Construction
public:
	CShapeGameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHAPEGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	
	enum MODES{CREATE, PLAY};
	MODES mode;

	CTypedPtrArray<CObArray, Shape*> figure;
	CTypedPtrArray<CObArray, Shape*> bank;
	CTypedPtrArray<CObArray, Shape*> create;
	Shape *currShape;
	Shape *bankCurrShape; // for creation only
	CPoint bank_point[MAX_SHAPES];
	int solution;

	CTypedPtrArray<CObArray, Command*> undo;
	CTypedPtrArray<CObArray, Command*> redo;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedCreate();
	CButton m_save;
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedRedo();
	CButton m_undo;
	CButton m_redo;
};
