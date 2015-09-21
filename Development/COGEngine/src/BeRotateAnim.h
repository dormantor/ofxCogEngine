#pragma once

#include "GBehavior.h"
#include "GNode.h"

/**x
* Behavior for rotation animation
*/
class BeRotateAnim : public GBehavior{

private:
	// starting rotation
	float from;
	// ending rotation
	float to;
	// rotation speed in angles per second
	float speed;
	// if true, rotation will be additive (doesn't override actual rotation)
	bool additive;
	// actual rotation value
	float actual;

public:
	/**
	* Creates a new behavior for rotation animation
	* @param from starting rotation
	* @param to ending rotation (if from == to, it is infinite rotation)
	* @param speed rotation speed in radians per second
	* @param additive if true, rotation will be additive
	*/
	BeRotateAnim(float from, float to, float speed, bool additive) : 
		GBehavior(ElemType::MODEL), from(from), to(to), speed(speed), additive(additive), actual(0){
	}


	void Update(const uint64 delta, const uint64 absolute){

		// calculate differencial
		float diff = (float)((to == from ? 1 : (to - from)) * 0.001f * speed * delta*360);
		actual += diff;

		// if to == from, it is infinite rotation
		if (to != from && ((to >= from && actual >= to) || (to < from && actual < to))){
			actual = to;
			Finish();
		}

		EnTransform& transform = owner->GetTransform();

		if (additive) transform.rotation += (diff);
		else transform.rotation = (actual);
	}
};