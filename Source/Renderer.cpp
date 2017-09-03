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
#include "Utility.hpp"

container::Array<glm::vec3> Renderer::positionBuffer_ = container::Array<glm::vec3>();
container::Array<float> Renderer::scaleBuffer_ = container::Array<float>();
container::Array<float> Renderer::rotationBuffer_ = container::Array<float>();
container::Array<float> Renderer::highlightBuffer_ = container::Array<float>();
container::Array<int> Renderer::resolutionBuffer_ = container::Array<int>();
container::Array<float> Renderer::contrastBuffer_ = container::Array<float>();
container::Array<glm::vec2> Renderer::offsetBuffer_ = container::Array<glm::vec2>();
Buffer* Renderer::bodyBuffer_ = nullptr;
ShaderMap* Renderer::shaderMap_ = new ShaderMap();
glm::mat4 Renderer::matrix_ = glm::mat4();
float Renderer::zoomFactor_ = 0.03f;
Texture* Renderer::perlinTexture_ = nullptr;

void Renderer::Initialize(Scene* scene)
{
	positionBuffer_.initialize(512);
	scaleBuffer_.initialize(512);
	rotationBuffer_.initialize(512);
	highlightBuffer_.initialize(512);
	resolutionBuffer_.initialize(512);
	contrastBuffer_.initialize(512);
	offsetBuffer_.initialize(512);

	shaderMap_->initialize(Shaders::SPRITE, "Shaders/Sprite.vert", "Shaders/Sprite.frag", nullptr);
	shaderMap_->initialize(Shaders::BODY, "Shaders/Body.vert", "Shaders/Body.frag", "Shaders/Body.geom");
	shaderMap_->initialize(Shaders::POSTPROCESS, "Shaders/Postprocess.vert", "Shaders/Postprocess.frag", nullptr);
	shaderMap_->initialize(Shaders::PARTICLES_COMPUTE, "Shaders/ParticlesCompute.comp", nullptr, nullptr);
	shaderMap_->initialize(Shaders::PARTICLES_INSTANCED, "Shaders/ParticlesInstanced.vert", "Shaders/ParticlesInstanced.frag", nullptr);
	shaderMap_->initialize(Shaders::MESH, "Shaders/Mesh.vert", "Shaders/Mesh.frag", nullptr);
	shaderMap_->initialize(Shaders::EXPLOSION, "Shaders/Explosion.vert", "Shaders/Explosion.frag", nullptr);
	shaderMap_->initialize(Shaders::HEALTH_BAR, "Shaders/HealthBar.vert", "Shaders/HealthBar.frag", nullptr);

	GLuint key;
	glGenVertexArrays(1, &key);
	glBindVertexArray(key);
	bodyBuffer_ = new Buffer(key, 7);

	//Position buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, positionBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Scale buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, scaleBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	//Rotation buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, rotationBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	//Highlight buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, highlightBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	//Resolution buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, resolutionBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 1, GL_INT, 0, NULL);

	//Resolution buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, contrastBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, NULL);

	//Resolution buffer
	glGenBuffers(1, &key);
	bodyBuffer_->AddBuffer(key);
	glBindBuffer(GL_ARRAY_BUFFER, key);
	glBufferData(GL_ARRAY_BUFFER, offsetBuffer_.getMemoryUse(), 0, GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for(Asteroid* asteroid = scene->asteroids_.getStart(); asteroid != scene->asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
			*contrastBuffer_.allocate() = 0.5f;
	for(Planet* planet = scene->planets_.getStart(); planet != scene->planets_.getEnd(); ++planet)
		if(planet->isValid_)
			*contrastBuffer_.allocate() = 0.1f;

	for(Asteroid* asteroid = scene->asteroids_.getStart(); asteroid != scene->asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
			*offsetBuffer_.allocate() = glm::vec2(utility::getRandom(0.0f, 0.9f), utility::getRandom(0.0f, 0.9f));
	for(Planet* planet = scene->planets_.getStart(); planet != scene->planets_.getEnd(); ++planet)
		if(planet->isValid_)
			*offsetBuffer_.allocate() = glm::vec2(utility::getRandom(0.0f, 0.9f), utility::getRandom(0.0f, 0.9f));

	container::Grid<float> result(1024, 1024);
	Perlin::generate(true, result, Range(0.0f, 1.0f), 3.0f, 1.5f, 0.5f, 4.0f);
	perlinTexture_ = new Texture();
	perlinTexture_->Upload(&result, GL_R32F, GL_RED, GL_FLOAT);
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
	highlightBuffer_.reset();
	resolutionBuffer_.reset();

	int count = 0;
	for(Asteroid* asteroid = scene->asteroids_.getStart(); asteroid != scene->asteroids_.getEnd(); ++asteroid)
		if(asteroid->isValid_)
		{
			*positionBuffer_.allocate() = asteroid->transform_->position_;
			*scaleBuffer_.allocate() = asteroid->GetTransform()->scale_ * 2.0f;
			*rotationBuffer_.allocate() = asteroid->transform_->rotation_.z;
			*highlightBuffer_.allocate() = asteroid == scene->ownShip_->sensor_.GetObject() ? 1.0f : 0.0f;
			*resolutionBuffer_.allocate() = 16 + int((asteroid->GetTransform()->scale_ - 1.0f) * 16.0f);
			count++;
		}
	for(Planet* planet = scene->planets_.getStart(); planet != scene->planets_.getEnd(); ++planet)
		if(planet->isValid_)
		{
			*positionBuffer_.allocate() = planet->transform_->position_;
			*scaleBuffer_.allocate() = 25.0f;
			*rotationBuffer_.allocate() = planet->transform_->rotation_.z;
			*highlightBuffer_.allocate() = 0.0f;
			*resolutionBuffer_.allocate() = 64;
			count++;
		}

	//std::cout<<MAX_GEOMETRY_OUTPUT_VERTICES<<"\n";
	//std::cout<<bodyBuffer_->GetKey(0)<<" "<<bodyBuffer_->GetKey(1)<<"\n";
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(0));
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionBuffer_.getMemorySize(), positionBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(1));
	glBufferSubData(GL_ARRAY_BUFFER, 0, scaleBuffer_.getMemorySize(), scaleBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(2));
	glBufferSubData(GL_ARRAY_BUFFER, 0, rotationBuffer_.getMemorySize(), rotationBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(3));
	glBufferSubData(GL_ARRAY_BUFFER, 0, highlightBuffer_.getMemorySize(), highlightBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(4));
	glBufferSubData(GL_ARRAY_BUFFER, 0, resolutionBuffer_.getMemorySize(), resolutionBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(5));
	glBufferSubData(GL_ARRAY_BUFFER, 0, contrastBuffer_.getMemorySize(), contrastBuffer_.getStart());
	glBindBuffer(GL_ARRAY_BUFFER, bodyBuffer_->GetKey(6));
	glBufferSubData(GL_ARRAY_BUFFER, 0, offsetBuffer_.getMemorySize(), offsetBuffer_.getStart());
	shaderMap_->use(Shaders::BODY);
	glUniformMatrix4fv(0, 1, GL_FALSE, &matrix_[0][0]);
	perlinTexture_->Bind(0, &shaderMap_->get(Shaders::BODY), "alpha");
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

