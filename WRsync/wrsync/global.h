#pragma once

// Windows dll export declarations
#ifdef _WIN32
	#ifdef DLL_BUILD
		#define WRSYNC_API __declspec(dllexport)
	#else
		#define WRSYNC_API __declspec(dllimport)
	#endif
#else
	#define WRSYNC_API
#endif
