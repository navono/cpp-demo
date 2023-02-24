
#ifndef SHAREDLIB_EXPORT_H
#define SHAREDLIB_EXPORT_H

#ifdef SHAREDLIB_STATIC
#  define SHAREDLIB_EXPORT
#  define SHAREDLIB_NO_EXPORT
#else
#  ifndef SHAREDLIB_EXPORT
#    ifdef sharedLib_EXPORTS
        /* We are building this library */
#      define SHAREDLIB_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define SHAREDLIB_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef SHAREDLIB_NO_EXPORT
#    define SHAREDLIB_NO_EXPORT 
#  endif
#endif

#ifndef SHAREDLIB_DEPRECATED
#  define SHAREDLIB_DEPRECATED __declspec(deprecated)
#endif

#ifndef SHAREDLIB_DEPRECATED_EXPORT
#  define SHAREDLIB_DEPRECATED_EXPORT SHAREDLIB_EXPORT SHAREDLIB_DEPRECATED
#endif

#ifndef SHAREDLIB_DEPRECATED_NO_EXPORT
#  define SHAREDLIB_DEPRECATED_NO_EXPORT SHAREDLIB_NO_EXPORT SHAREDLIB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SHAREDLIB_NO_DEPRECATED
#    define SHAREDLIB_NO_DEPRECATED
#  endif
#endif

#endif /* SHAREDLIB_EXPORT_H */
