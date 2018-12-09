#ifndef LANDING_GEAR_H
#define LANDING_GEAR_H

class LandingGear : public Test
{
public:
    LandingGear()
    {
        // Create ground
        {
            b2BodyDef bd;
            b2Body* ground = m_world->CreateBody(&bd);

            b2EdgeShape shape;
            shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
            ground->CreateFixture(&shape, 0.0f);
        }

        {
            b2Vec2 v_ship[] = {
                { 0.16, 1.2 },
                {-0.16, 1.2 },
                {-0.5, 0.16 },
                {-0.68, 0.14 },
                {-0.54, -0.46 },
                { 0.54, -0.46 },
                { 0.68, 0.14 },
                { 0.5, 0.16 }
            };

            b2Vec2 v_lg[] = {
                { 0.13, 0.17},
                { -0.13, 0.17},
                { 0.0, -0.45}
            };

            b2Body * ship_body;
            b2Body * llg;
            b2Body * rlg;
            
            // Ship Body
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0, 25.0);
            ship_body = m_world->CreateBody(&bd);

            b2PolygonShape shape;
            shape.Set(v_ship, 8);
            ship_body->CreateFixture(&shape, 1.0);

            // Left Landing Gear
            bd.type = b2_dynamicBody;
            bd.position.Set(-0.46, 25.0 -0.05);
            llg = m_world->CreateBody(&bd);

            shape.Set(v_lg, 3);
            llg->CreateFixture(&shape, 1.0);

            // Right Landing Gear
            bd.type = b2_dynamicBody;
            bd.position.Set(0.46, 25.0 -0.05);
            rlg = m_world->CreateBody(&bd);

            shape.Set(v_lg, 3);
            rlg->CreateFixture(&shape, 1.0);

            float maxForce = 20.0f; // 10000.0f;
            float speed = -0.5f;
            float lowerLim = -0.35f;
            float upperLim = 0.08f;

            b2Vec2 anchor(-0.46, 25 - 0.05);
            b2Vec2 axis(-1.25, 6.25);
            axis.Normalize();

            b2PrismaticJointDef pjdL;
            b2PrismaticJointDef pjdR;

            pjdL.Initialize(ship_body, llg, anchor, axis);
            axis.x = -axis.x;
            anchor.x = -anchor.x;
            pjdR.Initialize(ship_body, rlg, anchor, axis);

            pjdL.lowerTranslation = lowerLim;
            pjdR.lowerTranslation = lowerLim;
            pjdL.upperTranslation = upperLim;
            pjdR.upperTranslation = upperLim;
            pjdL.enableLimit = true;
            pjdR.enableLimit = true;

            pjdL.motorSpeed = speed;
            pjdR.motorSpeed = speed;
            pjdL.maxMotorForce = maxForce;
            pjdR.maxMotorForce = maxForce;
            pjdL.enableMotor = true;
            pjdR.enableMotor = true;

            b2PrismaticJoint * pjL = (b2PrismaticJoint*)m_world->CreateJoint(&pjdL);
            b2PrismaticJoint * pjR = (b2PrismaticJoint*)m_world->CreateJoint(&pjdR);
        }

        if(false)
        {
            b2Body * body_a;
            b2Body * leg_1;
            b2Body * leg_2;
    
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 25.0f);
            body_a = m_world->CreateBody(&bd);
            bd.position.Set(-5.0f, 15.0f);
            leg_1 = m_world->CreateBody(&bd);
            bd.position.Set(5.0f, 15.0f);
            leg_2 = m_world->CreateBody(&bd);
    
            b2CircleShape shape;
            shape.m_radius = 10.0f;
            body_a->CreateFixture(&shape, 1.0f);
            shape.m_radius = 5.0f;
            leg_1->CreateFixture(&shape, 1.0f);
            shape.m_radius = 5.0f;
            leg_2->CreateFixture(&shape, 1.0f);
    
            float maxForce = 10000.0f;
            float speed = -3.0f;
            float lowerLim = -1.0f;
            float upperLim = 2.0f;
    
            b2PrismaticJointDef pjd1;
            b2PrismaticJointDef pjd2;
            b2Vec2 anchor(-5, 25.0);
            b2Vec2 axis(0.0, 1.0);
            axis.Normalize();
            pjd1.Initialize(body_a, leg_1, anchor, axis);
            anchor.x = 5.0;
            pjd2.Initialize(body_a, leg_2, anchor, axis);
    
            pjd1.lowerTranslation = lowerLim;
            pjd1.upperTranslation = upperLim;
            pjd1.enableLimit = true;
    
            pjd2.lowerTranslation = lowerLim;
            pjd2.upperTranslation = upperLim;
            pjd2.enableLimit = true;
            pjd1.motorSpeed = speed;
            pjd1.maxMotorForce = maxForce;
            pjd1.enableMotor = true;
    
            pjd2.motorSpeed = speed;
            pjd2.maxMotorForce = maxForce;
            pjd2.enableMotor = true;
            b2PrismaticJoint * pj1 = (b2PrismaticJoint*)m_world->CreateJoint(&pjd1);
            b2PrismaticJoint * pj2 = (b2PrismaticJoint*)m_world->CreateJoint(&pjd2);
        }
    }

    void Keyboard(int key)
    {
    }

    static Test* Create()
    {
        return new LandingGear;
    }
};

#endif // LANDING_GEAR_H

