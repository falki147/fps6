#pragma once

#include <chrono>

class Clock {
public:
	void start() {
		m_start = std::chrono::high_resolution_clock::now();
	}

	void stop() {
		m_end = std::chrono::high_resolution_clock::now();
	}

	auto durationInMilliseconds() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
	}

private:
	std::chrono::high_resolution_clock::time_point m_start{}, m_end{};
};
