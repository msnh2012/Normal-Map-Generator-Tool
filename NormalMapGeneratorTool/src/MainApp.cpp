#include <iostream>
#include <string>
#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <thread> 

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "DrawingPanel.h"
#include "FrameBuffer.h"
#include "TextureData.h"
#include "ColourData.h"
#include "BrushData.h"
#include "TextureLoader.h"
#include "ShaderProgram.h"
#include "Transform.h"

//TODO : Rotation editor values
//TODO : Move framebuffer stuff to custom class
//TODO : Display brush preview
//TODO : Saving out notmal map in 512x512 irrespective of window size
//TODO : Diffuse & Specular lighting colour
//TODO : Custom Window Chrome
//TODO : Undo/Redo Capability
//TODO : Custom Model Import To View With Normals

const ImVec4 PRIMARY_COL = ImVec4(40 / 255.0f, 49 / 255.0f, 73.0f / 255.0f, 1.1f);
const ImVec4 SECONDARY_COL = ImVec4(247 / 255.0f, 56 / 255.0f, 89 / 255.0f, 1.1f);
const ImVec4 ACCENT_COL = ImVec4(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.1f);
const ImVec4 WHITE = ImVec4(255 / 255.0f, 247 / 255.0f, 240 / 255.0f, 1.1f);
const ImVec4 DARK_GREY = ImVec4(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.1f);

const float TOP_BAR_HEIGHT = 50.0f;
const float TOP_BAR_BUTTON_SIDE_SIZE = 25.0f;
const float TOP_BAR_BUTTON_X_GAP_SIZE = 80.0f;
const float TOP_BAR_BUTTON_Y_GAP_SIZE = 40.0f;

int windowWidth = 800;
int windowHeight = 800;
unsigned int framebuffer;
unsigned int textureColorbuffer;
GLFWwindow* window;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void CustomColourImGuiTheme(ImGuiStyle* dst = (ImGuiStyle*)0);
bool isKeyPressed(int key);
bool isKeyReleased(int key);
bool saveScreenshot(std::string filename, int xOff, int yOff, int w, int h);
void SetPixelValues(int startX, int width, int startY, int height, double xpos, double ypos, BrushData brushData);
void drawLine(int x0, int y0, int x1, int y1);
void plotLineLow(int x0, int y0, int x1, int y1);
void plotLineHigh(int x0, int y0, int x1, int y1);
float zoomLevel = 1;
float yUiScale = 1;
float xUiButtonScale = 1;
float yUiButtonScale = 1;
float xGapButtonGapSize = 1;
float yGapButtonGapSize = 1;
TextureData texData;

