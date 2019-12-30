
// This is a generated file. Do not edit!

#ifndef USQUE_COMPILER_DETECTION_H
#define USQUE_COMPILER_DETECTION_H

#ifdef __cplusplus
# define USQUE_COMPILER_IS_Comeau 0
# define USQUE_COMPILER_IS_Intel 0
# define USQUE_COMPILER_IS_PathScale 0
# define USQUE_COMPILER_IS_Embarcadero 0
# define USQUE_COMPILER_IS_Borland 0
# define USQUE_COMPILER_IS_Watcom 0
# define USQUE_COMPILER_IS_OpenWatcom 0
# define USQUE_COMPILER_IS_SunPro 0
# define USQUE_COMPILER_IS_HP 0
# define USQUE_COMPILER_IS_Compaq 0
# define USQUE_COMPILER_IS_zOS 0
# define USQUE_COMPILER_IS_XLClang 0
# define USQUE_COMPILER_IS_XL 0
# define USQUE_COMPILER_IS_VisualAge 0
# define USQUE_COMPILER_IS_PGI 0
# define USQUE_COMPILER_IS_Cray 0
# define USQUE_COMPILER_IS_TI 0
# define USQUE_COMPILER_IS_Fujitsu 0
# define USQUE_COMPILER_IS_GHS 0
# define USQUE_COMPILER_IS_SCO 0
# define USQUE_COMPILER_IS_ARMCC 0
# define USQUE_COMPILER_IS_AppleClang 0
# define USQUE_COMPILER_IS_ARMClang 0
# define USQUE_COMPILER_IS_Clang 0
# define USQUE_COMPILER_IS_GNU 0
# define USQUE_COMPILER_IS_MSVC 0
# define USQUE_COMPILER_IS_ADSP 0
# define USQUE_COMPILER_IS_IAR 0
# define USQUE_COMPILER_IS_MIPSpro 0

#if defined(__COMO__)
# undef USQUE_COMPILER_IS_Comeau
# define USQUE_COMPILER_IS_Comeau 1

#elif defined(__INTEL_COMPILER) || defined(__ICC)
# undef USQUE_COMPILER_IS_Intel
# define USQUE_COMPILER_IS_Intel 1

#elif defined(__PATHCC__)
# undef USQUE_COMPILER_IS_PathScale
# define USQUE_COMPILER_IS_PathScale 1

#elif defined(__BORLANDC__) && defined(__CODEGEARC_VERSION__)
# undef USQUE_COMPILER_IS_Embarcadero
# define USQUE_COMPILER_IS_Embarcadero 1

#elif defined(__BORLANDC__)
# undef USQUE_COMPILER_IS_Borland
# define USQUE_COMPILER_IS_Borland 1

#elif defined(__WATCOMC__) && __WATCOMC__ < 1200
# undef USQUE_COMPILER_IS_Watcom
# define USQUE_COMPILER_IS_Watcom 1

#elif defined(__WATCOMC__)
# undef USQUE_COMPILER_IS_OpenWatcom
# define USQUE_COMPILER_IS_OpenWatcom 1

#elif defined(__SUNPRO_CC)
# undef USQUE_COMPILER_IS_SunPro
# define USQUE_COMPILER_IS_SunPro 1

#elif defined(__HP_aCC)
# undef USQUE_COMPILER_IS_HP
# define USQUE_COMPILER_IS_HP 1

#elif defined(__DECCXX)
# undef USQUE_COMPILER_IS_Compaq
# define USQUE_COMPILER_IS_Compaq 1

#elif defined(__IBMCPP__) && defined(__COMPILER_VER__)
# undef USQUE_COMPILER_IS_zOS
# define USQUE_COMPILER_IS_zOS 1

#elif defined(__ibmxl__) && defined(__clang__)
# undef USQUE_COMPILER_IS_XLClang
# define USQUE_COMPILER_IS_XLClang 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ >= 800
# undef USQUE_COMPILER_IS_XL
# define USQUE_COMPILER_IS_XL 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ < 800
# undef USQUE_COMPILER_IS_VisualAge
# define USQUE_COMPILER_IS_VisualAge 1

#elif defined(__PGI)
# undef USQUE_COMPILER_IS_PGI
# define USQUE_COMPILER_IS_PGI 1

#elif defined(_CRAYC)
# undef USQUE_COMPILER_IS_Cray
# define USQUE_COMPILER_IS_Cray 1

#elif defined(__TI_COMPILER_VERSION__)
# undef USQUE_COMPILER_IS_TI
# define USQUE_COMPILER_IS_TI 1

#elif defined(__FUJITSU) || defined(__FCC_VERSION) || defined(__fcc_version)
# undef USQUE_COMPILER_IS_Fujitsu
# define USQUE_COMPILER_IS_Fujitsu 1

