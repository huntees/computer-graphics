/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "Cube.h"
#include "CatmullRom.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pFighterMesh = NULL;
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCube = NULL;
	m_pCatmullRom = NULL;
	m_pCity = NULL;
	m_pCenterCity = NULL;
	m_pDowntown = NULL;
	
	m_pStarship = NULL;

	m_pMan = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;

	m_routeWidth = 70.f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.0f;
	m_cameraSpeed = 0.0f;

	m_cameraMode = 3;
	m_freeview = false;
	m_showPath = true;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pFighterMesh;
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCube;
	delete m_pCity;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pFighterMesh = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;

	m_pCity = new COpenAssetImportMesh;
	m_pCenterCity = new COpenAssetImportMesh;
	m_pDowntown = new COpenAssetImportMesh;

	m_pStarship = new COpenAssetImportMesh;

	m_pMan = new COpenAssetImportMesh;

	m_pCube = new CCube;
	m_pCatmullRom = new CCatmullRom;

	m_t = 0;
	m_spaceShipPosition = glm::vec3(0.f);
	m_spaceShipOrientation = glm::mat4(1);

	m_routeWidth = 70.f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.0f;

	m_cameraMode = 3;
	m_freeview = false;
	m_showPath = true;

	m_starshipPosition = glm::vec3(0.f);
	m_starshipOrientation = glm::mat4(1);

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("spotlightShader.vert");
	sShaderFileNames.push_back("spotlightShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the spotlight shader program
	CShaderProgram* pSpotlightProgram = new CShaderProgram;
	pSpotlightProgram->CreateProgram();
	pSpotlightProgram->AddShaderToProgram(&shShaders[4]);
	pSpotlightProgram->AddShaderToProgram(&shShaders[5]);
	pSpotlightProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSpotlightProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pFighterMesh->Load("resources\\models\\Fighter\\fighter1.obj");
	m_pCity->Load("resources\\models\\City\\City.obj");
	m_pCenterCity->Load("resources\\models\\CenterCity\\CenterCity.obj");
	m_pDowntown->Load("resources\\models\\Downtown\\downtown.obj");

	m_pStarship->Load("resources\\models\\Starship\\Starship.obj");

	m_pMan->Load("resources\\models\\Man\\man.obj");


	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	m_pCube->Create("resources\\textures\\Tile41a.jpg");

	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves(m_routeWidth);
	m_pCatmullRom->CreateTrack("resources\\textures\\grid.png");
}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(0.8f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(0.8f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(0.8f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	//modelViewMatrixStack.Push();
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pPlanarTerrain->Render();
	//modelViewMatrixStack.Pop();

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Switch to the spotlight program
	CShaderProgram* pSpotlightProgram = (*m_pShaderPrograms)[2];
	pSpotlightProgram->UseProgram();
	pSpotlightProgram->SetUniform("sampler0", 0);

	pSpotlightProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// world light
	pSpotlightProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("light1.La", glm::vec3(0.2f));
	pSpotlightProgram->SetUniform("light1.Ld", glm::vec3(0.2f));
	pSpotlightProgram->SetUniform("light1.Ls", glm::vec3(0.2f));
	pSpotlightProgram->SetUniform("material1.shininess", 15.0f);
	pSpotlightProgram->SetUniform("material1.Ma", glm::vec3(0.5f));
	pSpotlightProgram->SetUniform("material1.Md", glm::vec3(0.5f));
	pSpotlightProgram->SetUniform("material1.Ms", glm::vec3(1.0f));

	// Set light and materials in spotlight programme
	//glm::vec4 spotlightPosition1(m_pCamera->GetPosition(), 1);
	glm::vec4 spotlightPosition1(23, 88, 438, 1);
	pSpotlightProgram->SetUniform("spotlight1.position", viewMatrix * spotlightPosition1); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight1.Ld", glm::vec3(4.0f, 0.f, 4.f));			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight1.Ls", glm::vec3(4.0f, 0.f, 4.f));			// Specular colour of light

	pSpotlightProgram->SetUniform("spotlight1.direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, -1)));
	pSpotlightProgram->SetUniform("spotlight1.exponent", 20.0f);
	pSpotlightProgram->SetUniform("spotlight1.cutoff", 30.0f);

	//glm::vec4 spotlightPosition2(200, 88, 438, 1);
	glm::vec4 spotlightPosition2(m_pCamera->GetPosition() , 1);
	pSpotlightProgram->SetUniform("spotlight2.position", viewMatrix* spotlightPosition2); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight2.Ld", glm::vec3(0.0f, 0.f, 4.f));			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight2.Ls", glm::vec3(0.0f, 0.f, 4.f));			// Specular colour of light

	pSpotlightProgram->SetUniform("spotlight2.direction", glm::normalize(viewNormalMatrix* glm::vec3(0, -1, 0)));
	pSpotlightProgram->SetUniform("spotlight2.exponent", 20.0f);
	pSpotlightProgram->SetUniform("spotlight2.cutoff", 30.0f);



	//// Render the horse 
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(180.0f));
	//	modelViewMatrixStack.Scale(2.5f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pHorseMesh->Render();
	//modelViewMatrixStack.Pop();	
	
	// Render the fighter 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_spaceShipPosition);
		modelViewMatrixStack *= m_spaceShipOrientation;
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		modelViewMatrixStack.Scale(1.f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pFighterMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Starship 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_starshipPosition);
		modelViewMatrixStack *= m_starshipOrientation;
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		modelViewMatrixStack.Scale(1.f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pStarship->Render();
	modelViewMatrixStack.Pop();
	
	// Render the Man
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(11.0f, 56.f, -840.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.f));
		modelViewMatrixStack.Scale(0.55f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pMan->Render();
	modelViewMatrixStack.Pop();

	//// Render the cube 
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 0.0f));
	//	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.0f));
	//	modelViewMatrixStack.Scale(2.f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pCube->Render();
	//modelViewMatrixStack.Pop();


	//// Render the barrel 
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(100.0f, 0.0f, 0.0f));
	//	modelViewMatrixStack.Scale(5.0f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	m_pBarrelMesh->Render();
	//modelViewMatrixStack.Pop();
	

	//// Render the sphere
	//modelViewMatrixStack.Push();
	//	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
	//	modelViewMatrixStack.Scale(2.0f);
	//	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	//	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	//	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//	//pMainProgram->SetUniform("bUseTexture", false);
	//	m_pSphere->Render();
	//modelViewMatrixStack.Pop();

	glDisable(GL_CULL_FACE);
	// Render the City 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
		modelViewMatrixStack.Scale(1.f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCity->Render();
	modelViewMatrixStack.Pop();

	// Render the Center City 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(1050.0f, -54.f, -450.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(60.f));
		modelViewMatrixStack.Scale(1.2f, 2.5f, 1.2f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCenterCity->Render();
	modelViewMatrixStack.Pop();

	// Render the Downtown 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(120.f, 0.f, 290.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(10.f));
		modelViewMatrixStack.Scale(1.7f, 3.5f, 1.7f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pDowntown->Render();
	modelViewMatrixStack.Pop();

	if (m_showPath) {

		//// Render Catmull Spline Route
		//modelViewMatrixStack.Push();
		//pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		//pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pCatmullRom->RenderCentreline();
		//modelViewMatrixStack.Pop();

		//// Render Catmull Spline Route offsets
		//modelViewMatrixStack.Push();
		//pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		//pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pCatmullRom->RenderOffsetCurves();
		//modelViewMatrixStack.Pop();

		// Render Catmull Spline Route Track
		modelViewMatrixStack.Push();
		pSpotlightProgram->SetUniform("light1.La", glm::vec3(1.f));
		pSpotlightProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();

	}
		
	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());		

}

// Update method runs repeatedly with the Render method
void Game::Update() 
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);

	m_currentDistance += m_cameraSpeed * m_dt;

	glm::vec3 p;
	glm::vec3 p_y;
	m_pCatmullRom->Sample(m_currentDistance, p, p_y);

	glm::vec3 pNext;
	m_pCatmullRom->Sample(m_currentDistance + 1.0f, pNext);

	glm::vec3 cam_T = glm::normalize(pNext - p); //(z axis)
	glm::vec3 cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	glm::vec3 cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), m_starshipStrafe, cam_T);

	if (!m_freeview) {
		if (m_cameraMode == 1) {
			m_pCamera->Set(p + (5.f * cam_B) + (4.f * cam_T) + (m_starshipStrafe * cam_N), p + (500.0f * cam_T), p_y);
		}
		else if (m_cameraMode == 2) {
			m_pCamera->Set(p + (5.f * cam_B) + (1.5f * cam_T) + (m_starshipStrafe * cam_N), p + (500.0f * cam_T), p_y);
		}
		else if (m_cameraMode == 3) {
			m_pCamera->Set(p + (13.f * cam_B) + (-30.f * cam_T) + ((m_starshipStrafe * 0.7f)  * cam_N), p + (200.0f * cam_T), p_y);
		}
	}

	m_starshipPosition = p + (2.9f * cam_B) + (m_starshipStrafe * cam_N);
	m_starshipOrientation = glm::mat4(glm::mat3(cam_T, cam_B, cam_N)); 

	HandleMovement();

	m_pAudio->Update();



	m_t += 0.001f * (float)m_dt;
	float r = 75.0f;
	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);
	m_spaceShipPosition = r * cos(m_t) * x + 50.0f * y + r * sin(m_t) * z;

	glm::vec3 T = glm::normalize(-r * sin(m_t) * x + r * cos(m_t) * z);
	glm::vec3 N = glm::normalize(glm::cross(T, y));
	glm::vec3 B = glm::normalize(glm::cross(N, T));
	
	m_spaceShipOrientation = glm::mat4(glm::mat3(T, B, N));
}

