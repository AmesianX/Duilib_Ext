#include "stdafx.h"
#include "TreeView.h"

#define IDC_TREEVIEWWND 1200

class TreeViewWnd : public DuiLib::CWindowWnd
{
	friend class TreeViewUI;
public:
	TreeViewWnd();
	void Init(TreeViewUI* pOwer);
	LPCTSTR GetWindowClassName() const;
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnFinalMessage(HWND hWnd);
	
	void OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	CTreeViewCtrlEx treeControl;
	TreeViewUI* pOwner;
	CTreeItem selectItem;
	CTreeItem rootItem;
};

TreeViewWnd::TreeViewWnd() 
	: pOwner(nullptr)
{

}

void TreeViewWnd::OnFinalMessage(HWND hWnd)
{
	delete this;
}

LPCTSTR TreeViewWnd::GetWindowClassName() const
{
	return L"#32770";
}

LRESULT TreeViewWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_CREATE:	OnCreate(uMsg, wParam, lParam); break;
	default:
		bHandled = FALSE;
		break;
	}
	if (!bHandled)
		return DefWindowProc(this->GetHWND(), uMsg, wParam, lParam);
	return 0;
}

void TreeViewWnd::Init(TreeViewUI* pOwer)
{
	this->pOwner = pOwer;
	RECT pos = pOwner->GetPos();
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	Create(pOwner->GetManager()->GetPaintWindow(), nullptr, dwStyle, 0, pos);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	assert(m_hWnd);
}

void TreeViewWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rtUI = {0};
	DWORD dwStyle = WS_CHILD | WS_VISIBLE |TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_INFOTIP | WS_BORDER | WS_HSCROLL | WS_TABSTOP |TVS_SHOWSELALWAYS;
	treeControl.Create(this->m_hWnd, &rtUI, _T(""), dwStyle, 0, IDC_TREEVIEWWND, NULL);
	assert(treeControl.m_hWnd);
	RECT rtParent = pOwner->GetPos();
	// ����treeControl��Ϊ�Ӵ��ڴ��ڣ������ڸ����ڵ�ˢ�£��������ﲻ��SetWinodwPos����MoveWindow
	::MoveWindow(treeControl.m_hWnd, rtParent.left, rtParent.top, rtParent.right - rtParent.left,
		rtParent.bottom - rtParent.top, TRUE);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
TreeViewUI::TreeViewUI()
	:pTreeViewWnd(nullptr)
{
	pTreeViewWnd = new TreeViewWnd;
	assert(pTreeViewWnd);
}

LPCTSTR TreeViewUI::GetClass() const
{
	return L"TreeViewUI";
}

void TreeViewUI::Init()
{
	pTreeViewWnd->Init(this);
}

void TreeViewUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	::SetWindowPos(pTreeViewWnd->GetHWND(), NULL, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	::SetWindowPos(pTreeViewWnd->treeControl.m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
}

CTreeItem TreeViewUI::InsertChildItem(LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	return pTreeViewWnd->treeControl.InsertItem(lpszItem, hParent, hInsertAfter);
}

CTreeItem TreeViewUI::GetRootItem()
{
	return pTreeViewWnd->treeControl.GetRootItem();
}

CTreeItem TreeViewUI::GetChildItem(HTREEITEM hItem)
{
	return pTreeViewWnd->treeControl.GetChildItem(hItem);
}

CTreeItem TreeViewUI::GetNextItem(HTREEITEM hItem)
{
	return pTreeViewWnd->treeControl.GetNextItem(hItem, TVGN_NEXT);
}

CTreeItem TreeViewUI::GetNextSiblingItem(HTREEITEM hItem)
{
	return pTreeViewWnd->treeControl.GetNextSiblingItem(hItem);
}

CTreeItem TreeViewUI::GetParentItem(HTREEITEM hItem)
{
	return pTreeViewWnd->treeControl.GetParentItem(hItem);
}

CTreeItem TreeViewUI::GetPrevSiblingItem(HTREEITEM hItem)
{
	return pTreeViewWnd->treeControl.GetPrevSiblingItem(hItem);
}

unsigned int TreeViewUI::GetCount()
{
	return pTreeViewWnd->treeControl.GetCount();
}

void TreeViewUI::ExpandItem(HTREEITEM hItem)
{
	pTreeViewWnd->treeControl.Expand(hItem);
}

CTreeItem TreeViewUI::GetSelectedItem()
{
	return pTreeViewWnd->treeControl.GetSelectedItem();
}

void TreeViewUI::GetItemText(HTREEITEM hItem, LPTSTR lptstr, int iLength)
{
	pTreeViewWnd->treeControl.GetItemText(hItem, lptstr, iLength);
}