#elif defined(__ghs__)
# undef USQUE_COMPILER_IS_GHS
# define USQUE_COMPILER_IS_GHS 1

#elif defined(__SCO_VERSION__)
# undef USQUE_COMPILER_IS_SCO
# define USQUE_COMPILER_IS_SCO 1

#elif defined(__ARMCC_VERSION) && !defined(__clang__)
# undef USQUE_COMPILER_IS_ARMCC
# define USQUE_COMPILER_IS_ARMCC 1

#elif defined(__clang__) && defined(__apple_build_version__)
# undef USQUE_COMPILER_IS_AppleClang
# define USQUE_COMPILER_IS_AppleClang 1

#elif defined(__clang__) && defined(__ARMCOMPILER_VERSION)
# undef USQUE_COMPILER_IS_ARMClang
# define USQUE_COMPILER_IS_ARMClang 1

#elif defined(__clang__)
# undef USQUE_COMPILER_IS_Clang
# define USQUE_COMPILER_IS_Clang 1

#elif defined(__GNUC__) || defined(__GNUG__)
# undef USQUE_COMPILER_IS_GNU
# define USQUE_COMPILER_IS_GNU 1

#elif defined(_MSC_VER)
# undef USQUE_COMPILER_IS_MSVC
# define USQUE_COMPILER_IS_MSVC 1

#elif defined(__VISUALDSPVERSION__) || defined(__ADSPBLACKFIN__) || defined(__ADSPTS__) || defined(__ADSP21000__)
# undef USQUE_COMPILER_IS_ADSP
# define USQUE_COMPILER_IS_ADSP 1

#elif defined(__IAR_SYSTEMS_ICC__) || defined(__IAR_SYSTEMS_ICC)
# undef USQUE_COMPILER_IS_IAR
# define USQUE_COMPILER_IS_IAR 1


#endif

#  if USQUE_COMPILER_IS_Clang

#    if !(((__clang_major__ * 100) + __clang_minor__) >= 301)
#      error Unsupported compiler version
#    endif

# define USQUE_COMPILER_VERSION_MAJOR (__clang_major__)
# define USQUE_COMPILER_VERSION_MINOR (__clang_minor__)
# define USQUE_COMPILER_VERSION_PATCH (__clang_patchlevel__)
# if defined(_MSC_VER)
   /* _MSC_VER = VVRR */
#  define USQUE_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#  define USQUE_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
# endif

#    if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_constexpr)
#      define USQUE_COMPILER_CXX_CONSTEXPR 1
#    else
#      define USQUE_COMPILER_CXX_CONSTEXPR 0
#    endif

#    if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_deleted_functions)
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 1
#    else
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 0
#    endif

#    if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __cplusplus >= 201103L
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 1
#    else
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 0
#    endif

#    if ((__clang_major__ * 100) + __clang_minor__) >= 301 && __has_feature(cxx_thread_local)
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 1
#    else
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 0
#    endif

#  elif USQUE_COMPILER_IS_GNU

#    if !((__GNUC__ * 100 + __GNUC_MINOR__) >= 404)
#      error Unsupported compiler version
#    endif

# if defined(__GNUC__)
#  define USQUE_COMPILER_VERSION_MAJOR (__GNUC__)
# else
#  define USQUE_COMPILER_VERSION_MAJOR (__GNUG__)
# endif
# if defined(__GNUC_MINOR__)
#  define USQUE_COMPILER_VERSION_MINOR (__GNUC_MINOR__)
# endif
# if defined(__GNUC_PATCHLEVEL__)
#  define USQUE_COMPILER_VERSION_PATCH (__GNUC_PATCHLEVEL__)
# endif

#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && (__cplusplus >= 201103L || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_CONSTEXPR 1
#    else
#      define USQUE_COMPILER_CXX_CONSTEXPR 0
#    endif

#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 && (__cplusplus >= 201103L || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 1
#    else
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 0
#    endif

#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 404 && (__cplusplus >= 201103L || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 1
#    else
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 0
#    endif

#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 408 && __cplusplus >= 201103L
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 1
#    else
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 0
#    endif

#  elif USQUE_COMPILER_IS_Intel

#    if !(__INTEL_COMPILER >= 1210)
#      error Unsupported compiler version
#    endif

  /* __INTEL_COMPILER = VRP */
# define USQUE_COMPILER_VERSION_MAJOR (__INTEL_COMPILER/100)
# define USQUE_COMPILER_VERSION_MINOR (__INTEL_COMPILER/10 % 10)
# if defined(__INTEL_COMPILER_UPDATE)
#  define USQUE_COMPILER_VERSION_PATCH (__INTEL_COMPILER_UPDATE)
# else
#  define USQUE_COMPILER_VERSION_PATCH (__INTEL_COMPILER   % 10)
# endif
# if defined(__INTEL_COMPILER_BUILD_DATE)
  /* __INTEL_COMPILER_BUILD_DATE = YYYYMMDD */