int main(void)
{
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_DECORATED, false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(windowWidth, windowHeight, "Normal Map Editor v0.5 alpha", NULL, NULL);

	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	yUiScale = TOP_BAR_HEIGHT / windowHeight;
	xUiButtonScale = TOP_BAR_BUTTON_SIDE_SIZE / windowWidth;
	yUiButtonScale = TOP_BAR_BUTTON_SIDE_SIZE / windowHeight;
	xGapButtonGapSize = TOP_BAR_BUTTON_X_GAP_SIZE / windowWidth;
	yGapButtonGapSize = TOP_BAR_BUTTON_Y_GAP_SIZE / windowHeight;

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Open GL init error" << std::endl;
		return EXIT_FAILURE;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window, true);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	CustomColourImGuiTheme();
	ImFont* font = io.Fonts->AddFontFromFileTTF("Resources\\arial.ttf", 16.0f);
	IM_ASSERT(font != NULL);

	DrawingPanel normalmapPanel;
	normalmapPanel.init(1.0f, 1.0f);
	DrawingPanel frameDrawingPanel;
	frameDrawingPanel.init(1.0f, 1.0f);
	DrawingPanel topBarWindowChrome;
	topBarWindowChrome.init(1.0f, 1.0f);
	DrawingPanel topBarCloseButton;
	DrawingPanel topBarRestoreDownMaximizeButton;
	DrawingPanel topBarMinimizeButton;
	topBarCloseButton.init(1.0f, 1.0f);
	topBarRestoreDownMaximizeButton.init(1.0f, 1.0f);
	topBarMinimizeButton.init(1.0f, 1.0f);

	unsigned int closeTexture = TextureManager::loadTextureFromFile("Resources\\UI\\closeIcon.png", "close", false);
	unsigned int restoreTexture = TextureManager::loadTextureFromFile("Resources\\UI\\maxWinIcon.png", "restore", false);
	unsigned int minimizeTexture = TextureManager::loadTextureFromFile("Resources\\UI\\toTrayIcon.png", "mini", false);

	topBarCloseButton.setTextureID(closeTexture);
	topBarRestoreDownMaximizeButton.setTextureID(restoreTexture);
	topBarMinimizeButton.setTextureID(minimizeTexture);

	TextureManager::getTextureDataFromFile("Resources\\goli.png", texData);
	unsigned int texId = TextureManager::loadTextureFromData(texData, false);
	normalmapPanel.setTextureID(texId);

	ShaderProgram normalmapShader;
	normalmapShader.compileShaders("Resources\\spriteBase.vs", "Resources\\spriteBase.fs");
	normalmapShader.linkShaders();

	ShaderProgram frameShader;
	frameShader.compileShaders("Resources\\spriteBase.vs", "Resources\\frameBuffer.fs");
	frameShader.linkShaders();

	ShaderProgram windowChromeShader;
	windowChromeShader.compileShaders("Resources\\spriteBase.vs", "Resources\\windowChrome.fs");
	windowChromeShader.linkShaders();

	int windowChromeModelUniform = windowChromeShader.getUniformLocation("model");
	int windowChromeColourUniform = windowChromeShader.getUniformLocation("_chromeColour");

	int frameModelMatrixUniform = normalmapShader.getUniformLocation("model");
	int modelMatrixUniform = normalmapShader.getUniformLocation("model");
	int strengthValueUniform = normalmapShader.getUniformLocation("_HeightmapStrength");
	int normalMapModeOnUniform = normalmapShader.getUniformLocation("_normalMapModeOn");
	int widthUniform = normalmapShader.getUniformLocation("_HeightmapDimX");
	int heightUniform = normalmapShader.getUniformLocation("_HeightmapDimY");
	int specularityUniform = normalmapShader.getUniformLocation("_Specularity");
	int lightIntensityUniform = normalmapShader.getUniformLocation("_LightIntensity");
	int flipXYdirUniform = normalmapShader.getUniformLocation("_flipX_Ydir");

	float normalMapStrength = 10.0f;
	float specularity = 0.5f;
	float lightIntensity = 0.5f;
	zoomLevel = 1;
	int mapViewMode = 1;
	float widthRes = texData.getWidth();
	float heightRes = texData.getHeight();
	bool flipX_Ydir = false;

	glm::vec3 rotation = glm::vec3(0);
	int k = 0;
	bool showHeightMapInput = true;
	bool isFullscreen = false;
	bool isMaximized = false;

	BrushData brushData;
	brushData.brushScale = 0.05f;
	brushData.brushOffset = 1.0f;
	brushData.brushStrength = 1.0f;
	brushData.brushMinHeight = 0.0f;
	brushData.brushMaxHeight = 1.0f;
	brushData.heightMapPositiveDir = false;
	double initTime = glfwGetTime();

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec2 prevMouseCoord = glm::vec2(-10, -10);
	glm::vec2 prevMiddleMouseButtonCoord = glm::vec2(-10, -10);
	glm::vec2 prevGlobalFirstMouseCoord = glm::vec2(-500, -500);

	while (!glfwWindowShouldClose(window))
	{
		double deltaTime = glfwGetTime() - initTime;
		initTime = glfwGetTime();

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, texId);
		normalmapShader.use();

		if (isKeyPressed(GLFW_KEY_J))
			mapViewMode = 1;
		if (isKeyPressed(GLFW_KEY_K))
			mapViewMode = 2;
		if (isKeyPressed(GLFW_KEY_L))
			mapViewMode = 3;

		if (isKeyPressed(GLFW_KEY_LEFT))
			normalmapPanel.getTransform()->translate(-1 * deltaTime, 0);
		if (isKeyPressed(GLFW_KEY_RIGHT))
			normalmapPanel.getTransform()->translate(1 * deltaTime, 0);
		if (isKeyPressed(GLFW_KEY_UP))
			normalmapPanel.getTransform()->translate(0, 1 * deltaTime);
		if (isKeyPressed(GLFW_KEY_DOWN))
			normalmapPanel.getTransform()->translate(0, -1 * deltaTime);
		if (isKeyPressed(GLFW_KEY_8))
			normalmapPanel.getTransform()->rotate(1.0f * deltaTime);
		if (isKeyPressed(GLFW_KEY_V))
		{
			glfwSetWindowSize(window, 800, 800);
			isMaximized = false;
		}

		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		static glm::vec2 initPos = glm::vec2(-1000, -1000);
		static int topBarButtonOver = 0;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		topBarButtonOver = 0;
		if (y < 40)
		{
			if (x > windowWidth - 125)
			{
				if (x > windowWidth - 50)
					topBarButtonOver = 3;
				else if (x > windowWidth - 80 && x < windowWidth - 50)
					topBarButtonOver = 2;
				else if (x < windowWidth - 90)
					topBarButtonOver = 1;
			}
		}
		if (state == GLFW_PRESS)
		{
			if (initPos == glm::vec2(-1000, -1000))
				initPos = glm::vec2(x, y);
			if (y < 40)
			{
				if (x > windowWidth - 125)
				{
					if (x > windowWidth - 50)
						glfwSetWindowShouldClose(window, true);
					else if (x > windowWidth - 100 && x < windowWidth - 50)
					{
						if (!isMaximized)
						{
							glfwMaximizeWindow(window);
							isMaximized = true;
						}
						else
						{
							glfwSetWindowSize(window, 800, 800);
							isMaximized = false;
						}
					}
					else
						glfwIconifyWindow(window);
				}
				else
				{
					glm::vec2 currentPos(x, y);
					if (prevGlobalFirstMouseCoord != currentPos && prevGlobalFirstMouseCoord != glm::vec2(-500, -500))
					{
						int winPosX, winPosY;
						glfwGetWindowPos(window, &winPosX, &winPosY);
						glfwSetWindowPos(window, winPosX + currentPos.x - initPos.x, winPosY + currentPos.y - initPos.y);
					}
				}
			}
			prevGlobalFirstMouseCoord = glm::vec2(x, y);
		}
		else
		{
			initPos = glm::vec2(-1000, -1000);
			prevGlobalFirstMouseCoord = glm::vec2(-500, -500);
		}

		state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		if (state == GLFW_PRESS)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 currentPos(x, y);
			glm::vec2 diff = (currentPos - prevMiddleMouseButtonCoord) * (1.0f + zoomLevel) * (float)deltaTime;
			normalmapPanel.getTransform()->translate(diff.x, -diff.y);
			prevMiddleMouseButtonCoord = currentPos;
		}
		else
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			prevMiddleMouseButtonCoord = glm::vec2(x, y);
		}

		state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			glm::vec2 currentPos(xpos, ypos);
			if (currentPos != prevMouseCoord)
			{
				xpos = xpos / windowWidth;
				ypos = 1.0f - (ypos / windowHeight);
				if (normalmapPanel.isPointInPanel(xpos, ypos))
				{
					float prevX = prevMouseCoord.x / windowWidth;
					float prevY = 1.0f - (prevMouseCoord.y / windowHeight);

					glm::vec4 worldDimensions = normalmapPanel.getPanelWorldDimension();
					xpos = (xpos - worldDimensions.x) / (worldDimensions.y - worldDimensions.x);
					ypos = (ypos - worldDimensions.w) / (worldDimensions.z - worldDimensions.w);

					float maxWidth = texData.getWidth();
					float convertedBrushScale = brushData.brushScale;
					int left = glm::clamp((int)((xpos - convertedBrushScale) * maxWidth), 0, (int)maxWidth);
					int right = glm::clamp((int)((xpos + convertedBrushScale) * maxWidth), 0, (int)maxWidth);
					int bottom = glm::clamp((int)((ypos - convertedBrushScale) * maxWidth), 0, (int)maxWidth);
					int top = glm::clamp((int)((ypos + convertedBrushScale) * maxWidth), 0, (int)maxWidth);

					SetPixelValues(left, right, bottom, top, xpos, ypos, brushData);

					GLenum format = TextureManager::getTextureFormatFromData(4);
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texData.getWidth(),
						texData.getHeight(), format, GL_UNSIGNED_BYTE, texData.getTextureData());
				}
				prevMouseCoord = currentPos;
			}
		}
		else
		{
			prevMouseCoord = glm::vec2(-10, -10);
		}

		if (isKeyPressed(GLFW_KEY_A))
			normalMapStrength += 2.5f * deltaTime;
		if (isKeyPressed(GLFW_KEY_D))
			normalMapStrength -= 2.5f * deltaTime;
		if (isKeyPressed(GLFW_KEY_W))
			zoomLevel += zoomLevel * 1.5f * deltaTime;
		if (isKeyPressed(GLFW_KEY_S))
			zoomLevel -= zoomLevel * 1.5f * deltaTime;
		zoomLevel = glm::clamp(zoomLevel, 0.1f, 5.0f);

		//---- Making sure the dimensions do not change for drawing panel ----//
		float aspectRatio = (float)windowWidth / (float)windowHeight;
		if (windowWidth < windowHeight)
			normalmapPanel.getTransform()->setScale(glm::vec2(1, aspectRatio) * zoomLevel);
		else
			normalmapPanel.getTransform()->setScale(glm::vec2(1.0f / aspectRatio, 1) * zoomLevel);
		normalmapPanel.getTransform()->setX(glm::clamp(normalmapPanel.getTransform()->getPosition().x, -0.5f, 0.9f));
		normalmapPanel.getTransform()->setY(glm::clamp(normalmapPanel.getTransform()->getPosition().y, -0.8f, 0.8f));
		normalmapPanel.getTransform()->update();
		//---- Applying Shader Uniforms---//
		normalmapShader.applyShaderUniformMatrix(modelMatrixUniform, normalmapPanel.getTransform()->getMatrix());
		normalmapShader.applyShaderFloat(strengthValueUniform, normalMapStrength);
		normalmapShader.applyShaderFloat(specularityUniform, specularity);
		normalmapShader.applyShaderFloat(lightIntensityUniform, lightIntensity);
		normalmapShader.applyShaderFloat(widthUniform, widthRes);
		normalmapShader.applyShaderFloat(heightUniform, heightRes);
		normalmapShader.applyShaderInt(normalMapModeOnUniform, mapViewMode);
		normalmapShader.applyShaderBool(flipXYdirUniform, flipX_Ydir);
		normalmapPanel.draw();
		static char saveLocation[500] = "D:\\scr.tga";
		static char imageLoadLocation[500] = "Resources\\goli.png";
		static bool shouldSaveNormalMap = false;
		if (isKeyPressed(GLFW_KEY_F10) || shouldSaveNormalMap)
		{
			glm::vec2 tempPos = normalmapPanel.getTransform()->getPosition();
			glm::vec2 tempScale = normalmapPanel.getTransform()->getScale();

			normalmapPanel.getTransform()->setPosition(glm::vec2());
			float aspectRatio = (float)windowWidth / (float)windowHeight;
			if (windowWidth < windowHeight)
				normalmapPanel.getTransform()->setScale(glm::vec2(1, aspectRatio));
			else
				normalmapPanel.getTransform()->setScale(glm::vec2(1.0f / aspectRatio, 1));
			normalmapPanel.getTransform()->update();
			normalmapShader.applyShaderUniformMatrix(modelMatrixUniform, normalmapPanel.getTransform()->getMatrix());
			normalmapPanel.draw();

			int tempWindowWidth = windowWidth;
			int tempWindowHeight = windowHeight;

			int widthSub = windowWidth - (int)(normalmapPanel.getPanelWorldDimension().y * windowWidth);
			int heightSub = windowHeight - (int)(normalmapPanel.getPanelWorldDimension().z * windowHeight);

			std::string locationStr = std::string(saveLocation);
			if (locationStr.length() > 4)
			{
				if (saveScreenshot(locationStr, widthSub, heightSub, windowWidth - (2 * widthSub), windowHeight - (2 * heightSub)))
					std::cout << "Saved at " << locationStr;
				shouldSaveNormalMap = false;
			}
			normalmapPanel.getTransform()->setPosition(tempPos);
			normalmapPanel.getTransform()->setScale(tempScale);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		frameShader.use();
		frameShader.applyShaderUniformMatrix(modelMatrixUniform, glm::mat4());
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		frameDrawingPanel.setTextureID(textureColorbuffer);
		frameDrawingPanel.draw();

		ImGui_ImplGlfwGL3_NewFrame();
		bool *opn = NULL;
		ImGuiWindowFlags window_flags = 0;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
		window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		bool *p_open = NULL;

		ImGui::SetNextWindowPos(ImVec2(0, 40), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(glm::clamp(windowWidth * 0.15f, 250.0f, 600.0f), windowHeight), ImGuiSetCond_Always);
		ImGui::Begin("Settings", p_open, window_flags);
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
		if (ImGui::Button("Toggle Fullscreen", ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
		{
			if (!isFullscreen)
				glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 60);
			else
				glfwSetWindowMonitor(window, NULL, 100, 100, (mode->width / 1.3f), (mode->height / 1.2f), 60);
			isFullscreen = !isFullscreen;
		}
		if (ImGui::Button("Toggle Height", ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
		{
			brushData.heightMapPositiveDir = !brushData.heightMapPositiveDir;
		}
		if (ImGui::Button("Reset View", ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
		{
			normalmapPanel.getTransform()->setPosition(0, 0);
			normalmapPanel.getTransform()->setRotation(0);
			zoomLevel = 1;
		}
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() / 1.45f);
		ImGui::InputText("## Load location", imageLoadLocation, sizeof(imageLoadLocation));
		ImGui::PopItemWidth();
		ImGui::SameLine(0, 5);
		if (ImGui::Button("LOAD", ImVec2(ImGui::GetContentRegionAvailWidth(), 27)))
		{
			TextureManager::getTextureDataFromFile(imageLoadLocation, texData);
			texId = TextureManager::loadTextureFromData(texData, false);
			normalmapPanel.setTextureID(texId);
		}
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() / 1.45f);
		ImGui::InputText("## Save location", saveLocation, sizeof(saveLocation));
		ImGui::PopItemWidth();
		ImGui::SameLine(0, 5);
		if (ImGui::Button("EXPORT", ImVec2(ImGui::GetContentRegionAvailWidth(), 27))) { shouldSaveNormalMap = true; }
		ImGui::PopStyleVar();
		ImGui::Spacing();
		ImGui::Text("VIEW MODE");
		ImGui::Separator();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
		int modeButtonWidth = (int)(ImGui::GetContentRegionAvailWidth() / 3.0f);
		ImGui::Spacing();

		if (mapViewMode == 3) ImGui::PushStyleColor(ImGuiCol_Button, ACCENT_COL);
		else ImGui::PushStyleColor(ImGuiCol_Button, SECONDARY_COL);
		if (ImGui::Button("Height", ImVec2(modeButtonWidth - 5, 40))) { mapViewMode = 3; }
		ImGui::PopStyleColor();

		ImGui::SameLine(0, 5);
		if (mapViewMode == 1) ImGui::PushStyleColor(ImGuiCol_Button, ACCENT_COL);
		else ImGui::PushStyleColor(ImGuiCol_Button, SECONDARY_COL);
		if (ImGui::Button("Normal", ImVec2(modeButtonWidth - 5, 40))) { mapViewMode = 1; }
		ImGui::PopStyleColor();

		ImGui::SameLine(0, 5);
		if (mapViewMode == 2) ImGui::PushStyleColor(ImGuiCol_Button, ACCENT_COL);
		else ImGui::PushStyleColor(ImGuiCol_Button, SECONDARY_COL);
		if (ImGui::Button("3D Plane", ImVec2(modeButtonWidth, 40))) { mapViewMode = 2; }
		ImGui::PopStyleColor();

		ImGui::PopStyleVar();
		ImGui::Text("BRUSH SETTINGS");
		ImGui::Separator();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, SECONDARY_COL);
		if (ImGui::SliderFloat(" Brush Scale", &brushData.brushScale, 0.0f, 1.0f, "%.3f", 1.0f)) {}
		if (ImGui::SliderFloat(" Brush Offset", &brushData.brushOffset, 1.0f, 100.0f, "%.2f", 1.0f)) {}
		if (ImGui::SliderFloat(" Brush Strength", &brushData.brushStrength, 0.0f, 1.0f, "%0.2f", 1.0f)) {}
		if (ImGui::SliderFloat(" Brush Min Height", &brushData.brushMinHeight, 0.0f, 1.0f, "%0.2f", 1.0f)) {}
		if (ImGui::SliderFloat(" Brush Max Height", &brushData.brushMaxHeight, 0.0f, 1.0f, "%0.2f", 1.0f)) {}

		if (brushData.brushMinHeight > brushData.brushMaxHeight)
			brushData.brushMinHeight = brushData.brushMaxHeight;
		else if (brushData.brushMaxHeight < brushData.brushMinHeight)
			brushData.brushMaxHeight = brushData.brushMinHeight;

		if (mapViewMode < 3)
		{
			ImGui::Text("NORMAL SETTINGS");
			ImGui::Separator();
			if (ImGui::SliderFloat(" Normal Strength", &normalMapStrength, -100.0f, 100.0f, "%.2f")) {}
			if (ImGui::Button("Flip X-Y", ImVec2(ImGui::GetContentRegionAvailWidth(), 40))) { flipX_Ydir = !flipX_Ydir; }
			if (mapViewMode == 2)
			{
				ImGui::Text("LIGHT SETTINGS");
				ImGui::Separator();
				if (ImGui::SliderFloat(" Light Intensity", &lightIntensity, 0.0f, 1.0f, "%.2f")) {}
				if (ImGui::SliderFloat(" Specularity", &specularity, 0.0f, 1.0f, "%.2f")) {}
			}
		}
		ImGui::PopStyleColor();

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		topBarCloseButton.getTransform()->setScale(glm::vec2(xUiButtonScale, yUiButtonScale));
		float initOffsetXpos = 1.0 - (xGapButtonGapSize * 0.5f);
		float yOffsetGap = 1.0f - yGapButtonGapSize;
		topBarCloseButton.getTransform()->setPosition(initOffsetXpos, yOffsetGap);

		topBarRestoreDownMaximizeButton.getTransform()->setScale(glm::vec2(xUiButtonScale, yUiButtonScale));
		topBarRestoreDownMaximizeButton.getTransform()->setPosition(1.0f - xGapButtonGapSize * 1.5f, yOffsetGap);

		topBarMinimizeButton.getTransform()->setScale(glm::vec2(xUiButtonScale, yUiButtonScale));
		topBarMinimizeButton.getTransform()->setPosition(1.0f - (xGapButtonGapSize * 2.5f), yOffsetGap);

		topBarCloseButton.getTransform()->update();
		topBarRestoreDownMaximizeButton.getTransform()->update();
		topBarMinimizeButton.getTransform()->update();

		topBarWindowChrome.getTransform()->setScale(glm::vec2(1, yUiScale));
		topBarWindowChrome.getTransform()->setPosition(0, 0.96f);
		topBarWindowChrome.getTransform()->update();
		windowChromeShader.use();
		glBindTexture(GL_TEXTURE_2D, 0);
		windowChromeShader.applyShaderUniformMatrix(windowChromeModelUniform, topBarWindowChrome.getTransform()->getMatrix());
		windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(PRIMARY_COL.x, PRIMARY_COL.y, PRIMARY_COL.z));
		topBarWindowChrome.draw();

		glBindTexture(1, closeTexture);
		windowChromeShader.applyShaderUniformMatrix(windowChromeModelUniform, topBarCloseButton.getTransform()->getMatrix());
		if (topBarButtonOver == 3)
			windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(SECONDARY_COL.x, SECONDARY_COL.y, SECONDARY_COL.z));
		topBarCloseButton.draw();
		windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(PRIMARY_COL.x, PRIMARY_COL.y, PRIMARY_COL.z));

		glBindTexture(1, restoreTexture);
		windowChromeShader.applyShaderUniformMatrix(windowChromeModelUniform, topBarRestoreDownMaximizeButton.getTransform()->getMatrix());
		if (topBarButtonOver == 2)
			windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(SECONDARY_COL.x, SECONDARY_COL.y, SECONDARY_COL.z));
		topBarRestoreDownMaximizeButton.draw();
		windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(PRIMARY_COL.x, PRIMARY_COL.y, PRIMARY_COL.z));

		glBindTexture(1, minimizeTexture);
		windowChromeShader.applyShaderUniformMatrix(windowChromeModelUniform, topBarMinimizeButton.getTransform()->getMatrix());
		if (topBarButtonOver == 1)
			windowChromeShader.applyShaderVector3(windowChromeColourUniform, glm::vec3(SECONDARY_COL.x, SECONDARY_COL.y, SECONDARY_COL.z));
		topBarMinimizeButton.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
