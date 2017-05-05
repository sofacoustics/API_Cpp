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
 *   @file       SOFAFile.cpp
 *   @brief      Class for SOFA files
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#include "../src/SOFAExceptions.h"
#include "../src/SOFAFile.h"
#include "../src/SOFAListener.h"
#include "../src/SOFASource.h"
#include "../src/SOFAReceiver.h"
#include "../src/SOFAEmitter.h"
#include "../src/SOFAString.h"
#include "../src/SOFANcUtils.h"

using namespace sofa;

/************************************************************************************/
/*!
 *  @brief          Class constructor
 *  @param[in]      path : the file path
 *  @param[in]      mode : opening mode
 *
 */
/************************************************************************************/
File::File(const std::string &path,
           const netCDF::NcFile::FileMode &mode)
: sofa::NetCDFFile( path, mode )
{
}

/************************************************************************************/
/*!
 *  @brief          Returns true if this is a valid SOFA file.
 *
 *  @details        this checks if the file is a valid netCDF file,
 *                    if it contains all the SOFA required attributes,
 *                    if the "Conventions" attribute is set to "SOFA"
 *                    if the SOFA dimensions are OK (I,M,R,E,N,C)
 */
/************************************************************************************/
bool File::IsValid() const
{
    return ( sofa::NetCDFFile::IsValid() == true
            && hasSOFARequiredAttributes() == true
            && hasSOFAConvention() == true 
            && SOFADimensionsAreValid() == true 
            && checkListenerVariables() == true
            && checkSourceVariables() == true
            && checkReceiverVariables() == true
            && checkEmitterVariables() == true
            && checkDimensions() == true
            && checkDataVariable() == true 
            );
}

/************************************************************************************/
/*!
 *  @brief          Prints the value of all (required) SOFA global attributes
 *                  for this file
 *  @param[in]      output : output stream
 *  @param[in]      withPadding : use padding for display
 *
 */
/************************************************************************************/
void File::PrintSOFAGlobalAttributes(std::ostream & output, const bool withPadding) const
{
    sofa::Attributes attributes;
    GetGlobalAttributes( attributes );
    
    attributes.Print( output, withPadding );
}

void File::PrintSOFADimensions(std::ostream & output, const bool withPadding) const
{
    
    const std::string numMeasurements   = sofa::String::Int2String( (int) GetNumMeasurements() );
    const std::string numReceivers      = sofa::String::Int2String( (int) GetNumReceivers() );
    const std::string numEmitters       = sofa::String::Int2String( (int) GetNumEmitters() );
    const std::string numDataSamples    = sofa::String::Int2String( (int) GetNumDataSamples() );
    
    if( withPadding == true )
    {        
        output << sofa::String::PadWith( "Number of measurements (M)" ) << " = " << numMeasurements << std::endl;
        output << sofa::String::PadWith( "Number of receivers (R)" ) << " = " << numReceivers << std::endl;
        output << sofa::String::PadWith( "Number of emitters (E)" ) << " = " << numEmitters << std::endl;
        output << sofa::String::PadWith( "Number of data samples (N)" ) << " = " << numDataSamples << std::endl;
    }
    else
    {
        output << "Number of measurements (M) = " << numMeasurements << std::endl;
        output << "Number of receivers (R) = " << numReceivers << std::endl;
        output << "Number of emitters (E) = " << numEmitters << std::endl;
        output << "Number of data samples (N) = " << numDataSamples << std::endl;
    }
    

}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains all the SOFA required dimensions
 *                  This does not check the actual values of the parameters
 *
 */
