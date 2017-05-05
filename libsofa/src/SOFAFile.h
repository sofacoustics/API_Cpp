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
 *   @file       SOFAFile.h
 *   @brief      Class for SOFA files
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#ifndef _SOFA_FILE_H__
#define _SOFA_FILE_H__

#include "../src/SOFANcFile.h"
#include "../src/SOFAAttributes.h"
#include "../src/SOFACoordinates.h"
#include "../src/SOFAUnits.h"

namespace sofa
{
    
    /************************************************************************************/
    /*!
     *  @class          File 
     *  @brief          Represents a sofa file
     *
     *  @details        Provides methods specific to SOFA files
     */
    /************************************************************************************/
    class SOFA_API File : public sofa::NetCDFFile
    {
    public:
        File(const std::string &path,
             const netCDF::NcFile::FileMode &mode = netCDF::NcFile::read);
        
        virtual ~File() {};
        
        virtual bool IsValid() const SOFA_OVERRIDE;
                
        //==============================================================================
        // SOFA Attributes
        //==============================================================================
        bool HasAttribute(const sofa::Attributes::Type &type_) const;
                
        void GetGlobalAttributes(sofa::Attributes &attributes) const;
        
        void PrintSOFAGlobalAttributes(std::ostream & output = std::cout,
                                       const bool withPadding = false) const;
        
        std::string GetSOFAConventions() const;
        
        bool IsFIRDataType() const;
        bool IsFIREDataType() const;
        bool IsTFDataType() const;
        bool IsSOSDataType() const;
        
        //==============================================================================
        // SOFA Dimensions
        //==============================================================================
        long GetNumMeasurements() const;
        long GetNumReceivers() const;
        long GetNumEmitters() const;
        long GetNumDataSamples() const;
        
        void PrintSOFADimensions(std::ostream & output = std::cout,
                                 const bool withPadding = false) const;

        //==============================================================================
        // SOFA Variables
        //==============================================================================
        bool GetListenerPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetListenerUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetListenerView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;

        bool GetReceiverPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetReceiverUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetReceiverView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool HasReceiverUp() const;
        bool HasReceiverView() const;

        bool GetSourcePosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetSourceUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetSourceView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool HasSourceUp() const;
        bool HasSourceView() const;
        
        bool GetEmitterPosition(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetEmitterUp(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool GetEmitterView(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units) const;
        bool HasEmitterUp() const;
        bool HasEmitterView() const;
        
        bool GetListenerPosition(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool GetListenerUp(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool GetListenerView(double *values, const unsigned long dim1, const unsigned long dim2) const;
        
        bool GetSourcePosition(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool GetSourceUp(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool GetSourceView(double *values, const unsigned long dim1, const unsigned long dim2) const;
        
        bool GetReceiverPosition(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        bool GetReceiverUp(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        bool GetReceiverView(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        
        bool GetEmitterPosition(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        bool GetEmitterUp(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        bool GetEmitterView(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        
        //==============================================================================
        bool GetListenerPosition(std::vector< double > &values) const;
        bool GetListenerUp(std::vector< double > &values) const;
        bool GetListenerView(std::vector< double > &values) const;
        
        bool GetSourcePosition(std::vector< double > &values) const;
        bool GetSourceUp(std::vector< double > &values) const;
        bool GetSourceView(std::vector< double > &values) const;
        
        bool GetReceiverPosition(std::vector< double > &values) const;
        bool GetReceiverUp(std::vector< double > &values) const;
        bool GetReceiverView(std::vector< double > &values) const;
        
        bool GetEmitterPosition(std::vector< double > &values) const;
        bool GetEmitterUp(std::vector< double > &values) const;
        bool GetEmitterView(std::vector< double > &values) const;
        
    protected:
        //==============================================================================
        bool hasSOFAConvention() const;
        bool hasSOFARequiredAttributes() const;
        bool hasSOFARequiredDimensions() const;
        bool SOFADimensionsAreValid() const;
        bool checkListenerVariables() const;
        bool checkSourceVariables() const;
        bool checkReceiverVariables() const;
        bool checkEmitterVariables() const;
        bool checkDimensions() const;
        bool checkDataVariable() const;
        bool checkFirDataType() const;
        bool checkFireDataType() const;
        bool checkTFDataType() const;
        bool checkSOSDataType() const;
        
        bool getCoordinates(sofa::Coordinates::Type &coordinates, const std::string &variableName) const;
        bool getUnits(sofa::Units::Type &units, const std::string &variableName) const;
        bool get(sofa::Coordinates::Type &coordinates, sofa::Units::Type &units, const std::string &variableName) const;
        
        //==============================================================================
        bool getDataIR(std::vector< double > &values) const;
        bool getDataIR(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        
        //==============================================================================
        bool getDataDelay(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool getDataDelay(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        bool getDataDelay(std::vector< double > &values) const;
        
        //==============================================================================
        bool isSamplingRateScalar() const;
        bool getSamplingRate(double &value) const;
        bool getSamplingRateUnits(sofa::Units::Type &units) const;
        
        //==============================================================================
        void ensureGlobalAttribute(const sofa::Attributes::Type &type_) const;
        void ensureGlobalAttribute(const std::string &attributeName) const;
        void ensureSOFAConvention(const std::string &conventionName) const;
        void ensureDataType(const std::string &typeName) const;
        
    private:
        //==============================================================================
        /// avoid shallow and copy constructor
        SOFA_AVOID_COPY_CONSTRUCTOR( File );
    };
    
}

#endif /* _SOFA_FILE_H__ */