void drawLine(int x0, int y0, int x1, int y1)
{
	if (abs(y1 - y0) < abs(x1 - x0))
	{
		if (x0 > x1)
			plotLineLow(x1, y1, x0, y0);
		else
			plotLineLow(x0, y0, x1, y1);
	}
	else
	{
		if (y0 > y1)
			plotLineHigh(x1, y1, x0, y0);
		else
			plotLineHigh(x0, y0, x1, y1);
	}
}

void plotLineLow(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int	dy = y1 - y0;
	int	yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y0;

	for (int x = x0; x < x1; x++)
	{
		/*texData.setTexelColor(255, 255, 255, 255, x, y);
		texData.setTexelColor(255, 255, 255, 255, x + 1, y);
		texData.setTexelColor(255, 255, 255, 255, x, y + 1);
		texData.setTexelColor(255, 255, 255, 255, x + 1, y + 1);*/
		if (D > 0)
		{
			y = y + yi;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;
	}
}

void plotLineHigh(int x0, int y0, int x1, int y1)
{
	int dx = x1 - x0;
	int	dy = y1 - y0;
	int	xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x0;

	for (int y = y0; y < y1; y++)
	{
		/*texData.setTexelColor(255, 255, 255, 255, x, y);
		texData.setTexelColor(255, 255, 255, 255, x + 1, y);
		texData.setTexelColor(255, 255, 255, 255, x, y + 1);
		texData.setTexelColor(255, 255, 255, 255, x + 1, y + 1);*/
		if (D > 0)
		{
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}

void SetPixelValues(int startX, int width, int startY, int height, double xpos, double ypos, BrushData brushData)
{
	ColourData colData;
	float rVal;
	float distance;
	glm::vec2 pixelPos(xpos, ypos);
	float px_width = texData.getWidth();
	float px_height = texData.getHeight();
	for (int i = startX; i < width; i++)
	{
		for (int j = startY; j < height; j++)
		{
			colData = texData.getTexelColor(i, j);
			rVal = colData.getColourIn_0_1_Range().r;
			distance = glm::distance(pixelPos, glm::vec2((double)i / px_width, (double)j / px_height));
			if (distance < brushData.brushScale)
			{
				distance = (1.0f - (distance / brushData.brushScale)) * brushData.brushOffset;
				distance = glm::clamp(distance, 0.0f, 1.0f) * brushData.brushStrength;
				rVal = rVal + distance * ((brushData.heightMapPositiveDir ? brushData.brushMaxHeight : brushData.brushMinHeight) - rVal);
				ColourData col(rVal, rVal, rVal, 1.0f);
				texData.setTexelColor(col, i, j);
			}
		}
	}
}

bool isKeyPressed(int key)
{
	int state = glfwGetKey(window, key);
	if (state == GLFW_PRESS)
		return true;
	return false;
}

bool isKeyReleased(int key)
{
	int state = glfwGetKey(window, key);
	if (state == GLFW_RELEASE)
		return true;
	return false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	yUiScale = TOP_BAR_HEIGHT / windowHeight;

	xUiButtonScale = TOP_BAR_BUTTON_SIDE_SIZE / windowWidth;
	yUiButtonScale = TOP_BAR_BUTTON_SIDE_SIZE / windowHeight;
	xGapButtonGapSize = TOP_BAR_BUTTON_X_GAP_SIZE / windowWidth;
	yGapButtonGapSize = TOP_BAR_BUTTON_Y_GAP_SIZE / windowHeight;

	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoomLevel += zoomLevel * 0.1f * yoffset;
}

bool saveScreenshot(std::string filename, int xOff, int yOff, int w, int h)
{
	//This prevents the images getting padded 
	// when the width multiplied by 3 is not a multiple of 4
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = w * h * 3;
	// First let's create our buffer, 3 channels per Pixel
	char* dataBuffer = (char*)malloc(nSize * sizeof(char));

	if (!dataBuffer) return false;
	glReadPixels((GLint)xOff, (GLint)yOff, (GLint)w, (GLint)h, GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);

	//Now the file creation
#pragma warning(suppress : 4996)
	FILE *filePtr = fopen(filename.c_str(), "wb");
	if (!filePtr) return false;

	unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	unsigned char header[6] = { w % 256,w / 256, h % 256, h / 256, 24, 0 };
	// We write the headers
	fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	fwrite(header, sizeof(unsigned char), 6, filePtr);
	// And finally our image data
	fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	fclose(filePtr);
	free(dataBuffer);
	return true;
}
void CustomColourImGuiTheme(ImGuiStyle* dst)
{
	ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = WHITE;
	colors[ImGuiCol_TextDisabled] = DARK_GREY;
	colors[ImGuiCol_WindowBg] = PRIMARY_COL;
	colors[ImGuiCol_ChildBg] = DARK_GREY;
	colors[ImGuiCol_PopupBg] = DARK_GREY;
	colors[ImGuiCol_Border] = ACCENT_COL;
	colors[ImGuiCol_BorderShadow] = DARK_GREY;
	colors[ImGuiCol_FrameBg] = ACCENT_COL;
	colors[ImGuiCol_FrameBgHovered] = PRIMARY_COL;
	colors[ImGuiCol_FrameBgActive] = SECONDARY_COL;
	colors[ImGuiCol_TitleBg] = ACCENT_COL;
	colors[ImGuiCol_TitleBgActive] = SECONDARY_COL;
	colors[ImGuiCol_TitleBgCollapsed] = ACCENT_COL;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.0f, 0.26f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ACCENT_COL;
	colors[ImGuiCol_ScrollbarGrab] = SECONDARY_COL;
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.59f, 0.59f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = SECONDARY_COL;
	colors[ImGuiCol_SliderGrab] = ACCENT_COL;
	colors[ImGuiCol_SliderGrabActive] = ACCENT_COL;
	colors[ImGuiCol_Button] = SECONDARY_COL;
	colors[ImGuiCol_ButtonHovered] = PRIMARY_COL;
	colors[ImGuiCol_ButtonActive] = ACCENT_COL;
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.70f, 0.46f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.20f, 0.45f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
}