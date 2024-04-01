
#ifndef SGUTILS_EXPORT_H
#define SGUTILS_EXPORT_H

#ifdef SGUTILS_STATIC_DEFINE
#  define SGUTILS_EXPORT
#  define SGUTILS_NO_EXPORT
#else
#  ifndef SGUTILS_EXPORT
#    ifdef SGUtils_EXPORTS
        /* We are building this library */
#      if defined(_MSC_VER)
#      define SGUTILS_EXPORT __declspec(dllexport)
#      elif defined(__GNUC__)
#      define SGUTILS_EXPORT __attribute__((visibility("default")))
#      endif
#    else
        /* We are using this library */
#      if defined(_MSC_VER)
#      define SGUTILS_EXPORT __declspec(dllimport)
#      elif defined(__GNUC__)
#      define SGUTILS_EXPORT __attribute__((visibility("default")))
#      endif
#    endif
#  endif

#  ifndef SGUTILS_NO_EXPORT
#    define SGUTILS_NO_EXPORT 
#  endif
#endif

#ifndef SGUTILS_DEPRECATED
#  if defined(_MSC_VER)
#  define SGUTILS_DEPRECATED __declspec(deprecated)
#  elif defined(__GNUC__)
#  define SGUTILS_DEPRECATED __attribute__((deprecated))
#  endif
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
