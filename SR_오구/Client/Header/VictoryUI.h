#include "GameObject.h"
#include "Export_Utility.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END
class CVictoryUI : public Engine::CGameObject
{
private:
	explicit CVictoryUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVictoryUI();

public:
	virtual   HRESULT   Ready_GameObject();
	virtual   _int      Update_GameObject(const _float& fTimeDelta);
	virtual   void	    LateUpdate_GameObject(const _float& fTimeDelta);
	virtual   void      Render_GameObject();

	void	  CallVictory() { m_bCall = true; m_fElapsedTime = 0; }

private:
	HRESULT    Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;


	_bool	m_bCall;
	_float	m_fElapsedTime;

public:
	static CVictoryUI* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free();

};

