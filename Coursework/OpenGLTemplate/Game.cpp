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
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "Cube.h"
#include "Tetrahedron.h"
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
	m_pTetrahedron = NULL;
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
	m_fogOn = true;
	m_headlightOn = true;
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
	delete m_pTetrahedron;
	delete m_pCity;
	delete m_pCenterCity;
	delete m_pDowntown;

	delete m_pStarship;
	delete m_pTransport;
	delete m_pFreighter;
	delete m_pFlyingCar;
	delete m_pPoliceCar;
	delete m_pPatrolCar;
	delete m_pMan;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
	delete m_pTimer;
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
	m_pTransport = new COpenAssetImportMesh;
	m_pFreighter = new COpenAssetImportMesh;
	m_pFlyingCar = new COpenAssetImportMesh;
	m_pPoliceCar = new COpenAssetImportMesh;
	m_pPatrolCar = new COpenAssetImportMesh;

	m_pMan = new COpenAssetImportMesh;

	m_pCube = new CCube;
	m_pTetrahedron = new CTetrahedron;
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
	m_fogOn = true;
	m_headlightOn = true;

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
	m_pPlanarTerrain->Create("resources\\textures\\", "sea.jpg", 10000.0f, 10000.0f, 50.0f);

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
	m_pTransport->Load("resources\\models\\Transport\\transport.obj");
	m_pFreighter->Load("resources\\models\\Freighter\\freighter.obj");
	m_pFlyingCar->Load("resources\\models\\FlyingCar\\FlyingCar.obj");
	m_pPoliceCar->Load("resources\\models\\PoliceCar\\policecar.obj");
	m_pPatrolCar->Load("resources\\models\\PatrolCar\\PatrolCar.obj");

	m_pMan->Load("resources\\models\\Man\\man.obj");


	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	m_pCube->Create("resources\\textures\\crate.jpg");
	m_pTetrahedron->Create("resources\\textures\\tetrahedron.jpg");

	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves(m_routeWidth);
	m_pCatmullRom->CreateTrack("resources\\textures\\grid.png");

	m_pCatmullRom->Env_CreateCentreline();
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

	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	
	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*

	// Switch to the spotlight program
	CShaderProgram* pSpotlightProgram = (*m_pShaderPrograms)[2];
	pSpotlightProgram->UseProgram();
	pSpotlightProgram->SetUniform("sampler0", 0);

	pSpotlightProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	pSpotlightProgram->SetUniform("fogOn", m_fogOn);

	// world light
	pSpotlightProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("light1.La", glm::vec3(0.1f));
	pSpotlightProgram->SetUniform("light1.Ld", glm::vec3(0.1f));
	pSpotlightProgram->SetUniform("light1.Ls", glm::vec3(0.1f));
	pSpotlightProgram->SetUniform("material1.shininess", 15.0f);
	pSpotlightProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pSpotlightProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pSpotlightProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance

	//render skybox
	pSpotlightProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	modelViewMatrixStack.Push();
		pSpotlightProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pSpotlightProgram->SetUniform("renderSkybox", false);
	modelViewMatrixStack.Pop();


	pSpotlightProgram->SetUniform("material1.Ma", glm::vec3(0.5f));
	pSpotlightProgram->SetUniform("material1.Md", glm::vec3(0.5f));
	pSpotlightProgram->SetUniform("material1.Ms", glm::vec3(1.0f));

	// Render the planar terrain
	modelViewMatrixStack.Push();
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();
	
	// Set light and materials in spotlight programme
	if (m_headlightOn) {
		headlightColour = glm::vec3(1.f);
	}
	else {
		headlightColour = glm::vec3(0.f);
	}

	glm::vec4 starshipFrontLightPosition(m_starshipFrontLightPosition, 1);
	pSpotlightProgram->SetUniform("spotlight[0].position", viewMatrix * starshipFrontLightPosition); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[0].Ld", glm::vec3(headlightColour));			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[0].Ls", glm::vec3(headlightColour));			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[0].direction", glm::normalize(viewNormalMatrix * m_starship_B * glm::vec3(0, 0, -1)));
	pSpotlightProgram->SetUniform("spotlight[0].exponent", 40.f); // the blend between outer circle and environment
	pSpotlightProgram->SetUniform("spotlight[0].cutoff", 15.f); // size of circle

	glm::vec4 pointlightPosition(m_starshipBackLightPosition, 1);
	//glm::vec4 pointlightPosition(m_pCamera->GetPosition() , 1);
	pSpotlightProgram->SetUniform("pointlight.position", viewMatrix* pointlightPosition); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("pointlight.Ld", glm::vec3(1.f, 0.f, 0.f));			// Diffuse colour of light
	pSpotlightProgram->SetUniform("pointlight.Ls", glm::vec3(1.f, 0.f, 0.f));			// Specular colour of light

	RenderLights(pSpotlightProgram, viewMatrix, viewNormalMatrix);


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

	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition, m_EnvStarshipOrientation);
	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition2, m_EnvStarshipOrientation2);
	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition3, m_EnvStarshipOrientation3);
	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition4, m_EnvStarshipOrientation4);
	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition5, m_EnvStarshipOrientation5);
	RenderEnvCars(pSpotlightProgram, modelViewMatrixStack, m_EnvStarshipPosition6, m_EnvStarshipOrientation6);

	// Render the Man
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(11.0f, 56.f, -840.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.f));
		modelViewMatrixStack.Scale(0.55f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pMan->Render();
	modelViewMatrixStack.Pop();

	// Render the cube 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(414.0f, 351.0f, 1746.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.0f));
		modelViewMatrixStack.Scale(2.f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCube->Render();
	modelViewMatrixStack.Pop();

	// Render the tetrahedron 
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(368.0f, 340.0f, 1726.0f));
		modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(0.0f));
		modelViewMatrixStack.Scale(2.f);
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pTetrahedron->Render();
	modelViewMatrixStack.Pop();


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

		// Render the Downtown 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(120.f, 0.f, 290.0f));
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(10.f));
	modelViewMatrixStack.Scale(1.7f, 3.5f, 1.7f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pDowntown->Render();
	modelViewMatrixStack.Pop();

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

	// Render Catmull Spline Route
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
		pSpotlightProgram->SetUniform("renderTrack", true);
		pSpotlightProgram->SetUniform("showTrack", m_showPath);
		pSpotlightProgram->SetUniform("discardTime", m_pathDiscardTime);
		pSpotlightProgram->SetUniform("light1.La", glm::vec3(1.f));
		pSpotlightProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
		pSpotlightProgram->SetUniform("renderTrack", false);
	modelViewMatrixStack.Pop();

		
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

	m_starship_B = cam_B;

	//glm::vec3 up = glm::rotate(glm::vec3(0, 1, 0), m_starshipStrafe, cam_T);

	if (!m_freeview) {
		if (m_cameraMode == 1) {
			m_pCamera->Set(p + (5.f * cam_B) + (4.f * cam_T) + (m_starshipStrafe * cam_N), p + (500.0f * cam_T), p_y);
		}
		else if (m_cameraMode == 2) {
			m_pCamera->Set(p + (5.f * cam_B) + (1.5f * cam_T) + (m_starshipStrafe * cam_N), p + (500.0f * cam_T), p_y);
		}
		else if (m_cameraMode == 3) {
			m_pCamera->Set(p + (13.f * cam_B) + (-30.f * cam_T) + ((m_starshipStrafe * 0.7f) * cam_N), p + (200.0f * cam_T), p_y);
		}
	}

	m_starshipFrontLightPosition = p + (2.9f * cam_B) + (m_starshipStrafe * cam_N) + (2.f * cam_T);
	m_starshipBackLightPosition = p + (3.8f * cam_B) + (m_starshipStrafe * cam_N) + (-9.f * cam_T);
	m_starshipPosition = p + (2.9f * cam_B) + (m_starshipStrafe * cam_N);
	m_starshipOrientation = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));

	HandleEnvShips();

	HandleMovement();

	m_pAudio->Update();

	if (!m_showPath) {	
		if (m_pathDiscardTime < 2.f) {
			m_pathDiscardTime += 0.00035f * (float)m_dt;
		}
	}

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

