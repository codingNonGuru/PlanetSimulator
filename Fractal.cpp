#include <stdlib.h>
#include <glm.hpp>
#include "GL/glew.h"

#include "Shader.hpp"
#include "Fractal.hpp"
#include "Utility.hpp"

Shader* Perlin::shader_ = nullptr;
Shader* Voronoi::shader_ = nullptr;

GLuint Perlin::octavesBuffer_ = 0;
GLuint Perlin::resultBuffer_ = 0;
GLuint Perlin::streamBuffer_ = 0;
GLuint Voronoi::seedBuffer_ = 0;
GLuint Voronoi::neighbourBuffer_ = 0;
GLuint Voronoi::neighbourSwapBuffer_ = 0;
GLuint Voronoi::resultBuffer_ = 0;
GLuint Voronoi::resultSwapBuffer_ = 0;
GLuint Voronoi::filterBuffer_ = 0;

void Voronoi::initialize() {
	container::Grid<float> gray(4096, 4096);
	for(auto pixel = gray.getStart(); pixel != gray.getEnd(); ++pixel)
		*pixel = 0.0f;

	glGenBuffers(1, &seedBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, seedBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, gray.getMemoryUse(), 0, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &neighbourBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, neighbourBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, gray.getMemoryUse() * 2, 0, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &neighbourSwapBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, neighbourSwapBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, gray.getMemoryUse() * 2, 0, GL_DYNAMIC_DRAW);
	glGenBuffers(1, &resultBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, gray.getMemoryUse(), gray.getStart(), GL_DYNAMIC_DRAW);
	glGenBuffers(1, &resultSwapBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultSwapBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, gray.getMemoryUse(), gray.getStart(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	shader_ = new Shader();
	shader_->initialize("Shaders/Voronoi.comp");
}

void Voronoi::destroy() {
	glDeleteBuffers(1, &seedBuffer_);
	glDeleteBuffers(1, &neighbourBuffer_);
	glDeleteBuffers(1, &neighbourSwapBuffer_);
	glDeleteBuffers(1, &resultBuffer_);
	glDeleteBuffers(1, &resultSwapBuffer_);
}

void Voronoi::loadSeeds(container::Grid<unsigned int> &seeds) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, seedBuffer_);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, seeds.getMemoryUse(), seeds.getStart());
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Voronoi::loadRandomTriangulation(int32_t width, int32_t height, uint32_t &seedCount) {
	container::Grid<unsigned int> seeds(width, height);
	for(auto pixel = seeds.getStart(); pixel != seeds.getEnd(); ++pixel)
		*pixel = 0;
	unsigned int seedIndex = 1;
	bool flip = true;
	float horIncr = 64.0f;
	float vertIncr = horIncr * 0.84f;
	float vertDif = float(seeds.getHeight()) / vertIncr;
	vertDif = float(seeds.getHeight()) - float(int(vertDif)) * vertIncr;
	vertDif *= 0.5f;
	for(float y = 10.0f; y < seeds.getHeight(); y += vertIncr) {
		for(float x = (flip == true ? 1.0f : 1.0f + horIncr * 0.5f); x < seeds.getWidth(); x += horIncr) {
			float angle = utility::getRandom(0.0f, 6.2831f);
			float radius = utility::getRandom(12.0f, 20.0f);
			int offX = float(x) + radius * cos(angle);
			int offY = float(y) + radius * sin(angle);
			*seeds(offX, offY) = seedIndex;
			seedIndex++;
		}
		flip = flip == true ? false : true;
	}
	seedCount = seedIndex;
	loadSeeds(seeds);
}

