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

#ifndef FIMEXDATASPECIFICATION_H_
#define FIMEXDATASPECIFICATION_H_

#include <dataTypes.h>
#include <fimex/CDMAttribute.h>
#include <string>

namespace data_access
{
namespace fimex
{

class FimexDataSpecification
{
public:
	FimexDataSpecification(const std::string & cfName, const std::vector<MetNoFimex::CDMAttribute> & attributes, const type::Duration & offsetFromWorkingTime = type::Duration(0,0,0,0));
	~FimexDataSpecification();

	void addExtraDimension(const std::string & cfname, float value);

	const std::string & cfName() const { return cfName_; }
	type::Time getParametersValidTime(const type::Time & workingTime) const;
	const std::vector<MetNoFimex::CDMAttribute> & attributes() const { return attributes_; }

	bool hasExtraDimension() const { return not extraDimensionName_.empty(); }
	const std::string & extraDimensionName() const { return extraDimensionName_; }
	float extraDimensionValue() const { return extraDimensionValue_; }

private:
	std::string cfName_;
	type::Duration offsetFromWorkingTime_;
	std::vector<MetNoFimex::CDMAttribute> attributes_;
	std::string extraDimensionName_;
	float extraDimensionValue_;
};

}
}

#endif /* FIMEXDATASPECIFICATION_H_ */
