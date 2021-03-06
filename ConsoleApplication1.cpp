// ConsoleApplication1.cpp : Defines the entry point for the console application.
// Author Sean Donachiue

#include "stdafx.h"
#include "VoiceRecording.h"
#include "cryptorand.h"
int main(int argc, int argv[])
{
	if (argc != 3) {
		std::cout << "incorrect number of arguments.\n please enter [size of one output] [number of outputs] as integers.";
		//return 0;
	}

	CVoiceRecording m_Record;
	//m_Record.DevCaps();
	m_Record.SetFormat(48000, 24, 2);	//48000 hz sample rate, 24 bit, 2 channel recording format
	m_Record.PrepareBuffer(1);    //prepare buffer for recording 1 second (48000 samples)
	while (!m_Record.IsOpen()) {
		m_Record.Open();
		std::cout << "trying to open\n";
	}
	if (!m_Record.Record()) {
		std::cout << "recording failed, please try again\n";
		return 0;
	}
	char* sound_buf = m_Record.buffer;
	m_Record.Close();

	std::ofstream myfile;
	myfile.open("testOutputs2.txt", std::ios_base::app);
	
	CryptoRand cryptorand;
	int num_bytes = argv[1];
	cryptorand.init(num_bytes, sound_buf);
	char* output = new char[num_bytes];
	output[num_bytes] = { '\0' }; //write NUL character to the end of each output to prevent garbage values
	for (int i = argv[2]; i > 0; i--) {
		cryptorand.pullRandom32(sound_buf, output, num_bytes);
		myfile << output;
	}
	delete output;
	myfile.close();
	return 1;
}

