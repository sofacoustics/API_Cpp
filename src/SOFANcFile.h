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
 *   @file       SOFANcFile.h
 *   @brief      Class for NetCDF Files (essentially this class wraps the NcFile class)
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#ifndef _SOFA_NC_FILE_H__
#define _SOFA_NC_FILE_H__

#include "../src/SOFAPlatform.h"
#include "netcdf.h"
#include "ncFile.h"

namespace sofa
{
    
    /************************************************************************************/
    /*!
     *  @class          Class for NetCDF Files (essentially this class wraps the NcFile class)
     *  @brief          Represents a netcdf file
     *
     *  @details        Provides methods generic for all netCDF files
     */
    /************************************************************************************/
    class SOFA_API NetCDFFile
    {
    public:
        NetCDFFile(const std::string &path,
                   const netCDF::NcFile::FileMode &mode = netCDF::NcFile::read);
        
        virtual ~NetCDFFile() {};
        
        const std::string & GetFilename() const;
        
        virtual bool IsValid() const;
        
        //==============================================================================
        // netCDF Attributes
        //==============================================================================
        unsigned int GetNumGlobalAttributes() const;
        bool HasAttribute(const std::string &attributeName) const;
        
        netCDF::NcType GetAttributeType(const std::string &attributeName) const;
        
        bool IsAttributeFloat(const std::string &attributeName) const;
        bool IsAttributeDouble(const std::string &attributeName) const;
        bool IsAttributeByte(const std::string &attributeName) const;
        bool IsAttributeChar(const std::string &attributeName) const;
        bool IsAttributeShort(const std::string &attributeName) const;
        bool IsAttributeInt(const std::string &attributeName) const;
        bool IsAttributeInt64(const std::string &attributeName) const;
        
        std::string GetAttributeValueAsString(const std::string &attributeName) const;
        
        void GetAllCharAttributes(std::vector< std::string > &attributeNames,
                                  std::vector< std::string > &attributeValues) const;
        
        void GetAllAttributesNames(std::vector< std::string > &attributeNames) const;
        
        void PrintAllAttributes(std::ostream & output = std::cout,
                                const bool withPadding = false) const;
        
        
        //==============================================================================
        // netCDF Dimensions
        //==============================================================================
        unsigned int GetNumDimensions() const;
        std::size_t GetDimension(const std::string &dimensionName) const;
        bool HasDimension(const std::string &dimensionName) const;
        
        void GetAllDimensionsNames(std::vector< std::string > &dimensionNames) const;
        
        void PrintAllDimensions(std::ostream & output = std::cout) const;
                
        
        //==============================================================================
        // netCDF Variables
        //==============================================================================
        unsigned int GetNumVariables() const;
        bool HasVariable(const std::string &variableName) const;
        
        void GetAllVariablesNames(std::vector< std::string > &variableNames) const;
        
        netCDF::NcType GetVariableType(const std::string &variableName) const;
        std::string GetVariableTypeName(const std::string &variableName) const;
        
        bool HasVariableType(const netCDF::NcType &type_, const std::string &variableName) const;
        
        int GetVariableDimensionality(const std::string &variableName) const;
        void GetVariableDimensions(std::vector< std::size_t > &dims, const std::string &variableName) const;
        void GetVariableDimensionsNames(std::vector< std::string > &dims, const std::string &variableName) const;
        std::string GetVariableDimensionsNamesAsString(const std::string &variableName) const;
        std::string GetVariableDimensionsAsString(const std::string &variableName) const;
        
        bool VariableIsScalar(const std::string &variableName) const;
        
        bool VariableHasDimension(const std::size_t dim,
                                  const std::string &variableName) const;
        bool VariableHasDimensions(const std::size_t dim1,
                                   const std::size_t dim2,
                                   const std::string &variableName) const;
        bool VariableHasDimensions(const std::size_t dim1,
                                   const std::size_t dim2,
                                   const std::size_t dim3,
                                   const std::string &variableName) const;
        bool VariableHasDimensions(const std::size_t dim1,
                                   const std::size_t dim2,
                                   const std::size_t dim3,
                                   const std::size_t dim4,
                                   const std::string &variableName) const;
        
        void GetVariablesAttributes(std::vector< std::string > &attributeNames, const std::string &variableName) const;
        void GetVariablesAttributes(std::vector< std::string > &attributeNames,
                                    std::vector< std::string > &attributeValues,
                                    const std::string &variableName) const;
        bool VariableHasAttribute(const std::string &attributeName, const std::string &variableName) const;
        
        void PrintAllVariables(std::ostream & output = std::cout) const;
        
        bool GetValues(double *values,
                       const std::size_t dim1,
                       const std::size_t dim2,
                       const std::string &variableName) const;
        
        bool GetValues(double *values,
                       const std::size_t dim1,
                       const std::size_t dim2,
                       const std::size_t dim3,
                       const std::string &variableName) const;
        
        bool GetValues(double *values,
                       const std::size_t dim1,
                       const std::size_t dim2,
                       const std::size_t dim3,
                       const std::size_t dim4,
                       const std::string &variableName) const;
        
        bool GetValues(std::vector< double > &values,
                       const std::string &variableName) const;
        
    protected:
        //==============================================================================
        netCDF::NcGroupAtt getAttribute(const std::string &attributeName) const;
        
        netCDF::NcDim getDimension(const std::string &dimensionName) const;
        
        netCDF::NcVar getVariable(const std::string &variableName) const;
        

    protected:
        netCDF::NcFile file;
        const std::string filename;
        
    private:
        //==============================================================================
        /// avoid shallow and copy constructor
        SOFA_AVOID_COPY_CONSTRUCTOR( NetCDFFile );         
    };
    
}

#endif /* _SOFA_NC_FILE_H__ */

