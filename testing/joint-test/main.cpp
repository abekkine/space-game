#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

struct SpaceShip {
    double x_, y_;
    double angle_;
    b2Body * body_;
    b2Vec2 v_body_[4];

    double llg_x_, llg_y_;
    double llg_angle_;
    b2Body * llg_body_;
    b2Vec2 v_llg_body_[4];

    SpaceShip()
    : v_body_{
        {1.3, 4.0},
        {2.6, -1.3},
        {-2.6, -1.3},
        {-1.3, 4.0}}
    , v_llg_body_{
        {0.66, 1.48},
        {-0.13, -1.4},
        {-0.63, -1.4},
        {0.16, 1.68}}
    {}
    ~SpaceShip() {}

    void Init(b2World * world) {
        // Main Ship Body
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.angularVelocity = 0.0;
        def.angle = angle_ * M_PI / 180.0;
        def.position.Set(x_, y_);
        body_ = world->CreateBody(&def);
        body_->SetUserData(this);

        b2PolygonShape shape;
        shape.Set(v_body_, 4);

        b2FixtureDef fixture;
        // fixture.filter.groupIndex = -1;
        fixture.shape = &shape;
        fixture.density = 1.0;
        fixture.friction = 1.0;
        body_->CreateFixture(&fixture);

        // Left Landing Gear
        def.angle = llg_angle_ * M_PI / 180.0;
        def.position.Set(llg_x_, llg_y_);
        llg_body_ = world->CreateBody(&def);
        llg_body_->SetUserData(this);

        shape.Set(v_llg_body_, 4);

        fixture.shape = &shape;
        fixture.density = 1.0;
        fixture.friction = 1.0;
        llg_body_->CreateFixture(&fixture);

        // Left Landing Gear Joint
        b2DistanceJointDef jointDef;
        jointDef.collideConnected = false;
        jointDef.bodyA = body_;
        jointDef.bodyB = llg_body_;
        jointDef.localAnchorA = b2Vec2(0.0, 0.0);
        jointDef.localAnchorB = b2Vec2(0.0, 0.0);
        jointDef.length = 1.0;
        
//        b2PrismaticJointDef jointDef;
//        b2Vec2 llgAxis(0.2, 0.8);
//        jointDef.Initialize(body_, llg_body_, body_->GetWorldCenter(), llgAxis);
//        jointDef.lowerTranslation = -1.0;
//        jointDef.upperTranslation = 1.0;
//        jointDef.enableLimit = true;
//        jointDef.maxMotorForce = 100.0;
//        jointDef.motorSpeed = 0.0;
//        jointDef.enableMotor = true;
        world->CreateJoint(&jointDef);
    }
    void Update() {
        // Main Ship Body
        b2Vec2 pos = body_->GetPosition();
        x_ = pos.x;
        y_ = pos.y;
        angle_ = body_->GetAngle() * 180.0 / M_PI;

        // Left Landing Gear
        pos = llg_body_->GetPosition();
        llg_x_ = pos.x;
        llg_y_ = pos.y;
        llg_angle_ = llg_body_->GetAngle() * 180.0 / M_PI;
    }
    void Render() {
        glPushMatrix();
        // Main Ship Body
        glLoadIdentity();
        glTranslated(x_, y_, 0.0);
        glRotated(angle_, 0.0, 0.0, 1.0);

        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (int i=0; i<4; ++i) {
            glVertex2d(v_body_[i].x, v_body_[i].y);
        }
        glEnd();

        // Left Landing Gear
        glLoadIdentity();
        glTranslated(llg_x_, llg_y_, 0.0);
        glRotated(llg_angle_, 0.0, 0.0, 1.0);

        glBegin(GL_LINE_LOOP);
        for (int i=0; i<4; ++i) {
            glVertex2d(v_llg_body_[i].x, v_llg_body_[i].y);
        }
        glEnd();

        glPopMatrix();
        Update();
    }
};

struct Ball {
    double x_, y_;
    double angle_;
    double radius_;
    b2Body * body_;

    Ball() {}
    ~Ball() {}

