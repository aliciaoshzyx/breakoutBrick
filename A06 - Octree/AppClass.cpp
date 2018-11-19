#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	//m_uOctantLevels = 3;
	//m_pEntityMngr->Update();
	//m_pRoot = new MyOctree(m_uOctantLevels, 5); //getentitycount read access violation???

	m_pMeshMngr->GenerateCylinder(2.f, 30.f, 10, C_BLUE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	////Update Entity Manager
	//m_pEntityMngr->Update();
	//m_pRoot->Display(m_uOctantID, C_YELLOW);

	////Add objects to render list
	//m_pEntityMngr->AddEntityToRenderList(-1, true);

	matrix4 model;
	matrix4 rotation = glm::rotate(IDENTITY_M4, glm::radians(90.f), AXIS_Z);
	model = rotation * glm::translate(IDENTITY_M4, vector3(-40.f, m_playerMovement, 0.f));
	m_pMeshMngr->AddCylinderToRenderList(model, C_BLUE);

}
void Application::Display(void)
{

	// Clear the screen
	ClearScreen();

	//display octree
	//m_pRoot->Display();
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();

	SafeDelete(m_pRoot);
}