void Game::HandleEnvShips() {
	m_EnvCurrentDistance += 0.05f * m_dt;

	glm::vec3 p;
	glm::vec3 p_y;
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance, p, p_y);

	glm::vec3 pNext;
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 1.0f, pNext);

	glm::vec3 cam_T = glm::normalize(pNext - p); //(z axis)
	glm::vec3 cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	glm::vec3 cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition = p;
	m_EnvStarshipOrientation = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));

	//2
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 800, p, p_y);
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 801.0f, pNext);

	cam_T = glm::normalize(pNext - p); //(z axis)
	cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition2 = p;
	m_EnvStarshipOrientation2 = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));


	//3
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 1800, p, p_y);
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 1801.0f, pNext);

	cam_T = glm::normalize(pNext - p); //(z axis)
	cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition3 = p;
	m_EnvStarshipOrientation3 = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));

	//4
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 3000, p, p_y);
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 3001.0f, pNext);

	cam_T = glm::normalize(pNext - p); //(z axis)
	cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition4 = p;
	m_EnvStarshipOrientation4 = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));

	//5
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 4800, p, p_y);
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 4801.0f, pNext);

	cam_T = glm::normalize(pNext - p); //(z axis)
	cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition5 = p;
	m_EnvStarshipOrientation5 = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));

	//6
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 5200, p, p_y);
	m_pCatmullRom->Env_Sample(m_EnvCurrentDistance + 5201.0f, pNext);

	cam_T = glm::normalize(pNext - p); //(z axis)
	cam_N = glm::normalize(glm::cross(cam_T, p_y)); //(x axis)
	cam_B = glm::normalize(glm::cross(cam_N, cam_T)); //(y axis)

	m_EnvStarshipPosition6 = p;
	m_EnvStarshipOrientation6 = glm::mat4(glm::mat3(cam_T, cam_B, cam_N));


}

