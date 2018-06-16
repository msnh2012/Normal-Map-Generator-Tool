#pragma once
#include <GLM\common.hpp>
struct ColourData
{
private:
	glm::vec4 colour;
public:
	ColourData()
	{
		colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	ColourData(float r, float g, float b, float a)
	{
		colour = glm::vec4(r, g, b, a);
	}
	void setColour_8_Bit(const glm::vec4& colour)
	{
		int r = glm::clamp((int)colour.r, 0, 255);
		int g = glm::clamp((int)colour.g, 0, 255);
		int b = glm::clamp((int)colour.b, 0, 255);
		int a = glm::clamp((int)colour.a, 0, 255);
		this->colour = glm::vec4(r, g, b, a);
	}
	void setColour_8_Bit(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		float _r = r / 255.0f;
		float _g = g / 255.0f;
		float _b = b / 255.0f;
		float _a = a / 255.0f;
		this->colour = glm::vec4(_r, _g, _b, _a);
	}
	void setColour_32_bit(const glm::vec4& colour)
	{
		this->colour = colour;
	}
	void setColour_32_bit(float r, float g, float b, float a)
	{
		this->colour = glm::vec4(r, g, b, a);
	}
	glm::vec4 getColourIn_0_1_Range()
	{
		return colour;
	}
	glm::vec4 getColour_8_Bit()
	{
		unsigned char rV = (unsigned char)(colour.r * 255.0f);
		unsigned char gV = (unsigned char)(colour.g * 255.0f);
		unsigned char bV = (unsigned char)(colour.b * 255.0f);
		unsigned char aV = (unsigned char)(colour.a * 255.0f);
		return glm::vec4(rV, gV, bV, aV);
	}
};