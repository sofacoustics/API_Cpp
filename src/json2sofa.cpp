/************************************************************************************/
/*  FILE DESCRIPTION */
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
#include "../src/SOFA.h"
#include <iostream>
#include <json-c/json.h>
#include <ncDim.h>
#include <ncVar.h>
#include <netcdf>

using namespace netCDF;
using namespace netCDF::exceptions;

/************************************************************************************/
/*!
 *  @brief          Testing whether a file match a SOFA convention or not
 *                  without raising any exception
 *
 */
/************************************************************************************/
static bool TestFileConvention(json_object *jobj, const std::string &filename)
{
  bool valid = sofa::IsValidSimpleFreeFieldHRIRFile(filename);
  json_object_object_add(
      jobj, "isNetCDF",
      json_object_new_boolean(sofa::IsValidNetCDFFile(filename)));
  json_object_object_add(
      jobj, "isSOFA", json_object_new_boolean(sofa::IsValidSOFAFile(filename)));
  json_object_object_add(jobj, "isSimpleFreeFieldHRIR",
                         json_object_new_boolean(valid));
  json_object_object_add(
      jobj, "isSimpleFreeFieldSOS",
      json_object_new_boolean(sofa::IsValidSimpleFreeFieldSOSFile(filename)));
  json_object_object_add(
      jobj, "isSimpleHeadphoneIRF",
      json_object_new_boolean(sofa::IsValidSimpleHeadphoneIRFile(filename)));
  json_object_object_add(
      jobj, "isGeneralFIR",
      json_object_new_boolean(sofa::IsValidGeneralFIRFile(filename)));
  json_object_object_add(
      jobj, "isGeneralTF",
      json_object_new_boolean(sofa::IsValidGeneralTFFile(filename)));

  return valid;
}

const NcDouble myNcDouble;

//==============================================================================
// global attributes
//==============================================================================
void writeAttributes(json_object *jobj, NcFile &dataFile)
{
  json_object_object_foreach(jobj, key, val)
  {
    switch (json_object_get_type(val))
    {
    case json_type_string:
      dataFile.putAtt(key, json_object_get_string(val));
      break;
    default:
      std::cerr << "Dropping dimensions " << key << std::endl;
    }
  }
}

//==============================================================================
// dimensions
//==============================================================================
void writeDimensions(json_object *jobj, NcFile &dataFile)
{
  json_object_object_foreach(jobj, key, val)
  {
    switch (json_object_get_type(val))
    {
    case json_type_int:
      dataFile.addDim(key, json_object_get_int(val));
      break;
    default:
      std::cerr << "Dropping attribute " << key << " " << json_object_get_type(val) << std::endl;
    }
  }
}

//==============================================================================
// variables
//==============================================================================
bool writeVariable(std::string name, json_object *jobj, NcFile &dataFile)
{
  std::string typeName;
  std::vector<size_t> dimensions;
  std::vector<std::string> dimensionNames;
  std::vector<std::string> attributeNames;
  std::vector<std::string> attributeValues;
  std::vector<double> values;
  
  json_object_object_foreach(jobj, key, val)
  {
    if (!strcmp(key, "TypeName") && json_object_get_type(val) == json_type_string)
    {
      typeName = json_object_get_string(val);
    }
    else if (!strcmp(key, "Dimensions") && json_object_get_type(val) == json_type_array)
    {
      dimensions.reserve(json_object_array_length(val));

      for (size_t i = 0; i < json_object_array_length(val); i++)
      {
        json_object *node = json_object_array_get_idx(val, i);
        if (node == nullptr || !json_object_is_type(node, json_type_int))
        {
          std::cerr << "variable " << name << " dimension array incorrect " << json_object_get_type(val) << std::endl;
          return false;
        }
        dimensions.push_back(json_object_get_int(node));
      }
    }
    else if (!strcmp(key, "DimensionNames") && json_object_get_type(val) == json_type_array)
    {
      for (size_t i = 0; i < json_object_array_length(val); i++)
      {
        json_object *node = json_object_array_get_idx(val, i);
        if (node == nullptr || !json_object_is_type(node, json_type_string))
        {
          std::cerr << "variable " << name << " dimensionNames array incorrect " << json_object_get_type(val) << std::endl;
          return false;
        }
                dimensionNames.push_back(json_object_get_string(node));
      }
    }
    else if (!strcmp(key, "Attributes") && json_object_get_type(val) == json_type_object)
    {
      json_object_object_foreach(val, akey, aval)
      {
        attributeNames.push_back(akey);
        attributeValues.push_back(json_object_get_string(aval));
      }
    }
    else if (!strcmp(key, "Values") && json_object_get_type(val) == json_type_array)
    {
        values.reserve(json_object_array_length(val));
        for (size_t i = 0; i < json_object_array_length(val); i++) {
        json_object *node = json_object_array_get_idx(val, i);
        if (node == nullptr || !(json_object_is_type(node, json_type_int) || json_object_is_type(node, json_type_double)))
        {
          std::cerr << "variable " << name << " values incorrect " << i << " " << json_object_get_type(node) << std::endl;
          return false;
        }
        values.push_back(json_object_get_double(node));
      }
    }
    else
      std::cerr << "variable " << name << " " << key << " " << json_object_get_type(val) << std::endl;
  }

  // print
  std::cerr << name << " typeName " << typeName << std::endl;
  for (auto i : dimensions)
    std::cerr << name << " dimension " << i << std::endl;
  for (auto i : dimensionNames)
    std::cerr << name << " dimensionName " << i << std::endl;
  for (auto i : attributeNames)
    std::cerr << name << " attributeNames " << i << std::endl;
  for (auto i : attributeValues)
    std::cerr << name << " attributeValues " << i << std::endl;
    std::cerr << name << " values " << values.size() << std::endl;

  NcVar var = dataFile.addVar(name, typeName, dimensionNames);
  for (size_t i=0; i < attributeNames.size();i++)
      var.putAtt(attributeNames[i], attributeValues[i]);
  var.putVar(values.data());
  
  return true;
}

