#pragma once
#include "Component.h"




class CAnimation2;
class CGameObject;


class CAnimatior2 :public CComponent
{
private:
	explicit CAnimatior2();
	explicit CAnimatior2(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CAnimatior2(const CAnimatior2& rhs);
	virtual  ~CAnimatior2();

private:
	map<wstring, CAnimation2*>		m_mapAnim;  //모든 애니메이션 정보 저장
	CAnimation2*					m_pCurAnim; // 현재 재생중인 Animation
	CGameObject*					m_pOwner;   //Animator 소유 오브젝트
	bool							m_bRepeat;  // 반복재생 여부 
	
	CAnimation2*					m_pAnim;    // 애니메이션


public:
	CGameObject* GetGameObj() { return m_pOwner; }


public:

	void CreateAnimation(const wstring& _strName,
						 IDirect3DTexture9* _pTex,
						 _vec2 _vLT,
						 _vec2 _vSliceSize,
						 _vec2 _vStep,
						 float _fDuration,
						 UINT  _iFrameCount);

	CAnimation2*  FindAnimation(const wstring& _strName);
	void		  Play(const wstring& _strName, bool _bRepeat);

public:
	virtual void  LateReady_Component();
	virtual _int  Update_Component(const _float& fTimeDelta);
	virtual void  LateUpdate_Component(const _float& fTimeDelta);

protected:
	LPDIRECT3DDEVICE9	 m_pGraphicDev;
	_bool				 m_bClone;

public:
	virtual CComponent*  Clone() ;

protected:
	virtual void  Free();
};

