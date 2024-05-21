
#include "../entities/Entity.h"
#include "SpacePhysics.h"


void SpacePhysics::EntityCollision(Entity& e1, Entity& e2, glm::vec3 collisionPoint, float deltaTime)
{ 
	if (GlobalVars::ALPHA_PHYSICS)
	{
		vec3 r1 = collisionPoint - vec3(CameraManager::GetInstance()->MVP() * 
			vec4(e1.GetPosition() - CameraManager::GetInstance()->WorldPivot(), 1.0f));
		vec3 r2 = collisionPoint - vec3(CameraManager::GetInstance()->MVP() * 
			vec4(e2.GetPosition() - CameraManager::GetInstance()->WorldPivot(), 1.0f));

		//printf("\nR1 (%f, %f, %f)", r1.x, r1.y, r1.z);
		//printf("\nR2 (%f, %f, %f)", r2.x, r2.y, r2.z);

		// Aplica a conservação do momento linear
		float mass1 = e1.GetMass();
		float inertia1 = (float) e1.GetInertia();
		vec3 linspd1 = e1.GetLinearSpeed();
		vec3 angspd1 = e1.GetAngularSpeed();
		vec3 p1 = e1.GetPosition();

		float mass2 = e2.GetMass();
		float inertia2 = (float) e2.GetInertia();
		vec3 linspd2 = e2.GetLinearSpeed();
		vec3 angspd2 = e2.GetAngularSpeed();
		vec3 p2 = e2.GetPosition();

		//Massa total
		float masst = mass1 + mass2;
		float inertiat = inertia1 + inertia2;

		r1.z = 0;
		r2.z = 0;

		//Normalizar o vetor direção da colisão
		vec3 rn1 = glm::normalize(r1);
		vec3 rn2 = glm::normalize(r2);

		//Cria o fator de separação.
		vec3 separationVec1 = 0.1f * rn1 * mass2 / masst; // Essa constante precisa variar de a cordo com a profundidade
		vec3 separationVec2 = 0.1f * rn2 * mass1 / masst; // da colisão.
		//vec3 separationVecRot1 = r1 * 0.05f; // Essa constante precisa variar de a cordo com a profundidade
		//vec3 separationVecRot2 = r2 * 0.05f; // da colisão.
		//Não queremos que as naves se desloquem no eixo Z, remove ele do vetor de separação.

		//Aplica o fator separação.
		e1.SetPosition(p1 - separationVec1);
		e2.SetPosition(p2 - separationVec2);
		//this->SetRotation(this->GetRotation() - separationVecRot1);
		//otherShip.SetRotation(otherShip.GetRotation() - separationVecRot2);


		Force collisionLinearForce, collisionAngularForce;
		collisionLinearForce.SetAsVec(((mass1 * mass2 * (linspd1 - linspd2)) / masst) * (float)(1 / deltaTime));

		// Calcule o impulso da colisão
		vec3 impulse = collisionLinearForce.GetVec() * deltaTime;
		impulse.z = 0.0f;

		// Velocidade Linear NO PONTO DE CONTATO DA COLISÃO com base na velocidade angular.
		vec3 linvel1 = glm::cross(e1.GetAngularSpeed(), r1);
		vec3 linvel2 = glm::cross(e2.GetAngularSpeed(), r2);
		//linvel1.z = 0; linvel2.z = 0;

		collisionAngularForce.SetAsVec(((inertia1 * inertia2 * (linvel1 - linvel2))
			/ inertiat) * (float)(1 / deltaTime));

		// Calcule o impulso da colisão
		vec3 impulseRot = collisionAngularForce.GetVec() * deltaTime;
		impulseRot.z = 0.0f;

		// Atualize a velocidade linear de cada objeto
		vec3 thisNewLinearSpeed = linspd1 - (impulse / mass1) - (impulseRot / inertia1);
		vec3 otherNewLinearSpeed = linspd2 + (impulse / mass2) + (impulseRot / inertia2);


		// Calcule o torque da colisão, que influencia na velocidade angular (rotação pós colisão)
		vec3 torque1 = glm::cross(rn1, -impulse);
		vec3 torque2 = glm::cross(rn2, impulse); // O impulso no outro objeto é oposto

		// Atualmente não há necessidade para rotações no eixo X. Elas permitiria que o usuário
		// Se deslocasse no eixo Z dependendo da inclinação da nave e o vetor direção não
		// Da suporte para o deslocamento no eixo Z. Então, remove essas componentes do torque.
		torque1.x = 0.0f;
		torque2.x = 0.0f;

		vec3 thisNewAngularSpeed = angspd1 + torque1 / inertia1;
		vec3 otherNewAngularSpeed = angspd2 + torque2 / inertia2;


		// Atualiza as velocidades pós-colisão
		e1.SetLinearSpeed(thisNewLinearSpeed);
		e2.SetLinearSpeed(otherNewLinearSpeed);

		e1.SetAngularSpeed(thisNewAngularSpeed);
		e2.SetAngularSpeed(otherNewAngularSpeed);

		// Atualize a velocidade angular de cada objeto
		collisionAngularForce.SetTargetPoint(rn1);
		collisionAngularForce.SetAsVec(-collisionAngularForce.GetVec());
		e1.AddInstantaneousForce(collisionAngularForce);
		collisionAngularForce.SetTargetPoint(rn2);
		collisionAngularForce.SetAsVec(-collisionAngularForce.GetVec());
		e2.AddInstantaneousForce(collisionAngularForce);

		return;

		e1.ReceiveDamage((MathUtils::Norm(collisionAngularForce.GetVec())) / inertia1 + 
			(MathUtils::Norm(collisionLinearForce.GetVec())) / mass1, DamageType::COLLISION_DAMAGE);
		e2.ReceiveDamage((MathUtils::Norm(collisionAngularForce.GetVec())) / inertia2 +
			(MathUtils::Norm(collisionLinearForce.GetVec())) / mass2, DamageType::COLLISION_DAMAGE);
	}
}

