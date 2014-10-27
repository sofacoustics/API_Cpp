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
 *   @file       SOFASimpleFreeFieldHRIR.cpp
 *   @brief      Class for SOFA files with SimpleFreeFieldHRIR convention
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#include "../src/SOFASimpleFreeFieldHRIR.h"
#include "../src/SOFAUtils.h"
#include "../src/SOFANcUtils.h"
#include "../src/SOFAString.h"
#include "../src/SOFAPoint3.h"
#include "../src/SOFAListener.h"

using namespace sofa;

const unsigned int SimpleFreeFieldHRIR::ConventionVersionMajor  =   1;
const unsigned int SimpleFreeFieldHRIR::ConventionVersionMinor  =   0;

const std::string SimpleFreeFieldHRIR::GetConventionVersion()
{
    return sofa::String::Int2String( SimpleFreeFieldHRIR::ConventionVersionMajor ) + std::string(".") + sofa::String::Int2String( SimpleFreeFieldHRIR::ConventionVersionMinor );
}

/************************************************************************************/
/*!
 *  @brief          Class constructor
 *  @param[in]      path : the file path
 *  @param[in]      mode : opening mode
 *
 */
/************************************************************************************/
SimpleFreeFieldHRIR::SimpleFreeFieldHRIR(const std::string &path,
                                         const netCDF::NcFile::FileMode &mode)
: sofa::File( path, mode )
{
}

/************************************************************************************/
/*!
 *  @brief          Class destructor
 *
 */
/************************************************************************************/
SimpleFreeFieldHRIR::~SimpleFreeFieldHRIR()
{
}

const bool SimpleFreeFieldHRIR::hasDatabaseName() const
{
    const netCDF::NcGroupAtt att = getAttribute( "DatabaseName" );
    
    return sofa::NcUtils::IsChar( att );
}

const bool SimpleFreeFieldHRIR::checkGlobalAttributes() const
{
    sofa::Attributes attributes;
    GetGlobalAttributes( attributes );
    
    if( attributes.Get( sofa::Attributes::kSOFAConventions ) != "SimpleFreeFieldHRIR" )
    {
        SOFA_THROW( "Not a 'SimpleFreeFieldHRIR' SOFAConvention" );
        return false;
    }
    
    if( attributes.Get( sofa::Attributes::kDataType ) != "FIR" )
    {
        SOFA_THROW( "invalid 'DataType'" );
        return false;
    }
    
    if( attributes.Get( sofa::Attributes::kRoomType ) != "free field" )
    {
        SOFA_THROW( "invalid 'RoomType'" );
        return false;
    }
    
    return true;
}

