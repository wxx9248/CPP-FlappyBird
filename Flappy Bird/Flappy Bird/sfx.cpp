#include "stdafx.hpp"
#include "sfx.hpp"

SFX::SFX(const HWND _hWnd) throw(...)
{
	if (NULL == _hWnd)
		throw stdWCexception(L"SFX::init()�����ھ����Ч");
	else
		hWnd = _hWnd;

	initDS();
}

SFX::SFX(const SFX &sfx)
{
	v = sfx.v;
	hWnd = sfx.hWnd;
}

SFX::SFX(const WAV _wav[], UINT n, const HWND _hWnd) throw(...)
{
	init(_wav, n, _hWnd);
}

SFX::SFX(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n, const HWND _hWnd) throw(...)
{
	init(wcResName, wcResType, n, _hWnd);
}

void SFX::init(const WAV _wav[], const UINT n, const HWND _hWnd) throw(...)
{
	if (NULL == _hWnd)
		throw stdWCexception(L"SFX::init()�����ھ����Ч");
	else
		hWnd = _hWnd;

	initDS();
	clear();

	for (UINT i = 0; i < n; ++i)
		v.push_back(_wav[i]);
}

void SFX::init(LPCWSTR wcResName[], LPCWSTR wcResType, UINT n, const HWND _hWnd) throw(...)
{
	WAV wav;
	LPVOID wavbuf;

	if (NULL == _hWnd)
		throw stdWCexception(L"SFX::init()�����ھ����Ч");
	else
		hWnd = _hWnd;

	initDS();
	clear();

	for (UINT i = 0; i < n; ++i)
	{
		wavbuf = GetRawWAVBufferW(wcResName[i], wcResType, &wav);

		if (NULL == _hWnd)
			throw stdWCexception(L"SFX::init()�����ھ����Ч");
		else
			hWnd = _hWnd;

		if (NULL == wavbuf)
		{
			wstring ws = L"SFX::init()��WAV�ڴ���Դָ��(";
			ws += std::to_wstring(i);
			ws += L")��Ч";
			throw stdWCexception(ws);
		}

		wav.lpWAVBuffer = wavbuf;

		v.push_back(wav);
		GetWAVInfo(&*v.rbegin());
	}
}

void SFX::clear()
{
	v.clear();
}

const WAV &SFX::get(UINT n) throw(...)
{
	if (n >= v.size())
		throw stdWCexception(L"SFX::get()���ڵ���������Դ�ṹ����ʱ�±�Խ�磡");
	else
		return v[n];
}

void SFX::erase(UINT n) throw(...)
{
	if (n >= v.size())
		throw stdWCexception(L"SFX::erase()���ڵ���������Դ�ṹ����ʱ�±�Խ�磡");
	else
		v.erase(v.begin() + n);
}

void SFX::play(UINT index) throw(...)
{
	// TODO: Play sound asyncly

	if(index >= v.size())
		throw stdWCexception(L"SFX::play()���ڵ���������Դ�ṹ����ʱ�±�Խ�磡");

	v[index].lpDSBuf->SetCurrentPosition(playOffsetSkip);
	v[index].lpDSBuf->Play(0, 0, 0);
}

const SFX SFX::operator+(const SFX &sfx) throw(...)
{
	SFX new_sfx(hWnd);

	new_sfx.v.insert(new_sfx.v.end(), v.begin(), v.end());
	new_sfx.v.insert(new_sfx.v.end(), sfx.v.begin(), sfx.v.end());

	return new_sfx;
}

const SFX &SFX::operator+=(const SFX &sfx)
{
	v.insert(v.end(), sfx.v.begin(), sfx.v.end());

	return *this;
}

const SFX &SFX::operator=(const SFX &sfx)
{
	hWnd = sfx.hWnd;
	v = sfx.v;

	return sfx;
}

const WAV &SFX::operator[](UINT index) throw(...)
{
	return get(index);
}

SFX &operator<<(SFX &sfx, const WAV &_wav)
{
	sfx.v.insert(sfx.v.end(), _wav);

	return sfx;
}

