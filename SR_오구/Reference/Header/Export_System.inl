#include "Export_System.h"
HRESULT	Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}
void		Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void		Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

_float			Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
void				Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_TimeDelta(pTimerTag);
}
HRESULT			Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

HRESULT			Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}
_bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

inline HRESULT Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

inline void Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color,
	TEXTRANGE _eTextRange,
	_vec2 vXYSize)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color, _eTextRange, vXYSize);
}

inline _byte Get_DIKeyState(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}

inline _bool GetKeyDown(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->GetKeyDown(byKeyID);
}

inline _bool GetKeyUp(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->GetKeyUp(byKeyID);
}

inline _bool GetKeyPress(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->GetKeyPress(byKeyID);
}

inline _bool GetAnyActionKeyDown()
{
	return CInputDev::GetInstance()->GetAnyActionKeyDown();
}

inline _byte Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouse);
}

inline _long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(eMouseState);
}

inline HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

inline void Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}


void		Release_System()
{
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}
