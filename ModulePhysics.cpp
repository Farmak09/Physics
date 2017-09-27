#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment (lib, "Box2D/libx86/Debug/Box2D.lib")
#endif
#ifndef _DEBUG
#pragma comment (lib, "Box2D/libx86/Release/Box2D.lib")
#endif

// TODO 1: Include Box 2 header and library 

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor 
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	warudo = new b2World(b2Vec2(0.0f, 9.81f));

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(PIXEL_TO_METERS(SCREEN_WIDTH/2),PIXEL_TO_METERS(SCREEN_HEIGHT/2));
	groundBodyDef.type = b2_staticBody;

	b2Body* groundBody = warudo->CreateBody(&groundBodyDef);

	b2CircleShape groundShape;
	groundShape.m_radius = PIXEL_TO_METERS(300);

	b2FixtureDef groundFixture;
	groundFixture.shape = &groundShape;
	groundFixture.friction = 0.1f;

	groundBody->CreateFixture(&groundFixture);
	// TODO 4: Create a big static circle as "ground"
	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	warudo->Step(1.0f / 60.0f, 8, 3);
	// TODO 3: Update the simulation ("step" the world)

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		b2BodyDef BodyDef;
		BodyDef.position.Set(PIXEL_TO_METERS(SCREEN_WIDTH / 2 - 1), PIXEL_TO_METERS(-50));
		BodyDef.type = b2_dynamicBody;

		b2Body* Body = warudo->CreateBody(&BodyDef);

		b2CircleShape Shape;
		Shape.m_radius = PIXEL_TO_METERS(50);

		b2FixtureDef Fixture;
		Fixture.shape = &Shape;
		Fixture.friction = 0.1f;
		Body->CreateFixture(&Fixture);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = warudo->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METER_TO_PIXELS(pos.x), METER_TO_PIXELS(pos.y), METER_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	delete warudo;
	// Delete the whole physics world!

	return true;
}
