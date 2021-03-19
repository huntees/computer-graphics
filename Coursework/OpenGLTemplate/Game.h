#pragma once

#include "Common.h"
#include "GameWindow.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCube;
class CCatmullRom;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CFreeTypeFont *m_pFtFont;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pFighterMesh;
	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CCube* m_pCube;
	CCatmullRom* m_pCatmullRom;
	COpenAssetImportMesh* m_pCity;
	COpenAssetImportMesh* m_pCenterCity;
	COpenAssetImportMesh* m_pDowntown;

	COpenAssetImportMesh* m_pStarship;


	COpenAssetImportMesh* m_pMan;

	void HandleMovement();

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;

	glm::vec3 p0 = glm::vec3(-500, 10, -200);
	glm::vec3 p1 = glm::vec3(0, 10, -200);
	glm::vec3 p2 = glm::vec3(0, 10, 200);
	glm::vec3 p3 = glm::vec3(-500, 10, 200);


public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;

	void RenderLights(CShaderProgram* pSpotlightProgram, glm::mat4 viewMatrix, glm::mat3 viewNormalMatrix);

	float m_t;
	glm::vec3 m_spaceShipPosition;
	glm::mat4 m_spaceShipOrientation;

	float m_routeWidth;
	float m_currentDistance;
	float m_cameraSpeed;

	glm::vec3 m_starshipPosition;
	glm::vec3 m_starshipFrontLightPosition;
	glm::vec3 m_starshipBackLightPosition;
	glm::mat4 m_starshipOrientation;
	float m_starshipStrafe;
	glm::vec3 m_starship_B;

	bool m_headlightOn;

	int m_cameraMode;
	bool m_freeview;

	bool m_showPath;
	const float m_topSpeed = 0.2f;

	bool m_fogOn;

	//light colours
	glm::vec3 headlightColour;
	const glm::vec3 white = glm::vec3(5.f);
	const glm::vec3 red = glm::vec3(1, 0.050, 0.2) * 5.f;
	const glm::vec3 green = glm::vec3(0.450, 0.941, 0.078) * 5.f;
	const glm::vec3 blue = glm::vec3(0.078, 0.262, 0.941) * 5.f;
	const glm::vec3 teal = glm::vec3(0.f, 5.f, 5.f);
	const glm::vec3 aqua = glm::vec3(0.043f, 0.796f, 0.486f) * 5.f;
	const glm::vec3 pink = glm::vec3(1, 0.321, 0.745) * 8.f;
	const glm::vec3 purple = glm::vec3(0.450, 0.141, 0.898) * 5.f;
	const glm::vec3 yellow = glm::vec3(0.996, 0.803, 0.403) * 5.f;
	const glm::vec3 magenta = glm::vec3(1, 0, 1) * 4.f;
	
};
