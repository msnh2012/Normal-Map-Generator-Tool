#pragma once
class FrameBufferSystem
{
private:
	unsigned int framebuffer = 0;
	unsigned int textureColorbuffer = 0;
	unsigned int textureDepthBuffer = 0;
	static unsigned int currentlyBoundFBO;
public:
	FrameBufferSystem();
	void init(int windowWidth, int windowHeight);
	void BindFrameBuffer();
	void BindBufferTexture();
	unsigned int getBufferTexture();
	void updateTextureDimensions(int windowWidth, int windowHeight);
	static int GetCurrentlyBoundFBO();
	~FrameBufferSystem();
};

