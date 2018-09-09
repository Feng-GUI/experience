#pragma once

#include <inttypes.h>
#include <stdint.h>


extern "C" {
#pragma warning( push )
#pragma warning( disable: 4201 )
#include <libobs/util/base.h>
#include <libobs/obs-source.h>
#include <libobs/obs-module.h>
#include <libobs/graphics/matrix4.h>
#include <libobs/obs-source.h>
#include <libobs/graphics/graphics.h>
#include <libobs/graphics/image-file.h>
#include <libobs/util/threading.h>
#include <libobs/util/platform.h>
#include <libobs/util/dstr.h>
#include <libobs/util/darray.h>
#pragma warning( pop )
}


// Plugin
#define PLUGIN_ID			"fortnite-justdance-plugin"
#define PLUGIN_NAME			"Fortnite Justdance Plugin"
#define PLOG(level, ...)		blog(level, "[" PLUGIN_NAME "] " __VA_ARGS__);
#define PLOG_ERROR(...)			PLOG(LOG_ERROR,   __VA_ARGS__)
#define PLOG_WARNING(...)		PLOG(LOG_WARNING, __VA_ARGS__)
#define PLOG_INFO(...)			PLOG(LOG_INFO,    __VA_ARGS__)
#define PLOG_DEBUG(...)			PLOG(LOG_DEBUG,   __VA_ARGS__)

namespace Plugin {
	template <typename T>
	static T clamp(T val, T low, T high) {
		return (val < low ? low : (val > high ? high : val));
	}

	template <typename T>
	static T max(T val, T high) {
		return (val > high ? val : high);
	}

	template <typename T>
	static T min(T val, T low) {
		return (val < low ? val : low);
	}

	template <typename T>
	static T wrap(T val, T low, T high) {
		if (low > high) { // Swap if high is smaller than low.
			std::swap(low, high);
		}
		T size = high - low;
		return (val - (size * floor((val - low) / size)));
	}
}

#ifndef __FUNCTION_NAME__
#if defined(_WIN32) || defined(_WIN64)   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__  
#else          //*NIX
#define __FUNCTION_NAME__   __func__ 
#endif
#endif
