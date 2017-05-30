#ifndef GFW_UTIL
#define GFW_UTIL

#define GFW_TYPE_UNKNOWN 0
#define GFW_TYPE_BLACK 1
#define GFW_TYPE_WHITE 2

int LoadGFWData(const char* conf);
int GetGFWType(const char* host);

#endif
