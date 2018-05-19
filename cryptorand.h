//Core Particle Based PRNG Class
//Author Sean Donachiue

#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <vector>
using Clock = std::chrono::high_resolution_clock;
static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
class CryptoRand
{
public:
	CryptoRand();
	~CryptoRand();

	void init(int num_bytes_to_gen, char* soundBuf);
	void update(float dt);
	void pullRandom32(char* soundBuf, char output[], int num_bytes);

private:
	struct vec2 { int x, y; };
	struct vec3 { int x, y, z; };

	struct Particle {
		vec2 m_pos;
		vec2 m_vel;
		Particle() {
			m_pos = { 0,0 };
			m_vel = { 0,0 };
		}
		Particle(int px, int py, int vx, int vy) {
			m_pos.x = px;
			m_pos.y = py;
			m_vel.x = vx;
			m_vel.y = vy;
		}
		void p_update(float dt) {
			m_pos.x += ((int)(m_vel.x * dt/1000.f) + 1);				//sometimes velocity values are very small
			m_pos.y += ((int)(m_vel.y * dt/1000.f) + 1);
			//Wrap coords in range of base64 encoding
			if (m_pos.x > 63 || m_pos.x < 0) m_pos.x %= 64;
			if (m_pos.y > 63 || m_pos.y < 0) m_pos.y %= 64;
		}
	};
	std::vector<Particle> particle_container;
	Particle* m_particle;

	void initOne(float dt, char* soundBuf, vec2 v);
	int getLowOrderClockDigits(Clock::time_point oldT);
};