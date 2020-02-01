/************************************************************************************/
/*  FILE DESCRIPTION                                                                */
/*----------------------------------------------------------------------------------*/
/*!
 *   @file       sofa2json.cpp
 *   @brief      converts a sofa file to json.
 *   @author     Christian Hoene, Symonics GmbH
 *
 *   @date       29/09/2016
 *
 */
/************************************************************************************/
#include <iostream>
#include "../src/SOFA.h"
#include <ncDim.h>
#include <ncVar.h>
#include <json-c/json.h>

/************************************************************************************/
/*!
 *  @brief          Testing whether a file match a SOFA convention or not
 *                  without raising any exception
 *
 */
/************************************************************************************/
static bool TestFileConvention(json_object *jobj, const std::string & filename)
{
	bool valid = sofa::IsValidSimpleFreeFieldHRIRFile( filename );
	json_object_object_add(jobj, "isNetCDF", json_object_new_boolean(sofa::IsValidNetCDFFile( filename )));
	json_object_object_add(jobj, "isSOFA", json_object_new_boolean(sofa::IsValidSOFAFile( filename )));
	json_object_object_add(jobj, "isSimpleFreeFieldHRIR", json_object_new_boolean(valid));
	json_object_object_add(jobj, "isSimpleFreeFieldSOS", json_object_new_boolean(sofa::IsValidSimpleFreeFieldSOSFile( filename )));
	json_object_object_add(jobj, "isSimpleHeadphoneIRF", json_object_new_boolean(sofa::IsValidSimpleHeadphoneIRFile( filename )));
	json_object_object_add(jobj, "isGeneralFIR", json_object_new_boolean(sofa::IsValidGeneralFIRFile( filename )));
	json_object_object_add(jobj, "isGeneralTF", json_object_new_boolean(sofa::IsValidGeneralTFFile( filename )));

	return valid;
}

/************************************************************************************/
/*!
 *  @brief          Convert all informations about a NetCDFFile file to JSON
 *
 */
/************************************************************************************/
static void DisplayInformations(json_object *jobj, const std::string & filename)
{
    ///@n this doesnt check whether the file corresponds to SOFA conventions...
    const sofa::NetCDFFile file( filename );
    
    //==============================================================================
    // global attributes
    //==============================================================================
    {
	json_object * jobj2 = json_object_new_object();
	json_object_object_add(jobj, "Attributes", jobj2);

        std::vector< std::string > attributeNames;
        file.GetAllAttributesNames( attributeNames );
        
        for( std::size_t i = 0; i < attributeNames.size(); i++ )
        {
            	const std::string name = attributeNames[i];
            	const std::string value= file.GetAttributeValueAsString( name );

		json_object_object_add(jobj2, name.c_str(), json_object_new_string(value.c_str()));
        }
    }
    
    //==============================================================================
    // dimensions
    //==============================================================================
    {
	json_object * jobj2 = json_object_new_object();
	json_object_object_add(jobj, "Dimensions", jobj2);

        std::vector< std::string > dimensionNames;
        file.GetAllDimensionsNames( dimensionNames );
        
        for( std::size_t i = 0; i < dimensionNames.size(); i++ )
        {
            const std::string name = dimensionNames[i];
            const std::size_t dim  = file.GetDimension( name );

		json_object_object_add(jobj2, name.c_str(), json_object_new_int64(dim));
        }
    }

    //==============================================================================
    // variables
    //==============================================================================
    {
	json_object * jobj2 = json_object_new_object();
	json_object_object_add(jobj, "Variables", jobj2);

        std::vector< std::string > variableNames;
        file.GetAllVariablesNames( variableNames );
     
        for( std::size_t i = 0; i < variableNames.size(); i++ )
        {
            	const std::string name = variableNames[i];
		json_object *jobj3 = json_object_new_object();
		json_object_object_add(jobj2, name.c_str(), jobj3);

            	const std::string typeName  = file.GetVariableTypeName( name );
		json_object_object_add(jobj3, "TypeName", json_object_new_string(typeName.c_str()));

		std::vector<std::size_t> dims;
		file.GetVariableDimensions(dims, name);
		json_object *jarray1 = json_object_new_array();
		json_object_object_add(jobj3, "Dimensions", jarray1);
		for(size_t i=0; i<dims.size(); i++)
			json_object_array_add(jarray1,json_object_new_int64(dims[i]));

		std::vector<std::string> dimNames;
        	file.GetVariableDimensionsNames(dimNames, name);
		json_object *jarray2 = json_object_new_array();
		json_object_object_add(jobj3, "DimensionNames", jarray2);
		for(size_t i=0; i<dimNames.size(); i++)
			json_object_array_add(jarray2,json_object_new_string(dimNames[i].c_str()));
	
		std::vector< std::string > attributeNames;
            	std::vector< std::string > attributeValues;
            	file.GetVariablesAttributes( attributeNames, attributeValues, name );
            
            	SOFA_ASSERT( attributeNames.size() == attributeValues.size() );
            
            	if( attributeNames.size() > 0 )
            	{
			json_object *jobj4 = json_object_new_object();
			json_object_object_add(jobj3, "Attributes", jobj4);

			for( std::size_t j = 0; j < attributeNames.size(); j++ )
				json_object_object_add(jobj4, attributeNames[j].c_str(), json_object_new_string(attributeValues[j].c_str()));
	    	}

		std::vector<double> values;
		file.GetValues(values, name);
		json_object *jarray3 = json_object_new_array();
		json_object_object_add(jobj3, "Values", jarray3);
    	for(size_t i=0; i<values.size();)
				json_object_array_add(jarray3,json_object_new_double(values[i++]));
	    }
    }
}


/************************************************************************************/
/*!
 *  @brief          Main entry point
 *
 */
/************************************************************************************/
int main(int argc, char *argv[])
{
	if(argc!=2) {
		std::cerr << "Usage: " << argv[0] << " hrtf.sofa" << std::endl;
		return 1;
	}
 
	json_object * jobj = json_object_new_object();

#if 0
	json_object_object_add(jobj, "filename", json_object_new_string(argv[1]));

	if(!TestFileConvention(jobj, argv[1] )) {
		puts(json_object_to_json_string(jobj));
		return 1;
	}

	json_object * jobj2 = json_object_new_object();
	json_object_object_add(jobj, "HRTF", jobj2);
	DisplayInformations(jobj2, argv[1]);
#else
	DisplayInformations(jobj, argv[1]);
#endif

	puts(json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
	return 0;
}

