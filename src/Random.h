#include "raylib.h"
#include <algorithm>
//return 0-1
inline float GetRandomFloat() {
	return GetRandomValue(0, 10000) / 10000.0f;
}
//return 0-max
inline float GetRandomFloat(float max) {
	return GetRandomFloat() * max;
}
//return min-max
inline float GetRandomFloat(float min, float max) {
	if (min > max) std::swap(min, max);
	return min + GetRandomFloat() * (max - min);
}
