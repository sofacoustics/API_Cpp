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
 *   @file       SOFAAPI.h
 *   @brief      Informations about this API
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 */
/************************************************************************************/
#ifndef _SOFA_API_H__
#define _SOFA_API_H__

#include "../src/SOFAPlatform.h"

namespace sofa
{
    
    /************************************************************************************/
    /*!
     *  @class          ApiInfos 
     *  @brief          Retrieves generic infos about sofa API
     *
     */
    /************************************************************************************/
    class SOFA_API ApiInfos
    {
    public:
        //==============================================================================
        // API
        //==============================================================================
        static std::string GetAPIName();
        
        static std::string GetAPIVersion();
        
        static unsigned int GetAPIVersionMinor();
        static unsigned int GetAPIVersionMajor();
        static unsigned int GetAPIVersionRelease();
        
        static std::string GetAPICopyright();
        
        //==============================================================================
        // Specifications
        //==============================================================================
        static std::string GetSpecificationsVersion();
        
        static unsigned int GetSpecificationsVersionMinor();
        static unsigned int GetSpecificationsVersionMajor();
                
        //==============================================================================
        // Conventions
        //==============================================================================
        static std::string GetSimpleFreeFieldHRIRConventionVersion();
        static unsigned int GetSimpleFreeFieldHRIRConventionVersionMajor();
        static unsigned int GetSimpleFreeFieldHRIRConventionVersionMinor();

        static std::string GetSimpleFreeFieldSOSConventionVersion();
        static unsigned int GetSimpleFreeFieldSOSConventionVersionMajor();
        static unsigned int GetSimpleFreeFieldSOSConventionVersionMinor();

        static std::string GetSimpleHeadphoneIRConventionVersion();
        static unsigned int GetSimpleHeadphoneIRConventionVersionMajor();
        static unsigned int GetSimpleHeadphoneIRConventionVersionMinor();
        
        static std::string GetMultiSpeakerBRIRConventionVersion();
        static unsigned int GetMultiSpeakerBRIRConventionVersionMajor();
        static unsigned int GetMultiSpeakerBRIRConventionVersionMinor();
        
        static std::string GetGeneralFIRConventionVersion();
        static unsigned int GetGeneralFIRConventionVersionMajor();
        static unsigned int GetGeneralFIRConventionVersionMinor();
        
    private:
        ApiInfos() SOFA_DELETED_FUNCTION;
    };
    
}

#endif /* _SOFA_API_H__ */