#  define USQUE_COMPILER_VERSION_TWEAK (__INTEL_COMPILER_BUILD_DATE)
# endif
# if defined(_MSC_VER)
   /* _MSC_VER = VVRR */
#  define USQUE_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#  define USQUE_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
# endif
# if defined(__GNUC__)
#  define USQUE_SIMULATE_VERSION_MAJOR (__GNUC__)
# elif defined(__GNUG__)
#  define USQUE_SIMULATE_VERSION_MAJOR (__GNUG__)
# endif
# if defined(__GNUC_MINOR__)
#  define USQUE_SIMULATE_VERSION_MINOR (__GNUC_MINOR__)
# endif
# if defined(__GNUC_PATCHLEVEL__)
#  define USQUE_SIMULATE_VERSION_PATCH (__GNUC_PATCHLEVEL__)
# endif

#    if __cpp_constexpr >= 200704 || __INTEL_COMPILER >= 1400 && ((__cplusplus >= 201103L) || defined(__INTEL_CXX11_MODE__) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_CONSTEXPR 1
#    else
#      define USQUE_COMPILER_CXX_CONSTEXPR 0
#    endif

#    if __INTEL_COMPILER >= 1210 && ((__cplusplus >= 201103L) || defined(__INTEL_CXX11_MODE__) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 1
#    else
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 0
#    endif

#    if __INTEL_COMPILER >= 1210 && ((__cplusplus >= 201103L) || defined(__INTEL_CXX11_MODE__) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 1
#    else
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 0
#    endif

#    if __INTEL_COMPILER >= 1500 && ((__cplusplus >= 201103L) || defined(__INTEL_CXX11_MODE__) || defined(__GXX_EXPERIMENTAL_CXX0X__))
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 1
#    else
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 0
#    endif

#  elif USQUE_COMPILER_IS_MSVC

#    if !(_MSC_VER >= 1600)
#      error Unsupported compiler version
#    endif

  /* _MSC_VER = VVRR */
# define USQUE_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
# define USQUE_COMPILER_VERSION_MINOR (_MSC_VER % 100)
# if defined(_MSC_FULL_VER)
#  if _MSC_VER >= 1400
    /* _MSC_FULL_VER = VVRRPPPPP */
#   define USQUE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#  else
    /* _MSC_FULL_VER = VVRRPPPP */
#   define USQUE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
#  endif
# endif
# if defined(_MSC_BUILD)
#  define USQUE_COMPILER_VERSION_TWEAK (_MSC_BUILD)
# endif

#    if _MSC_VER >= 1900
#      define USQUE_COMPILER_CXX_CONSTEXPR 1
#    else
#      define USQUE_COMPILER_CXX_CONSTEXPR 0
#    endif

#    if _MSC_VER >= 1900
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 1
#    else
#      define USQUE_COMPILER_CXX_DELETED_FUNCTIONS 0
#    endif

#    if _MSC_VER >= 1900
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 1
#    else
#      define USQUE_COMPILER_CXX_FUNC_IDENTIFIER 0
#    endif

#    if _MSC_VER >= 1900
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 1
#    else
#      define USQUE_COMPILER_CXX_THREAD_LOCAL 0
#    endif

#  else
#    error Unsupported compiler
#  endif

#  if defined(USQUE_COMPILER_CXX_CONSTEXPR) && USQUE_COMPILER_CXX_CONSTEXPR
#    define USQUE_CONSTEXPR constexpr
#  else
#    define USQUE_CONSTEXPR 
#  endif


#  if defined(USQUE_COMPILER_CXX_DELETED_FUNCTIONS) && USQUE_COMPILER_CXX_DELETED_FUNCTIONS
#    define USQUE_DELETED_FUNCTION = delete
#  else
#    define USQUE_DELETED_FUNCTION 
#  endif


#  if defined(USQUE_COMPILER_CXX_THREAD_LOCAL) && USQUE_COMPILER_CXX_THREAD_LOCAL
#    define USQUE_THREAD_LOCAL thread_local
#  elif USQUE_COMPILER_IS_GNU || USQUE_COMPILER_IS_Clang || USQUE_COMPILER_IS_AppleClang
#    define USQUE_THREAD_LOCAL __thread
#  elif USQUE_COMPILER_IS_MSVC
#    define USQUE_THREAD_LOCAL __declspec(thread)
#  else
// USQUE_THREAD_LOCAL not defined for this configuration.
#  endif

#endif

#endif