void Game::HandleMovement() {
	if (m_cameraSpeed > 0.f) {
		m_cameraSpeed -= 0.00004 * m_dt;

		if (m_cameraSpeed < 0.f) {
			m_cameraSpeed = 0.f;
		}
	}

	if (GetKeyState(VK_UP) & 0x80) {
		m_cameraSpeed += 0.0001f * m_dt;
		if (m_cameraSpeed > m_topSpeed) {
			m_cameraSpeed = m_topSpeed;
		}
	}
	else if (GetKeyState(VK_DOWN) & 0x80) {
		m_cameraSpeed -= 0.0001f * m_dt;
		if (m_cameraSpeed < -m_topSpeed) {
			m_cameraSpeed = -m_topSpeed;
		}
	}

	if (GetKeyState(VK_RIGHT) & 0x80) {

		m_starshipStrafe += 0.1f * m_dt;

		if (m_starshipStrafe > m_routeWidth * 0.4) {
			m_starshipStrafe = m_routeWidth * 0.4;
		}
	}
	else if (GetKeyState(VK_LEFT) & 0x80) {

		m_starshipStrafe -= 0.1f * m_dt;

		if (m_starshipStrafe < -m_routeWidth * 0.4) {
			m_starshipStrafe = -m_routeWidth * 0.4;
		}
	}
}

void Game::DisplayFrameRate()
{


	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		m_pFtFont->Render(20, height - 40, 20, "X: %f", m_pCamera->GetPosition().x);
		m_pFtFont->Render(20, height - 60, 20, "Y: %f", m_pCamera->GetPosition().y);
		m_pFtFont->Render(20, height - 80, 20, "Z: %f", m_pCamera->GetPosition().z);
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;
		//case VK_UP:
		//	m_cameraSpeed += 0.01f;
		//	if (m_cameraSpeed > 0.3f) {
		//		m_cameraSpeed = 0.3f;
		//	}
		//	break;
		//case VK_DOWN:
		//	m_cameraSpeed -= 0.01f;
		//	break;

		case 'F':
			m_freeview = !m_freeview;
			break;		
		
		case 'P':
			m_showPath = !m_showPath;
			break;

		case 'C':
			m_cameraMode++;
			if (m_cameraMode > 3) {
				m_cameraMode = 1;
			}
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
