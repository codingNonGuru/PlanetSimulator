/*
 * Transform.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm.hpp>

typedef glm::vec3 Position;
typedef glm::vec3 Rotation;
typedef glm::vec3 Direction;
typedef float Scale;
typedef glm::mat4 Matrix;

class Transform {
public:
	Position position_;
	Rotation rotation_;
	Scale scale_;

	Direction GetForward();
	Transform() {}
	Transform(Position, Rotation, Scale);
	Matrix GetMatrix();
	Matrix GetPositionMatrix();
	virtual ~Transform();
};

#endif /* TRANSFORM_HPP_ */