//==============================================================================
// variables
//==============================================================================
void writeVariables(json_object *jobj, NcFile &dataFile)
{
  json_object_object_foreach(jobj, key, val)
  {
    switch (json_object_get_type(val))
    {
    case json_type_object:
      writeVariable(key, val, dataFile);
      break;
    default:
      std::cerr << "Dropping variable " << key << " " << json_object_get_type(val) << std::endl;
    }
  }
#if 0


      std::vector<std::string> dimNames;
      file.GetVariableDimensionsNames(dimNames, name);
      json_object *jarray2 = json_object_new_array();
      json_object_object_add(jobj3, "DimensionNames", jarray2);
      for (size_t i = 0; i < dimNames.size(); i++)
        json_object_array_add(jarray2,
                              json_object_new_string(dimNames[i].c_str()));

      std::vector<std::string> attributeNames;
      std::vector<std::string> attributeValues;
      file.GetVariablesAttributes(attributeNames, attributeValues, name);

      SOFA_ASSERT(attributeNames.size() == attributeValues.size());

      if (attributeNames.size() > 0) {
        json_object *jobj4 = json_object_new_object();
        json_object_object_add(jobj3, "Attributes", jobj4);

        for (std::size_t j = 0; j < attributeNames.size(); j++)
          json_object_object_add(
              jobj4, attributeNames[j].c_str(),
              json_object_new_string(attributeValues[j].c_str()));
      }

      std::vector<double> values;
      file.GetValues(values, name);
      json_object *jarray3 = json_object_new_array();
      json_object_object_add(jobj3, "Values", jarray3);
      for (size_t i = 0; i < values.size();)
        json_object_array_add(jarray3, json_object_new_double(values[i++]));
    }
#endif
}

/************************************************************************************/
/*!
 *  @brief          Convert all informations about a NetCDFFile file to JSON
 *
 */
