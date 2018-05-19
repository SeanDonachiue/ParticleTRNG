// VoiceRecording.cpp: implementation of the CVoiceRecording class.
//Code by: A.Riazi, Shafiee, 
//Accessed at: https://www.codeproject.com/Articles/10138/Voice-Recording-Playing-back-using-simple-classes
//Slight alterations have been made for debugging purposes and audio format.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VoiceRecording.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVoiceRecording::CVoiceRecording()
{
	hWaveIn = new HWAVEIN;
	waveInCaps = new WAVEINCAPS;
}

CVoiceRecording::~CVoiceRecording()
{
	if (IsOpen())
		Close();
	delete hWaveIn;
	delete waveInCaps;
}

BOOL CVoiceRecording::Record()
{
	res=waveInPrepareHeader(*hWaveIn,&WaveHeader,sizeof(WAVEHDR));
	GetMMResult(res);
	if (res!=MMSYSERR_NOERROR)
		return FALSE;


	res=waveInAddBuffer(*hWaveIn,&WaveHeader,sizeof(WAVEHDR));
	GetMMResult(res);
	if (res!=MMSYSERR_NOERROR)
		return FALSE;
		
	res=waveInStart(*hWaveIn) ;
	GetMMResult(res);
	if (res!=MMSYSERR_NOERROR)
		return FALSE;
	else
		return TRUE;
	
		
}
BOOL CVoiceRecording::DevCaps() 
{
	res = waveInGetDevCaps(WAVE_MAPPER, waveInCaps, sizeof(waveInCaps));
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR)
	{
		
		std::cout << "waveInGetDevCaps device query fails with error: ";
		//std::cout << GetLastError().GetString() << "\n";
		std::wcout << GetLastError().GetString() << std::endl;
		waveInCaps = NULL;
		return FALSE;
	}
	else {
		std::cout << "waveInGetDevCaps device query succeeds";
		return TRUE;
	}
}
BOOL CVoiceRecording::Open()
{
	if (IsOpen())
		return FALSE;
	res=waveInOpen(hWaveIn, (UINT) WAVE_MAPPER, &PCMfmt, (DWORD) VoiceWaveInProc, (DWORD) this, WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE);
	GetMMResult(res);
	if (res!=MMSYSERR_NOERROR)
	{
		std::cout << "waveInOpen format query fails with error: ";
		std::wcout << GetLastError().GetString() << std::endl;
		hWaveIn=NULL;
		return FALSE;
	}
	else {
		std::cout << "waveInOpen succeeds\n";
		return TRUE;
	}
}
BOOL CVoiceRecording::Open2()
{
	if (IsOpen())
		return FALSE;
	res = waveInOpen(hWaveIn, (UINT)WAVE_MAPPER, &PCMfmt, (DWORD)VoiceWaveInProc, (DWORD)this, CALLBACK_FUNCTION);
	GetMMResult(res);
	if (res != MMSYSERR_NOERROR)
	{
		std::cout << "waveInOpen format query fails with error: ";
		std::wcout << GetLastError().GetString() << std::endl;
		hWaveIn = NULL;
		return FALSE;
	}
	else {
		std::cout << "waveInOpen format query succeeds\n";
		return TRUE;
	}
}

BOOL CVoiceRecording::Close()
{
	//res=waveInReset(hWaveIn);
	//GetMMResult(res);
	//if (res!=MMSYSERR_NOERROR)
	//	return FALSE;
		
	res=waveInClose (*hWaveIn);
	GetMMResult(res);
	if (res!=MMSYSERR_NOERROR)
		return FALSE;
	else
		return TRUE;
}

BOOL CVoiceRecording::IsOpen()
{
	if (*hWaveIn!=NULL)
		return TRUE;
	else
		return FALSE;
}

BOOL CALLBACK VoiceWaveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	if (uMsg==WIM_DATA)
	{
		CVoiceRecording* pVoice=(CVoiceRecording*) dwInstance;
		
		pVoice->res=waveInUnprepareHeader(*pVoice->hWaveIn, &pVoice->WaveHeader, sizeof(WAVEHDR));
		pVoice->GetMMResult(pVoice->res);
		pVoice->RecordFinished();

		if (pVoice->res!=MMSYSERR_NOERROR)
			return FALSE;
		else
			return TRUE;
	}

	return TRUE;
}


void CVoiceRecording::RecordFinished()
{
	//write your handler here
	std::cout << "callback";
	Close();
	//or create your own classes that derived from this class
	//and override this virtual function
}
