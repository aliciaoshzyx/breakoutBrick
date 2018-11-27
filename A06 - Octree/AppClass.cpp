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

	//bricks
	m_brick1 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick2 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick3 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick4 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick5 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());

	m_brick6 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick7 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick8 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick9 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick10 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());

	m_brick11 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick12 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick13 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick14 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());
	m_brick15 = new RigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCube(1.f, C_RED))->GetVertexList());

	m_bricks.push_back(m_brick1);
	m_bricks.push_back(m_brick2);
	m_bricks.push_back(m_brick3);
	m_bricks.push_back(m_brick4);
	m_bricks.push_back(m_brick5);

	m_bricks.push_back(m_brick6);
	m_bricks.push_back(m_brick7);
	m_bricks.push_back(m_brick8);
	m_bricks.push_back(m_brick9);
	m_bricks.push_back(m_brick10);

	m_bricks.push_back(m_brick11);
	m_bricks.push_back(m_brick12);
	m_bricks.push_back(m_brick13);
	m_bricks.push_back(m_brick14);
	m_bricks.push_back(m_brick15);
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

	//int index = 0;
	//float x = -22.f;
	////bricks
	//for (size_t i = 0; i < 5; i++)
	//{
	//	float y = 20.f;

	//	for (size_t j = 0; j < 3; j++)
	//	{
	//		
	//		if (m_bricks[index])
	//		{
	//			m_bricks[i]->SetModelMatrix(glm::translate(vector3(x, y, 0.f)) * glm::scale(vector3(10.0f)));
	//			m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(x, y, 0.f)) * glm::scale(vector3(10.0f)), C_BROWN);
	//		}
	//		index++;

	//		y += 11.f;
	//	}
	//	
	//	x += 11.f;
	//}
	
	m_brick1->SetModelMatrix(glm::translate(vector3(0, 20, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(0, 20.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick2->SetModelMatrix(glm::translate(vector3(-11, 20.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-11, 20.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick3->SetModelMatrix(glm::translate(vector3(11, 20.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(11, 20.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick4->SetModelMatrix(glm::translate(vector3(-22, 20.f, -100.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-22, 20.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick5->SetModelMatrix(glm::translate(vector3(22, 20.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(22, 20.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	
	m_brick6->SetModelMatrix(glm::translate(vector3(0, 31.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(0, 31.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick7->SetModelMatrix(glm::translate(vector3(-11, 31.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-11, 31.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick8->SetModelMatrix(glm::translate(vector3(11, 31.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(11, 31.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick9->SetModelMatrix(glm::translate(vector3(-22, 31.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-22, 31.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick10->SetModelMatrix(glm::translate(vector3(22, 31.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(22, 31.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	
	m_brick11->SetModelMatrix(glm::translate(vector3(0, 42.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(0, 42.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick12->SetModelMatrix(glm::translate(vector3(-11, 42.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-11, 42.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick13->SetModelMatrix(glm::translate(vector3(11, 42.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(11, 42.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick14->SetModelMatrix(glm::translate(vector3(-22, 42.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-22, 42.f, 0.0f)) * glm::scale(vector3(10.0f)), C_BROWN);
	m_brick15->SetModelMatrix(glm::translate(vector3(22, 42.f, 0.0f)) * glm::scale(vector3(10.0f)));
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(22, 42.f, 0.f)) * glm::scale(vector3(10.0f)), C_BROWN);

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

		for (size_t i = 0; i < m_bricks.size(); i++)
		{
			if (m_bricks[i])
			{
				if (m_ball->IsColliding(m_bricks[i]))
				{
					m_bounceReverse = true;
					SafeDelete(m_bricks[i]);
					m_bricks.erase(m_bricks.begin() + (i - 1));
				}
			}
		}

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

	m_bricks.clear();

	SafeDelete(m_brick1);
	SafeDelete(m_brick2);
	SafeDelete(m_brick3);
	SafeDelete(m_brick4);
	SafeDelete(m_brick5);

	SafeDelete(m_brick6);
	SafeDelete(m_brick7);
	SafeDelete(m_brick8);
	SafeDelete(m_brick9);
	SafeDelete(m_brick10);

	SafeDelete(m_brick11);
	SafeDelete(m_brick12);
	SafeDelete(m_brick13);
	SafeDelete(m_brick14);
	SafeDelete(m_brick15);
}