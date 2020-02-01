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
 *   @file       SOFASingleRoomDRIR.h
 *   @brief      Class for SOFA files with SingleRoomDRIR convention
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       14/04/2017
 *
 */
/************************************************************************************/
#ifndef _SOFA_SINGLE_ROOM_DRIR_H__
#define _SOFA_SINGLE_ROOM_DRIR_H__

#include "../src/SOFAFile.h"

namespace sofa
{
    
    /************************************************************************************/
    /*!
     *  @class          SingleRoomDRIR
     *  @brief          Class for SOFA files with SingleRoomDRIR convention
     *
     *  @details        Provides methods specific to SOFA files with SingleRoomDRIR convention
     */
    /************************************************************************************/
    class SOFA_API SingleRoomDRIR : public sofa::File
    {
    public:
        static const unsigned int ConventionVersionMajor;
        static const unsigned int ConventionVersionMinor;
        static std::string GetConventionVersion();
        
    public:
        SingleRoomDRIR(const std::string &path,
                       const netCDF::NcFile::FileMode &mode = netCDF::NcFile::read);
        
        virtual ~SingleRoomDRIR() {};
        
        virtual bool IsValid() const SOFA_OVERRIDE;
        
        bool GetSamplingRate(double &value) const;
        bool GetSamplingRateUnits(sofa::Units::Type &units) const;
        
        //==============================================================================
        bool GetDataIR(std::vector< double > &values) const;
        bool GetDataIR(double *values, const unsigned long dim1, const unsigned long dim2, const unsigned long dim3) const;
        
        //==============================================================================
        bool GetDataDelay(double *values, const unsigned long dim1, const unsigned long dim2) const;
        bool GetDataDelay(std::vector< double > &values) const;
        
    private:
        //==============================================================================
        bool checkGlobalAttributes() const;
        bool checkListenerVariables() const;
        
    private:
        /// avoid shallow and copy constructor
        SOFA_AVOID_COPY_CONSTRUCTOR( SingleRoomDRIR );
    };
    
}

#endif /* _SOFA_SINGLE_ROOM_DRIR_H__ */

