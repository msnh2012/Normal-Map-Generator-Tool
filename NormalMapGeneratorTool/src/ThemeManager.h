#pragma once
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
class ThemeManager
{
private:
	enum class Theme;
	Theme currentTheme = Theme::DEFAULT;
	void StyleColorsLight()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = AccentColour2;
		colors[ImGuiCol_TextDisabled] = SecondaryColour;
		colors[ImGuiCol_WindowBg] = PrimaryColour;
		colors[ImGuiCol_ChildBg] = PrimaryColour;
		colors[ImGuiCol_PopupBg] = PrimaryColour;
		colors[ImGuiCol_Border] = AccentColour1;
		colors[ImGuiCol_BorderShadow] = AccentColour3;
		colors[ImGuiCol_FrameBg] = AccentColour1;
		colors[ImGuiCol_FrameBgHovered] = PrimaryColour;
		colors[ImGuiCol_FrameBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBg] = TitleColour;
		colors[ImGuiCol_TitleBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBgCollapsed] = AccentColour1;
		colors[ImGuiCol_MenuBarBg] = TitleColour;
		colors[ImGuiCol_ScrollbarBg] = AccentColour1;
		colors[ImGuiCol_ScrollbarGrab] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabHovered] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabActive] = PrimaryColour;
		colors[ImGuiCol_CheckMark] = SecondaryColour;
		colors[ImGuiCol_SliderGrab] = AccentColour1;
		colors[ImGuiCol_SliderGrabActive] = AccentColour1;
		colors[ImGuiCol_Button] = SecondaryColour;
		colors[ImGuiCol_ButtonHovered] = PrimaryColour;
		colors[ImGuiCol_ButtonActive] = AccentColour1;
		colors[ImGuiCol_Header] = AccentColour1;
		colors[ImGuiCol_HeaderHovered] = ActiveColour1;
		colors[ImGuiCol_HeaderActive] = DisabledColour1;
		colors[ImGuiCol_Separator] = AccentColour2;
		colors[ImGuiCol_SeparatorHovered] = DisabledColour2;
		colors[ImGuiCol_SeparatorActive] = ActiveColour2;
		colors[ImGuiCol_ResizeGrip] = Sate1Colour;
		colors[ImGuiCol_ResizeGripHovered] = Sate2Colour;
		colors[ImGuiCol_ResizeGripActive] = Sate3Colour;
		colors[ImGuiCol_PlotLines] = Sate1Colour;
		colors[ImGuiCol_PlotLinesHovered] = Sate2Colour;
		colors[ImGuiCol_PlotHistogram] = Sate1Colour;
		colors[ImGuiCol_PlotHistogramHovered] = Sate2Colour;
		colors[ImGuiCol_TextSelectedBg] = Sate3Colour;
		colors[ImGuiCol_ModalWindowDarkening] = Sate3Colour;
		colors[ImGuiCol_DragDropTarget] = Sate3Colour;
	}
	void StyleColorsDark()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = AccentColour2;
		colors[ImGuiCol_TextDisabled] = SecondaryColour;
		colors[ImGuiCol_WindowBg] = PrimaryColour;
		colors[ImGuiCol_ChildBg] = PrimaryColour;
		colors[ImGuiCol_PopupBg] = PrimaryColour;
		colors[ImGuiCol_Border] = AccentColour1;
		colors[ImGuiCol_BorderShadow] = AccentColour3;
		colors[ImGuiCol_FrameBg] = AccentColour1;
		colors[ImGuiCol_FrameBgHovered] = PrimaryColour;
		colors[ImGuiCol_FrameBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBg] = TitleColour;
		colors[ImGuiCol_TitleBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBgCollapsed] = AccentColour1;
		colors[ImGuiCol_MenuBarBg] = TitleColour;
		colors[ImGuiCol_ScrollbarBg] = AccentColour1;
		colors[ImGuiCol_ScrollbarGrab] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabHovered] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabActive] = PrimaryColour;
		colors[ImGuiCol_CheckMark] = SecondaryColour;
		colors[ImGuiCol_SliderGrab] = AccentColour1;
		colors[ImGuiCol_SliderGrabActive] = AccentColour1;
		colors[ImGuiCol_Button] = SecondaryColour;
		colors[ImGuiCol_ButtonHovered] = PrimaryColour;
		colors[ImGuiCol_ButtonActive] = AccentColour1;
		colors[ImGuiCol_Header] = AccentColour1;
		colors[ImGuiCol_HeaderHovered] = ActiveColour1;
		colors[ImGuiCol_HeaderActive] = DisabledColour1;
		colors[ImGuiCol_Separator] = AccentColour2;
		colors[ImGuiCol_SeparatorHovered] = DisabledColour2;
		colors[ImGuiCol_SeparatorActive] = ActiveColour2;
		colors[ImGuiCol_ResizeGrip] = Sate1Colour;
		colors[ImGuiCol_ResizeGripHovered] = Sate2Colour;
		colors[ImGuiCol_ResizeGripActive] = Sate3Colour;
		colors[ImGuiCol_PlotLines] = Sate1Colour;
		colors[ImGuiCol_PlotLinesHovered] = Sate2Colour;
		colors[ImGuiCol_PlotHistogram] = Sate1Colour;
		colors[ImGuiCol_PlotHistogramHovered] = Sate2Colour;
		colors[ImGuiCol_TextSelectedBg] = Sate3Colour;
		colors[ImGuiCol_ModalWindowDarkening] = Sate3Colour;
		colors[ImGuiCol_DragDropTarget] = Sate3Colour;

	}
	void CustomColourImGuiTheme()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = AccentColour2;
		colors[ImGuiCol_TextDisabled] = SecondaryColour;
		colors[ImGuiCol_WindowBg] = PrimaryColour;
		colors[ImGuiCol_ChildBg] = PrimaryColour;
		colors[ImGuiCol_PopupBg] = PrimaryColour;
		colors[ImGuiCol_Border] = AccentColour1;
		colors[ImGuiCol_BorderShadow] = AccentColour3;
		colors[ImGuiCol_FrameBg] = AccentColour1;
		colors[ImGuiCol_FrameBgHovered] = PrimaryColour;
		colors[ImGuiCol_FrameBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBg] = TitleColour;
		colors[ImGuiCol_TitleBgActive] = SecondaryColour;
		colors[ImGuiCol_TitleBgCollapsed] = AccentColour1;
		colors[ImGuiCol_MenuBarBg] = TitleColour;
		colors[ImGuiCol_ScrollbarBg] = AccentColour1;
		colors[ImGuiCol_ScrollbarGrab] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabHovered] = SecondaryColour;
		colors[ImGuiCol_ScrollbarGrabActive] = PrimaryColour;
		colors[ImGuiCol_CheckMark] = SecondaryColour;
		colors[ImGuiCol_SliderGrab] = AccentColour1;
		colors[ImGuiCol_SliderGrabActive] = AccentColour1;
		colors[ImGuiCol_Button] = SecondaryColour;
		colors[ImGuiCol_ButtonHovered] = PrimaryColour;
		colors[ImGuiCol_ButtonActive] = AccentColour1;
		colors[ImGuiCol_Header] = AccentColour1;
		colors[ImGuiCol_HeaderHovered] = ActiveColour1;
		colors[ImGuiCol_HeaderActive] = DisabledColour1;
		colors[ImGuiCol_Separator] = AccentColour2;
		colors[ImGuiCol_SeparatorHovered] = DisabledColour2;
		colors[ImGuiCol_SeparatorActive] = ActiveColour2;
		colors[ImGuiCol_ResizeGrip] = Sate1Colour;
		colors[ImGuiCol_ResizeGripHovered] = Sate2Colour;
		colors[ImGuiCol_ResizeGripActive] = Sate3Colour;
		colors[ImGuiCol_PlotLines] = Sate1Colour;
		colors[ImGuiCol_PlotLinesHovered] = Sate2Colour;
		colors[ImGuiCol_PlotHistogram] = Sate1Colour;
		colors[ImGuiCol_PlotHistogramHovered] = Sate2Colour;
		colors[ImGuiCol_TextSelectedBg] = Sate3Colour;
		colors[ImGuiCol_ModalWindowDarkening] = Sate3Colour;
		colors[ImGuiCol_DragDropTarget] = Sate3Colour;
	}
