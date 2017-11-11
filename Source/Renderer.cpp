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
#include "ParticleManager.hpp"

container::Array <Position> Renderer::positionBuffer_ = container::Array <Position>();
container::Array <Scale> Renderer::scaleBuffer_ = container::Array <Scale>();
container::Array <Rotation> Renderer::rotationBuffer_ = container::Array <Rotation>();
container::Array <float> Renderer::highlightBuffer_ = container::Array <float>();
container::Array <int> Renderer::resolutionBuffer_ = container::Array <int>();
container::Array <float> Renderer::contrastBuffer_ = container::Array <float>();
container::Array <glm::vec2> Renderer::offsetBuffer_ = container::Array <glm::vec2>();
MasterBuffer* Renderer::bodyBuffer_ = nullptr;
ShaderMap* Renderer::shaderMap_ = new ShaderMap();
glm::mat4 Renderer::matrix_ = glm::mat4();
float Renderer::zoomFactor_ = 0.03f;
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
	shaderMap_->initialize(Shaders::MESH, "Shaders/Mesh.vert", "Shaders/Mesh.frag", nullptr);
	shaderMap_->initialize(Shaders::EXPLOSION, "Shaders/Explosion.vert", "Shaders/Explosion.frag", nullptr);
	shaderMap_->initialize(Shaders::HEALTH_BAR, "Shaders/HealthBar.vert", "Shaders/HealthBar.frag", nullptr);
	shaderMap_->initialize(Shaders::SHELL, "Shaders/Shell.vert", "Shaders/Shell.frag", nullptr);

	bodyBuffer_ = new MasterBuffer(8);

	SlaveBuffer* positionBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, positionBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(0, positionBuffer, "position", 2, true);

	SlaveBuffer* scaleBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, scaleBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(1, scaleBuffer, "scale", 1, true);

	SlaveBuffer* rotationBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, rotationBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(2, rotationBuffer, "rotation", 1, true);

	SlaveBuffer* highlightBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, highlightBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(3, highlightBuffer, "highlight", 1, true);

	SlaveBuffer* resolutionBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, resolutionBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(4, resolutionBuffer, "resolution", 1, false);

	SlaveBuffer* contrastBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, contrastBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(5, contrastBuffer, "contrast", 1, true);

	SlaveBuffer* offsetBuffer = new SlaveBuffer(GL_ARRAY_BUFFER, offsetBuffer_.getMemoryCapacity(), nullptr);
	bodyBuffer_->AddBuffer(6, offsetBuffer, "offset", 2, true);

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

	/*frameBuffers_->get(Framebuffers::DEFAULT).bindBuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClearDepth(1.0f);*/
}

void Renderer::DrawScene(Scene* scene)
{
	Ship* ownShip = scene->ownShip_;
	glm::vec3 screenCenter(ownShip->transform_->position_.x + -Engine::screen_->getWidthFloating() * 0.5f * zoomFactor_, ownShip->transform_->position_.y + -Engine::screen_->getHeightFloating() * 0.5f * zoomFactor_, 0.0f);
	glm::mat4 projectionMatrix = glm::ortho<float> (0.0f, Engine::screen_->getWidthFloating() * zoomFactor_, Engine::screen_->getHeightFloating() * zoomFactor_, 0.0f, 0.1f, 10.0f);
	glm::mat4 viewMatrix = glm::lookAt<float> (screenCenter + glm::vec3(0.0f, 0.0f, 1.0f), screenCenter, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = projectionMatrix * viewMatrix;

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LEQUAL);
	//glDepthRange(0.0f, 1.0f);
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
			*rotationBuffer_.allocate() = asteroid->transform_->rotation_;
			*highlightBuffer_.allocate() = asteroid == scene->ownShip_->sensor_.GetObject() ? 1.0f : 0.0f;
			*resolutionBuffer_.allocate() = 16 + int((asteroid->GetTransform()->scale_ - 1.0f) * 16.0f);
			count++;
		}
	for(Planet* planet = scene->planets_.getStart(); planet != scene->planets_.getEnd(); ++planet)
		if(planet->isValid_)
		{
			*positionBuffer_.allocate() = planet->transform_->position_;
			*scaleBuffer_.allocate() = planet->GetTransform()->scale_;
			*rotationBuffer_.allocate() = planet->transform_->rotation_;
			*highlightBuffer_.allocate() = 0.0f;
			*resolutionBuffer_.allocate() = 64;
			count++;
		}

	//bodyBuffer_->GetBuffer("position")->UploadData(positionBuffer_.getStart(), positionBuffer_.getMemorySize());
	bodyBuffer_->UploadData("position", positionBuffer_.getStart(), positionBuffer_.getMemorySize());
	bodyBuffer_->UploadData("scale", scaleBuffer_.getStart(), scaleBuffer_.getMemorySize());
	bodyBuffer_->UploadData("rotation", rotationBuffer_.getStart(), rotationBuffer_.getMemorySize());
	bodyBuffer_->UploadData("highlight", highlightBuffer_.getStart(), highlightBuffer_.getMemorySize());
	bodyBuffer_->UploadData("resolution", resolutionBuffer_.getStart(), resolutionBuffer_.getMemorySize());
	bodyBuffer_->UploadData("contrast", contrastBuffer_.getStart(), contrastBuffer_.getMemorySize());
	bodyBuffer_->UploadData("offset", offsetBuffer_.getStart(), offsetBuffer_.getMemorySize());

	shaderMap_->use(Shaders::BODY);
	glUniformMatrix4fv(0, 1, GL_FALSE, &matrix_[0][0]);
	perlinTexture_->Bind(0, &shaderMap_->get(Shaders::BODY), "alpha");
	bodyBuffer_->Bind();
	glDrawArrays(GL_POINTS, 0, count);
	bodyBuffer_->Unbind();
	shaderMap_->unuse(Shaders::BODY);

	shaderMap_->use(Shaders::MESH);
	for(Ship* ship = scene->ships_.getStart(); ship != scene->ships_.getEnd(); ++ship)
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
			glUniform1f(2, shell->GetTransform()->scale_ * 2.0f);
			float speed = glm::length(shell->GetRigidBody()->velocity_);
			speed *= speed * 15.0f;
			if(speed > 1.0f)
				speed = 1.0f;
			glUniform1f(4, speed);
			shell->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::SHELL);

	shaderMap_->use(Shaders::EXPLOSION);
	for(Explosion* explosion = scene->explosions_.getStart(); explosion != scene->explosions_.getEnd(); ++explosion)
		if(explosion->isValid_ && explosion->isWorking_) {
			glUniform1f(2, explosion->GetTransform()->scale_);
			glUniform1f(3, explosion->lifeTime_);
			//glUniform1f(4, 1.0f);
			explosion->Draw(matrix_);
		}
	shaderMap_->unuse(Shaders::EXPLOSION);

	ParticleManager::Render(matrix_);
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

