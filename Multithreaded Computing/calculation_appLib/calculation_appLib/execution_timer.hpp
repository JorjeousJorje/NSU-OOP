#pragma once
#include <chrono>


class timer
{
public:
	timer() : start_(std::chrono::high_resolution_clock::now()) {}

	auto time() {
		current_ = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<float> duration = current_ - start_;
		return duration.count();
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> start_, current_;
};