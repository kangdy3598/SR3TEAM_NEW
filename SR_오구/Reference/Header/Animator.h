#pragma once

#include "Engine_Define.h"

class CGameObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*> m_mapAnim; // 모든 Animation
	CAnimation*  m_pCurAnim; // 현재 재생중인 Animation
	CGameObject* m_pOwner;  // Animator 소유 오브젝트
	bool         m_bRepeat; // 반복 재생 여부

	CAnimation* m_pAnim; // 애니메이션

public:

	CGameObject* GetObj() { return m_pOwner; }


public:

	void CreateAnimation(const wstring& _strName, 
							CTexture* _pTex, 
							_vec2 _vLT, 
							_vec2 _vSliceSize,
							_vec2 _vStep,
							float _fDuration,
							UINT _iFrameCount);

	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName, bool _bRepeat);
	void update();
	void finalupdate();
	void render();


};

