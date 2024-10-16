#include "Renderer.h"

IMPLEMENT_SINGLETON(CRenderer)


CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	if (RENDER_END <= eType || nullptr == pGameObject)
		return;

	m_RenderGroup[eType].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_Trancelucent(pGraphicDev);
	Render_UI(pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[RENDER_PRIORITY])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	for (auto& pGameObject : m_RenderGroup[RENDER_NONALPHA])
		pGameObject->Render_GameObject();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	for (auto& pGameObject : m_RenderGroup[RENDER_ALPHA])
		pGameObject->Render_GameObject();


	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}


void CRenderer::Render_UI(LPDIRECT3DDEVICE9& pGraphicDev)
{
	//0913
	D3DVIEWPORT9 viewport;
	pGraphicDev->GetViewport(&viewport); // 뷰포트 가져옴

	// pre projection matrix (old projection matrix)
	D3DXMATRIX matOldProj;
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);


	D3DXMATRIX matOrthoProj;
	D3DXMatrixOrthoLH(&matOrthoProj, WINCX, WINCY, 0.1f, 100.f);

	// 직교 투영 행렬 설정
	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOrthoProj);

	// 뷰 행렬 설정
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	// 카메라가 평행하게 화면을 바라보도록

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_RenderGroup[RENDER_UI].sort([](CGameObject* pDst, CGameObject* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		});


	for (auto& pGameObject : m_RenderGroup[RENDER_UI])
		pGameObject->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

void CRenderer::Render_Trancelucent(LPDIRECT3DDEVICE9& pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(128, 255, 255, 255));
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


	for (auto& pGameObject : m_RenderGroup[RENDER_TRANSLUCNET])
		pGameObject->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
}

void CRenderer::Free()
{
	Clear_RenderGroup();
}
