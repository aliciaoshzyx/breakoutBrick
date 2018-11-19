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

	m_platform = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCylinder(2.f, 30.f, 10.f, C_BLUE))->GetVertexList());
	m_ball = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateIcoSphere(3.f, 15, C_BLACK))->GetVertexList());
	
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
	model = glm::translate(IDENTITY_M4, vector3(m_playerMovement, -40.f, 0.f)) * rotation;

	m_platform->SetModelMatrix(model);
	m_pMeshMngr->AddCylinderToRenderList(model, C_BLUE);
	

	if (m_isSphere)//If the sphere is active, let it move in a straight line until Y = 100, then do not render it anymore
	{
		if (m_bounceReverse)
			m_spherePosY -= 1.5f;
		else
			m_spherePosY += 1.5f;
		
		matrix4 sphereModel = glm::translate(IDENTITY_M4, vector3(m_spherePosX, m_spherePosY, 0.f));
		m_ball->SetModelMatrix(sphereModel);
		m_pMeshMngr->AddIcoSphereToRenderList(sphereModel, C_BLACK);

		if (m_spherePosY >= 55.f)
			m_bounceReverse = true;
		if (m_platform->IsColliding(m_ball))
			m_bounceReverse = false;

		if (m_spherePosY <= -60.f)//Do not render the sphere and reset its values
		{
			m_isSphere = false;
			m_bounceReverse = false;
			m_spherePosY = -35.f;
		}
	}
	

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
	SafeDelete(m_platform);
	SafeDelete(m_ball);
}