SFX &operator<<(SFX &sfx, const pair<LPCWSTR, LPCWSTR> &ResNameTypePair) throw(...)
{
	WAV wav;
	LPVOID wavbuf;

	wavbuf = sfx.GetRawWAVBufferW(ResNameTypePair.first, ResNameTypePair.second, &wav);

	if (NULL == wavbuf)
		throw stdWCexception(L"SFX��WAV�ڴ���Դָ����Ч");

	wav.lpWAVBuffer = wavbuf;

	sfx.v.push_back(wav);
	sfx.GetWAVInfo(&*sfx.v.rbegin());

	return sfx;
}

LPVOID SFX::GetRawWAVBufferW(LPCWSTR lpResID, LPCWSTR lpResType, WAV *wav) throw(...)
{
	wstring ws;

	HRSRC hResource = FindResourceW(NULL, lpResID, lpResType);
	if (NULL == hResource)
	{
		ws = L"SFX::GetRawWAVBufferW()���޷���ȡWAV������Դ��";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;

		throw stdWCexception(ws);
	}

	wav->dwSzRes = SizeofResource(NULL, hResource);

	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	if (NULL == hGlobal)
	{
		ws = L"SFX::GetRawWAVBufferW()���޷�װ��WAV������Դ��";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;

		throw stdWCexception(ws);
	}

	LPVOID lpRawWAV = LockResource(hGlobal);
	if (NULL == lpRawWAV)
	{
		ws = L"SFX::GetRawWAVBufferW()��������Դ ";
		ws += lpResType;
		ws += L"::";
		ws += lpResID;
		ws += L" ��Ч";

		throw stdWCexception(ws);
	}

	return lpRawWAV;
}

void SFX::initDS() throw(...)
{
	// Initliaze DirectSound
	if (DS_OK != DirectSoundCreate(NULL, &lpDS, NULL))
		throw stdWCexception(L"SFX::initDS()���޷�����DirectSound����");

	if (DS_OK != lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL))
		throw stdWCexception(L"SFX::initDS()������Э���ȼ�ʧ�ܣ�");
}

WAV *SFX::GetWAVInfo(WAV *wav) throw(...)
{
	WAV_HEADER wh;
	DWORD len = 0;
	LPVOID lpBuffer = NULL;

	if (NULL == wav)
		throw stdWCexception(L"SFX::GetWAVInfo()��WAV�ṹ��ָ����Ч��");

	// Initliaze wave format

	memcpy(&wh, wav->lpWAVBuffer, sizeof(WAV_HEADER));

	wav->wfmex.wFormatTag = WAVE_FORMAT_PCM;
	wav->wfmex.nChannels = wh.channels;
	wav->wfmex.nSamplesPerSec = wh.sample_rate;
	wav->wfmex.wBitsPerSample = wh.bits_per_sample;
	wav->wfmex.nBlockAlign = wav->wfmex.wBitsPerSample / 8 * wav->wfmex.nChannels;
	wav->wfmex.nAvgBytesPerSec = wav->wfmex.nSamplesPerSec * wav->wfmex.nBlockAlign;

	// Initliaze Sound buffers

	wav->DSBufDesc.dwSize = sizeof(wav->DSBufDesc);
	wav->DSBufDesc.dwFlags = DSBCAPS_GLOBALFOCUS;
	wav->DSBufDesc.dwBufferBytes = wav->dwSzRes - sizeof(WAV_HEADER);
	wav->DSBufDesc.lpwfxFormat = &wav->wfmex;

	if (DS_OK != lpDS->CreateSoundBuffer(&wav->DSBufDesc, &wav->lpDSBuf, NULL))
		throw stdWCexception(L"SFX::GetWAVInfo()������������������ʧ�ܣ�");

	wav->lpDSBuf->Lock(0, wav->dwSzRes - sizeof(WAV_HEADER), &lpBuffer, &len, NULL, NULL, 0);
 	memcpy(lpBuffer, (LPVOID)((BYTE *)wav->lpWAVBuffer + sizeof(WAV_HEADER)), len);
   	wav->lpDSBuf->Unlock(lpBuffer, len, NULL, 0);

	return wav;
}
