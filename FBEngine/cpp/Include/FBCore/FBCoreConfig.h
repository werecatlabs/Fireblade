#ifndef _FB_CONFIG_H
#define _FB_CONFIG_H

// detect compiler
#if defined( __GCCE__ )
#    define FB_COMPILER FB_COMPILER_GCCE
#    define FB_COMP_VER _MSC_VER
//#	include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE
#elif defined( __WINSCW__ )
#    define FB_COMPILER FB_COMPILER_WINSCW
#    define FB_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#    define FB_COMPILER FB_COMPILER_MSVC
#    define FB_COMP_VER _MSC_VER
#elif defined( __clang__ )
#    define FB_COMPILER FB_COMPILER_CLANG
#    define FB_COMP_VER ( ( (__clang_major__)*100 ) + ( __clang_minor__ * 10 ) + __clang_patchlevel__ )
#elif defined( __GNUC__ )
#    define FB_COMPILER FB_COMPILER_GNUC
#    define FB_COMP_VER ( ( (__GNUC__)*100 ) + ( __GNUC_MINOR__ * 10 ) + __GNUC_PATCHLEVEL__ )
#elif defined( __BORLANDC__ )
#    define FB_COMPILER FB_COMPILER_BORL
#    define FB_COMP_VER __BCPLUSPLUS__
#    define __FUNCTION__ __FUNC__
#else
#    pragma error "No known compiler."
#endif

// detect platform
#if defined( __WIN32__ ) || defined( _WIN32 )
#    if defined( WINAPI_FAMILY )
#        define __FB_HAVE_DIRECTXMATH 1
#        include <winapifamily.h>
#        if WINAPI_FAMILY == WINAPI_FAMILY_APP || WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#            define DESKTOP_APP 1
#            define PHONE 2
#            define FB_PLATFORM_WINRT
#            //define _CRT_SECURE_NO_WARNINGS
#            define _SCL_SECURE_NO_WARNINGS
#            if WINAPI_FAMILY == WINAPI_FAMILY_APP
#                define FB_WINRT_TARGET_TYPE DESKTOP_APP
#            endif
#            if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#                define FB_WINRT_TARGET_TYPE PHONE
#            endif
#        else
#            define FB_PLATFORM_WIN32
#        endif
#    else
#        define FB_PLATFORM_WIN32
#    endif
#elif defined( __FLASHCC__ )
#    define FB_PLATFORM_FLASHCC
#elif defined( __APPLE_CC__ )
#    if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || \
        __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
#        define FB_PLATFORM_IOS
#    else
#        define FB_PLATFORM_APPLE
#    endif
#elif defined( __ANDROID__ )
#    define FB_PLATFORM_ANDROID
#elif defined( __native_client__ )
#    define FB_PLATFORM_NACL
#else
#    define FB_PLATFORM_LINUX
#endif

#define FB_ARCHITECTURE_32 1
#define FB_ARCHITECTURE_64 2

/* Find the arch type */
#if defined( __x86_64__ ) || defined( _M_X64 ) || defined( __powerpc64__ ) || defined( __alpha__ ) || \
    defined( __ia64__ ) || defined( __s390__ ) || defined( __s390x__ ) || defined( __arm64__ ) || \
    defined( __aarch64__ ) || defined( __mips64 ) || defined( __mips64_ )
#    define FB_ARCH_TYPE FB_ARCHITECTURE_64
#else
#    define FB_ARCH_TYPE FB_ARCHITECTURE_32
#endif

#if defined( WIN32 ) || defined( __WATCOMC__ ) || defined( _WIN32 ) || defined( __WIN32__ )
#    define __PACKED /* dummy */
#else
#    define __PACKED __attribute__( ( packed ) ) /* gcc packed */
#endif

#ifdef FB_PLATFORM_WIN32
#    ifndef _FB_STATIC_LIB_
#        ifdef FBCore_EXPORTS
#            define FBCore_API __declspec( dllexport )
#        else
#            define FBCore_API __declspec( dllimport )
#        endif  // FB_EXPORT
#    else
#        define FBCore_API
#    endif  // _FB_STATIC_LIB_
#else
#    define FBCore_API
#endif