/************************************************************************************/
static void writeSofa(json_object *jobj,
                      const std::string &filename)
{
  try
  {
    NcFile dataFile(filename, NcFile::replace);

    json_object_object_foreach(jobj, key, val)
    {
      if (json_object_get_type(val) != json_type_object)
      {
        std::cerr << "Variable with invalid type " << key << std::endl;
        continue;
      }
      if (!strcmp(key, "Attributes"))
        writeAttributes(val, dataFile);
      else if (!strcmp(key, "Dimensions"))
        writeDimensions(val, dataFile);
      else if (!strcmp(key, "Variables"))
        writeVariables(val, dataFile);
      else
      {
        std::cerr << "Unknown variable " << key << std::endl;
        continue;
      }
    }
  }
  catch (NcException &e)
  {
    e.what();
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
  int sz;
  char *filestring;

  // parse command line
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " hrtf.json hrtf.sofa" << std::endl;
    return 1;
  }

  // open file
  FILE *fhd = fopen(argv[1], "rb");
  if (fhd == NULL)
  {
    std::cerr << "Cannot open file " << argv[1] << std::endl;
    return 2;
  }

  // get file size
  fseek(fhd, 0L, SEEK_END);
  sz = ftell(fhd);
  fseek(fhd, 0L, SEEK_SET);
  std::cout << "File " << argv[1] << " has size " << sz << std::endl;

  // alloc memory for file
  filestring = (char *)malloc(sz + 1);
  if (!filestring)
  {
    std::cerr << "Out of memory" << std::endl;
    free(filestring);
    fclose(fhd);
    return 3;
  }

  // read file
  if (fread(filestring, sz, 1, fhd) != 1)
  {
    std::cerr << "Cannot read file" << std::endl;
    free(filestring);
    fclose(fhd);
    return 4;
  }
  filestring[sz] = 0;

  // parse file
  json_object *jobj = json_tokener_parse(filestring);
  if (jobj == NULL)
  {
    std::cerr << "JSON error" << std::endl;
    free(filestring);
    fclose(fhd);
    return 5;
  }

  // free file memory
  free(filestring);
  fclose(fhd);

  // convert to SOFA format

  /*
function [Obj] = SOFAsave(filename,Obj,varargin)
%SOFASAVE 
%   [] = SOFAsave(filename,Obj,Compression) creates a new SOFA file and
%   writes an entire data set to it.
%
%   filename specifies the name of the SOFA file to which the data is written.
%   Obj is a struct containing the data and meta
%   data to be written to the SOFA file (see below for exact format).
%   Compression is an optional numeric value between 0 and 9 specifying the
%   amount of compression to be applied to the data when writing to the netCDF file.
%   0 is no compression and 9 is the most compression.
% 
%   The existence of mandatory variables will be checked. The dimensions
%   will be updated.

% SOFA API - function SOFAsave
% Copyright (C) 2012-2013 Acoustics Research Institute - Austrian Academy of Sciences
% Licensed under the EUPL, Version 1.1 or - as soon they will be approved by the European Commission - subsequent versions of the EUPL (the "License")
% You may not use this work except in compliance with the License.
% You may obtain a copy of the License at: http://joinup.ec.europa.eu/software/page/eupl
% Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing  permissions and limitations under the License. 

Def = SOFAdefinitions;

%% check file name
filename=SOFAcheckFilename(filename);

%% Remove private data
if isfield(Obj,'PRIVATE'), Obj=rmfield(Obj,'PRIVATE'); end

%% Check convention: mandatory variables
ObjCheck = SOFAgetConventions(Obj.GLOBAL_SOFAConventions,'m');

varNames = fieldnames(ObjCheck);
for ii=1:size(varNames,1);
    if ~isfield(Obj,varNames{ii})
        error(['Mandatory variable/attribute not existing: ' varNames{ii}]);
    end
end

%% Get & set dimensions
Obj=SOFAupdateDimensions(Obj);

%% Check convention: read-only variables
ObjCheck = SOFAgetConventions(Obj.GLOBAL_SOFAConventions,'r');
varNames = fieldnames(ObjCheck);

for ii=1:size(varNames,1);
	if ischar(Obj.(varNames{ii}))
		if ~strcmp(Obj.(varNames{ii}), ObjCheck.(varNames{ii}))
			warning('SOFA:save',[varNames{ii} ' is read-only and was reset to '  ObjCheck.(varNames{ii})],0);
			Obj.(varNames{ii})=ObjCheck.(varNames{ii});
		end
	else
		if Obj.(varNames{ii}) ~= ObjCheck.(varNames{ii})
			warning('SOFA:save',[varNames{ii} ' is read-only and was reset to '  ObjCheck.(varNames{ii})],0);
			Obj.(varNames{ii})=ObjCheck.(varNames{ii});
		end
	end
end

%% check attributes (syntax, 1-dimensional string)
varNames = fieldnames(Obj);
for ii=1:size(varNames,1);
    
    if size(strfind(varNames{ii},'_'),2) == 1
        if ~ischar(Obj.(varNames{ii}))
            error(['Attribute not a valid string: ' varNames{ii} ' = ' num2str(Obj.(varNames{ii}))]);
        end
    elseif size(strfind(varNames{ii},'_'),2) > 1
        error(['Attribute not valid (only one underscore "_" is allowed in attribute name): ' varNames{ii}]);
    end
    
end

%% check varargin (compression)
if ~isempty(varargin) && isnumeric(varargin{1})
	if isscalar(varargin{1}) && varargin{1}>=0 && varargin{1}<=9
        Compression = varargin{1};
	else
        error('Error: Compression must be a numeric scalar value between 0 and 9.');
	end
else
    Compression = 1; % default
end

%% Set/modify time information
Obj.GLOBAL_DateModified=datestr(now,Def.dateFormat);
if isempty(Obj.GLOBAL_DateCreated), Obj.GLOBAL_DateCreated=Obj.GLOBAL_DateModified; end

%% Save file
NETCDFsave(filename,Obj,Compression);
*/

  //  puts(json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
  writeSofa(jobj, argv[2]);

  json_object_put(jobj);
  return 0;
}
