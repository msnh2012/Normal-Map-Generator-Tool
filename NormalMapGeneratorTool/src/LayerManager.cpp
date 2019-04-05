#include <set>
#include <iostream>
#include "GL\glew.h"
#include "LayerManager.h"
#include "FileExplorer.h"
#include "TextureLoader.h"

int LayerManager::getLayerCount()
{
	return layers.size();
}

void LayerManager::init(const glm::vec2 & windowRes, const glm::vec2& maxBufferResolution)
{
	this->windowRes = windowRes;
	this->maxBufferResolution = maxBufferResolution;
}

void LayerManager::updateLayerTexture(int index, unsigned int textureId)
{
	layers.at(index).inputTextureId = textureId;
}

void LayerManager::addLayer(int texId, LayerType layerType, const std::string& layerName)
{
	LayerInfo layerInfo;
	layerInfo.layerType = layerType;
	layerInfo.isActive = true;
	layerInfo.strength = 2.0f;
	layerInfo.inputTextureId = texId;
	layerInfo.layerName = new char[200];
	if (layerName == "")
	{
		std::string layerText = std::string("Layer ") + std::to_string(layers.size());
		std::memcpy(&layerInfo.layerName[0], &layerText[0], layerText.size());
		layerInfo.layerName[layerText.size()] = '\0';
	}
	else
	{
		std::memcpy(layerInfo.layerName, &layerName[0], layerName.size());
		layerInfo.layerName[layerName.size()] = '\0';
	}
	layerInfo.fbs.init(windowRes, maxBufferResolution);
	layers.push_back(layerInfo);
}
void LayerManager::setLayerActiveState(int index, bool isActive)
{
	layers.at(index).isActive = isActive;
}

NormalBlendMethod LayerManager::getNormalBlendMethod(int index)
{
	return layers.at(index).normalBlendMethod;
}
bool LayerManager::isLayerActive(int index)
{
	return layers.at(index).isActive;
}
void LayerManager::updateFramebufferTextureDimensions(const glm::vec2 resolution)
{
	for (int layerIndex = 0; layerIndex < layers.size(); layerIndex++)
		layers.at(layerIndex).fbs.updateTextureDimensions(resolution);
}
float LayerManager::getLayerStrength(int index)
{
	return layers.at(index).strength;
}
LayerType LayerManager::getLayerType(int index)
{
	return layers.at(index).layerType;
}

void LayerManager::draw()
{
	std::set<unsigned int> markedForDeletionLayerIndices;
	for (unsigned int i = 0; i < layers.size(); i++)
	{
		ImGui::Image((ImTextureID)layers.at(i).fbs.getColourTexture(), ImVec2(50, 50)); ImGui::SameLine();
		char* buffer = &layers.at(i).layerName[0];
		std::string inputText = "##Input text" + std::to_string(i);
		ImGui::InputText(inputText.c_str(), buffer, 200);
		std::string heightStrengthSliderName = "##Slider Value" + std::to_string(i);
		ImGui::SliderFloat(heightStrengthSliderName.c_str(), &layers.at(i).strength, -10.0f, 10.0f, "Strength: %.2f");

		const char* textureTypeItems[] = { "Height Map", "Normal Map" };
		int item_current = (int)layers.at(i).layerType;

		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth()*0.5f);
		std::string comboBoxName = "##combo" + std::to_string(i);
		if (ImGui::Combo(comboBoxName.c_str(), &item_current, textureTypeItems, IM_ARRAYSIZE(textureTypeItems)))
			layers.at(i).layerType = (LayerType)item_current;

		ImGui::SameLine();
		const char* blendingMethodItems[] = { "Reoriented Normal Blending", "Unreal Blending", "Partial Derivative Blending" };
		item_current = (int)layers.at(i).normalBlendMethod;
		comboBoxName = "##combo2" + std::to_string(i);
		if (ImGui::Combo(comboBoxName.c_str(), &item_current, blendingMethodItems, IM_ARRAYSIZE(blendingMethodItems)))
			layers.at(i).normalBlendMethod = (NormalBlendMethod)item_current;
		ImGui::PopItemWidth();

		std::string removeLayerButtonName = "Remove Layer##" + std::to_string(i);
		if (ImGui::Button(removeLayerButtonName.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth()*0.5f, 30)))
			markedForDeletionLayerIndices.insert(i);
		ImGui::SameLine();
		std::string hideLayerButtonName = "Toggle Visibility##" + std::to_string(i);
		if (ImGui::Button(hideLayerButtonName.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 30)))
			setLayerActiveState(i, !isLayerActive(i));
	}
	if (ImGui::Button("Add Layer", ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
	{
		FileExplorer::instance->displayDialog(FileType::IMAGE, [&](std::string str)
		{
			addLayer(TextureManager::loadTextureFromFile(str, true), LayerType::HEIGHT_MAP);
		});
	}
	std::set<unsigned int>::iterator it;
	for (it = markedForDeletionLayerIndices.begin(); it != markedForDeletionLayerIndices.end(); it++)
	{
		glDeleteTextures(1, &layers.at(*it).inputTextureId);
		delete[] layers.at(*it).layerName;
		layers.erase(layers.begin() + *it);
	}
}

void LayerManager::bindFrameBuffer(int index)
{
	layers.at(index).fbs.bindFrameBuffer();
}

unsigned int LayerManager::getColourTexture(int index)
{
	return layers.at(index).fbs.getColourTexture();
}

int LayerManager::getInputTexId(int index)
{
	return layers.at(index).inputTextureId;
}

LayerManager::~LayerManager()
{
	for (unsigned int i = 0; i < layers.size(); i++)
		delete[] layers.at(i).layerName;
}
