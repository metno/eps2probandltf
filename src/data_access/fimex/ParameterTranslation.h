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

#ifndef PARAMETERTRANSLATION_H_
#define PARAMETERTRANSLATION_H_

#include "FimexDataSpecification.h"
#include <WeatherData.h>
#include <fimex/CDMVariable.h>
#include <boost/filesystem/path.hpp>
#include <map>
#include <set>

extern "C"
{
typedef struct _xmlXPathContext xmlXPathContext;
typedef xmlXPathContext *xmlXPathContextPtr;
typedef struct _xmlNode xmlNode;
typedef xmlNode* xmlNodePtr;
}

namespace data_access
{
namespace fimex
{
struct cdm_variable_cmp
{
	bool operator () (const MetNoFimex::CDMVariable & a, const MetNoFimex::CDMVariable & b) const
	{
		return a.getName() < b.getName();
	}
};

class ParameterTranslation
{
public:
	explicit ParameterTranslation(const boost::filesystem::path & translationFile);
	~ParameterTranslation();

	const FimexDataSpecification & get(WeatherData weatherData) const;

	const std::vector<MetNoFimex::CDMAttribute> & globalAttributes() const { return globalAttributes_; }

	const std::set<std::string> & directTransferVariables() const { return directTransferVariables_; }

	typedef std::map<std::string, std::vector<MetNoFimex::CDMAttribute> > DimensionVariableList;
	const DimensionVariableList & dimensionVariables() const { return dimensionVariables_; }

	typedef std::map<MetNoFimex::CDMVariable, std::vector<MetNoFimex::CDMAttribute>, cdm_variable_cmp> VariableList;
	const VariableList & extraVariables() const { return extraVariables_; }

	typedef std::map<std::string, std::set<float> > DimensionList;
	const DimensionList & dimensionValues() const { return extraDimensions_; }
	const std::set<float> & dimensionValues(const std::string & cfname) const;

private:

	static const std::string & getIdentifyingName_(WeatherData weatherData);

	void addGlobalAttributes_(xmlXPathContextPtr context);
	void addDirectTransferVariables_(xmlXPathContextPtr context);
	void addExtraDimensions_(xmlXPathContextPtr context);
	void addExtraVariables_(xmlXPathContextPtr context);
	void addParameters_(WeatherData weatherData, xmlXPathContextPtr context);
	std::string getAttribute_(xmlNodePtr elementNode, const std::string & name) const;
	std::string getAttribute_(xmlNodePtr elementNode, const std::string & name, const std::string & defaultValue) const;

	std::vector<MetNoFimex::CDMAttribute> globalAttributes_;
	std::set<std::string> directTransferVariables_;

	typedef std::map<WeatherData, FimexDataSpecification> SpecificationMap;
	SpecificationMap translations_;

	DimensionList extraDimensions_;
	DimensionVariableList dimensionVariables_;

	VariableList extraVariables_;

	static const std::map<WeatherData, std::string> configFileIdentifiers_;
};

}
}
#endif /* PARAMETERTRANSLATION_H_ */
