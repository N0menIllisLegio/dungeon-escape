#pragma once
#include <chrono>

struct WinParams
{
	static const int BLOCK_WIDTH = 16;
	static const int BLOCK_HEIGHT = 16;

	static std::chrono::high_resolution_clock clock;
};