#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(255.f, -90.f, 335.f), //Position
		vector3(255.f, 70.f, -20.f),	//Target
		AXIS_Y);					//Up
	//m_pCameraMngr->SetFOV();
	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	m_uOctantLevels = 3;
	m_pEntityMngr->Update();
	m_pRoot = new MyOctree(m_uOctantLevels, 5);

	m_platform = new MyRigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateCylinder(2.f, 30.f, 10, C_BLUE))->GetVertexList());
	m_ball = new MyEntity("BreakoutBrick\\brick.obj", "Ball");//new MyRigidBody(m_pMeshMngr->GetMesh(m_pMeshMngr->GenerateIcoSphere(3.f, 15, C_BLACK))->GetVertexList());
	m_ball->AddDimension(0);

	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	uint uInstances = 2500;
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	uInstances = nSquare * nSquare;
	m_uDimentionCount = nSquare;
	uint uIndex = 0;

	float xPosition = 0;
	float yPosition = 0;
	float zPosition = 0;
	
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			m_pEntityMngr->AddEntity("BreakoutBrick\\brick.obj", "Brick1");
			vector3 v3Position = vector3(xPosition, yPosition, zPosition);
			matrix4 m4Position = glm::translate(v3Position) * glm::scale(vector3(2.f));
			//std::cout << "x:" << v3Position.x << " y:" << v3Position.y << " z:" << v3Position.z << std::endl;
			m_pEntityMngr->SetModelMatrix(m4Position);
			m_pEntityMngr->AddDimension(-1, j);
			m_vDimentions.push_back(j);
			//++uIndex;

			/*if (v3Position.x < 0.0f)
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
			}*/

			xPosition += 10.5f;
		}

		xPosition = 0;
		yPosition += 5.5f;
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
	if(/*m_bToggle && */m_pEntityMngr->GetEntityCount() > 0)
		m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	//ChangeBallDimention();
	ToggleSpacial();

	m_pRoot->Display(m_uOctantID, C_YELLOW);

	matrix4 model;
	matrix4 rotation = glm::rotate(IDENTITY_M4, glm::radians(90.f), AXIS_Z);
	model = glm::translate(IDENTITY_M4, vector3(m_playerMovement, -40.f, 0.f)) * rotation;

	m_platform->SetModelMatrix(model);
	m_pMeshMngr->AddCylinderToRenderList(model, C_BLUE);
	
	static float bounceAngle = (glm::dot(m_ball->GetRigidBody()->GetCenterGlobal(), m_platform->GetCenterGlobal()) - 2000.f) / 2000.f;

	if (m_isSphere)//If the sphere is active, let it move in a straight line until Y = 100, then do not render it anymore
	{
		if (m_verticalBounce)
			m_spherePosY -= 1.5f;
		else
			m_spherePosY += 1.5f;
		
		if (m_horizontalBounce)
			m_spherePosX -= bounceAngle;
		else
			m_spherePosX += bounceAngle;

		matrix4 sphereModel = glm::translate(IDENTITY_M4, vector3(m_spherePosX, m_spherePosY, 0.f));
		m_ball->SetModelMatrix(sphereModel);
		m_ball->AddToRenderList();

		if (m_spherePosY >= 352.f)
			m_verticalBounce = true;

		if (m_spherePosX >= 525.f)
			m_horizontalBounce = false;
		else if (m_spherePosX <= 0.f)
			m_horizontalBounce = true;

		if (m_platform->IsColliding(m_ball->GetRigidBody()))
		{
			m_verticalBounce = false;
		}

		if (m_spherePosY <= -80.f)//Do not render the sphere and reset its values
		{
			m_isSphere = false;
			m_verticalBounce = false;
			m_horizontalBounce = false;
			m_spherePosY = -35.f;
		}

		//Check if the ball is hitting with the brick, if it gets hit, set the brick's attribute of being hit by true
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			//if (m_ball->SharesDimension(m_pEntityMngr->GetEntity(i)))
			//{
				if (m_ball->GetRigidBody()->IsColliding(m_pEntityMngr->GetEntity(i)->GetRigidBody()))
				{
					//set the ball directional attribute as well as the brick entity's hit attribute

					if (m_pEntityMngr->GetEntity(i)->getHasBeenHit() == false)
					{
						m_pEntityMngr->GetEntity(i)->setHasBeenHit(true);
						m_verticalBounce = !m_verticalBounce;
					}
					
					break;
				}
			//}
			
		}
	}
	
	//Decreases the brick's y coordinate if it has been hit by the ball once before
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		//getting the center coordinate and use it to translate the model matrix of the brick
		vector3 temp = m_pEntityMngr->GetEntity(i)->GetRigidBody()->GetCenterGlobal();
		matrix4 brickModel = glm::translate(IDENTITY_M4, vector3(temp.x, temp.y - 3.0f, temp.z));

		if (m_pEntityMngr->GetEntity(i)->getHasBeenHit() == true)
		{
			//Update the model matrix of the brick
			m_pEntityMngr->GetEntity(i)->SetModelMatrix(brickModel);
		}

		//If the brick reaches certain y coordinate, it gets deleted
		if (temp.y <= -80.0f)
		{
			m_pEntityMngr->RemoveEntity(i);
		}
	}

	m_v3PrevBall = m_ball->GetRigidBody()->GetCenterGlobal();

}

void Application::ChangeBallDimention() {
	if (m_bToggle)
	{
		m_ball->ClearDimensionSet();

		for (uint i = 0; i < m_uDimentionCount; i++)
		{
			m_ball->AddDimension(i);

		}
		std::cout << "off" << std::endl;
	}
	else
	{
		if ((m_v3PrevBall - m_ball->GetRigidBody()->GetCenterGlobal()).length() >= 5.4f)
		{
			if(m_verticalBounce)
				m_uPrevBallDimention--;
			else
				m_uPrevBallDimention++;

			m_ball->ClearDimensionSet();
			m_ball->AddDimension(m_uPrevBallDimention);
		}
		/*for (int i = 0; i < 33 * 6; i++)
		{
			if (m_ball->GetRigidBody()->GetCenterGlobal().y + m_ball->GetRigidBody()->GetHalfWidth().y >= 6 * i)
			{
				m_ball->ClearDimensionSet();
				if (i == 0)
				{
					m_ball->AddDimension(0);

				}
				else m_ball->AddDimension(33 * 6 % i);
			}
		}*/
		std::cout << "on" << std::endl;

	}
	//std::cout << "is in correct dimention: " << m_ball->IsInDimension(m_uPrevBallDimention) << std::endl;
}

void Simplex::Application::ToggleSpacial()
{
	if (m_bToggle){
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			m_pEntityMngr->GetEntity(i)->ClearDimensionSet();
			m_pEntityMngr->GetEntity(i)->AddDimension(0);
		}
	}
	else
	{
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
		{
			m_pEntityMngr->GetEntity(i)->ClearDimensionSet();
			m_pEntityMngr->GetEntity(i)->AddDimension(m_vDimentions[i]);
		}
	}
}

void Application::Display(void)
{

	// Clear the screen
	ClearScreen();

	//display octree
	m_pRoot->Display(C_YELLOW);
	
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