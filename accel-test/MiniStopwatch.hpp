#pragma once
#include <windows.h>

namespace accel::test {

	class MiniStopwatch {
	private:
		LARGE_INTEGER _Start;
		LARGE_INTEGER _Stop;
		LARGE_INTEGER _Frequency;
	public:
		MiniStopwatch() : _Start{ }, _Stop{ } {
			QueryPerformanceFrequency(&_Frequency);
		}
		
		VOID Start() {
			QueryPerformanceCounter(&_Start);
			_Stop.QuadPart = 0;
		}

		VOID Stop() {
			QueryPerformanceCounter(&_Stop);
		}

		DOUBLE GetSecond() {
			if (_Stop.QuadPart == 0)
				return 0.0;

			LONGLONG Count = _Stop.QuadPart - _Start.QuadPart;
			return static_cast<DOUBLE>(Count) / static_cast<DOUBLE>(_Frequency.QuadPart);
		}

		LONGLONG GetTick() {
			return _Stop.QuadPart - _Start.QuadPart;
		}
	};
}
