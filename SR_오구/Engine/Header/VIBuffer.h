#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();


public:
	virtual  HRESULT  Ready_Buffer();
	virtual  void	  Render_Buffer();
	virtual  void     SetTextureScale(float fScaleSize);
	virtual  void	  SetTexturePos(float fPos);

protected:
	LPDIRECT3DVERTEXBUFFER9     m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	_ulong						m_dwVtxCnt;
	_ulong						m_dwVtxSize;
	_ulong						m_dwTriCnt;
	_ulong						m_dwFVF;

	_ulong						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt; 
	_vec3* m_pPos;

public:
	virtual void  Free();

};

END
