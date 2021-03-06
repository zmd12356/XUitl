#ifndef __DEF_H__
#define __DEF_H__

// namespace don {}
#define NS_ZMD_BEGIN                     namespace zmd {
#define NS_ZMD_END                       }
#define USING_NS_ZMD                     using namespace zmd

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef _WIN32
#define	XSPRINTF(out, out_size, format, ...)	snprintf(out, out_size, format, __VA_ARGS__)
#else
#define XSPRINTF(out, out_size, format, ...)	sprintf_s(out, out_size, format, __VA_ARGS__)
#endif

#endif	//__DEF_H__
