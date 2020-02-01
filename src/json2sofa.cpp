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
//#include <ncDim.h>
//#include <ncVar.h>
#include <netcdf>

using namespace netCDF;
using namespace netCDF::exceptions;

//==============================================================================
// global attributes
//==============================================================================
bool writeAttributes(json_object *jobj, NcFile &dataFile)
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
      return false;
    }
  }
  return true;
}

//==============================================================================
// dimensions
//==============================================================================
bool writeDimensions(json_object *jobj, NcFile &dataFile)
{
  json_object_object_foreach(jobj, key, val)
  {
    switch (json_object_get_type(val))
    {
    case json_type_int:
      dataFile.addDim(key, json_object_get_int(val));
      break;
    default:
      std::cerr << "Dropping dimension " << key << " " << json_object_get_type(val) << std::endl;
      return false;
    }
  }
  return true;
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
      for (size_t i = 0; i < json_object_array_length(val); i++)
      {
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
    {
      std::cerr << "variable " << name << " " << key << " " << json_object_get_type(val) << std::endl;
      return false;
    }
  }

  // write to netcdf file
  NcVar var = dataFile.addVar(name, typeName, dimensionNames);
  for (size_t i = 0; i < attributeNames.size(); i++)
    var.putAtt(attributeNames[i], attributeValues[i]);
  var.putVar(values.data());

  return true;
}

//==============================================================================
// variables
//==============================================================================
bool writeVariables(json_object *jobj, NcFile &dataFile)
{
  json_object_object_foreach(jobj, key, val)
  {
    switch (json_object_get_type(val))
    {
    case json_type_object:
      if (!writeVariable(key, val, dataFile))
        return false;
      break;
    default:
      std::cerr << "Dropping variable " << key << " " << json_object_get_type(val) << std::endl;
      return false;
    }
  }
}

/************************************************************************************/
/*!
 *  @brief          Convert all informations about a NetCDFFile file to JSON
 *
 */
/************************************************************************************/
bool writeSofa(json_object *jobj,
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
      {
        if (!writeAttributes(val, dataFile))
          return false;
      }
      else if (!strcmp(key, "Dimensions"))
      {
        if (!writeDimensions(val, dataFile))
          return false;
      }
      else if (!strcmp(key, "Variables"))
      {
        if (!writeVariables(val, dataFile))
          return false;
      }
      else
      {
        std::cerr << "Unknown variable " << key << std::endl;
        return false;
      }
    }
  }
  catch (NcException &e)
  {
    e.what();
    return false;
  }
  return true;
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

  // write files
  bool result = writeSofa(jobj, argv[2]);

  // quit
  json_object_put(jobj);
  return result?1:0;
}
