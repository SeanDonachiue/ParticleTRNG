// VoiceBase.h: interface for the CVoiceBase class.
//Code by: A.Riazi, Shafiee, 
//Accessed at: https://www.codeproject.com/Articles/10138/Voice-Recording-Playing-back-using-simple-classes
//Slight alterations have been made for debugging purposes and audio format.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOICEBASE_H__9DF058FB_F678_4DA0_99FF_18814968A3A5__INCLUDED_)
#define AFX_VOICEBASE_H__9DF058FB_F678_4DA0_99FF_18814968A3A5__INCLUDED_
#include "windows.h"
#include "Mmsystem.h"
#include "mmeapi.h"
#include "atlstr.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVoiceBase  
{
public:
	
	CString m_result;
	MMRESULT res;
	enum 
	{
		SPS_8K=8000,
		SPS_11K=11025,
		SPS_22K=22050,
		SPS_44K=44100
	};

	enum
	{
		CH_MONO=1,
		CH_STEREO=2
	};

	char* buffer;
	WAVEHDR WaveHeader;
	WAVEFORMATEX PCMfmt;

	void SetFormat(DWORD nSamplesPerSec,  WORD  wBitsPerSample,WORD  nChannels);
	BOOL CopyBuffer(LPVOID lpBuffer, DWORD ntime);
	CString GetLastError();
	void GetMMResult(MMRESULT res);
	void DestroyBuffer();
	BOOL PrepareBuffer(DWORD ntime);
	
	CVoiceBase();
	virtual ~CVoiceBase();

};

#endif // !defined(AFX_VOICEBASE_H__9DF058FB_F678_4DA0_99FF_18814968A3A5__INCLUDED_)
