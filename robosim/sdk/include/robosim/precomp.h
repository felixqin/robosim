#ifndef SETTING_H__
#define SETTING_H__


#if defined(_MSC_VER)
//	#pragma warning(disable:4099)
#endif


#if defined(DEBUG) && !defined(_DEBUG)
#   define _DEBUG
#endif


#if defined(_MSC_VER)
#   define DLL_IMPORT __declspec(dllimport)
#   define DLL_EXPORT __declspec(dllexport)
#else
#   define DLL_IMPORT
#   define DLL_EXPORT
#endif


#ifdef SDK_EXPORTS
#   define SDK_API      DLL_EXPORT
#else
#   define SDK_API      DLL_IMPORT
#endif // SDK_EXPORTS

#ifdef PLUGIN_EXPORTS
#   define PLUGIN_API   DLL_EXPORT
#else
#   define PLUGIN_API   DLL_IMPORT
#endif  // PLUGIN_EXPORTS


#endif //SETTING_H__
