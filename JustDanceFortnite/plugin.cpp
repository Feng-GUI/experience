
#include "plugin.h"
#include "FortniteFilter.h"

static Plugin::FortniteFilter* g_fortniteFilter;

OBS_DECLARE_MODULE();
//OBS_MODULE_AUTHOR("Author");
//OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_ID, "en-US");

MODULE_EXPORT bool obs_module_load(void) {
	g_fortniteFilter = new Plugin::FortniteFilter();
	return true;
}

MODULE_EXPORT void obs_module_unload(void) {
	delete g_fortniteFilter;
}

MODULE_EXPORT const char* obs_module_name() {
	static const char pluginName[] = PLUGIN_NAME;
	return pluginName;
}

MODULE_EXPORT const char* obs_module_description() {
	static const char pluginDescription[] = "This plugin adds a Fortnite Justdance Filter.";
	return pluginDescription;
}

#ifdef _WIN32
#include "windows.h"

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) {
	return TRUE;
}
#endif
