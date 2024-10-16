#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CPureVesselTex :public CVIBuffer
{
private:
	explicit CPureVesselTex();
	explicit CPureVesselTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPureVesselTex(const CPureVesselTex& rhs);
	virtual ~CPureVesselTex();

public:
	const _vec3* Get_VtxPos() const { return m_pPos; }

public:
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual		void		Render_Buffer();

private:
	BITMAPFILEHEADER  m_fH;
	BITMAPINFOHEADER  m_iH;

	HANDLE			  m_hFile;
	_vec3* m_pPos;


public:
	static		CPureVesselTex* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwVtxItv = VTXITV);
	virtual		CComponent* Clone();

private:
	virtual void		Free();
};

END