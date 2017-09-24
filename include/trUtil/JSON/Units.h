/*
* True Reality Open Source Game and Simulation Engine
* Copyright � 2017 Acid Rain Studios LLC
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Maxim Serebrennik
*/

#pragma once

#include <trUtil/Export.h>

#include <json/value.h>
//#include <json/writer.h>  //for the << operator

#include <ostream>

namespace trUtil
{
	namespace JSON
	{
		class TR_UTIL_EXPORT Units
		{
		public:

			typedef Json::ValueType ValueType;
			typedef Json::Value Value;
			typedef Json::Value::Members Members;
			typedef Json::UInt UInt;
			typedef Json::Int Int;
			typedef Json::UInt64 UInt64;
			typedef Json::Int64 Int64;
			typedef Json::LargestInt LargestInt;
			typedef Json::LargestUInt LargestUInt;
			typedef Json::ArrayIndex ArrayIndex;
		};

		typedef TR_UTIL_EXPORT Units::ValueType ValueType;
		typedef TR_UTIL_EXPORT Units::Value Value;
		typedef TR_UTIL_EXPORT Units::Value::Members Members;
		typedef TR_UTIL_EXPORT Units::Value::UInt UInt;
		typedef TR_UTIL_EXPORT Units::Value::Int Int;
		typedef TR_UTIL_EXPORT Units::Value::UInt64 UInt64;
		typedef TR_UTIL_EXPORT Units::Value::Int64 Int64;
		typedef TR_UTIL_EXPORT Units::Value::LargestInt LargestInt;
		typedef TR_UTIL_EXPORT Units::Value::LargestUInt LargestUInt;
		typedef TR_UTIL_EXPORT Units::Value::ArrayIndex ArrayIndex;		

		TR_UTIL_EXPORT std::ostream& operator<<(std::ostream&, const Value& root);
	}
}