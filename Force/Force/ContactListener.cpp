#include "stdafx.h"
#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	int a = (int)A->GetUserData();
	int b = (int)B->GetUserData();
	if (a == 1 && b == 2)
	{
		b2Vec2 delta = manifold.points[0] - B->GetBody()->GetPosition();
		delta.Normalize();	
		delta *= 0.15f;
		//A->GetBody()->ApplyForce(delta, manifold.points[0], false);
		A->GetBody()->ApplyLinearImpulse(delta, manifold.points[0], false);
	}
	else if (b == 1 && a == 2)
	{
		b2Vec2 delta = manifold.points[0] - A->GetBody()->GetPosition();
		delta.Normalize();
		delta *= 0.15f;
		//B->GetBody()->ApplyForce(delta, manifold.points[0], false);
		B->GetBody()->ApplyLinearImpulse(delta, manifold.points[0], false);
	}
	if (a == 1 && b == 3)
	{
		b2Body* body = B->GetBody();
		Pull* pull = (Pull*)body->GetUserData();
		if (pull->num < 9)
		{
			b2Vec2 delta = body->GetPosition() - manifold.points[0];
			delta.Normalize();
			delta *= 0.15f;
			//A->GetBody()->ApplyForce(delta, manifold.points[0], false);
			A->GetBody()->ApplyLinearImpulse(delta, manifold.points[0], false);
		}
		else
		{
			pull->player->fpickup = true;
			pull->player->ball = A->GetBody();
			b2Filter filter = {};
			A->SetFilterData(filter);
		}
	}
	else if (b == 1 && a == 3)
	{
		b2Body* body = A->GetBody();
		Pull* pull = (Pull*)body->GetUserData();
		if (pull->num < 9)
		{
			b2Vec2 delta = body->GetPosition() - manifold.points[0];
			delta.Normalize();
			delta *= 0.15f;
			//B->GetBody()->ApplyForce(delta, manifold.points[0], false);
			B->GetBody()->ApplyLinearImpulse(delta, manifold.points[0], false);
		}
		else
		{
			pull->player->fpickup = true;
			pull->player->ball = B->GetBody();
			b2Filter filter = {};
			B->SetFilterData(filter);
		}
	}
}

void ContactListener::EndContact(b2Contact* contact)
{

}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture* A = contact->GetFixtureA();
	b2Fixture* B = contact->GetFixtureB();
	int a = (int)A->GetUserData();
	int b = (int)B->GetUserData();
	if (a == 1 && b == 2 || b == 1 && a == 2)
		contact->SetEnabled(false);
	if (a == 1 && b == 3 || b == 1 && a == 3)
		contact->SetEnabled(false);
	if (a == 2 && b == 3 || b == 2 && a == 3)
		contact->SetEnabled(false);
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}