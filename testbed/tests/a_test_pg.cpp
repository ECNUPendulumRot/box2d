

#include "test.h"

static const int SPHERE_COUNT[5] = {4, 3, 4, 3, 2};

class CircleTest : public Test
{
    b2Body* m_bodies[1024];

    b2Vec2 m_left_box_pos;
    b2Vec2 m_right_box_pos;

    b2Vec2 m_center_pos;

    // odd number
    int layer = 7;

    b2Body* m_dynamic_body;

    // modify the y-axis of velocity
    b2Vec2 m_dynamic_body_velocity = b2Vec2(0, 10);

public:
    CircleTest() {
        m_world->SetGravity(b2Vec2_zero);

        m_center_pos.Set(-5, 20);

        m_left_box_pos.Set(-20, 10);
        m_right_box_pos.Set(10, 10);

        {
            b2CircleShape circle_shape;
            circle_shape.m_radius = 1.f;

            b2BodyDef body_def;
            body_def.type = b2_dynamicBody;

            float x_offset = 2.f;
            float y_offset = sqrtf(3.0f);

            int index = 0;

            // Circles are located between the two boxes
            for(int i = 0; i < 5; i++) {
                b2Vec2 left_pos(m_center_pos.x - SPHERE_COUNT[i] + 1.f, m_center_pos.y - i * y_offset);
                for(int j = 0; j < SPHERE_COUNT[i]; j++) {
                    body_def.position.Set(left_pos.x + x_offset * j, left_pos.y);
                    m_bodies[index] = m_world->CreateBody(&body_def);
                    m_bodies[index]->SetLinearDamping(0);
                    m_bodies[index]->SetAngularDamping(0);
                    b2Fixture* fixture = m_bodies[index]->CreateFixture(&circle_shape, 1.f);
                    fixture->SetFriction(0);
                    fixture->SetRestitution(1.0);

                    index++;
                }
            }

            for(int i = 1; i <= layer; i++) {
                b2Vec2 left_pos(m_center_pos.x - layer + i , m_center_pos.y + i * y_offset);
                for(int j = 0; j <= layer - i; j++) {
                    body_def.position.Set(left_pos.x + j * x_offset, left_pos.y);
                    m_bodies[index] = m_world->CreateBody(&body_def);
                    m_bodies[index]->SetLinearDamping(0);
                    m_bodies[index]->SetAngularDamping(0);
                    b2Fixture* fixture = m_bodies[index]->CreateFixture(&circle_shape, 1.f);
                    fixture->SetFriction(0);
                    fixture->SetRestitution(1.0);

                    index++;
                }
            }
        }
        {
            b2CircleShape circle_shape;
            circle_shape.m_radius = 1.f;

            b2BodyDef body_def;
            body_def.type = b2_dynamicBody;
            body_def.position.Set(m_center_pos.x, m_center_pos.y - 14);

            m_dynamic_body = m_world->CreateBody(&body_def);
            m_dynamic_body->SetLinearDamping(0);
            m_dynamic_body->SetAngularDamping(0);

            m_dynamic_body->SetLinearVelocity(m_dynamic_body_velocity);

            b2Fixture* fixture = m_dynamic_body->CreateFixture(&circle_shape, 1.f);
            fixture->SetFriction(0);
            fixture->SetRestitution(1.0);
        }
        {
            float hx = m_center_pos.x - m_left_box_pos.x - 4;
            float hy = m_center_pos.y - m_left_box_pos.y + sqrtf(3.0f) - 1.f;
            b2PolygonShape box_shape;
            box_shape.SetAsBox(hx, hy);
            b2BodyDef body_def;
            body_def.type = b2_staticBody;
            body_def.position = m_left_box_pos;

            b2Body* box_body1 = m_world->CreateBody(&body_def);
            b2Fixture* box_fixture1 = box_body1->CreateFixture(&box_shape, 0.f);
            box_fixture1->SetFriction(0);
            box_fixture1->SetRestitution(1.0);

            hx = m_right_box_pos.x - m_center_pos.x - 4;
            hy = m_center_pos.y - m_right_box_pos.y + sqrtf(3.0f) - 1.f;
            box_shape.SetAsBox(hx, hy);
            body_def.position = m_right_box_pos;
            b2Body* box_body2 = m_world->CreateBody(&body_def);
            b2Fixture* box_fixture2 = box_body2->CreateFixture(&box_shape, 0.f);
            box_fixture2->SetFriction(0);
            box_fixture2->SetRestitution(1.0);
        }
    }

    static Test* Create()
    {
        return new CircleTest;
    }
};

static int testIndex = RegisterTest("Benchmark", "Circle PG Test", CircleTest::Create);
