#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>

using namespace std;
using namespace sf;
using namespace sfp;

int main()
{
    // Create our window and world with gravity 0,1
    RenderWindow window(VideoMode(800, 600), "Bounce");
    World world(Vector2f(0, 1));

    // Create the ball (starts left of the center box, pushed to the right)
    PhysicsCircle ball;
    ball.setCenter(Vector2f(150, 300));
    ball.setRadius(20);
    world.AddPhysicsBody(ball);
    ball.applyImpulse(Vector2f(1.0, 0));

    // Helper: size a rectangle and put its CENTER at the given point.
    // (setPosition is called too so the drawn rectangle and the collision
    // rectangle always end up in the same place.)
    auto place = [](PhysicsRectangle& r, Vector2f size, Vector2f center) {
        r.setSize(size);
        r.setCenter(center);
        r.RectangleShape::setPosition(center - size / 2.0f);
        r.setStatic(true);
        };

    // Create the four walls
    PhysicsRectangle floor;
    place(floor, Vector2f(800, 20), Vector2f(400, 590));
    world.AddPhysicsBody(floor);

    PhysicsRectangle ceiling;
    place(ceiling, Vector2f(800, 20), Vector2f(400, 10));
    world.AddPhysicsBody(ceiling);

    PhysicsRectangle leftWall;
    place(leftWall, Vector2f(20, 600), Vector2f(10, 300));
    world.AddPhysicsBody(leftWall);

    PhysicsRectangle rightWall;
    place(rightWall, Vector2f(20, 600), Vector2f(790, 300));
    world.AddPhysicsBody(rightWall);

    // Create the center box
    PhysicsRectangle center;
    place(center, Vector2f(120, 120), Vector2f(400, 300));
    world.AddPhysicsBody(center);

    // Wall collisions: "thud"
    int thudCount(0);
    floor.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };
    ceiling.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };
    leftWall.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };
    rightWall.onCollision = [&thudCount](PhysicsBodyCollisionResult result) {
        cout << "thud " << thudCount << endl;
        thudCount++;
        };

    // Center box collisions: "bang", exit after three hits
    int bang(0);
    center.onCollision = [&bang](PhysicsBodyCollisionResult result) {
        cout << "bang " << bang << endl;
        bang++;
        if (bang >= 3) {
            exit(0);
        }
        };

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    while (true) {
        // calculate MS since last frame
        Time currentTime(clock.getElapsedTime());
        Time deltaTime(currentTime - lastTime);
        int deltaTimeMS(deltaTime.asMilliseconds());
        if (deltaTimeMS > 0) {
            // cap the step so the ball can't jump through objects on a slow frame
            if (deltaTimeMS > 10) {
                deltaTimeMS = 10;
            }
            world.UpdatePhysics(deltaTimeMS);
            lastTime = currentTime;
        }
        window.clear(Color(0, 0, 0));
        window.draw(ball);
        window.draw(floor);
        window.draw(ceiling);
        window.draw(leftWall);
        window.draw(rightWall);
        window.draw(center);
        window.display();
    }
}