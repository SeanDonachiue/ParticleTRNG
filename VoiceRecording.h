// VoiceRecording.h: interface for the CVoiceRecording class.
//Code by: A.Riazi, Shafiee, 
//Accessed at: https://www.codeproject.com/Articles/10138/Voice-Recording-Playing-back-using-simple-classes
//Slight alterations have been made for debugging purposes and audio format.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOICEPLAYING_H__D3C0CFEA_0015_4A9A_BEF9_C5A132CB0Aaa__INCLUDED_)
#define AFX_VOICEPLAYING_H__D3C0CFEA_0015_4A9A_BEF9_C5A132CB0Aaa__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VoiceBase.h"
BOOL CALLBACK VoiceWaveInProc(
						 HWAVEIN hwi,       
						 UINT uMsg,         
						 DWORD dwInstance,  
						 DWORD dwParam1,    
						 DWORD dwParam2     
						 );


class CVoiceRecording : public CVoiceBase  
{
public:
	virtual void RecordFinished();
	BOOL IsOpen();
	BOOL Close();
	BOOL DevCaps();
	BOOL Open();
	BOOL Open2();
    BOOL Record();
    HWAVEIN* hWaveIn;		//Pointer to pointer to buffer
	WAVEINCAPS* waveInCaps;	//pointer to waveIncaps
	CVoiceRecording();
	virtual ~CVoiceRecording();
   
	
};

#endif // !defined(AFX_VOICEPLAYING_H__D3C0CFEA_0015_4A9A_BEF9_C5A132CB0Aaa__INCLUDED_)
