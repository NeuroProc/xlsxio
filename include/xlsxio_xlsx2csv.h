#ifndef XLSXIO_XLSX2CSV_H
#define XLSXIO_XLSX2CSV_H


#include <stdlib.h>
#if defined(_MSC_VER) && _MSC_VER < 1600
typedef signed __int64 int64_t;
#else
#include <stdint.h>
#endif
#include <time.h>

/*! \cond PRIVATE */
#ifndef DLL_EXPORT_XLSXIO
#ifdef _WIN32
#if defined(BUILD_XLSXIO_DLL) || defined(BUILD_XLSXIO_SHARED)
#define DLL_EXPORT_XLSXIO __declspec(dllexport)
#elif !defined(STATIC) && !defined(BUILD_XLSXIO_STATIC) && !defined(BUILD_XLSXIO)
#define DLL_EXPORT_XLSXIO __declspec(dllimport)
#else
#define DLL_EXPORT_XLSXIO
#endif
#else
#define DLL_EXPORT_XLSXIO
#endif
#endif
/*! \endcond */

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORT_XLSXIO int xlsx2csv (int argc, char* argv[]);

#ifdef __cplusplus
}
#endif

#endif // XLSXIO_XLSX2CSV_H