const bool SimpleFreeFieldHRIR::checkListenerVariables() const
{
    const long I = GetDimension( "I" );
    if( I != 1 )
    {
        SOFA_THROW( "invalid SOFA dimension : I" );
        return false;
    }
    
    const long C = GetDimension( "C" );
    if( C != 3 )
    {
        SOFA_THROW( "invalid SOFA dimension : C" );
        return false;
    }
    
    const long M = GetNumMeasurements();
    if( M <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : M" );
        return false;
    }
    
    const netCDF::NcVar varListenerPosition        = NetCDFFile::getVariable( "ListenerPosition" );
    const netCDF::NcVar varListenerUp              = NetCDFFile::getVariable( "ListenerUp" );
    const netCDF::NcVar varListenerView            = NetCDFFile::getVariable( "ListenerView" );
    
    const sofa::Listener listener( varListenerPosition, varListenerUp, varListenerView );
    
    if( listener.IsValid() == false )
    {
        SOFA_THROW( "invalid 'Listener' variables" );
        return false;
    }
    
    if( listener.ListenerPositionHasDimensions(  I,  C ) == false
       && listener.ListenerPositionHasDimensions(  M,  C ) == false )
    {
        SOFA_THROW( "invalid 'ListenerPosition' dimensions" );
        return false;
    }
    
    if( listener.HasListenerUp() == true )
    {
        /// ListenerUp is not required in the Specifications
        /// but if it is present, is should be [ I C ] or [ M C ]
        
        if( listener.ListenerUpHasDimensions(  I,  C ) == false
           && listener.ListenerUpHasDimensions(  M,  C ) == false )
        {
            SOFA_THROW( "invalid 'ListenerUp' dimensions" );
            return false;
        }
    }
    else
    {
        SOFA_THROW( "missing 'ListenerUp' variable" );
        return false;
    }
    
    if( listener.HasListenerView() == true )
    {
        /// ListenerView is not required in the Specifications
        /// but if it is present, is should be [ I C ] or [ M C ]
        
        if( listener.ListenerViewHasDimensions(  I,  C ) == false
           && listener.ListenerViewHasDimensions(  M,  C ) == false )
        {
            SOFA_THROW( "invalid 'ListenerView' dimensions" );
            return false;
        }
    }
    else
    {
        SOFA_THROW( "missing 'ListenerView' variable" );
        return false;
    }
    
    /// everything is OK !
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Returns true if this is a valid SOFA file with SimpleFreeFieldHRIR convention
 *
 */
/************************************************************************************/
const bool SimpleFreeFieldHRIR::IsValid() const
{
    if( sofa::File::IsValid() == false )
    {
        return false;
    }
    
    if( hasDatabaseName() == false )
    {
        SOFA_THROW( "missing 'DatabaseName' global attribute" );
        return false;
    }
    
    if( IsFIRDataType() == false )
    {
        SOFA_THROW( "'DataType' shall be FIR" );
        return false;
    }
    
    if( checkGlobalAttributes() == false )
    {
        return false;
    }
    
    
    /// The number of emitters is 1
    if( GetNumEmitters() != 1 )
    {
        SOFA_THROW( "invalid number of emitters" );
        return false;
    }
    
    /// SamplingRate is a scalar
    {
        if( VariableIsScalar( "Data.SamplingRate" ) == false )
        {
            SOFA_THROW( "invalid dimensionality for 'Data.SamplingRate'");
            return false;
        }
        
        if( HasVariableType( netCDF::NcType::nc_DOUBLE, "Data.SamplingRate") == false )
        {
            SOFA_THROW( "invalid type for 'Data.SamplingRate'" );
            return false;
        }
    }
    
    /*
    /// Data.IR is [M 2 N].
    {
        std::vector< long > dims;
        GetVariableDimensions( dims, "Data.IR" );
        
        if( dims.size() != 3 )
        {
            SOFA_THROW( "invalid dimensionality for 'Data.IR'" );
            return false;
        }
        
        const long M = GetNumMeasurements();
        const long N = GetNumDataSamples();
        
        if( dims[0] != M || dims[1] != 2 || dims[2] != N )
        {
            SOFA_THROW( "invalid dimensions for 'Data.IR'" );
            return false;
        }
    }
     */
    
    if( checkListenerVariables() == false )
    {
        return false;
    }
    
    
    SOFA_ASSERT( GetDimension( "I" ) == 1 );
    SOFA_ASSERT( GetDimension( "C" ) == 3 );
     
    return true;
}

const bool SimpleFreeFieldHRIR::GetSamplingRate(double &value) const
{
    SOFA_ASSERT( SimpleFreeFieldHRIR::IsValid() == true );
    
    const netCDF::NcVar var = getVariable( "Data.SamplingRate" );
        
    return sofa::NcUtils::GetValue( value, var );        
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the Data.IR values
 *  @param[in]      values : array containing the values. 
 *                  The array must be allocated large enough
 *  @param[in]      dim1 : first dimension (M)
 *  @param[in]      dim2 : second dimension (R)
 *  @param[in]      dim3 : third dimension (N)
 *  @return         true on success
 *
 */
/************************************************************************************/
const bool SimpleFreeFieldHRIR::GetDataIR(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "Data.IR" );
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the Data.IR values
 *  @param[in]      values : the array is resized if needed
 *  @return         true on success
 *
 */
/************************************************************************************/
const bool SimpleFreeFieldHRIR::GetDataIR(std::vector< double > &values) const
{
    const long M = GetNumMeasurements();
    const long R = GetNumReceivers();
    const long N = GetNumDataSamples();
    
    SOFA_ASSERT( M > 0 );
    SOFA_ASSERT( R > 0 );
    SOFA_ASSERT( N > 0 );
    
    const std::size_t size_ = M * R * N;
    
    values.resize( size_ );
    
    SOFA_ASSERT( values.empty() == false );
    
    return GetDataIR( &values[0], M, R, N );
}

const bool SimpleFreeFieldHRIR::GetDataDelay(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "Data.Delay" );
}