    void Init(b2World * world) {
        b2BodyDef def;
        def.type = b2_dynamicBody;
        def.angularVelocity = 0.0;
        def.angle = angle_ * M_PI / 180.0;
        def.position.Set(x_, y_);
        body_ = world->CreateBody(&def);
        body_->SetUserData(this);

        b2CircleShape shape;
        shape.m_p.Set(0.0, 0.0);
        shape.m_radius = radius_;

        b2FixtureDef fixture;
        fixture.density = 1.0;
        fixture.friction = 1.0;

        fixture.shape = &shape;
        body_->CreateFixture(&fixture);
    }

    void Update() {
        b2Vec2 pos = body_->GetPosition();
        x_ = pos.x;
        y_ = pos.y;
        angle_ = body_->GetAngle() * 180.0 / M_PI;       
    }

    void Render() {
        glPushMatrix();
        glLoadIdentity();
        glTranslated(x_, y_, 0.0);

        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (double a = 0.0; a<2.0 * M_PI + 0.01; a+=0.01) {
            glVertex2d(radius_ * cos(a), radius_ * sin(a));
        }
        glEnd();

        glPopMatrix();
        Update();
    }
};

struct CelestialBody {

    CelestialBody() {}
    ~CelestialBody() {}

    void Init(b2World * world) {
        b2BodyDef def;
        def.type = b2_kinematicBody;
        def.position.Set(x_, y_);
        body_ = world->CreateBody(&def);

        b2CircleShape shape;
        shape.m_p.Set(0.0, 0.0);
        shape.m_radius = radius_;

        b2FixtureDef fixture;
        fixture.density = 1.0;
        fixture.friction = 1.0;

        fixture.shape = &shape;
        body_->CreateFixture(&fixture);
    }

    void Update() {
        body_->SetAngularVelocity(0.0);
        b2Vec2 pos = body_->GetPosition();
        x_ = pos.x;
        y_ = pos.y;
        angle_ = body_->GetAngle() * 180.0 / M_PI;
    }

    void Render() {
        glPushMatrix();
        glLoadIdentity();
        glTranslated(x_, y_, 0.0);

        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (double a = 0.0; a<2.0 * M_PI + 0.01; a+=0.01) {
            glVertex2d(radius_ * cos(a), radius_ * sin(a));
        }
        glEnd();

        glPopMatrix();

        Update();
    }

    double x_, y_;
    double angle_;
    double radius_;
    b2Body * body_;
};

CelestialBody * planet = 0;
Ball * ball = 0;
SpaceShip * ship = 0;
b2World * world;
void init_physics() {

    b2Vec2 gravity(0.0, -9.8);
    world = new b2World(gravity);

    planet = new CelestialBody();
    planet->x_ = 0.0;
    planet->y_ = -100.0;
    planet->angle_ = 0.0;
    planet->radius_ = 80.0;
    planet->Init(world);

//    ball = new Ball();
//    ball->x_ = 1.0;
//    ball->y_ = 25.0;
//    ball->angle_ = 0.0;
//    ball->radius_ = 5.0;
//    ball->Init(world);

    ship = new SpaceShip();
    ship->x_ = 0.0;
    ship->y_ = 25.0;
    ship->angle_ = 0.0;

    ship->llg_x_ = 0.0 - 1.96; // - 1.59;
    ship->llg_y_ = 25.0 - 1.18; // + 0.22;
    ship->llg_angle_ = 0.0;

    ship->Init(world);
}


void render();
void idle();

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

	switch(key) {
		case 27:
			exit(0);
			break;
	}
}

void reshape(int width, int height) {

	glViewport(0, 0, width, height);
}

void init(void) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void update_viewport() {
    const double size = 50.0;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-size, size, -size, size, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    int screen_size = 1600;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(screen_size, screen_size);

	glutInitWindowPosition(10, 10);

	glutCreateWindow("Joints");

	init();
    update_viewport();

    init_physics();

	glutMainLoop();

	return 0;
}


void render() {

//    if (ball != 0) {
//        ball->Render();
//    }

    if (ship != 0) {
        ship->Render();
    }

    if (planet != 0) {
        planet->Render();
    }
}

void idle() {

    world->Step(0.02, 12, 6);

    usleep(20000);
}

