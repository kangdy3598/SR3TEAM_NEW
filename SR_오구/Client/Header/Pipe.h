#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CRcTex;
class CTransform;
class CPipeCom;

END

class CPipe :public Engine::CGameObject
{
private:
	explicit CPipe(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPipe();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

public:
	void Initialize_Pipe_Option(PIPEFLOW _eUp, PIPEFLOW _eDown, PIPEFLOW _eLeft, PIPEFLOW _eRight, _bool _bIsFixed, _int _iImageID, _vec3 _vPos, _vec3 _fAngle);	
	void Set_PipeID(_int _iID) { m_iPipeID = _iID; }
	_int Get_PipeID() { return m_iPipeID; }
private:
	HRESULT    Add_Component();	

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CPipeCom* m_pPipeCom;

public:
	static CPipe* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	vector<IDirect3DTexture9*> m_vecTexture;
	_int m_iImageID;
	_int m_iPipeID;

private:
	bool LoadTextureFromFile(LPDIRECT3DDEVICE9 d3dDevice, const char* filePath, IDirect3DTexture9** outTexture)
	{
		HRESULT hr = D3DXCreateTextureFromFileA(d3dDevice, filePath, outTexture);
		return SUCCEEDED(hr);
	}

private:
	virtual void Free();
};