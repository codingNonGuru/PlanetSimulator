#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>

#include "Engine.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Buffer.hpp"
#include "Planet.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Fractal.hpp"
#include "Texture.hpp"

container::Array<glm::vec3> Renderer::positionBuffer_ = container::Array<glm::vec3>();
container::Array<float> Renderer::scaleBuffer_ = container::Array<float>();
container::Array<float> Renderer::rotationBuffer_ = container::Array<float>();
Buffer* Renderer::bodyBuffer_ = nullptr;
ShaderMap* Renderer::shaderMap_ = new ShaderMap();
glm::mat4 Renderer::matrix_ = glm::mat4();
float Renderer::zoomFactor_ = 0.03f;

Texture texture;

void Renderer::Initialize()
{
	positionBuffer_.initialize(128);
	scaleBuffer_.initialize(128);
	rotationBuffer_.initialize(128);

	shaderMap_->initialize(Shaders::SPRITE, "Shaders/Sprite.vert", "Shaders/Sprite.frag", nullptr);
	shaderMap_->initialize(Shaders::BODY, "Shaders/Body.vert", "Shaders/Body.frag", "Shaders/Body.geom");
	shaderMap_->initialize(Shaders::POSTPROCESS, "Shaders/Postprocess.vert", "Shaders/Postprocess.frag", nullptr);
	shaderMap_->initialize(Shaders::PARTICLES_COMPUTE, "Shaders/ParticlesCompute.comp", nullptr, nullptr);
	shaderMap_->initialize(Shaders::PARTICLES_INSTANCED, "Shaders/ParticlesInstanced.vert", "Shaders/ParticlesInstanced.frag", nullptr);
	shaderMap_->initialize(Shaders::SPACESHIP, "Shaders/Spaceship.vert", "Shaders/Spaceship.frag", nullptr);

	GLuint key;
	glGenVertexArrays(1, &key);
	glBindVertexArray(key);
	bodyBuffer_ = new Buffer(key, 3);

	//Position buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 128, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Scale buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 128, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	//Rotation buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 128, 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	container::Grid<float> result(1024, 1024);
	Perlin::generate(true, result, Range(0.7f, 1.0f), 2.0f, 3.0f, 0.5f, 1.0f);
	texture.Upload(&result, GL_R32F, GL_RED, GL_FLOAT);
}

void Renderer::Draw(Scene* scene)
{
	Spaceship* ownShip = scene->ownShip_;
	glm::vec3 screenCenter(ownShip->transform_->position_.x + -Engine::screen_->getWidthFloating() * 0.5f * zoomFactor_, ownShip->transform_->position_.y + -Engine::screen_->getHeightFloating() * 0.5f * zoomFactor_, 0.0f);
	glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, Engine::screen_->getWidthFloating() * zoomFactor_, Engine::screen_->getHeightFloating() * zoomFactor_, 0.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 1.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = projectionMatrix * viewMatrix;

	positionBuffer_.reset();
	scaleBuffer_.reset();
	rotationBuffer_.reset();

	int count = 0;
	for(auto asteroid = scene->asteroids_.getStart(); asteroid != scene->asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
		{
			*positionBuffer_.allocate() = asteroid->transform_->position_;
			*scaleBuffer_.allocate() = 3.0f;
			count++;
		}
	for(Planet* planet = scene->planets_.getStart(); planet != scene->planets_.getEnd(); ++planet)
		if(planet->isValid_)
		{
			*positionBuffer_.allocate() = planet->transform_->position_;
			*scaleBuffer_.allocate() = 25.0f;
			count++;
		}

	//std::cout<<MAX_GEOMETRY_OUTPUT_VERTICES<<"\n";
	//std::cout<<bodyBuffer_->GetKey(0)<<" "<<bodyBuffer_->GetKey(1)<<"\n";
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(0));
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionBuffer_.getMemorySize(), positionBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(1));
	glBufferSubData(GL_ARRAY_BUFFER, 0, scaleBuffer_.getMemorySize(), scaleBuffer_.getStart());
	shaderMap_->use(Shaders::BODY);
	glUniformMatrix4fv(0, 1, GL_FALSE, &matrix_[0][0]);
	texture.Bind(0, &shaderMap_->get(Shaders::BODY), "alpha");
	glBindVertexArray(bodyBuffer_->GetKey());
	glDrawArrays(GL_POINTS, 0, count);
	glBindVertexArray(0);
	shaderMap_->unuse(Shaders::BODY);

	/*shaderMap_->use(Shaders::SPRITE);
	bindTexture(Shaders::SPRITE, "alpha", 0, Engine::sprites_[2].textureKey_);
	glUniform2f(2, Engine::sprites_[2].scale_.x, Engine::sprites_[2].scale_.y);
	for(auto asteroid = scene->asteroids_.getStart(); asteroid != scene->asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_ == true) {
			glUniform1i(3, asteroid == ships->sensor_.GetObject() ? 1 : 0);
			asteroid->draw(finalMatrix);
		}
	shaderMap_->unuse(Shaders::SPRITE);*/
}

