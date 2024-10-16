#include "Animator2.h"
#include "Animation2.h"

CAnimator2::CAnimator2()
	:m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
	, m_pAnim(nullptr)
{
}

CAnimator2::CAnimator2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
	, m_pAnim(nullptr)
{
}

CAnimator2::CAnimator2(const CAnimator2& rhs)
	:CComponent(rhs)
	, m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
	, m_pAnim(nullptr)
{


}

CAnimator2::~CAnimator2()
{
}

CAnimator2* CAnimator2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

	CAnimator2* pAnimator2 = new CAnimator2(pGraphicDev);

	//if (FAILED(pAnimator2->)	
	//{
	//	Safe_Release(pAnimator2);	
	//	MSG_BOX("Transform Create Failed");	
	//	return nullptr;
	//}

	return pAnimator2;
}

void CAnimator2::CreateAnimation(const wstring& _strName, IDirect3DTexture9* _pTex, _vec2 _vLT, _vec2 _vSliceSize, _vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation2* pAnim = FindAnimation(_strName);

	if (pAnim != nullptr)
	{
		MSG_BOX("오류 발생 .이미 만든 애니메이션 또 만들었음 ");
	}

	pAnim = new CAnimation2(m_pGraphicDev);
	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;

	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));

}

CAnimation2* CAnimator2::FindAnimation(const wstring& _strName)
{

	map<wstring, CAnimation2*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2::LateReady_Component()
{

}


_int CAnimator2::Update_Component(const _float& fTimeDelta)
{

	return _int();
}

void CAnimator2::LateUpdate_Component(const _float& fTimeDelta)
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0); // 반복재생을 위해 처음으로 되돌리는 코드	
		}
	}
}

void CAnimator2::render()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->Render_Buffer();
	}
}




void CAnimator2::Play(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}

void CAnimator2::SetAnimationFrame(const wstring& _strName, int _idx)
{
	m_pAnim = FindAnimation(_strName);
	m_pAnim->SetFrame(_idx);
}


CComponent* CAnimator2::Clone()
{
	return new CAnimator2(*this);
}

void CAnimator2::Free()
{
	CComponent::Free();
}
