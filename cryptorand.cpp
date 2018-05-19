//Core Particle Based PRNG Class
//Author Sean Donachiue

#include "cryptorand.h"
#include "stdafx.h"


CryptoRand::CryptoRand() { m_particle = new Particle; }
CryptoRand::~CryptoRand() {
	delete m_particle;
	m_particle = nullptr;
	particle_container.clear();
}
//Initialize 16 particles to obtain 32 bytes
void CryptoRand::init(int num_bytes_to_gen, char* soundBuf) {
	auto t = Clock::now();
	float small_dt = 13;
	for (int i = 1; i < num_bytes_to_gen / 2 + 1; i++) {
		int a = (int)(small_dt) % 10;
		int b = (int)(small_dt / 10.f) % 10;
		small_dt = a + b * 10;

		int xpos = (int)soundBuf[(int)(i * small_dt)] % 64;
		int ypos = (int)soundBuf[(int)(i * 2 * small_dt)] % 64;
		int xvel = (int)soundBuf[(int)(i * 3 * small_dt)] % 256;
		int yvel = (int)soundBuf[(int)(i * 4 * small_dt)] % 256;
		
		//prevent velocities of 0
		if (xvel == 0) {
			xvel += -small_dt * (int)soundBuf[ypos];
			yvel += small_dt * (int)soundBuf[xpos];
		}
		else if (yvel == 0) {
			xvel += small_dt * (int)soundBuf[ypos];
			yvel += -small_dt * (int)soundBuf[xpos];
		}
		particle_container.emplace_back(xpos, ypos, xvel, yvel);
		std::cout << "time to init particle " << particle_container.size() <<  " is: " << small_dt << "\n";
		std::cout << "initial x: " << xpos << "\n";
		std::cout << "initial y: " << ypos << "\n";
		std::cout << "initial xvel: " << xvel << "\n";
		std::cout << "initial yvel: " << yvel << "\n";

		auto now = Clock::now();
		small_dt = (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count();
		t = now;

		update(small_dt);

	}
}

void CryptoRand::initOne(float dt, char * soundBuf, vec2 v) {
	int a = (int)dt % 10;
	int b = (int)(dt / 10) % 10;
	int lowdigits = a + b * 10;

	int xpos = (int)(soundBuf[v.x] + lowdigits) % 64;
	int ypos = (int)(soundBuf[v.y] + lowdigits) % 64;
	int xvel = (int)(soundBuf[(int)(6 * dt)] - lowdigits) % 256;
	int yvel = (int)(soundBuf[(int)(7 * dt)] - lowdigits) % 256;
	particle_container.emplace_back(xpos, ypos, xvel, yvel);
	update(dt);
}

void CryptoRand::update(float dt) {
	for (int i = 0; i < particle_container.size(); i++) {
		particle_container[i].p_update(dt);
	}
}

void CryptoRand::pullRandom32(char* soundBuf, char output[], int num_bytes) {
	auto t = Clock::now();
	auto t2 = Clock::now();
	float loop_dt;
	for(int j = 0; j < num_bytes; j ++) {
		int random_index = getLowOrderClockDigits(t2) % (particle_container.size());
		t2 = Clock::now();
		//std::cout << random_index << std::endl;			
			
		memcpy(m_particle, &particle_container[random_index], sizeof(Particle));

		//split cases for x and y by even and odd iterations and only take one at a time
		//but delete the entire particle just the same, in hopes of removing the high incidence of duplicate values.
		char out = '0';
		char x = base64_chars[m_particle->m_pos.x];
		char y = base64_chars[m_particle->m_pos.y];
		if (j % 2 == 0)
			out = x;
		else
			out = y;
		//occasionally characters outside the base64 charset appear in outputs, this checks and prevents that.
		if (out == 43 || (out > 46 && out < 58) || (out > 64 && out < 91) || (out > 96 && out < 123)) {
			output[j] = out;
		}
		else j--;
		//output[j + 1] = y;
		
		particle_container.erase(particle_container.begin() + random_index);
		auto now = Clock::now();
		loop_dt = (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count();
		initOne(loop_dt, soundBuf, m_particle->m_vel);
		t = Clock::now();
	}
	//std::cout << output << std::endl;
}

int CryptoRand::getLowOrderClockDigits(Clock::time_point oldT) {
	float time = (float)(std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - oldT)).count();
	if (time < 1) {
		time *= 100;
	}
	int a = (int)time % 10;
	int b = ((int)time / 10) % 10;
	return a + b * 10;
}

