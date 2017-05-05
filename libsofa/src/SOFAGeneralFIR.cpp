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
 *   @file       SOFAGeneralFIR.cpp
 *   @brief      Class for SOFA files with GeneralFIR convention
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 *
 */
/************************************************************************************/
#include "../src/SOFAGeneralFIR.h"
#include "../src/SOFAExceptions.h"
#include "../src/SOFAUtils.h"
#include "../src/SOFANcUtils.h"
#include "../src/SOFAString.h"
#include "../src/SOFAListener.h"

using namespace sofa;

const unsigned int GeneralFIR::ConventionVersionMajor  =   1;
const unsigned int GeneralFIR::ConventionVersionMinor  =   0;

std::string GeneralFIR::GetConventionVersion()
{
    return sofa::String::Int2String( GeneralFIR::ConventionVersionMajor ) + std::string(".") + sofa::String::Int2String( GeneralFIR::ConventionVersionMinor );
}

/************************************************************************************/
/*!
 *  @brief          Class constructor
 *  @param[in]      path : the file path
 *  @param[in]      mode : opening mode
 *
 */
/************************************************************************************/
GeneralFIR::GeneralFIR(const std::string &path,
                       const netCDF::NcFile::FileMode &mode)
: sofa::File( path, mode )
{
}

bool GeneralFIR::checkGlobalAttributes() const
{
    sofa::Attributes attributes;
    GetGlobalAttributes( attributes );
    
    sofa::File::ensureSOFAConvention( "GeneralFIR" );
    sofa::File::ensureDataType( "FIR" );
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Returns true if this is a valid SOFA file with GeneralFIR convention
 *
 */
/************************************************************************************/
bool GeneralFIR::IsValid() const
{
    if( sofa::File::IsValid() == false )
    {
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
    
    SOFA_ASSERT( GetDimension( "I" ) == 1 );
    SOFA_ASSERT( GetDimension( "C" ) == 3 );
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          In case Data.SamplingRate is of dimension [I], this function returns
 *                  its value. In case Data.SamplingRate is of dimension [M], an error is thrown
 *  @return         true on success
 *
 */
/************************************************************************************/
bool GeneralFIR::GetSamplingRate(double &value) const
{
    SOFA_ASSERT( GeneralFIR::IsValid() == true );
    
    return sofa::File::getSamplingRate( value );
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the units of the Data.SamplingRate variable
 *  @return         true on success
 *
 */
/************************************************************************************/
bool GeneralFIR::GetSamplingRateUnits(sofa::Units::Type &units) const
{
    return sofa::File::getSamplingRateUnits( units );
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
bool GeneralFIR::GetDataIR(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    /// Data.IR is [ M R N ]
    
    return sofa::File::getDataIR( values, dim1, dim2, dim3 );
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the Data.IR values
 *  @param[in]      values : the array is resized if needed
 *  @return         true on success
 *
 */
/************************************************************************************/
bool GeneralFIR::GetDataIR(std::vector< double > &values) const
{
    /// Data.IR is [ M R N ]
    
    return sofa::File::getDataIR( values );
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the Data.Delay values
 *  @param[in]      values : the array is resized if needed
 *  @return         true on success
 *
 */
/************************************************************************************/
bool GeneralFIR::GetDataDelay(std::vector< double > &values) const
{
    /// Data.Delay is [ I R ] or [ M R ]
    
    return sofa::File::getDataDelay( values );
}

bool GeneralFIR::GetDataDelay(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    /// Data.Delay is [ I R ] or [ M R ]
    
    return sofa::File::getDataDelay( values, dim1, dim2 );
}

