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
#include "Framebuffer.hpp"
#include "Interface.hpp"
#include "Structure.hpp"
#include "Explosion.hpp"
#include "HealthBar.hpp"
#include "RigidBody.hpp"

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
float Renderer::zoomFactor_ = 0.05f;
Texture* Renderer::perlinTexture_ = nullptr;
FramebufferAtlas* Renderer::frameBuffers_ = new FramebufferAtlas();

void Renderer::Initialize(Scene* scene)
{
	frameBuffers_->initialize(
		Framebuffers::INITIAL,
		FramebufferTypes::MULTISAMPLE,
		FramebufferTypes::TEXTURE,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		false
		);
	frameBuffers_->initialize(
		Framebuffers::POSTPROCESS,
		FramebufferTypes::SINGLESAMPLE,
		FramebufferTypes::RENDERBUFFER,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		false
		);
	frameBuffers_->initialize(
		Framebuffers::DEFAULT,
		FramebufferTypes::SINGLESAMPLE,
		FramebufferTypes::RENDERBUFFER,
		Engine::screen_->getWidthInteger(),
		Engine::screen_->getHeightInteger(),
		true
		);

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
	shaderMap_->initialize(Shaders::SHELL, "Shaders/Shell.vert", "Shaders/Shell.frag", nullptr);

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

void Renderer::DrawScene(Scene* scene)
{
	Spaceship* ownShip = scene->ownShip_;
	glm::vec3 screenCenter(ownShip->transform_->position_.x + -Engine::screen_->getWidthFloating() * 0.5f * zoomFactor_, ownShip->transform_->position_.y + -Engine::screen_->getHeightFloating() * 0.5f * zoomFactor_, 0.0f);
	glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, Engine::screen_->getWidthFloating() * zoomFactor_, Engine::screen_->getHeightFloating() * zoomFactor_, 0.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 1.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = projectionMatrix * viewMatrix;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_SAMPLE_SHADING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	frameBuffers_->get(Framebuffers::DEFAULT).bindBuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClearDepth(1.0f);

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

	shaderMap_->use(Shaders::MESH);
	for(Spaceship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
		if(ship->isValid_) {
			glUniform1f(2, ship->GetTransform()->scale_);
			ship->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::MESH);

	shaderMap_->use(Shaders::MESH);
	for(Structure* structure = scene->structures_.getStart(); structure != scene->structures_.getEnd(); ++structure)
		if(structure->isValid_) {
			glUniform1f(2, structure->GetTransform()->scale_);
			structure->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::MESH);

	shaderMap_->use(Shaders::SHELL);
	for(Shell* shell = scene->shells_.getStart(); shell != scene->shells_.getEnd(); ++shell)
		if(shell->isValid_) {
			glUniform1f(2, shell->GetTransform()->scale_ * 1.5f);
			float speed = glm::length(shell->GetRigidBody()->velocity_);
			speed *= speed * 4.0f;
			if(speed > 1.0f)
				speed = 1.0f;
			glUniform1f(4, speed);
			shell->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::SHELL);

	shaderMap_->use(Shaders::EXPLOSION);
	for(Explosion* explosion = scene->explosions_.getStart(); explosion != scene->explosions_.getEnd(); ++explosion)
		if(explosion->isValid_ && explosion->isWorking_) {
			glUniform1f(2, 5.0f);
			glUniform1f(3, explosion->lifeTime_);
			//glUniform1f(4, 1.0f);
			explosion->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::EXPLOSION);
}

void Renderer::DrawInterface(Interface* interface)
{
	shaderMap_->use(Shaders::HEALTH_BAR);
	for(HealthBar* bar = interface->healthBars_.getStart(); bar != interface->healthBars_.getEnd(); ++bar)
		if(bar->IsValid() && bar->IsWorking()) {
			glUniform1f(2, 5.0f);
			glUniform1f(3, bar->GetShip()->hull_.GetDamage());
			glUniform1f(4, bar->GetShip()->weapon_->GetHeatFactor());
			bar->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::HEALTH_BAR);
}