// Declare the calling convention.
#    ifndef _FB_STATIC_LIB_
#if defined( _STDCALL_SUPPORTED )
#    define FBCALLCONV __stdcall
#else
#    define FBCALLCONV  __cdecl
#endif                  
#else 
#    define FBCALLCONV
#endif

#if defined( _MSC_VER ) && _MSC_VER > 1200
#    if FB_USE_FORCE_INLINE
#        define FBForceInline __forceinline
#    else
#        define FBForceInline inline
#    endif
#elif defined( __clang__ )
#    if FB_USE_FORCE_INLINE
#        define FBForceInline inline __attribute__((always_inline))
#    else
#        define FBForceInline inline
#    endif
#elif defined( __MINGW32__ )
#    if FB_USE_FORCE_INLINE
#        define FBForceInline __inline
#    else
#        define FBForceInline inline
#    endif
#else
#    if FB_USE_FORCE_INLINE
#        define FBForceInline inline
#    else
#        define FBForceInline inline
#    endif
#endif

#if !defined( _MSC_VER )
#    define FB_THREAD_LOCAL_STORAGE thread_local
#else
#    define FB_THREAD_LOCAL_STORAGE thread_local
#endif

#if defined( _MSC_VER )
#    define FB_USE_INTERLOCKED_FUNCTIONS
#endif

#if defined( __sparc__ ) || defined( __sun__ )
#    define __BIG_ENDIAN__
#endif

#if defined FB_PLATFORM_WIN32
#    define FB_NOEXCEPT noexcept
#else
#    define FB_NOEXCEPT _NOEXCEPT
#endif

#define NOMINMAX  

#if FB_ENABLE_MEMORY_TRACKER
#    if FB_USE_CUSTOM_NEW_DELETE
#        define FB_NEW new( __FILE__, __LINE__, __FUNCTION__ ? __FUNCTION__ : "" )
#        define FB_DELETE delete
#    else
#        define FB_NEW new
#        define FB_DELETE delete
#    endif
#else
#    define FB_NEW new
#    define FB_DELETE delete
#endif

#define FB_ENDIAN_LITTLE 1
#define FB_ENDIAN_BIG 2

// endian config
#ifdef FB_CONFIG_BIG_ENDIAN
#    define FB_ENDIAN FB_ENDIAN_BIG
#else
#    define FB_ENDIAN FB_ENDIAN_LITTLE
#endif

#if FB_RESTRICT_ALIASING != 0
#    if FB_COMPILER == FB_COMPILER_MSVC
#        define RESTRICT_ALIAS __restrict  // MSVC
#        define RESTRICT_ALIAS_RETURN __restrict
#    else
#        define RESTRICT_ALIAS __restrict__  // GCC... and others?
#        define RESTRICT_ALIAS_RETURN
#    endif
#else
#    define RESTRICT_ALIAS
#    define RESTRICT_ALIAS_RETURN
#endif

#define FB_MEMORY_ALLOCATOR 0

// define the memory allocator configuration to use
#define FB_MEMORY_ALLOCATOR_NONE 0
#define FB_MEMORY_ALLOCATOR_STD 1
#define FB_MEMORY_ALLOCATOR_USER 3
#define FB_MEMORY_ALLOCATOR_TRACK 5

#define FB_COMPILER_MSVC 1
#define FB_COMPILER_GNUC 2
#define FB_COMPILER_BORL 3
#define FB_COMPILER_WINSCW 4
#define FB_COMPILER_GCCE 5
#define FB_COMPILER_CLANG 6

#if FB_COMPILER == FB_COMPILER_MSVC
#    define FB_DECL_MALLOC __declspec( restrict ) __declspec( noalias )
#else
#    define FB_DECL_MALLOC __attribute__( ( malloc ) )
#endif

#define FB_CPP_2020 202002L
#define FB_CPP_2017 201703L
#define FB_CPP_2011 201103L
#define FB_CPP_STANDARD __cplusplus

#endif
