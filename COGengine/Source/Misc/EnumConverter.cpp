#include "EnumConverter.h"
#include "Shape.h"
#include "AttrAnimEnt.h"
#include "TransformEnt.h"

namespace Cog {

	ShapeType EnumConverter::StrToShapeType(string val) {
		if (val.compare("image") == 0) {
			return ShapeType::IMAGE;
		}
		else if (val.compare("rectangle") == 0) {
			return ShapeType::RECTANGLE;
		}
		else if (val.compare("text") == 0) {
			return ShapeType::TEXT;
		}
		else if (val.compare("plane") == 0) {
			return ShapeType::PLANE;
		}
		else if (val.compare("sprite") == 0) {
			return ShapeType::SPRITE;
		}
		else if (val.compare("multisprite") == 0) {
			return ShapeType::MULTISPRITE;
		}
		else if (val.compare("label") == 0) {
			return ShapeType::LABEL;
		}
		else if (val.compare("bounding_box") == 0) {
			return ShapeType::BOUNDING_BOX;
		}

		return ShapeType::NONE;
	}

	/**
	* Transforms string into TransformType enum
	*/
	TransformType EnumConverter::StrToMeasureType(string val) {
		if (val.compare("direct") == 0) return TransformType::DIRECT;
		else if (val.compare("diff") == 0) return TransformType::DIFF;

		return TransformType::DIRECT;
	}

	/**
	* Transforms string into AttributeType enum
	*/
	AttributeType EnumConverter::StrToAttributeType(string val) {
		if (val.compare("pos_x") == 0) return AttributeType::POS_X;
		else if (val.compare("pos_y") == 0) return AttributeType::POS_Y;
		else if (val.compare("rotation") == 0) return AttributeType::ROTATION;
		else if (val.compare("rot_center_x") == 0) return AttributeType::ROT_CENTER_X;
		else if (val.compare("rot_center_y") == 0) return AttributeType::ROT_CENTER_Y;
		else if (val.compare("size_x") == 0) return AttributeType::SIZE_X;
		else if (val.compare("size_y") == 0) return AttributeType::SIZE_Y;
		else if (val.compare("size") == 0) return AttributeType::SIZE;
		else if (val.compare("size_center_x") == 0) return AttributeType::SIZE_CENTER_X;
		else if (val.compare("size_center_y") == 0)return AttributeType::SIZE_CENTER_Y;

		return AttributeType::COMMON;
	}

	CalcType EnumConverter::GetUnitValue(string val, float& floatVal) {
		stringstream ss;
		unsigned i;
		for (i = 0; i < val.length(); i++) {
			if (isdigit(val[i]) || val[i] == '.' || val[i] == '-') ss << val[i];
			else break;
		}

		if (i != 0) floatVal = ofToFloat(ss.str());
		return StrToCalcType(val.substr(i));
	}


	CalcType EnumConverter::StrToCalcType(string val) {
		if (val.compare("r") == 0) return CalcType::PER;
		else if (val.compare("gr") == 0) return CalcType::GRID;
		else if (val.compare("rp") == 0) return CalcType::ABS_PER;
		else if (val.compare("un") == 0) return CalcType::ABS;
		else if (val.empty()) return CalcType::LOC;

		CogLogError("CalcTypeConverter", "Error! %s is not valid unit, expected [r, gr, rp, un] or empty", val.c_str());

		return CalcType::LOC;
	}

} // namespace