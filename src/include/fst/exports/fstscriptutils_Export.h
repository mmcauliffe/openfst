
#ifndef fstscriptutils_EXPORT_H
#define fstscriptutils_EXPORT_H

#ifdef fstscriptutils_BUILT_AS_STATIC
#  define fstscriptutils_EXPORT
#  define FSTSCRIPTUTILS_NO_EXPORT
#else
#  ifndef fstscriptutils_EXPORT
#    ifdef fstscriptutils_EXPORTS
        /* We are building this library */
#      define fstscriptutils_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define fstscriptutils_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef FSTSCRIPTUTILS_NO_EXPORT
#    define FSTSCRIPTUTILS_NO_EXPORT 
#  endif
#endif

#ifndef FSTSCRIPTUTILS_DEPRECATED
#  define FSTSCRIPTUTILS_DEPRECATED __declspec(deprecated)
#endif

#ifndef FSTSCRIPTUTILS_DEPRECATED_EXPORT
#  define FSTSCRIPTUTILS_DEPRECATED_EXPORT fstscriptutils_EXPORT FSTSCRIPTUTILS_DEPRECATED
#endif

#ifndef FSTSCRIPTUTILS_DEPRECATED_NO_EXPORT
#  define FSTSCRIPTUTILS_DEPRECATED_NO_EXPORT FSTSCRIPTUTILS_NO_EXPORT FSTSCRIPTUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef FSTSCRIPTUTILS_NO_DEPRECATED
#    define FSTSCRIPTUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* fstscriptutils_EXPORT_H */
