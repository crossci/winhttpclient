#ifdef _DLL_EXPORT
#define DLL_SAMPLE_API __declspec(dllexport)
#else
#define DLL_SAMPLE_API __declspec(dllimport)
#endif

#define MD5_DIGEST_LENGTH 16
