#pragma once

#include <chrono>

typedef void (*FN_CALLBACK) (void); /* Callback Function, Does not receive parameters and returns nothing. */

typedef bool (*FN_BOOL_CALLBACK) (void); /* Callback Function, Does not receive parameters and returns a boolean. */

template <typename Clock, typename Duration>
std::tm to_calendar_time(std::chrono::time_point<Clock, Duration> tp)
{
	std::time_t now_c = std::chrono::system_clock::to_time_t(tp);
	std::tm time_tm = *std::localtime(&now_c);
	return time_tm;
}
