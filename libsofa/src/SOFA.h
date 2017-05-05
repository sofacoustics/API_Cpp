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
 *   @file       SOFA.h
 *   @brief      Spatially Oriented Format for Acoustics
 *   @author     Thibaut Carpentier, UMR STMS 9912 - Ircam-Centre Pompidou / CNRS / UPMC
 *
 *   @date       10/05/2013
 * 
 *
 *   @see        "Spatially Oriented Format for Acoustics:
 *               A Data Exchange Format Representing Head-Related Transfer Functions"
 *                 
 *               Piotr Majdak, Yukio Iwaya, Thibaut Carpentier, Rozenn Nicol, Matthieu Parmentier,
 *                 Agnieszka Roginska, Yôiti Suzuki, Kanji Watanabe, Hagen Wierstorf, Harald Ziegelwanger, and Markus Noisternig
 *                 
 *               Presented at the 134th Audio Engineering Society Convention,
 *                 2013 May 4–7 Rome, Italy
 */
/************************************************************************************/
#ifndef _SOFA_H__
#define _SOFA_H__

//==============================================================================
/// public API
//==============================================================================
#include "../src/SOFAAPI.h"
#include "../src/SOFAAttributes.h"
#include "../src/SOFACoordinates.h"
#include "../src/SOFAFile.h"
#include "../src/SOFANcFile.h"
#include "../src/SOFAPlatform.h"
#include "../src/SOFASimpleFreeFieldHRIR.h"
#include "../src/SOFASimpleFreeFieldSOS.h"
#include "../src/SOFASimpleHeadphoneIR.h"
#include "../src/SOFAMultiSpeakerBRIR.h"
#include "../src/SOFAGeneralFIR.h"
#include "../src/SOFAGeneralFIRE.h"
#include "../src/SOFAGeneralTF.h"
#include "../src/SOFASingleRoomDRIR.h"
#include "../src/SOFAUnits.h"
#include "../src/SOFAVersion.h"
#include "../src/SOFAHelper.h"

//==============================================================================
/// private files
//==============================================================================
//#include "../src/SOFAHostArchitecture.h"
//#include "../src/SOFAExceptions.h"
//#include "../src/SOFAString.h"
//#include "../src/SOFADate.h"
//#include "../src/SOFAEmitter.h"
//#include "../src/SOFAListener.h"
//#include "../src/SOFANcUtils.h"
//#include "../src/SOFAPosition.h"
//#include "../src/SOFAReceiver.h"
//#include "../src/SOFASource.h"
//#include "../src/SOFAUtils.h"

#endif /* _SOFA_H__ */