public:
	enum class Theme
	{
		DEFAULT, DARK, LIGHT, BLUE, GREEN, ULTRA_VIOLET
	};
	ImVec4 PrimaryColour;
	//Applied on title and menu bar
	ImVec4 TitleColour;
	ImVec4 SecondaryColour;
	ImVec4 AccentColour1;
	//Applied on text and separator
	ImVec4 AccentColour2;
	//Applied on border shadow
	ImVec4 AccentColour3;
	ImVec4 ActiveColour1;
	ImVec4 ActiveColour2;
	ImVec4 DisabledColour1;
	ImVec4 DisabledColour2;
	ImVec4 Sate1Colour;
	ImVec4 Sate2Colour;
	ImVec4 Sate3Colour;

	void Init()
	{
		PrimaryColour = ImVec4(40 / 255.0f, 49 / 255.0f, 73.0f / 255.0f, 1.1f);
		TitleColour = ImVec4(30 / 255.0f, 39 / 255.0f, 63.0f / 255.0f, 1.1f);
		SecondaryColour = ImVec4(247 / 255.0f, 56 / 255.0f, 89 / 255.0f, 1.1f);
		AccentColour1 = ImVec4(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.1f);
		AccentColour2 = ImVec4(1, 1, 1, 1.1f);
		AccentColour3 = ImVec4(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.1f);
		ActiveColour1 = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		DisabledColour1 = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		ActiveColour2 = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
		DisabledColour2 = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
		Sate1Colour = ImVec4(0.30f, 0.30f, 0.70f, 0.46f);
		Sate2Colour = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		Sate3Colour = ImVec4(0.10f, 0.1f, 0.25f, 0.01f);
	}
	void EnableInBuiltTheme(Theme theme)
	{
		std::cout << "\nset theme called !!";
		currentTheme = theme;
		switch (theme)
		{
		case Theme::DEFAULT:
			PrimaryColour = ImVec4(40 / 255.0f, 49 / 255.0f, 73.0f / 255.0f, 1.1f);
			TitleColour = ImVec4(30 / 255.0f, 39 / 255.0f, 63.0f / 255.0f, 1.1f);
			SecondaryColour = ImVec4(247 / 255.0f, 56 / 255.0f, 89 / 255.0f, 1.1f);
			AccentColour1 = ImVec4(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.1f);
			AccentColour2 = ImVec4(1, 1, 1, 1.1f);
			AccentColour3 = ImVec4(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.1f);
			ActiveColour1 = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			DisabledColour1 = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			ActiveColour2 = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
			DisabledColour2 = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
			Sate1Colour = ImVec4(0.30f, 0.30f, 0.70f, 0.46f);
			Sate2Colour = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
			CustomColourImGuiTheme();
			break;
		case Theme::DARK:
			PrimaryColour = ImVec4(0.2f, 0.2f, 0.2f, 1.1f);
			TitleColour = ImVec4(0.13f, 0.13f, 0.13f, 1.1f);
			SecondaryColour = ImVec4(0.3f, 0.3f, 0.3f, 1.1f);
			AccentColour1 = ImVec4(0.15f, 0.15f, 0.15f, 1.1f);
			AccentColour2 = ImVec4(1, 1, 1, 1.1f);
			AccentColour3 = ImVec4(0.9f, 0.9f, 0.9f, 1.1f);
			ActiveColour1 = ImVec4(0.93f, 0.93f, 0.93f, 0.80f);
			DisabledColour1 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			ActiveColour2 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			DisabledColour2 = ImVec4(0.6f, 0.6f, 0.6f, 0.78f);
			Sate1Colour = ImVec4(0.80f, 0.80f, 0.80f, 0.46f);
			Sate2Colour = ImVec4(0.8f, 0.8f, 0.8f, 0.67f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
			StyleColorsDark();
			break;
		case Theme::LIGHT:
			PrimaryColour = ImVec4(0.9f, 0.9f, 0.95f, 1.1f);
			TitleColour = ImVec4(0.93f, 0.93f, 0.95f, 1.1f);
			SecondaryColour = ImVec4(0.8f, 0.8f, 0.8f, 1.1f);
			AccentColour1 = ImVec4(0.65f, 0.65f, 0.65f, 1.1f);
			AccentColour2 = ImVec4(0.15f, 0.15f, 0.15f, 1.1f);
			AccentColour3 = ImVec4(0.2f, 0.2f, 0.2f, 1.1f);
			ActiveColour1 = ImVec4(0.75f, 0.75f, 0.77f, 0.80f);
			DisabledColour1 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			ActiveColour2 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			DisabledColour2 = ImVec4(0.6f, 0.6f, 0.6f, 0.78f);
			Sate1Colour = ImVec4(0.50f, 0.50f, 0.50f, 0.46f);
			Sate2Colour = ImVec4(0.8f, 0.8f, 0.8f, 0.67f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
			StyleColorsLight();
			break;
		case Theme::BLUE:
			PrimaryColour = ImVec4(0.2f, 0.5f, 0.6f, 1.1f);
			TitleColour = ImVec4(0.2f, 0.4f, 0.5f, 1.1f);
			SecondaryColour = ImVec4(0.2f, 0.3f, 0.4f, 1.1f);
			AccentColour1 = ImVec4(0.1f, 0.2f, 0.3f, 1.1f);
			AccentColour2 = ImVec4(1.0f, 1.0f, 1.0f, 1.1f);
			AccentColour3 = ImVec4(0.9f, 0.9f, 0.9f, 1.1f);
			ActiveColour1 = ImVec4(0.93f, 0.93f, 0.93f, 0.80f);
			DisabledColour1 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			ActiveColour2 = ImVec4(0.6f, 0.6f, 0.8f, 1.00f);
			DisabledColour2 = ImVec4(0.6f, 0.6f, 0.6f, 0.78f);
			Sate1Colour = ImVec4(0.10f, 0.10f, 0.80f, 0.46f);
			Sate2Colour = ImVec4(0.8f, 0.8f, 0.8f, 0.97f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.2f);
			StyleColorsLight();
			break;
		case Theme::GREEN:
			PrimaryColour = ImVec4(0.15f, 0.15f, 0.15f, 1.1f);
			TitleColour = ImVec4(0.13f, 0.13f, 0.13f, 1.1f);
			SecondaryColour = ImVec4(0.55f, 0.75f, 0.25f, 1.1f);
			AccentColour1 = ImVec4(0.25f, 0.25f, 0.25f, 1.1f);
			AccentColour2 = ImVec4(0.85f, 1.0f, 0.85f, 1.1f);
			AccentColour3 = ImVec4(0.9f, 0.9f, 0.9f, 1.1f);
			ActiveColour1 = ImVec4(0.93f, 0.93f, 0.93f, 0.80f);
			DisabledColour1 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			ActiveColour2 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			DisabledColour2 = ImVec4(0.6f, 0.6f, 0.6f, 0.78f);
			Sate1Colour = ImVec4(0.80f, 0.80f, 0.80f, 0.46f);
			Sate2Colour = ImVec4(0.8f, 0.8f, 0.8f, 0.67f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
			StyleColorsDark();
			break;
		case Theme::ULTRA_VIOLET:
			PrimaryColour = ImVec4(0.15f, 0.0f, 0.2f, 1.1f);
			TitleColour = ImVec4(0.18f, 0.0f, 0.22f, 1.1f);
			SecondaryColour = ImVec4(0.45f, 0.1f, 0.5f, 1.1f);
			AccentColour1 = ImVec4(0.1f, 0.0f, 0.1f, 1.1f);
			AccentColour2 = ImVec4(0.8f, 0.7f, 1.0f, 1.1f);
			AccentColour3 = ImVec4(0.9f, 0.9f, 0.9f, 1.1f);
			ActiveColour1 = ImVec4(0.93f, 0.93f, 0.93f, 0.80f);
			DisabledColour1 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			ActiveColour2 = ImVec4(0.6f, 0.6f, 0.6f, 1.00f);
			DisabledColour2 = ImVec4(0.6f, 0.6f, 0.6f, 0.78f);
			Sate1Colour = ImVec4(0.80f, 0.80f, 0.80f, 0.46f);
			Sate2Colour = ImVec4(0.8f, 0.8f, 0.8f, 0.67f);
			Sate3Colour = ImVec4(0.0f, 0.0f, 0.0f, 0.6f);
			StyleColorsLight();
			break;
		default:
			break;
		}
	}
	void SetThemeFromFile(const std::string& filePath)
	{
		std::string data = "";
		std::ifstream myfile(filePath);
		if (myfile.is_open())
		{
			std::string line;
			while (getline(myfile, line))
			{
				if (line[0] == '#' || line.size() < 10)
					continue;
				std::string colourType, colourValue;
				int indexOfColon = line.find(':');
				int indexOfSemiColon = line.find(';');
				colourType = line.substr(0, indexOfColon - 1);
				colourValue = line.substr(indexOfColon + 1, indexOfSemiColon - (indexOfColon + 1));

				int indexOfStartBrac = colourValue.find('(') + 1;
				int indexOfEndBrac = colourValue.find(')');
				colourValue = colourValue.substr(indexOfStartBrac, indexOfEndBrac - indexOfStartBrac);

				int indexOfFirstComma = colourValue.find(',');
				int intdexOfSecondComma = colourValue.find(',', indexOfFirstComma + 1);
				int indexOfThirdComma = colourValue.find(',', intdexOfSecondComma + 1);
				std::string num1 = colourValue.substr(0, indexOfFirstComma);
				std::string num2 = colourValue.substr(indexOfFirstComma + 1, intdexOfSecondComma - (indexOfFirstComma + 1));
				std::string num3 = colourValue.substr(intdexOfSecondComma + 1, indexOfThirdComma - (intdexOfSecondComma + 1));
				std::string num4 = colourValue.substr(indexOfThirdComma + 1);

				ImVec4 col(std::atof(num1.c_str()), std::atof(num2.c_str()), std::atof(num3.c_str()), std::atof(num4.c_str()));
				std::cout << "\nColour type : " << colourType << "." << col.x << "," << col.y << "," << col.z << "," << col.w;
				if (colourType == "PrimaryColour")
				{
					PrimaryColour = col;
				}
				else if (colourType == "TitleColour")
				{
					TitleColour = col;
				}
				else if (colourType == "SecondaryColour")
				{
					SecondaryColour = col;
				}
				else if (colourType == "AccentColour1")
				{
					AccentColour1 = col;
				}
				else if (colourType == "AccentColour2")
				{
					AccentColour2 = col;
				}
				else if (colourType == "AccentColour3")
				{
					AccentColour3 = col;
				}
				else if (colourType == "ActiveColour1")
				{
					ActiveColour1 = col;
				}
				else if (colourType == "ActiveColour2")
				{
					ActiveColour2 = col;
				}
				else if (colourType == "DisabledColour1")
				{
					DisabledColour1 = col;
				}
				else if (colourType == "DisabledColour2")
				{
					DisabledColour2 = col;
				}
				else if (colourType == "Sate1Colour")
				{
					Sate1Colour = col;
				}
				else if (colourType == "Sate2Colour")
				{
					Sate2Colour = col;
				}
				else if (colourType == "Sate3Colour")
				{
					Sate3Colour = col;
				}
			}
			myfile.close();
		}
		StyleColorsDark();
		//std::cout << "\nDATA : \n" << data;
	}
};