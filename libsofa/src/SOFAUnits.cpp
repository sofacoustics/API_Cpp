/*
Copyright (c) 2013-2014, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
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

Spatial acoustic data file format - AES X212 standard
http://www.aes.org

SOFA (Spatially Oriented Format for Acoustics)
http://www.sofaconventions.org

*/


/************************************************************************************/
/*  FILE DESCRIPTION                                                                */
/*----------------------------------------------------------------------------------*/
/*!
 *   @file       SOFAUnits.cpp
 *   @brief      SOFA units systems
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#include "../src/SOFAUnits.h"
#include "../src/SOFANcUtils.h"
#include <map>

using namespace sofa;

namespace UnitsHelper
{
    static std::map< std::string, sofa::Units::Type > typeMap;
    
    /************************************************************************************/
    /*!
     *  @brief          Creates a mapping between units type and their names
     *
     */
    /************************************************************************************/
    static void initTypeMap()
    {
        if( typeMap.empty() == true )
        {    
            typeMap["meter"]                        = sofa::Units::kMeter;
            typeMap["cubic meter"]                  = sofa::Units::kCubicMeter;
            typeMap["hertz"]                        = sofa::Units::kHertz;
            typeMap["samples"]                      = sofa::Units::kSamples;
            typeMap["degree, degree, meter"]        = sofa::Units::kSphericalUnits;
            typeMap["degrees Kelvin"]               = sofa::Units::kKelvin;
        }
    }
}


const std::string sofa::Units::GetName(const sofa::Units::Type &type_)
{
    switch( type_ )
    {
        case sofa::Units::kMeter                : return "meter";
        case sofa::Units::kCubicMeter           : return "cubic meter";
        case sofa::Units::kHertz                : return "hertz";
        case sofa::Units::kSamples              : return "samples";
        case sofa::Units::kSphericalUnits       : return "degree, degree, meter";
        case sofa::Units::kKelvin               : return "degrees Kelvin";
            
        default                                 : SOFA_ASSERT( false ); return "";    
        case sofa::Units::kNumUnitsTypes        : SOFA_ASSERT( false ); return "";    
    }
}

const sofa::Units::Type sofa::Units::GetType(const std::string &name)
{
    UnitsHelper::initTypeMap();
    
    if( UnitsHelper::typeMap.count( name ) == 0 )
    {        
        SOFA_ASSERT( false );
        
        return sofa::Units::kNumUnitsTypes;
    }
    else
    {
        return UnitsHelper::typeMap[ name ];
    }
}

const bool sofa::Units::IsValid(const std::string &name)
{
    UnitsHelper::initTypeMap();
    
    return ( UnitsHelper::typeMap.count( name ) != 0 );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a NcAtt properly represents a SOFA Units
 *  @param[in]      attr : the NcAtt
 *
 */
/************************************************************************************/
const bool sofa::Units::IsValid(const netCDF::NcAtt & attr)
{
    if( sofa::NcUtils::IsValid( attr ) == false )
    {
        return false;
    }
    
    if( sofa::NcUtils::IsChar( attr ) == false )
    {
        return false;
    }
    
    const std::string positionUnits = sofa::NcUtils::GetAttributeValueAsString( attr );
    
    if( sofa::Units::IsValid( positionUnits ) == false )
    {
        return false;
    }
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a distance metric
 *  @param[in]      type_ : the sofa units
 *
 */
/************************************************************************************/
const bool sofa::Units::IsDistanceUnit(const sofa::Units::Type &type_)
{
    switch( type_ )
    {
        case sofa::Units::kMeter    : return true;
        default                     : return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a frequency metric
 *  @param[in]      type_ : the sofa units
 *
 */
/************************************************************************************/
const bool sofa::Units::IsFrequencyUnit(const sofa::Units::Type &type_)
{
    switch( type_ )
    {
        case sofa::Units::kHertz    : return true;
        default                        : return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a time metric
 *  @param[in]      type_ : the sofa units
 *
 */
/************************************************************************************/
const bool sofa::Units::IsTimeUnit(const sofa::Units::Type &type_)
{
    switch( type_ )
    {
        case sofa::Units::kSamples    : return true;
        default                       : return false;
    }    
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a distance metric
 *  @param[in]      name : the sofa units, as a astring
 *
 */
/************************************************************************************/
const bool sofa::Units::IsDistanceUnit(const std::string &name)
{
    const sofa::Units::Type type_ = sofa::Units::GetType( name );
    return IsDistanceUnit( type_ );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a frequency metric
 *  @param[in]      name : the sofa units, as a astring
 *
 */
/************************************************************************************/
const bool sofa::Units::IsFrequencyUnit(const std::string &name)
{
    const sofa::Units::Type type_ = sofa::Units::GetType( name );
    return IsFrequencyUnit( type_ );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if a given unit corresponds to a time metric
 *  @param[in]      name : the sofa units, as a astring
 *
 */
/************************************************************************************/
const bool sofa::Units::IsTimeUnit(const std::string &name)
{
    const sofa::Units::Type type_ = sofa::Units::GetType( name );
    return IsTimeUnit( type_ );
}

