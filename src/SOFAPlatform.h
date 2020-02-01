/*
Copyright (c) 2013--2017, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**

Spatial acoustic data file format - AES69-2015 - Standard for File Exchange - Spatial Acoustic Data File Format
http://www.aes.org

SOFA (Spatially Oriented Format for Acoustics)
http://www.sofaconventions.org

*/


/************************************************************************************/
/*!
 *   @file       SOFAPlatform.h
 *   @brief      Configure the platform dependent macros
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#ifndef _SOFA_PLATFORM_H__
#define _SOFA_PLATFORM_H__

#include "../src/SOFAVersion.h"

//==============================================================================
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <string>
#include <iosfwd>
#include <cassert>
#include <assert.h>

//==============================================================================
// GCC compiler
//==============================================================================
#if (__cplusplus >= 201103L || defined (__GXX_EXPERIMENTAL_CXX0X__)) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 405

    #define SOFA_COMPILER_SUPPORTS_NOEXCEPT 1
    #define SOFA_COMPILER_SUPPORTS_NULLPTR 1
    #define SOFA_COMPILER_SUPPORTS_MOVE_SEMANTICS 1

    #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined ( SOFA_COMPILER_SUPPORTS_OVERRIDE )
        #define SOFA_COMPILER_SUPPORTS_OVERRIDE 1
    #endif

    /// This version of GCC supports constexpr, but not extended constexpr (C++14)
    #define SOFA_COMPILER_SUPPORTS_CONSTEXPR 0

    #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && ! defined( SOFA_COMPILER_SUPPORTS_LAMBDAS )
        #define SOFA_COMPILER_SUPPORTS_LAMBDAS 1
    #endif

    #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined( SOFA_COMPILER_SUPPORTS_OVERRIDE )
        #define SOFA_COMPILER_SUPPORTS_OVERRIDE 1
    #endif

    #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined( SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION )
        #define SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION 1
    #endif

#endif

//==============================================================================
// clang compiler
//==============================================================================
#if defined( __clang__ )

    #if( !defined (__has_feature) )
        #error "This version of Clang is not supported!"
    #endif

    #if __has_feature( cxx_nullptr )
        #define SOFA_COMPILER_SUPPORTS_NULLPTR 1
    #endif

    #if __has_feature( cxx_noexcept )
        #define SOFA_COMPILER_SUPPORTS_NOEXCEPT 1
    #endif

    #if __has_feature( cxx_override_control )
        #define SOFA_COMPILER_SUPPORTS_OVERRIDE 1
    #endif

    #if __has_feature( cxx_rvalue_references )
        #define SOFA_COMPILER_SUPPORTS_MOVE_SEMANTICS 1
    #endif

    #if __has_feature( cxx_deleted_functions )
        #define SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION 1
    #endif

    #if __has_feature( cxx_constexpr )
        #define SOFA_COMPILER_SUPPORTS_CONSTEXPR 1
    #endif

    #if __has_feature( cxx_lambdas )
        #define SOFA_COMPILER_SUPPORTS_LAMBDAS 1
    #endif

#endif

//==============================================================================
// MSVC compiler
//==============================================================================
#if defined (_MSC_VER) 

    #if _MSC_VER < 1600
        #error "Visual Studio 2008 and earlier are no longer supported!"
    #endif

    #if( _MSC_VER >= 1600 )
        #define SOFA_COMPILER_SUPPORTS_NULLPTR 1
        #define SOFA_COMPILER_SUPPORTS_MOVE_SEMANTICS 1
    #endif

    #if( _MSC_VER >= 1700 )
        #define SOFA_COMPILER_SUPPORTS_OVERRIDE 1
        #define SOFA_COMPILER_SUPPORTS_LAMBDAS 1
    #endif

    #if( _MSC_VER >= 1800 )
        #define SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION 1
    #endif

    #if( _MSC_VER >= 1900 )
        #define SOFA_COMPILER_SUPPORTS_NOEXCEPT 1

        /// This version of Visual supports constexpr, but not extended constexpr (C++14)
        #define SOFA_COMPILER_SUPPORTS_CONSTEXPR 0
    #endif

#endif

//==============================================================================
// override
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_OVERRIDE == 1 )
    #define SOFA_OVERRIDE override
#else
    #define SOFA_OVERRIDE
#endif

//==============================================================================
// final
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_OVERRIDE == 1 )
    #define SOFA_FINAL final
#else
    #define SOFA_FINAL
#endif

//==============================================================================
// override + final
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_OVERRIDE == 1 )
    #define SOFA_OVERRIDE_FINAL SOFA_OVERRIDE SOFA_FINAL
#else
    #define SOFA_OVERRIDE_FINAL
#endif

//==============================================================================
// constexpr
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_CONSTEXPR == 1 )
    #define SOFA_CONSTEXPR constexpr
#else
    #define SOFA_CONSTEXPR const
#endif

//==============================================================================
// nullptr
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_NULLPTR == 0 )
    #ifndef nullptr
        #define nullptr NULL
    #endif
#endif

//==============================================================================
// delete
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION == 1 )
    #define SOFA_DELETED_FUNCTION = delete
#else
    #define SOFA_DELETED_FUNCTION
#endif

//==============================================================================
// noexcept
//==============================================================================
/**
 noexcept is an improved version of throw(), which is deprecated in C++11.
 Unlike throw(), noexcept will not call std::unexpected and may or may not unwind the stack,
 which potentially allows the compiler to implement noexcept without the runtime overhead of throw().
 */
#if( SOFA_COMPILER_SUPPORTS_NOEXCEPT == 1 )
    #define SOFA_NOEXCEPT  noexcept
#else
    #define SOFA_NOEXCEPT  throw()
#endif

//==============================================================================
// assert
//==============================================================================
#define SOFA_ASSERT( expr ) assert( expr )


//==============================================================================
// shorthand macro for declaring stubs for a class's copy constructor and operator=
//==============================================================================
#if ( SOFA_COMPILER_SUPPORTS_DELETED_FUNCTION == 1 )

    #define SOFA_AVOID_COPY_CONSTRUCTOR( ClassName ) \
        ClassName( const ClassName & other ) SOFA_DELETED_FUNCTION; \
        const ClassName & operator= ( const ClassName & other ) SOFA_DELETED_FUNCTION;

#else

    #define SOFA_AVOID_COPY_CONSTRUCTOR( ClassName ) \
        private: \
        ClassName( const ClassName &other ) SOFA_NOEXCEPT; \
        const ClassName& operator= ( const ClassName &other ) SOFA_NOEXCEPT;

#endif

//==============================================================================
#define SOFA_API

#endif /* _SOFA_PLATFORM_H__ */
