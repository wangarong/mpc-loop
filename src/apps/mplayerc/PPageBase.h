/*
 * $Id: PPageBase.h 2786 2010-12-17 16:42:55Z XhmikosR $
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

// CPPageBase dialog

class CPPageBase : public CCmdUIPropertyPage
{
	DECLARE_DYNAMIC(CPPageBase)

protected:
	CToolTipCtrl m_wndToolTip;
	void CreateToolTip();

public:
	CPPageBase(UINT nIDTemplate, UINT nIDCaption = 0);
	virtual ~CPPageBase();

	// Dialog Data

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
};