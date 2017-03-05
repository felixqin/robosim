#ifndef CONFIG_H__
#define CONFIG_H__

#include "../precomp.h"


namespace config
{
class IOImpl;
class SectionImpl;
}

typedef config::IOImpl* ConfigReader;
typedef config::SectionImpl* ConfigSection;


extern "C" SDK_API ConfigReader CreateConfigReader(const char* fname);
extern "C" SDK_API void DestroyConfigReader(ConfigReader cfg);

extern "C" SDK_API ConfigSection GetConfigSection(ConfigReader cfg, const char* sect_name);
extern "C" SDK_API const char* GetConfigValue(ConfigSection sec, const char* val_name);

#endif
