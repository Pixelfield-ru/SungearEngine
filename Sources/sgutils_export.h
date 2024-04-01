
#ifndef SGUTILS_EXPORT_H
#define SGUTILS_EXPORT_H

#ifdef SGUTILS_STATIC_DEFINE
#  define SGUTILS_EXPORT
#  define SGUTILS_NO_EXPORT
#else
#  ifndef SGUTILS_EXPORT
#    ifdef SGUtils_EXPORTS
        /* We are building this library */
#      define SGUTILS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define SGUTILS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SGUTILS_NO_EXPORT
#    define SGUTILS_NO_EXPORT 
#  endif
#endif

#ifndef SGUTILS_DEPRECATED
#  define SGUTILS_DEPRECATED __declspec(deprecated)
#endif

#ifndef SGUTILS_DEPRECATED_EXPORT
#  define SGUTILS_DEPRECATED_EXPORT SGUTILS_EXPORT SGUTILS_DEPRECATED
#endif

#ifndef SGUTILS_DEPRECATED_NO_EXPORT
#  define SGUTILS_DEPRECATED_NO_EXPORT SGUTILS_NO_EXPORT SGUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SGUTILS_NO_DEPRECATED
#    define SGUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* SGUTILS_EXPORT_H */
