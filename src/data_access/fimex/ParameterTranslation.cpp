/*
 pgen

 Copyright (C) 2011 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#include "ParameterTranslation.h"
#include <data_access/Source.h>
#include <fimex/Data.h>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem.hpp>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdexcept>
#include <limits>


namespace data_access
{
namespace fimex
{
const std::map<WeatherData, std::string> ParameterTranslation::configFileIdentifiers_ =
		boost::assign::map_list_of
			(CloudCover, "cloud_cover")
			(YrSymbol, "yr_symbol")
			(ProbabilityOfYrSymbol, "probability_yr_symbol")
			(Temperature, "temperature")
			(TemperatureMin, "temperature_min")
			(TemperatureMax, "temperature_max")
			(Temperature10Percentile, "temperature_10")
			(Temperature25Percentile, "temperature_25")
			(Temperature50Percentile, "temperature_50")
			(Temperature75Percentile, "temperature_75")
			(Temperature90Percentile, "temperature_90")
			(TemperatureProbability1, "temperature_1_prob")
			(TemperatureProbability2, "temperature_2_prob")
			(WindU, "x_wind")
			(WindV, "y_wind")
			(WindProbability, "wind_prob")
			(Wind10Percentile, "wind_10")
			(Wind25Percentile, "wind_25")
			(Wind50Percentile, "wind_50")
			(Wind75Percentile, "wind_75")
			(Wind90Percentile, "wind_90")
			(PrecipitationInFrom, "precipitation_from_in")
			(PrecipitationInTo, "precipitation_to_in")
			(PrecipitationOut, "precipitation_out")
			(Precipitation10Percentile, "precipitation_10")
			(Precipitation25Percentile, "precipitation_25")
			(Precipitation50Percentile, "precipitation_50")
			(Precipitation75Percentile, "precipitation_75")
			(Precipitation90Percentile, "precipitation_90")
			(PrecipitationAbove0_1mm, "precipitation_1")
			(PrecipitationAbove0_2mm, "precipitation_2")
			(PrecipitationAbove0_5mm, "precipitation_3")
			(PrecipitationAbove1_0mm, "precipitation_4")
			(PrecipitationAbove2_0mm, "precipitation_5")
			(PrecipitationAbove5_0mm, "precipitation_6")
			(AirPressure, "air_pressure")
			(RelativeHumidity, "relative_humidity")
			(DewPointTemperature, "dew_point_temperature")
			(LowCloudCover, "low_clouds")
			(MediumCloudCover, "medium_clouds")
			(HighCloudCover, "high_clouds")
			(CloudCover10Percentile, "cloud_10")
			(CloudCover25Percentile, "cloud_25")
			(CloudCover50Percentile, "cloud_50")
			(CloudCover75Percentile, "cloud_75")
			(CloudCover90Percentile, "cloud_90")
			(SignificantWaveHeight, "sea_surface_wave_significant_height")
			(SignificantWaveHeight10Percentile, "significant_wave_height_10")
			(SignificantWaveHeight25Percentile, "significant_wave_height_25")
			(SignificantWaveHeight50Percentile, "significant_wave_height_50")
			(SignificantWaveHeight75Percentile, "significant_wave_height_75")
			(SignificantWaveHeight90Percentile, "significant_wave_height_90")
;


namespace
{
struct XmlSession
{
	XmlSession()
	{
		xmlInitParser();
	}
	~XmlSession()
	{
		xmlCleanupParser();
	}
};

}

ParameterTranslation::ParameterTranslation(const boost::filesystem::path & translationFile)
{
	if ( not exists(translationFile) )
		throw std::runtime_error(translationFile.string() + ": No such file");
	if ( is_directory(translationFile) )
		throw std::runtime_error(translationFile.string() + " is a directory");

	const std::string & fileName = translationFile.string();

	XmlSession session;
	boost::shared_ptr<xmlDoc> doc(xmlParseFile(fileName.c_str()), xmlFreeDoc);
	if ( ! doc )
		throw std::runtime_error("Unable to parse doc");

	boost::shared_ptr<xmlXPathContext> xpathCtx(xmlXPathNewContext(doc.get()), xmlXPathFreeContext);
	if ( ! xpathCtx )
		throw std::runtime_error("unable to create xpath context");

	addGlobalAttributes_(xpathCtx.get());

	addDirectTransferVariables_(xpathCtx.get());

	addExtraDimensions_(xpathCtx.get());

	addExtraVariables_(xpathCtx.get());

	for ( std::map<WeatherData, std::string>::const_iterator it = configFileIdentifiers_.begin(); it != configFileIdentifiers_.end(); ++ it )
		addParameters_(it->first, xpathCtx.get());
}

ParameterTranslation::~ParameterTranslation()
{
}

const FimexDataSpecification & ParameterTranslation::get(WeatherData weatherData) const
{
	SpecificationMap::const_iterator find = translations_.find(weatherData);
	if ( find == translations_.end() )
		throw data_access::Source::DataNotAvailable("Unknown parameter with ID " + boost::lexical_cast<std::string>(int(weatherData)));

	return find->second;
}

const std::set<float> & ParameterTranslation::dimensionValues(const std::string & cfname) const
{
	DimensionList::const_iterator ret = extraDimensions_.find(cfname);
	if ( ret == extraDimensions_.end() )
		throw std::runtime_error(cfname + " no such dimension");
	return ret->second;
}

const std::string & ParameterTranslation::getIdentifyingName_(WeatherData weatherData)
{
	std::map<WeatherData, std::string>::const_iterator find = configFileIdentifiers_.find(weatherData);
	if ( find == configFileIdentifiers_.end() )
		throw std::runtime_error("Unable to find metadata for parameter");
	return find->second;
}


namespace
{
template<typename T>
T getValue(const std::string & s)
{
	try
	{
		return boost::lexical_cast<T>(s);
	}
	catch ( boost::bad_lexical_cast & )
	{
		throw std::runtime_error("Unable to interpret value: " + s);
	}
}
}

void ParameterTranslation::addGlobalAttributes_(xmlXPathContextPtr context)
{
	boost::shared_ptr<xmlXPathObject> xpathObj(
			xmlXPathEvalExpression((const xmlChar *) "/eps2probandltf/global_attribute", context),
			xmlXPathFreeObject);

	if ( ! xpathObj )
        return;
	xmlNodeSetPtr nodes = xpathObj->nodesetval;

	for ( int i = 0; i < nodes->nodeNr; ++ i )
	{
		xmlNodePtr elementNode =  nodes->nodeTab[i];
		if ( elementNode->type != XML_ELEMENT_NODE )
			throw std::runtime_error("Expected element node");

		std::string name = getAttribute_(elementNode, "cfname");
		std::string value = getAttribute_(elementNode, "value");
		globalAttributes_.push_back(MetNoFimex::CDMAttribute(name, value));
	}
}

void ParameterTranslation::addDirectTransferVariables_(xmlXPathContextPtr context)
{
	boost::shared_ptr<xmlXPathObject> xpathObj(
			xmlXPathEvalExpression((const xmlChar *) "/eps2probandltf/direct_transfer", context),
			xmlXPathFreeObject);

	if ( ! xpathObj )
        return;
	xmlNodeSetPtr nodes = xpathObj->nodesetval;

	for ( int i = 0; i < nodes->nodeNr; ++ i )
	{
		xmlNodePtr elementNode =  nodes->nodeTab[i];
		if ( elementNode->type != XML_ELEMENT_NODE )
			throw std::runtime_error("Expected element node");

		std::string name = getAttribute_(elementNode, "cfname");

		directTransferVariables_.insert(name);
	}
}


void ParameterTranslation::addExtraDimensions_(xmlXPathContextPtr context)
{
	boost::shared_ptr<xmlXPathObject> xpathObj(
			xmlXPathEvalExpression((const xmlChar *) "/eps2probandltf/dimension", context),
			xmlXPathFreeObject);

	if ( ! xpathObj )
        return;
	xmlNodeSetPtr nodes = xpathObj->nodesetval;

	for ( int i = 0; i < nodes->nodeNr; ++ i )
	{
		xmlNodePtr elementNode =  nodes->nodeTab[i];
		if ( elementNode->type != XML_ELEMENT_NODE )
			throw std::runtime_error("Expected element node");

		std::string cfName = getAttribute_(elementNode, "cfname");
		std::vector<MetNoFimex::CDMAttribute> & attributes = dimensionVariables_[cfName];
		std::set<float> & values = extraDimensions_[cfName];
		for ( xmlNodePtr subNode = elementNode->children; subNode; subNode = subNode->next )
		{
			if ( xmlStrEqual(subNode->name, (xmlChar*) "attribute") )
			{
				std::string name = getAttribute_(subNode, "cfname");
				std::string value = getAttribute_(subNode, "value");
				attributes.push_back(MetNoFimex::CDMAttribute(name, value));
			}
			else if ( xmlStrEqual(subNode->name, (xmlChar*) "value") )
			{
				float value = getValue<float>(getAttribute_(subNode, "value"));
				values.insert(value);
			}
		}
	}
}

void ParameterTranslation::addExtraVariables_(xmlXPathContextPtr context)
{
	boost::shared_ptr<xmlXPathObject> xpathObj(
			xmlXPathEvalExpression((const xmlChar *) "/eps2probandltf/variable", context),
			xmlXPathFreeObject);

	if ( ! xpathObj )
        return;
	xmlNodeSetPtr nodes = xpathObj->nodesetval;

	for ( int i = 0; i < nodes->nodeNr; ++ i )
	{
		xmlNodePtr elementNode =  nodes->nodeTab[i];
		if ( elementNode->type != XML_ELEMENT_NODE )
			throw std::runtime_error("Expected element node");

		std::string cfName = getAttribute_(elementNode, "cfname");

		std::vector<std::string> shape;
		std::vector<float> values;
		std::vector<MetNoFimex::CDMAttribute> attributes;

		for ( xmlNodePtr subNode = elementNode->children; subNode; subNode = subNode->next )
		{
			if ( xmlStrEqual(subNode->name, (xmlChar*) "dimension") )
			{
				std::string name = getAttribute_(subNode, "cfname");
				shape.push_back(name);
			}
			else if ( xmlStrEqual(subNode->name, (xmlChar*) "attribute") )
			{
				std::string name = getAttribute_(subNode, "cfname");
				std::string value = getAttribute_(subNode, "value");
				attributes.push_back(MetNoFimex::CDMAttribute(name, value));
			}
			else if ( xmlStrEqual(subNode->name, (xmlChar*) "value") )
			{
				float value = getValue<float>(getAttribute_(subNode, "value"));
				values.push_back(value);
			}
		}

		MetNoFimex::CDMVariable var(cfName, MetNoFimex::CDM_FLOAT, shape);
		var.setData(MetNoFimex::createData(MetNoFimex::CDM_FLOAT, values.begin(), values.end()));

		extraVariables_[var] = attributes;
	}

}



void ParameterTranslation::addParameters_(WeatherData weatherData, xmlXPathContextPtr context)
{
	const std::string basePath = "/eps2probandltf/parameter/";

	const std::string & identifier = getIdentifyingName_(weatherData);
	std::string path = basePath + identifier;

	boost::shared_ptr<xmlXPathObject> xpathObj(
			xmlXPathEvalExpression((const xmlChar *) path.c_str(), context),
			xmlXPathFreeObject);
    if ( ! xpathObj )
        throw std::runtime_error("Unable to evaluate xpath expression :" + path);

    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if ( nodes->nodeNr == 0 )
    {
    	//throw std::runtime_error(path + ": no such element");
    	return;
    }
    if ( nodes->nodeNr > 1 )
    	throw std::runtime_error(path + ": Only one instance of element is allowed");

	xmlNodePtr elementNode =  * nodes->nodeTab;
	if ( elementNode->type != XML_ELEMENT_NODE )
		throw std::runtime_error("Expected element node");

	std::string cfName = getAttribute_(elementNode, "cfname");
	int time_offset = 0;
	try
	{
		time_offset = boost::lexical_cast<int>(getAttribute_(elementNode, "time_offset"));
	}
	catch ( boost::bad_lexical_cast & )
	{
		throw std::runtime_error("Invalid value for time_offset");
	}
	catch ( std::runtime_error & )
	{}

	std::vector<MetNoFimex::CDMAttribute> attributes;
	std::pair<std::string, float> extraDimension;
	for ( xmlNodePtr subNode = elementNode->children; subNode; subNode = subNode->next )
	{
		if ( xmlStrEqual(subNode->name, (xmlChar*) "attribute") )
		{
			std::string name = getAttribute_(subNode, "cfname");
			std::string unit = getAttribute_(subNode, "unit", "String");
			std::string value = getAttribute_(subNode, "value");
//#error nan values does not convert properly
			attributes.push_back(MetNoFimex::CDMAttribute(name, unit, value));
		}
		else if ( xmlStrEqual(subNode->name, (xmlChar*) "dimension") )
		{
			std::string cfname = getAttribute_(subNode, "cfname");
			float value = getValue<float>(getAttribute_(subNode, "value"));
			if ( not extraDimension.first.empty() )
				throw std::runtime_error("multiple extra dimensions for a variable is not allowed");
			extraDimension.first = cfname;
			extraDimension.second = value;
		}
	}

	FimexDataSpecification spec(cfName, attributes, type::Duration(time_offset, 0, 0, 0));
	if ( not extraDimension.first.empty() )
		spec.addExtraDimension(extraDimension.first, extraDimension.second);

	translations_.insert(std::make_pair(weatherData, spec));
}

std::string ParameterTranslation::getAttribute_(xmlNodePtr elementNode, const std::string & name) const
{
	std::string ret = getAttribute_(elementNode, name, std::string());

	if ( ret.empty() )
		throw std::runtime_error("Missing attribute " + name);

	return ret;
}

std::string ParameterTranslation::getAttribute_(xmlNodePtr elementNode, const std::string & name, const std::string & defaultValue) const
{
	std::string ret;
	for ( xmlAttrPtr attr = elementNode->properties; attr; attr = attr->next )
		if ( xmlStrEqual(attr->name,(xmlChar*) name.c_str()) )
			return (char*) attr->children->content;

	return defaultValue;
}

}
}
