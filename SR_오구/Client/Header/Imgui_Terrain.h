#pragma once

#include "GraphicDev.h"
#include "Engine_Define.h"

class CImgui_Terrain
{
public:
	explicit CImgui_Terrain();
	virtual ~CImgui_Terrain();


public:
	HRESULT Ready_Buffer();
	HRESULT update();
	void Render();
	void Release();

private:

	LPDIRECT3DDEVICE9       m_pGraphicDev;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9  m_pIB;


	_ulong      m_dwVtxCnt; 
	_ulong		m_dwVtxSize;
	_ulong		m_dwTriCnt;
	_ulong      m_dwFVF;
	_ulong		m_dwIdxSize;

	D3DFORMAT   m_IdxFmt; 

	

};

