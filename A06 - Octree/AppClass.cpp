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

	m_platform = new MyRigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCylinder(2.f, 30.f, 10, C_BLUE))->GetVertexList());
	m_ball = new MyRigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateIcoSphere(3.f, 15, C_BLACK))->GetVertexList());

	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	uint uInstances = 20;
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	uInstances = nSquare * nSquare;
	uint uIndex = 0;

	uint xPosition = 0;
	uint yPosition = 0;
	uint zPosition = 0;
	
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			m_pEntityMngr->AddEntity("BreakoutBrick\\brick.obj", "Brick1");
			vector3 v3Position = vector3(xPosition, yPosition, zPosition);
			matrix4 m4Position = glm::translate(v3Position) * glm::scale(vector3(2.f));
			m_pEntityMngr->SetModelMatrix(m4Position);
			//m_pEntityMngr->AddDimension(-1, uIndex);
			//++uIndex;

			if (v3Position.x < 0.0f)
			{
				if (v3Position.x < -17.0f)
					m_pEntityMngr->AddDimension(-1, 1);
				else
					m_pEntityMngr->AddDimension(-1, 2);
			}
			else if (v3Position.x > 0.0f)
			{
				if (v3Position.x > 17.0f)
					m_pEntityMngr->AddDimension(-1, 3);
				else
					m_pEntityMngr->AddDimension(-1, 4);
			}

			xPosition += 14;
		}

		xPosition = 0;
		yPosition += 10;
	}

	m_pEntityMngr->Update();
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
	if(m_pEntityMngr->GetEntityCount() > 0)
		m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	//m_pRoot->Display(m_uOctantID, C_YELLOW);

	matrix4 model;
	matrix4 rotation = glm::rotate(IDENTITY_M4, glm::radians(90.f), AXIS_Z);
	model = glm::translate(IDENTITY_M4, vector3(m_playerMovement, -40.f, 0.f)) * rotation;

	m_platform->SetModelMatrix(model);
	m_pMeshMngr->AddCylinderToRenderList(model, C_BLUE);
	
	static float bounceAngle = (glm::dot(m_ball->GetCenterGlobal(), m_platform->GetCenterGlobal()) - 3000.f) / 2000.f;

	if (m_isSphere)//If the sphere is active, let it move in a straight line until Y = 100, then do not render it anymore
	{
		if (m_verticalBounce)
			m_spherePosY -= 1.f;
		else
			m_spherePosY += 1.f;
		
		if (m_horizontalBounce)
			m_spherePosX -= bounceAngle;
		else
			m_spherePosX += bounceAngle;

		matrix4 sphereModel = glm::translate(IDENTITY_M4, vector3(m_spherePosX, m_spherePosY, 0.f));
		m_ball->SetModelMatrix(sphereModel);
		m_pMeshMngr->AddIcoSphereToRenderList(sphereModel, C_BLACK);

		if (m_spherePosY >= 55.f)
			m_verticalBounce = true;

		if (m_spherePosX >= 95.f)
			m_horizontalBounce = false;
		else if (m_spherePosX <= -95.f)
			m_horizontalBounce = true;

		if (m_platform->IsColliding(m_ball))
		{
			m_verticalBounce = false;
		}

		if (m_spherePosY <= -60.f)//Do not render the sphere and reset its values
		{
			m_isSphere = false;
			m_verticalBounce = false;
			m_horizontalBounce = false;
			m_spherePosY = -35.f;
		}

		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			if (m_ball->IsColliding(m_pEntityMngr->GetEntity(i)->GetRigidBody()))
			{
				m_pEntityMngr->RemoveEntity(i);
				m_verticalBounce = !m_verticalBounce;
			}
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