void Voronoi::generate(ShaderMap& shaders, bool isDownload, container::Grid<float> &result, container::Grid<unsigned int> *finalSeeds, unsigned int relaxPassCount) {
	clock_t start = clock();
	int blockFactor = 16;
	clock_t floodTime, relaxTime, allocateTime, downloadTime;
	allocateTime = clock();
	allocateTime = clock() - allocateTime;

	shader_->use();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, seedBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, seedBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, neighbourBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, neighbourBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, neighbourSwapBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, neighbourSwapBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resultBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultSwapBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, resultSwapBuffer_);
	//Initialize
	glUniform2ui(1, result.getWidth(), result.getHeight());
	glUniform2i(2, result.getWidth(), result.getHeight());
	glUniform1ui(0, 0);
	glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	floodTime = clock();
	int floodPassCount = 25;
	for(int pass = 0; pass < floodPassCount; ++pass) {
		int step;
		if(pass < 18)
			step = pass / 2;
		else
			step = 0;
		glUniform1i(3, pow(2, step));
		//Flood
		glUniform1ui(0, 1);
		glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		//Swap
		glUniform1ui(0, 2);
		glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
	glFinish();
	floodTime = clock() - floodTime;
	//Separate
	glUniform1ui(0, 3);
	glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	utility::DistanceFilter filter(30);
	filter.initialize(utility::FilterTypes::GAUSS, 100.0f, 1.0f, true);
	float weights[61];
	for(int x = 0; x < filter.values_.getWidth(); ++x) {
		weights[x] = 0.0f;
		for(int y = 0; y < filter.values_.getHeight(); ++y)
			weights[x] += *filter.values_.get(x, y);
	}
	glGenBuffers(1, &filterBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, filterBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(weights), weights, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, filterBuffer_);
	glUniform1ui(4, filter.side_ * 2 + 1);
	glUniform1ui(5, filter.side_);

	relaxTime = clock();
	for(unsigned int mode = 0; mode < 2; ++mode) {
		glUniform1ui(6, mode);
		//Relax
		glUniform1ui(0, 4);
		glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		//Swap
		glUniform1ui(0, 5);
		glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
	glUniform1ui(0, 6);
	glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	shader_->unuse();
	glFinish();
	relaxTime = clock() - relaxTime;

	if(isDownload) {
		downloadTime = clock();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, result.getMemoryUse(), result.getStart());
		if(finalSeeds != nullptr) {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, seedBuffer_);
			glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, finalSeeds->getMemoryUse(), finalSeeds->getStart());
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		glFinish();
		downloadTime = clock() - downloadTime;
	}
	std::cout<<"Finished generating Voronoi texture on the GPU of size "<<result.getWidth()<<"x"<<result.getHeight()<<" in time "<<clock() - start<<" nanosecs.\n";
	std::cout<<"   Allocation phase took "<<allocateTime<<" nanosecs.\n";
	std::cout<<"   Flood phase took "<<floodTime<<" nanosecs."<<" One pass lasts "<<floodTime / floodPassCount<<" nanosecs.\n";
	std::cout<<"   Relax phase took "<<relaxTime<<" nanosecs."<<" One pass lasts "<<relaxTime / relaxPassCount<<" nanosecs.\n";
	if(isDownload)
		std::cout<<"   Download phase took "<<downloadTime<<" nanosecs.\n";
}

void Perlin::initialize() {
	clock_t start = clock();
	container::Grid<float> octave(2048, 2048);
	container::Grid<float> result(2048, 2048);
	container::Array<float, int> stream(pow(2, 24));
	for(auto pixel = octave.getStart(); pixel != octave.getEnd(); ++pixel)
		*pixel = 0.0f;
	for(auto pixel = result.getStart(); pixel != result.getEnd(); ++pixel)
		*pixel = 0.0f;
	for(int index = 0; index < stream.getCapacity(); ++index) {
		auto pixel = stream.allocate();
		*pixel = utility::getRandom(0.0f, 1.0f);
	}
	std::cout<<"octaves memory "<<octave.getMemoryUse()<<"\n";
	std::cout<<"result memory "<<result.getMemoryUse()<<"\n";
	glGenBuffers(1, &octavesBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, octavesBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, octave.getMemoryUse(), octave.getStart(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glGenBuffers(1, &resultBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, result.getMemoryUse(), result.getStart(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glGenBuffers(1, &streamBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, streamBuffer_);
	glBufferData(GL_SHADER_STORAGE_BUFFER, stream.getMemoryUse(), stream.getStart(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	shader_ = new Shader();
	shader_->initialize("Shaders/Perlin.comp");

	std::cout<<clock() - start<<"\n";
}

void Perlin::destroy() {
	glDeleteBuffers(1, &octavesBuffer_);
	glDeleteBuffers(1, &resultBuffer_);
	glDeleteBuffers(1, &streamBuffer_);
}

void Perlin::generate(ShaderMap& shaders, bool isDownload, container::Grid<float> &result, Range range, float strongestOctave, float decay, float contrast, float contrastStrength) {
	clock_t start = clock();
	int blockFactor = 16;
	//Bind shader and buffers
	shader_->use();
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, octavesBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, octavesBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, resultBuffer_);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, streamBuffer_);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, streamBuffer_);
	//Set all pixels to zero
	glUniform1ui(0, 4);
	//glDispatchCompute(2048, 2048, 1);
	glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	//Generate layers and add them to result
	glUniform3i(1, result.getWidth(), result.getHeight(), 1);
	unsigned int order = 512;
	unsigned int octaveCount = 9;
	glUniform1ui(3, octaveCount);
	for(unsigned int tier = octaveCount; tier >= 1; --tier, order /= 2) {
		glUniform1ui(2, order);
		glUniform1ui(5, tier);
		glUniform1ui(4, utility::getRandom(0, pow(2, 24)));
		glUniform1f(7, strongestOctave);
		glUniform1f(8, decay);
		for(unsigned int stage = 0; stage < 3; ++stage) {
			glUniform1ui(0, stage);
			glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		}
	}
	//Finish result
	glUniform1ui(0, 3);
	glUniform1ui(3, octaveCount);
	glUniform2f(6, range.x, range.y);
	glUniform1f(9, contrast);
	glUniform1f(10, contrastStrength);
	glDispatchCompute(result.getWidth() / blockFactor, result.getHeight() / blockFactor, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	shader_->unuse();

	if(isDownload) {
		//Download texture from graphical memory into central memory
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, resultBuffer_);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, result.getMemoryUse(), result.getStart());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	std::cout<<"Finished generating Perlin texture on the GPU of size "<<result.getWidth()<<"x"<<result.getHeight()<<" in time "<<clock() - start<<" nanosecs.\n";
}

