#include "Buffer.hpp"

Buffer::Buffer(GLuint key, int count)
{
	key_ = key;
	if(count != 0)
		subBuffers_.initialize(count);
}

void Buffer::AddBuffer(GLuint key)
{
	Buffer* buffer = subBuffers_.allocate();
	buffer->SetKey(key);
}
