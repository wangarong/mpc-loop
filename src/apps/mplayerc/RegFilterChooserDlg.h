/*
 * $Id: RegFilterChooserDlg.h 2786 2010-12-17 16:42:55Z XhmikosR $
 *
 * (C) 2003-2006 Gabest
 * (C) 2006-2010 see AUTHORS
 *
 * This file is part of mplayerc.
 *
 * Mplayerc is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mplayerc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "afxwin.h"


// CRegFilterChooserDlg dialog

class CRegFilterChooserDlg : public CResizableDialog
{
	//	DECLARE_DYNAMIC(CRegFilterChooserDlg)

	CInterfaceList<IMoniker> m_monikers;
	void AddToList(IMoniker* pMoniker);

public:
	CRegFilterChooserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegFilterChooserDlg();

	CList<FilterOverride*> m_filters;

	// Dialog Data
	enum { IDD = IDD_ADDREGFILTER };
	CListCtrl m_list;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnUpdateOK(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
};