void Game::HandleMovement() {

	//Vehicle Drag
	if (m_cameraSpeed > 0.f) {
		m_cameraSpeed -= 0.00004 * m_dt;

		if (m_cameraSpeed < 0.f) {
			m_cameraSpeed = 0.f;
		}
	}
	else if (m_cameraSpeed < 0.f) {
		m_cameraSpeed += 0.00004 * m_dt;

		if (m_cameraSpeed > 0.f) {
			m_cameraSpeed = 0.f;
		}
	}

	//Movement
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
	int width = dimensions.right - dimensions.left;

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
		m_pFtFont->Render(100, height * 0.1f, 20, "KM/H: %.0f", m_cameraSpeed * 800);
		m_pFtFont->Render(width * 0.48f, height * 0.9f, 20, "Lap: %d", m_pCatmullRom->CurrentLap(m_currentDistance));
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
		
		case VK_F5:
			m_showPath = !m_showPath;
			m_pathDiscardTime = 0;
			break;

		case VK_F6:
			m_fogOn = !m_fogOn;
			break;

		case 'V':
			m_freeview = !m_freeview;
			break;

		case 'F':
			m_headlightOn = !m_headlightOn;
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

void Game::RenderLights(CShaderProgram* pSpotlightProgram, glm::mat4 viewMatrix, glm::mat3 viewNormalMatrix) {
	pSpotlightProgram->SetUniform("spotlight[1].position", viewMatrix * glm::vec4(-1018, 20 - 120, 489, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[1].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[1].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[1].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[1].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[1].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[2].position", viewMatrix * glm::vec4(-1304, 60 - 120, 145, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[2].Ld", white);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[2].Ls", white);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[2].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[2].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[2].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[3].position", viewMatrix * glm::vec4(-1221, 104 - 120, 26, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[3].Ld", white);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[3].Ls", white);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[3].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[3].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[3].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[4].position", viewMatrix * glm::vec4(-1180, 60 - 120, -129, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[4].Ld", white);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[4].Ls", white);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[4].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[4].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[4].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[5].position", viewMatrix * glm::vec4(-779, 20 - 120, -198, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[5].Ld", pink);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[5].Ls", pink);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[5].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[5].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[5].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[6].position", viewMatrix * glm::vec4(-600, 132 - 120, -79, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[6].Ld", green);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[6].Ls", green);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[6].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[6].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[6].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[7].position", viewMatrix * glm::vec4(-462, 245 - 120, -256, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[7].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[7].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[7].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[7].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[7].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[8].position", viewMatrix * glm::vec4(-613, 211 - 120, -461, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[8].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[8].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[8].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[8].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[8].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[9].position", viewMatrix * glm::vec4(-720, 15 - 120, -617, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[9].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[9].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[9].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[9].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[9].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[10].position", viewMatrix * glm::vec4(-344, 10 - 120, -787, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[10].Ld", aqua);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[10].Ls", aqua);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[10].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[10].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[10].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[11].position", viewMatrix * glm::vec4(614, 15 - 120, -1082, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[11].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[11].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[11].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[11].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[11].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[12].position", viewMatrix * glm::vec4(849, 12 - 120, -1204, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[12].Ld", red);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[12].Ls", red);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[12].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[12].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[12].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[13].position", viewMatrix * glm::vec4(1106, 12 - 120, -1179, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[13].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[13].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[13].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[13].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[13].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[14].position", viewMatrix * glm::vec4(1295, 12 - 120, -987, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[14].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[14].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[14].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[14].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[14].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[15].position", viewMatrix * glm::vec4(1627, 24 - 120, -813, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[15].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[15].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[15].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[15].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[15].cutoff", 30.f);
	
	pSpotlightProgram->SetUniform("spotlight[16].position", viewMatrix * glm::vec4(1883, 212 - 120, -732, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[16].Ld", green);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[16].Ls", green);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[16].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[16].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[16].cutoff", 30.f);
	
	pSpotlightProgram->SetUniform("spotlight[17].position", viewMatrix* glm::vec4(906, 17 - 120, 230, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[17].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[17].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[17].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[17].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[17].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[18].position", viewMatrix* glm::vec4(708, 17 - 120, 180, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[18].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[18].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[18].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[18].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[18].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[19].position", viewMatrix * glm::vec4(1806, 231 - 120, -516, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[19].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[19].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[19].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[19].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[19].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[20].position", viewMatrix * glm::vec4(1658, 100 - 120, -353, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[20].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[20].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[20].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[20].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[20].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[21].position", viewMatrix * glm::vec4(1648, 619 - 120, -331, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[21].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[21].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[21].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[21].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[21].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[22].position", viewMatrix * glm::vec4(1968, 13 - 120, -360, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[22].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[22].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[22].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[22].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[22].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[23].position", viewMatrix * glm::vec4(1910, 13 - 120, 23, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[23].Ld", blue * 2.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[23].Ls", blue * 2.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[23].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[23].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[23].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[24].position", viewMatrix * glm::vec4(1858, 15 - 120, 257, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[24].Ld", blue * 2.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[24].Ls", blue * 2.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[24].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[24].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[24].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[25].position", viewMatrix * glm::vec4(1586, 13 - 120, 117, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[25].Ld", aqua);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[25].Ls", aqua);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[25].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[25].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[25].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[26].position", viewMatrix * glm::vec4(1581, 445 - 120, 29, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[26].Ld", yellow * 2.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[26].Ls", yellow * 2.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[26].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[26].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[26].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[27].position", viewMatrix * glm::vec4(1296, 363 - 120, -9, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[27].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[27].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[27].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[27].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[27].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[28].position", viewMatrix * glm::vec4(1272, 15 - 120, 22, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[28].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[28].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[28].direction", glm::normalize(viewNormalMatrix * glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[28].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[28].cutoff", 30.f);

	//left 1
	pSpotlightProgram->SetUniform("spotlight[29].position", viewMatrix* glm::vec4(1101, 15 - 120, 79, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[29].Ld", pink);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[29].Ls", pink);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[29].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[29].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[29].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[30].position", viewMatrix* glm::vec4(888, 143 - 120, 82, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[30].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[30].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[30].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[30].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[30].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[31].position", viewMatrix* glm::vec4(985, 15 - 120, -148, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[31].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[31].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[31].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[31].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[31].cutoff", 30.f);

	//right 1
	pSpotlightProgram->SetUniform("spotlight[32].position", viewMatrix* glm::vec4(1342, 15 - 120, -367, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[32].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[32].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[32].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[32].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[32].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[33].position", viewMatrix* glm::vec4(1109, 15 - 120, -356, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[33].Ld", teal);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[33].Ls", teal);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[33].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[33].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[33].cutoff", 30.f);

	//left 2
	pSpotlightProgram->SetUniform("spotlight[34].position", viewMatrix* glm::vec4(689, 125 - 120, -20, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[34].Ld", green);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[34].Ls", green);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[34].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[34].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[34].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[35].position", viewMatrix* glm::vec4(788, 15 - 120, -253, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[35].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[35].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[35].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[35].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[35].cutoff", 30.f);

	//right 2
	pSpotlightProgram->SetUniform("spotlight[36].position", viewMatrix* glm::vec4(955, 15 - 120, -556, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[36].Ld", red);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[36].Ls", red);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[36].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[36].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[36].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[37].position", viewMatrix* glm::vec4(888, 15 - 120, -453, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[37].Ld", magenta);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[37].Ls", magenta);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[37].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[37].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[37].cutoff", 30.f);

	//left 3
	pSpotlightProgram->SetUniform("spotlight[38].position", viewMatrix* glm::vec4(446, 15 - 120, -116, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[38].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[38].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[38].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[38].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[38].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[39].position", viewMatrix* glm::vec4(597, 15 - 120, -389, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[39].Ld", green);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[39].Ls", green);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[39].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[39].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[39].cutoff", 30.f);

	//right 3
	pSpotlightProgram->SetUniform("spotlight[40].position", viewMatrix* glm::vec4(764, 15 - 120, -668, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[40].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[40].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[40].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[40].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[40].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[41].position", viewMatrix* glm::vec4(700, 15 - 120, -552, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[41].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[41].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[41].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[41].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[41].cutoff", 30.f);

	//left 4
	pSpotlightProgram->SetUniform("spotlight[42].position", viewMatrix* glm::vec4(249, 15 - 120, -276, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[42].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[42].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[42].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[42].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[42].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[43].position", viewMatrix* glm::vec4(391, 15 - 120, -476, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[43].Ld", red);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[43].Ls", red);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[43].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[43].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[43].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[44].position", viewMatrix* glm::vec4(139, 15 - 120, -495, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[44].Ld", magenta);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[44].Ls", magenta);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[44].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[44].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[44].cutoff", 30.f);

	//right 4
	pSpotlightProgram->SetUniform("spotlight[45].position", viewMatrix* glm::vec4(637, 15 - 120, -736, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[45].Ld", red);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[45].Ls", red);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[45].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[45].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[45].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[46].position", viewMatrix* glm::vec4(514, 15 - 120, -672, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[46].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[46].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[46].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[46].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[46].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[47].position", viewMatrix* glm::vec4(353, 15 - 120, -875, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[47].Ld", aqua);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[47].Ls", aqua);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[47].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[47].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[47].cutoff", 30.f);

	//end of center city

	pSpotlightProgram->SetUniform("spotlight[48].position", viewMatrix* glm::vec4(-399, 209 - 120, -481, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[48].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[48].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[48].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[48].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[48].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[49].position", viewMatrix* glm::vec4(-64, 85 - 120, -656, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[49].Ld", red);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[49].Ls", red);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[49].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[49].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[49].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[50].position", viewMatrix* glm::vec4(-83, 268 - 120, -211, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[50].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[50].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[50].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[50].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[50].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[51].position", viewMatrix* glm::vec4(-226, 170 - 120, -241, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[51].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[51].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[51].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[51].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[51].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[52].position", viewMatrix* glm::vec4(-118, 19 - 123, 90, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[52].Ld", aqua * 2.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[52].Ls", aqua * 2.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[52].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[52].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[52].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[53].position", viewMatrix* glm::vec4(25, 19 - 120, 380, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[53].Ld", magenta);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[53].Ls", magenta);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[53].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, -0.5)));
	pSpotlightProgram->SetUniform("spotlight[53].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[53].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[54].position", viewMatrix* glm::vec4(221, 19 - 120, 351, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[54].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[54].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[54].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, -0.5)));
	pSpotlightProgram->SetUniform("spotlight[54].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[54].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[55].position", viewMatrix* glm::vec4(135, 17 -120, 412, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[55].Ld", blue);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[55].Ls", blue);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[55].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[55].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[55].cutoff", 30.f);


	pSpotlightProgram->SetUniform("spotlight[56].position", viewMatrix* glm::vec4(525, 19 - 120, 227, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[56].Ld", red * 3.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[56].Ls", red * 3.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[56].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[56].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[56].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[57].position", viewMatrix* glm::vec4(1332, 10 - 120, 270, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[57].Ld", aqua);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[57].Ls", aqua);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[57].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[57].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[57].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[58].position", viewMatrix* glm::vec4(-758, 19 - 120, 518, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[58].Ld", purple * 3.f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[58].Ls", purple * 3.f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[58].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[58].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[58].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[59].position", viewMatrix* glm::vec4(-815, 0 - 120, 240, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[59].Ld", green * 0.2f);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[59].Ls", green * 0.2f);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[59].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[59].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[59].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[60].position", viewMatrix* glm::vec4(-965, 13 - 120, -89, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[60].Ld", purple);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[60].Ls", purple);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[60].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, 1, 0)));
	pSpotlightProgram->SetUniform("spotlight[60].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[60].cutoff", 30.f);

	pSpotlightProgram->SetUniform("spotlight[61].position", viewMatrix* glm::vec4(-391, 276, 209, 1)); // Light position in eye coordinates
	pSpotlightProgram->SetUniform("spotlight[61].Ld", yellow);			// Diffuse colour of light
	pSpotlightProgram->SetUniform("spotlight[61].Ls", yellow);			// Specular colour of light
	pSpotlightProgram->SetUniform("spotlight[61].direction", glm::normalize(viewNormalMatrix* glm::vec3(0, -1, 0)));
	pSpotlightProgram->SetUniform("spotlight[61].exponent", 5.f);
	pSpotlightProgram->SetUniform("spotlight[61].cutoff", 30.f);


}

void Game::RenderEnvCars(CShaderProgram* pSpotlightProgram, glutil::MatrixStack modelViewMatrixStack, glm::vec3 EnvStarshipPosition, glm::mat4 EnvStarshipOrientation) {

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(0, 0, 0));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pStarship->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(10, 10, 70));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pFreighter->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(-10, -10, 50));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pTransport->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(0, 15, 130));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pTransport->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(0, 0, -130));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pFreighter->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(25, -10, 30));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pFlyingCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(-25, 5, 30));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pFlyingCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(25, 10, -30));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pFlyingCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(-25, 10, -30));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPoliceCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(30, -4, -10));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPoliceCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(-25, 10, -100));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPatrolCar->Render();
	modelViewMatrixStack.Pop();

	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(EnvStarshipPosition + glm::vec3(30, -4, -70));
	modelViewMatrixStack *= EnvStarshipOrientation;
	modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);
	modelViewMatrixStack.Scale(1.f);
	pSpotlightProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pSpotlightProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPatrolCar->Render();
	modelViewMatrixStack.Pop();
}