/************************************************************************************/
bool File::hasSOFARequiredDimensions() const
{
    const bool valid = ( HasDimension("I") == true 
                        && HasDimension("M") == true  
                        && HasDimension("R") == true  
                        && HasDimension("E") == true  
                        && HasDimension("N") == true 
                        && HasDimension("C") == true  );
    
    if( valid == true )
    {
        return true;
    }
    else
    {
        SOFA_THROW( "missing SOFA dimension(s)" );
        return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains all the SOFA required dimensions
 *                  and if the dimensions values are OK with respect to SOFA specifications
 *
 */
/************************************************************************************/
bool File::SOFADimensionsAreValid() const
{    
    const long M = GetDimension( "M" );
    const long N = GetDimension( "N" );
    const long R = GetDimension( "R" );
    const long E = GetDimension( "E" );
    const long I = GetDimension( "I" );
    const long C = GetDimension( "C" );
    
    const bool valid = ( I == 1L && C == 3 && M >= 1L && R >= 1L && E >= 1L && N >= 1L );
    
    if( valid == true )
    {
        return true;
    }
    else
    {
        SOFA_THROW( "invalid SOFA dimension(s)" );
        return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns the number of measurements, i.e. the M dimension
 *                  or -1 if the dimension does not exist
 *
 */
/************************************************************************************/
long File::GetNumMeasurements() const
{
    return GetDimension( "M" );
}

/************************************************************************************/
/*!
 *  @brief          Returns the number of receivers, i.e. the R dimension
 *                  or -1 if the dimension does not exist
 *
 */
/************************************************************************************/
long File::GetNumReceivers() const
{
    return GetDimension( "R" );
}

/************************************************************************************/
/*!
 *  @brief          Returns the number of emitters, i.e. the E dimension
 *                  or -1 if the dimension does not exist
 *
 */
/************************************************************************************/
long File::GetNumEmitters() const
{
    return GetDimension( "E" );
}


/************************************************************************************/
/*!
 *  @brief          Returns the number of data samples describing one measurement, i.e. the N dimension
 *                  or -1 if the dimension does not exist
 *
 */
/************************************************************************************/
long File::GetNumDataSamples() const
{
    return GetDimension( "N" );    
}


/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a given attribute
 *  @param[in]      type_ : the attribute to query
 *
 */
/************************************************************************************/
bool File::HasAttribute(const sofa::Attributes::Type &type_) const
{
    const std::string attributeName = sofa::Attributes::GetName( type_ );
    
    return NetCDFFile::HasAttribute( attributeName );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains all the SOFA required attributes
 *                    This does not check the actual values of the parameters
 *
 */
/************************************************************************************/
bool File::hasSOFARequiredAttributes() const
{
    for( unsigned int i = 0; i < sofa::Attributes::kNumAttributes; i++ )
    {
        const sofa::Attributes::Type type_ = static_cast< const sofa::Attributes::Type >( i );
        const bool required   = sofa::Attributes::IsRequired( type_ );
        
        if( required == true )
        {        
            const bool hasIt = HasAttribute( type_ );
            
            if( hasIt == false )
            {
                const std::string err = "Missing SOFA attribute : " + sofa::Attributes::GetName( type_ );
                SOFA_THROW( err );
                return false;
            }
        }
    }
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Retrieves all the SOFA global attribues
 *                  (attributes not in the file are set to default)
 *
 */
/************************************************************************************/
void File::GetGlobalAttributes(sofa::Attributes &attributes) const
{
    attributes.ResetToDefault();
    
    for( unsigned int i = 0; i < sofa::Attributes::kNumAttributes; i++ )
    {
        const sofa::Attributes::Type type_ = static_cast< const sofa::Attributes::Type >( i );
        
        const bool hasIt = HasAttribute( type_ );
        
        if( hasIt == true )
        {
            const std::string name  = sofa::Attributes::GetName( type_ );
            const std::string value = GetAttributeValueAsString( name );
            
            attributes.Set( type_, value );
        }
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "Conventions" attribute, and if this attribute
 *                  is set to "SOFA"
 *
 */
/************************************************************************************/
bool File::hasSOFAConvention() const
{
    const std::string value = GetAttributeValueAsString( "Conventions" );
    
    if( value == "SOFA" )
    {
        return true;
    }
    else
    {
        SOFA_THROW( "invalid SOFA Conventions" );
        return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns the "SOFAConventions" attribute if it exists;
 *                  throw an exception otherwise
 *  @details        This doesnt check if the file conforms to the given convention
 *
 */
/************************************************************************************/
std::string File::GetSOFAConventions() const
{
    return GetAttributeValueAsString( "SOFAConventions" );
}

/************************************************************************************/
/*!
 *  @brief          Checks if the file has the appropriate Listener variables, i.e.
 *                    ListenerPosition of size [I C] or [M C]
 *                        ListenerPosition:Type
 *                        ListenerPosition:Unit
 *                    ListenerUp of size [I C] or [M C] (optional)
 *                    ListenerView of size [I C] or [M C] (optional)
 *                        ListenerView:Type
 *                        ListenerView:Units
 *
 *                    Returns true if everything is conform to the specifications
 *                    False otherwise or if any error occured
 *
 *  @details        some of the tests are redundant, but anyway they should be rather fast
 */
/************************************************************************************/
bool File::checkListenerVariables() const
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
    
    
    /// everything is OK !
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Checks if the file has the appropriate Source variables, i.e.
 *                    SourcePosition of size [I C] or [M C]
 *                        SourcePosition:Type
 *                        SourcePosition:Unit
 *                    SourceUp of size [I C] or [M C] (optional)
 *                    SourceView of size [I C] or [M C] (optional)
 *                        SourceView:Type
 *                        SourceView:Units
 *
 *                    Returns true if everything is conform to the specifications
 *                    False otherwise or if any error occured
 *
 *  @details        some of the tests are redundant, but anyway they should be rather fast
 */
/************************************************************************************/
bool File::checkSourceVariables() const
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
    
    const netCDF::NcVar varSourcePosition        = NetCDFFile::getVariable( "SourcePosition" );
    const netCDF::NcVar varSourceUp              = NetCDFFile::getVariable( "SourceUp" );
    const netCDF::NcVar varSourceView            = NetCDFFile::getVariable( "SourceView" );
    
    const sofa::Source source( varSourcePosition, varSourceUp, varSourceView );
    
    if( source.IsValid() == false )
    {
        SOFA_THROW( "invalid 'Source' variables" );
        return false;
    }
    
    if( source.SourcePositionHasDimensions(  I,  C ) == false
       && source.SourcePositionHasDimensions(  M,  C ) == false )
    {
        SOFA_THROW( "invalid 'SourcePosition' dimensions" );
        return false;
    }
    
    if( source.HasSourceUp() == true )
    {
        /// SourceUp is not required in the Specifications
        /// but if it is present, is should be [ I C ] or [ M C ]
        
        if( source.SourceUpHasDimensions(  I,  C ) == false
           && source.SourceUpHasDimensions(  M,  C ) == false )
        {
            SOFA_THROW( "invalid 'SourceUp' dimensions" );
            return false;
        }
    }
    
    if( source.HasSourceView() == true )
    {
        /// SourceView is not required in the Specifications
        /// but if it is present, is should be [ I C ] or [ M C ]
        
        if( source.SourceViewHasDimensions(  I,  C ) == false
           && source.SourceViewHasDimensions(  M,  C ) == false )
        {
            SOFA_THROW( "invalid 'SourceView' dimensions" );
            return false;
        }
    }
    
            
    /// everything is OK !
    return true;
}


/************************************************************************************/
/*!
 *  @brief          Checks if the file has the appropriate Receiver variables, i.e.
 *                    ReceiverPosition of size [R C I] or [R C M]
 *                        ReceiverPosition:Type
 *                        ReceiverPosition:Unit
 *                    ReceiverUp of size [R C I] or [R C M]  (optional)
 *                    ReceiverView of size [R C I] or [R C M] (optional)
 *                        ReceiverView:Type
 *                        ReceiverView:Units
 *
 *                    Returns true if everything is conform to the specifications
 *                    False otherwise or if any error occured
 *
 *  @details        some of the tests are redundant, but anyway they should be rather fast
 */
/************************************************************************************/
bool File::checkReceiverVariables() const
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
    
    const long R = GetNumReceivers();    
    if( R <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : R" );
        return false;
    }    
    
    const netCDF::NcVar varReceiverPosition        = NetCDFFile::getVariable( "ReceiverPosition" );
    const netCDF::NcVar varReceiverUp              = NetCDFFile::getVariable( "ReceiverUp" );
    const netCDF::NcVar varReceiverView            = NetCDFFile::getVariable( "ReceiverView" );
    
    const sofa::Receiver receiver( varReceiverPosition, varReceiverUp, varReceiverView );
    
    if( receiver.IsValid() == false )        
    {
        SOFA_THROW( "invalid 'Receiver' variables" );
        return false;
    }
    
    if( receiver.ReceiverPositionHasDimensions(  R,  C,  I ) == false
     && receiver.ReceiverPositionHasDimensions(  R,  C,  M ) == false )
    {
        SOFA_THROW( "invalid 'ReceiverPosition' dimensions" );
        return false;
    }
    
    if( receiver.HasReceiverUpVariable() == true )
    {
        /// ReceiverUp is not required in the Specifications
        /// but if it is present, is should be [ R C I ] or [ R C M ]
        
        if( receiver.ReceiverUpHasDimensions(  R,  C,  I ) == false
           && receiver.ReceiverUpHasDimensions(  R,  C,  M ) == false )
        {
            SOFA_THROW( "invalid 'ReceiverUp' dimensions" );
            return false;
        }
    }
    
    if( receiver.HasReceiverViewVariable() == true )
    {
        /// ReceiverUp is not required in the Specifications
        /// but if it is present, is should be [ R C I ] or [ R C M ]
        
        if( receiver.ReceiverViewHasDimensions(  R,  C,  I ) == false
           && receiver.ReceiverViewHasDimensions(  R,  C,  M ) == false )
        {
            SOFA_THROW( "invalid 'ReceiverView' dimensions" );
            return false;
        }
    }
    
    /// everything is OK !
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Checks if the file has the appropriate Emitter variables, i.e.
 *                    EmitterPosition of size [E C I] or [E C M]
 *                        EmitterPosition:Type
 *                        EmitterPosition:Unit
 *                    EmitterUp of size [E C I] or [E C M]  (optional)
 *                        EmitterUp:Type
 *                        EmitterUp:Units
 *                    EmitterView of size [E C I] or [E C M] (optional)
 *                        EmitterView:Type
 *                        EmitterView:Units
 *
 *                    Returns true if everything is conform to the specifications
 *                    False otherwise or if any error occured
 *
 *  @details        some of the tests are redundant, but anyway they should be rather fast
 */
/************************************************************************************/
bool File::checkEmitterVariables() const
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
    
    const long E = GetNumEmitters();    
    if( E <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : E" );
        return false;
    }    
    
    const netCDF::NcVar varEmitterPosition        = NetCDFFile::getVariable( "EmitterPosition" );
    const netCDF::NcVar varEmitterUp              = NetCDFFile::getVariable( "EmitterUp" );
    const netCDF::NcVar varEmitterView            = NetCDFFile::getVariable( "EmitterView" );
    
    const sofa::Emitter emitter( varEmitterPosition, varEmitterUp, varEmitterView );
    
    if( emitter.IsValid() == false )
    {
        SOFA_THROW( "invalid 'Emitter' variables" );
        return false;
    }
    
    if( emitter.EmitterPositionHasDimensions(  E,  C,  I ) == false
       && emitter.EmitterPositionHasDimensions(  E,  C,  M ) == false )
    {
        SOFA_THROW( "invalid 'EmitterPosition' dimensions" );
        return false;
    }
    
    if( emitter.HasEmitterUpVariable() == true )
    {
        /// EmitterUp is not required in the Specifications
        /// but if it is present, is should be [ E C I ] or [ E C M ]
        
        if( emitter.EmitterUpHasDimensions(  E,  C,  I ) == false
           && emitter.EmitterUpHasDimensions(  E,  C,  M ) == false )
        {
            SOFA_THROW( "invalid 'EmitterUp' dimensions" );
            return false;
        }
    }
    
    if( emitter.HasEmitterViewVariable() == true )
    {
        /// EmitterUp is not required in the Specifications
        /// but if it is present, is should be [ E C I ] or [ E C M ]
        
        if( emitter.EmitterViewHasDimensions(  E,  C,  I ) == false
           && emitter.EmitterViewHasDimensions(  E,  C,  M ) == false )
        {
            SOFA_THROW( "invalid 'EmitterView' dimensions" );
            return false;
        }
    }
    
    /// everything is OK !
    return true;
}

bool File::checkDimensions() const
{
    const long I = GetDimension( "I" );
    if( I != 1 )
    {
        SOFA_THROW( "invalid SOFA dimension : I" );
        return false;
    }
    
    const long M = GetNumMeasurements();
    if( M <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : M" );
        return false;
    }
    
    const long R = GetNumReceivers();
    if( R <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : R" );
        return false;
    }
    
    const long N = GetNumDataSamples();
    if( N <= 0 )
    {
        SOFA_THROW( "invalid SOFA dimension : N" );
        return false;
    }
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Checks if the file has the appropriate Data variables, i.e.
 *                    Data.IR of size [m R n]
 *                    Data.Delay of size [I R] or [M R]
 *                  Data.SamplingRate of size [I] or [M]
 *                      Data.SamplingRate:Units
 *
 *
 *                    Returns true if everything is conform to the specifications
 *                    False otherwise or if any error occured
 *
 *  @details        some of the tests are redundant, but anyway they should be rather fast
 */
/************************************************************************************/
bool File::checkDataVariable() const
{
    if( IsFIRDataType() == true )
    {
        return checkFirDataType();
    }
    else if( IsTFDataType() == true )
    {
        return checkTFDataType();
    }
    else if( IsSOSDataType() == true )
    {
        return checkSOSDataType();
    }
    else if( IsFIREDataType() == true )
    {
        return checkFireDataType();
    }
    else
    {
        SOFA_THROW( "invalid 'DataType'" );
        return false;
    }
}

bool File::IsFIRDataType() const
{
    const std::string value = GetAttributeValueAsString( "DataType" );
    return ( value == "FIR" );
}

bool File::IsFIREDataType() const
{
    const std::string value = GetAttributeValueAsString( "DataType" );
    return ( value == "FIRE" );
}

bool File::IsTFDataType() const
{
    const std::string value = GetAttributeValueAsString( "DataType" );
    return ( value == "TF" );
}

bool File::IsSOSDataType() const
{
    const std::string value = GetAttributeValueAsString( "DataType" );
    return ( value == "SOS" );
}

/************************************************************************************/
/*!
 *  @brief          Checks requirements for DataType 'TF'
 *                  returns true if everything conforms to the standard
 *
 */
/************************************************************************************/
bool File::checkTFDataType() const
{
    //const long I = GetDimension( "I" );
    const long M = GetNumMeasurements();
    const long R = GetNumReceivers();
    const long N = GetNumDataSamples();
    
    /// NB : this is specific to DataType 'TF'
    {
        const netCDF::NcVar varReal        = NetCDFFile::getVariable( "Data.Real" );
        
        if( sofa::NcUtils::IsValid( varReal ) == false )
        {
            SOFA_THROW( "invalid 'Data.Real' variable" );
            return false;
        }
        
        if( sofa::NcUtils::IsDouble( varReal ) == false )
        {
            SOFA_THROW( "invalid 'Data.Real' variable" );
            return false;
        }
        
        if( sofa::NcUtils::HasDimensions(  M,  R,  N, varReal ) == false )
        {
            SOFA_THROW( "invalid dimensions for 'Data.Real'" );
            return false;
        }
    }
    {
        const netCDF::NcVar varImag        = NetCDFFile::getVariable( "Data.Imag" );
        
        if( sofa::NcUtils::IsValid( varImag ) == false )
        {
            SOFA_THROW( "invalid 'Data.Imag' variable" );
            return false;
        }
        
        if( sofa::NcUtils::IsDouble( varImag ) == false )
        {
            SOFA_THROW( "invalid 'Data.Imag' variable" );
            return false;
        }
        
        if( sofa::NcUtils::HasDimensions(  M,  R,  N, varImag ) == false )
        {
            SOFA_THROW( "invalid dimensions for 'Data.Imag'" );
            return false;
        }
    }
    
    
    
    const netCDF::NcVar varN        = NetCDFFile::getVariable( "N" );
    
    if( sofa::NcUtils::IsValid( varN ) == false )
    {
        SOFA_THROW( "missing 'N' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varN ) == false )
    {
        SOFA_THROW( "invalid 'N' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimension( N, varN ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'N'" );
        return false;
    }
    
    const netCDF::NcVarAtt attNUnits = sofa::NcUtils::GetAttribute( varN, "Units" );
    
    if( sofa::Units::IsValid( attNUnits ) == false )
    {
        SOFA_THROW( "invalid 'N:Units'" );
        return false;
    }
    
    const std::string unitsName = sofa::NcUtils::GetAttributeValueAsString( attNUnits );
    
    if( sofa::Units::IsFrequencyUnit( unitsName ) == false )
    {
        SOFA_THROW( "invalid 'N:Units'" );
        return false;
    }
    
    const netCDF::NcVarAtt attNLongName = sofa::NcUtils::GetAttribute( varN, "LongName" );
    
    if( sofa::Units::IsValid( attNLongName ) == false )
    {
        SOFA_THROW( "invalid 'LongName'" );
        return false;
    }
    
    return true;
}


/************************************************************************************/
/*!
 *  @brief          Checks requirements for DataType 'FIR'
 *                  returns true if everything conforms to the standard
 *
 */
/************************************************************************************/
bool File::checkFirDataType() const
{
    const long I = GetDimension( "I" );
    const long M = GetNumMeasurements();
    const long R = GetNumReceivers();
    const long N = GetNumDataSamples();
    
    /// NB : this is specific to DataType 'FIR'
    const netCDF::NcVar varIR        = NetCDFFile::getVariable( "Data.IR" );
    
    if( sofa::NcUtils::IsValid( varIR ) == false )
    {
        SOFA_THROW( "missing 'Data.IR' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varIR ) == false )
    {
        SOFA_THROW( "invalid 'Data.IR' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( M,  R,  N, varIR ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.IR'" );
        return false;
    }
    
    const netCDF::NcVar varSamplingRate        = NetCDFFile::getVariable( "Data.SamplingRate" );
    
    if( sofa::NcUtils::IsValid( varSamplingRate ) == false )
    {
        SOFA_THROW( "missing 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimension( I, varSamplingRate ) == false
     && sofa::NcUtils::HasDimension( M, varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.SamplingRate'" );
        return false;
    }
    
    const netCDF::NcVarAtt attSamplingRateUnits = sofa::NcUtils::GetAttribute( varSamplingRate, "Units" );
    
    if( sofa::Units::IsValid( attSamplingRateUnits ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    const std::string unitsName = sofa::NcUtils::GetAttributeValueAsString( attSamplingRateUnits );
    
    if( sofa::Units::IsFrequencyUnit( unitsName ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    
    const netCDF::NcVar varDelay        = NetCDFFile::getVariable( "Data.Delay" );
    
    if( sofa::NcUtils::IsValid( varDelay ) == false )
    {
        SOFA_THROW( "missing 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varDelay ) == false )
    {
        SOFA_THROW( "invalid 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( I, R, varDelay ) == false
     && sofa::NcUtils::HasDimensions( M, R, varDelay ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.Delay'" );
        return false;
    }
    
    return true;
}



/************************************************************************************/
/*!
 *  @brief          Checks requirements for DataType 'FIRE'
 *                  returns true if everything conforms to the standard
 *
 */
/************************************************************************************/
bool File::checkFireDataType() const
{
    const long I = GetDimension( "I" );
    const long M = GetNumMeasurements();
    const long R = GetNumReceivers();
    const long N = GetNumDataSamples();
    const long E = GetNumEmitters();
    
    /// NB : this is specific to DataType 'FIRE'
    const netCDF::NcVar varIR        = NetCDFFile::getVariable( "Data.IR" );
    
    if( sofa::NcUtils::IsValid( varIR ) == false )
    {
        SOFA_THROW( "missing 'Data.IR' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varIR ) == false )
    {
        SOFA_THROW( "invalid 'Data.IR' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( M, R, E, N, varIR ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.IR'" );
        return false;
    }
    
    const netCDF::NcVar varSamplingRate        = NetCDFFile::getVariable( "Data.SamplingRate" );
    
    if( sofa::NcUtils::IsValid( varSamplingRate ) == false )
    {
        SOFA_THROW( "missing 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimension( I, varSamplingRate ) == false
     && sofa::NcUtils::HasDimension( M, varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.SamplingRate'" );
        return false;
    }
    
    const netCDF::NcVarAtt attSamplingRateUnits = sofa::NcUtils::GetAttribute( varSamplingRate, "Units" );
    
    if( sofa::Units::IsValid( attSamplingRateUnits ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    const std::string unitsName = sofa::NcUtils::GetAttributeValueAsString( attSamplingRateUnits );
    
    if( sofa::Units::IsFrequencyUnit( unitsName ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    
    const netCDF::NcVar varDelay        = NetCDFFile::getVariable( "Data.Delay" );
    
    if( sofa::NcUtils::IsValid( varDelay ) == false )
    {
        SOFA_THROW( "missing 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varDelay ) == false )
    {
        SOFA_THROW( "invalid 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( I, R, E, varDelay ) == false
     && sofa::NcUtils::HasDimensions( M, R, E, varDelay ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.Delay'" );
        return false;
    }
    
    return true;
}


/************************************************************************************/
/*!
 *  @brief          Checks requirements for DataType 'SOS'
 *                  returns true if everything conforms to the standard
 *
 */
/************************************************************************************/
bool File::checkSOSDataType() const
{
    const long I = GetDimension( "I" );
    const long M = GetNumMeasurements();
    const long R = GetNumReceivers();
    const long N = GetNumDataSamples();
    
    /// NB : this is specific to DataType 'SOS'
    const netCDF::NcVar varSOS        = NetCDFFile::getVariable( "Data.SOS" );
    
    if( sofa::NcUtils::IsValid( varSOS ) == false )
    {
        SOFA_THROW( "missing 'Data.SOS' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varSOS ) == false )
    {
        SOFA_THROW( "invalid 'Data.SOS' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( M,  R,  N, varSOS ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.SOS'" );
        return false;
    }
    
    const netCDF::NcVar varSamplingRate        = NetCDFFile::getVariable( "Data.SamplingRate" );
    
    if( sofa::NcUtils::IsValid( varSamplingRate ) == false )
    {
        SOFA_THROW( "missing 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimension( I, varSamplingRate ) == false
     && sofa::NcUtils::HasDimension( M, varSamplingRate ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.SamplingRate'" );
        return false;
    }
    
    const netCDF::NcVarAtt attSamplingRateUnits = sofa::NcUtils::GetAttribute( varSamplingRate, "Units" );
    
    if( sofa::Units::IsValid( attSamplingRateUnits ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    const std::string unitsName = sofa::NcUtils::GetAttributeValueAsString( attSamplingRateUnits );
    
    if( sofa::Units::IsFrequencyUnit( unitsName ) == false )
    {
        SOFA_THROW( "invalid 'Data.SamplingRate:Units'" );
        return false;
    }
    
    
    const netCDF::NcVar varDelay        = NetCDFFile::getVariable( "Data.Delay" );
    
    if( sofa::NcUtils::IsValid( varDelay ) == false )
    {
        SOFA_THROW( "missing 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::IsDouble( varDelay ) == false )
    {
        SOFA_THROW( "invalid 'Data.Delay' variable" );
        return false;
    }
    
    if( sofa::NcUtils::HasDimensions( I, R, varDelay ) == false
     && sofa::NcUtils::HasDimensions( M, R, varDelay ) == false )
    {
        SOFA_THROW( "invalid dimensions for 'Data.Delay'" );
        return false;
    }
    
    return true;
}

bool File::getCoordinates(sofa::Coordinates::Type &coordinates,
                          const std::string &variableName) const
{
    const netCDF::NcVar var        = NetCDFFile::getVariable( variableName );
    const sofa::PositionVariable pos( var );
    
    if( pos.IsValid() == false )
    {
        return false;
    }
    else
    {
        coordinates = pos.GetCoordinates();
        return true;
    }
}

bool File::getUnits(sofa::Units::Type &units,
                    const std::string &variableName) const
{
    const netCDF::NcVar var        = NetCDFFile::getVariable( variableName );
    const sofa::PositionVariable pos( var );
    
    if( pos.IsValid() == false )
    {
        return false;
    }
    else
    {
        units = pos.GetUnits();
        return true;
    }
}

bool File::get(sofa::Coordinates::Type &coordinates,
               sofa::Units::Type &units,
               const std::string &variableName) const
{
    const netCDF::NcVar var        = NetCDFFile::getVariable( variableName );
    const sofa::PositionVariable pos( var );
    
    if( pos.IsValid() == false )
    {
        return false;
    }
    else
    {
        coordinates = pos.GetCoordinates();
        units = pos.GetUnits();
        return true;
    }
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "SourceUp" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasSourceUp() const
{
    return sofa::NetCDFFile::HasVariable( "SourceUp" );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "SourceView" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasSourceView() const
{
    return sofa::NetCDFFile::HasVariable( "SourceView" );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "ReceiverUp" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasReceiverUp() const
{
    return sofa::NetCDFFile::HasVariable( "ReceiverUp" );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "ReceiverView" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasReceiverView() const
{
    return sofa::NetCDFFile::HasVariable( "ReceiverView" );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "EmitterUp" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasEmitterUp() const
{
    return sofa::NetCDFFile::HasVariable( "EmitterUp" );
}

/************************************************************************************/
/*!
 *  @brief          Returns true if the file contains a "EmitterView" variable
 *                  (this is not a required variable)
 *
 */
/************************************************************************************/
bool File::HasEmitterView() const
{
    return sofa::NetCDFFile::HasVariable( "EmitterView" );
}


bool File::GetListenerPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{    
    return File::get( coordinates, units, "ListenerPosition" ); 
}

bool File::GetListenerUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "ListenerUp" ); 
}

bool File::GetListenerView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "ListenerView" ); 
}

bool File::GetSourcePosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "SourcePosition" ); 
}

bool File::GetSourceUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "SourceUp" ); 
}

bool File::GetSourceView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "SourceView" ); 
}

bool File::GetReceiverPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{    
    return File::get( coordinates, units, "ReceiverPosition" ); 
}

bool File::GetReceiverUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "ReceiverUp" ); 
}

bool File::GetReceiverView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "ReceiverView" ); 
}

bool File::GetEmitterPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{    
    return File::get( coordinates, units, "EmitterPosition" ); 
}

bool File::GetEmitterUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "EmitterUp" ); 
}

bool File::GetEmitterView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const
{
    return File::get( coordinates, units, "EmitterView" ); 
}

bool File::GetReceiverPosition(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "ReceiverPosition" );
}

bool File::GetReceiverUp(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "ReceiverUp" );
}

bool File::GetReceiverView(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "ReceiverView" );
}

bool File::GetEmitterPosition(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "EmitterPosition" );
}

bool File::GetEmitterUp(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "EmitterUp" );
}

bool File::GetEmitterView(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "EmitterView" );
}

bool File::GetListenerPosition(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "ListenerPosition" );
}

bool File::GetListenerUp(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "ListenerUp" );
}

bool File::GetListenerView(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "ListenerView" );
}

bool File::GetSourcePosition(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "SourcePosition" );
}

bool File::GetSourceUp(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "SourceUp" );
}

bool File::GetSourceView(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    return NetCDFFile::GetValues( values, dim1, dim2, "SourceView" );
}

bool File::GetListenerPosition(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ListenerPosition" );
}

bool File::GetListenerUp(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ListenerUp" );
}

bool File::GetListenerView(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ListenerView" );
}

bool File::GetSourcePosition(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "SourcePosition" );
}

bool File::GetSourceUp(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "SourceUp" );
}

bool File::GetSourceView(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "SourceView" );
}

bool File::GetReceiverPosition(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ReceiverPosition" );
}

bool File::GetReceiverUp(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ReceiverUp" );
}

bool File::GetReceiverView(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "ReceiverView" );
}

bool File::GetEmitterPosition(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "EmitterPosition" );
}

bool File::GetEmitterUp(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "EmitterUp" );
}

bool File::GetEmitterView(std::vector< double > &values) const
{
    return NetCDFFile::GetValues( values, "EmitterView" );
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
bool File::getDataIR(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    SOFA_ASSERT( HasVariable( "Data.IR" ) == true );
    SOFA_ASSERT( GetVariableDimensionality( "Data.IR" ) == 3 );
    
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
bool File::getDataIR(std::vector< double > &values) const
{
    SOFA_ASSERT( HasVariable( "Data.IR" ) == true );
    
    return NetCDFFile::GetValues( values, "Data.IR" );
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the Data.Delay values
 *  @param[in]      values : the array is resized if needed
 *  @return         true on success
 *
 */
/************************************************************************************/
bool File::getDataDelay(std::vector< double > &values) const
{
    SOFA_ASSERT( HasVariable( "Data.Delay" ) == true );
    
    return NetCDFFile::GetValues( values, "Data.Delay" );
}

bool File::getDataDelay(double *values, const unsigned long dim1, const unsigned long dim2) const
{
    SOFA_ASSERT( HasVariable( "Data.Delay" ) == true );
    SOFA_ASSERT( GetVariableDimensionality( "Data.Delay" ) == 2 );
    
    return NetCDFFile::GetValues( values, dim1, dim2, "Data.Delay" );
}

bool File::getDataDelay(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const
{
    SOFA_ASSERT( HasVariable( "Data.Delay" ) == true );
    SOFA_ASSERT( GetVariableDimensionality( "Data.Delay" ) == 3 );
    
    return NetCDFFile::GetValues( values, dim1, dim2, dim3, "Data.Delay" );
}

/************************************************************************************/
/*!
 *  @brief          The Data.SamplingRate variable can be either [I] or [M],
 *                  according to the specifications.
 *                  This function returns true if Data.SamplingRate is [I]
 *
 */
/************************************************************************************/
bool File::isSamplingRateScalar() const
{
    SOFA_ASSERT( HasVariable( "Data.SamplingRate" ) == true );
    
    return VariableIsScalar( "Data.SamplingRate" ) == true
        && HasVariableType( netCDF::NcType::nc_DOUBLE, "Data.SamplingRate");
}

/************************************************************************************/
/*!
 *  @brief          In case Data.SamplingRate is of dimension [I], this function returns
 *                  its value. In case Data.SamplingRate is of dimension [M], an error is thrown
 *  @return         true on success
 *
 */
/************************************************************************************/
bool File::getSamplingRate(double &value) const
{
    SOFA_ASSERT( HasVariable( "Data.SamplingRate" ) == true );
    
    if( isSamplingRateScalar() == true )
    {
        const netCDF::NcVar var = getVariable( "Data.SamplingRate" );
        
        return sofa::NcUtils::GetValue( value, var );
    }
    else
    {
        SOFA_THROW( "'Data.SamplingRate' is not a scalar" );
        return false;
    }
}

/************************************************************************************/
/*!
 *  @brief          Retrieves the units of the Data.SamplingRate variable
 *  @return         true on success
 *
 */
/************************************************************************************/
bool File::getSamplingRateUnits(sofa::Units::Type &units) const
{
    SOFA_ASSERT( HasVariable( "Data.SamplingRate" ) == true );
    
    const netCDF::NcVar var = getVariable( "Data.SamplingRate" );
    
    const netCDF::NcVarAtt attNUnits    = sofa::NcUtils::GetAttribute( var, "Units" );
    const std::string unitsName         = sofa::NcUtils::GetAttributeValueAsString( attNUnits );
    
    units = sofa::Units::GetType( unitsName );
    
    return true;
}

/************************************************************************************/
/*!
 *  @brief          Throws an exception if the file does not contain the requested global attribute
 *
 */
/************************************************************************************/
void File::ensureGlobalAttribute(const sofa::Attributes::Type &type_) const
{
    if( HasAttribute( type_ ) == false )
    {
        const std::string err = "Missing SOFA attribute '" + sofa::Attributes::GetName( type_ ) + "'";
        SOFA_THROW( err );
    }
}

/************************************************************************************/
/*!
 *  @brief          Throws an exception if the file does not contain the requested global attribute
 *
 */
/************************************************************************************/
void File::ensureGlobalAttribute(const std::string &attributeName) const
{
    const netCDF::NcGroupAtt att = getAttribute( attributeName );
    
    /// in SOFA, the global attributes must always be strings
    if( sofa::NcUtils::IsChar( att ) != true )
    {
        const std::string err = "Missing '" + attributeName + "' global attribute";
        SOFA_THROW( err );
    }
}

/************************************************************************************/
/*!
 *  @brief          Throws an exception if the file does not match the requested convention
 *
 */
/************************************************************************************/
void File::ensureSOFAConvention(const std::string &conventionName) const
{
    const std::string attrName  = sofa::Attributes::GetName( sofa::Attributes::kSOFAConventions );
    const std::string attrValue = GetAttributeValueAsString( attrName );
    
    if( attrValue != conventionName )
    {
        SOFA_THROW( "Not a '" + conventionName + "' SOFAConvention" );
    }
}

/************************************************************************************/
/*!
 *  @brief          Throws an exception if the file does not match the requested DataType
 *
 */
/************************************************************************************/
void File::ensureDataType(const std::string &typeName) const
{
    const std::string attrName  = sofa::Attributes::GetName( sofa::Attributes::kDataType );
    const std::string attrValue = GetAttributeValueAsString( attrName );
    
    if( attrValue != typeName )
    {
        SOFA_THROW( "Invalid 'DataType' : " + attrValue );
    }
}

