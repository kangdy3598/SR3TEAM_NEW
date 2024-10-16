#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMyFont : public CBase
{
private:
	explicit CMyFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMyFont();

public:
	HRESULT     Ready_Font(const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void		Render_Font(const _tchar* pString,
		const _vec2* pLTPos,
		D3DXCOLOR Color,
		TEXTRANGE _eTextRange,
		_vec2 pXYSize);

private:
	LPDIRECT3DDEVICE9  m_pGraphicDev;
	LPD3DXSPRITE       m_pSprite;
	LPD3DXFONT		   m_pFont;


public:
	static CMyFont* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

private:
	virtual void Free();


};

END