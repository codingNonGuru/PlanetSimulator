/*
 * Transform.hpp
 *
 *  Created on: Sep 25, 2016
 *      Author: andrei
 */

#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm.hpp>

class Transform {
public:
	glm::vec3 position_;
	glm::vec3 rotation_;
	float scale_;

	Transform(glm::vec3, glm::vec3);
	virtual ~Transform();
};

#endif /* TRANSFORM_HPP_ */
