#include "Tools.h"

char* randomStrGen(int length)
{
	static std::atomic<int> id{ 0 }; // #include <atomic>
	int inc = id.fetch_add(1, std::memory_order_relaxed) + 1;

	srand((unsigned)GetTickCount() % 10000 * (inc * inc + inc));

	static std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	std::string result;
	result.resize(length);

	for (int i = 0; i < length; i++)
		result[i] = charset[rand() % charset.length()];

	char Cresult[20] = {};
	strcpy_s(Cresult, result.c_str());
	return Cresult;
}