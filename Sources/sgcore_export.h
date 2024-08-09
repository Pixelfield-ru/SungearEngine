
#ifndef SGCORE_EXPORT_H
#define SGCORE_EXPORT_H

#ifdef SGCORE_STATIC_DEFINE
#  define SGCORE_EXPORT
#  define SGCORE_NO_EXPORT
#else
#  ifndef SGCORE_EXPORT
#    ifdef SGCore_EXPORTS
        /* We are building this library */
#      if defined(_MSC_VER)
#      define SGCORE_EXPORT __declspec(dllexport)
#      elif defined(__GNUC__)
#      define SGCORE_EXPORT __attribute__((visibility("default")))
#      endif
#    else
        /* We are using this library */
#      if defined(_MSC_VER)
#      define SGCORE_EXPORT __declspec(dllimport)
#      elif defined(__GNUC__)
#      define SGCORE_EXPORT __attribute__((visibility("default")))
#      endif
#    endif
#  endif

#  ifndef SGCORE_NO_EXPORT
#    define SGCORE_NO_EXPORT 
#  endif
#endif

#ifndef SGCORE_DEPRECATED
#  if defined(_MSC_VER)
#  define SGCORE_DEPRECATED __declspec(deprecated)
#  elif defined(__GNUC__)
#  define SGCORE_DEPRECATED __attribute__((deprecated))
#  endif
#endif

#ifndef SGCORE_DEPRECATED_EXPORT
#  define SGCORE_DEPRECATED_EXPORT SGCORE_EXPORT SGCORE_DEPRECATED
#endif

#ifndef SGCORE_DEPRECATED_NO_EXPORT
#  define SGCORE_DEPRECATED_NO_EXPORT SGCORE_NO_EXPORT SGCORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SGCORE_NO_DEPRECATED
#    define SGCORE_NO_DEPRECATED
#  endif
#endif

#endif /* SGCORE_EXPORT_H */
