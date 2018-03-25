#ifndef GENERIC_HUD_DEVICE_H_
#define GENERIC_HUD_DEVICE_H_

#include <GLFW/glfw3.h>

#include "ship_device.h"

class GenericHudDevice : public ShipDevice {
public:
    GenericHudDevice()
    : ShipDevice()
    {
        gx = gy = 0.0;
        tx = ty = 0.0;
        vx = vy = 0.0;
        px = py = pa = 0.0;
    }
    ~GenericHudDevice() {
    }
    void Init() {
        using std::placeholders::_1;
        bus_->Subscribe(db_PlayerPosition,
            std::bind(&GenericHudDevice::hndPlayerPosition, this, _1));
        bus_->Subscribe(db_PlayerGravity,
            std::bind(&GenericHudDevice::hndPlayerGravity, this, _1));
        bus_->Subscribe(db_PlayerVelocity,
            std::bind(&GenericHudDevice::hndPlayerVelocity, this, _1));
        bus_->Subscribe(db_PlayerThrust,
            std::bind(&GenericHudDevice::hndPlayerThrust, this, _1));
    }
    void Render() {
        glPushMatrix();
        glLoadIdentity();
        glRotated(pa, 0.0, 0.0, -1.0);

        // HUD Circle
        const double size = 8.0;
        glLineWidth(4.0);
        glColor4f(0.4, 1.0, 0.4, 0.5);
        glBegin(GL_LINE_LOOP);
        for (double a = 0.0; a < 2.0 * M_PI; a+=0.1) {
            glVertex2d(size * cos(a), size * sin(a));
        }
        glEnd();
        glLineWidth(8.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, size+0.05);
            glVertex2d(0.0, size+0.4);
        glEnd();
        glLineWidth(2.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, -size-0.05);
            glVertex2d(0.0, -size-0.25);
            glVertex2d(-size-0.05, 0.0);
            glVertex2d(-size-0.25, 0.0);
            glVertex2d(size+0.05, 0.0);
            glVertex2d(size+0.25, 0.0);
        glEnd();

        // Thrust  : green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(0.25 * tx, 0.25 * ty);
        glEnd();
        // Gravity : yellow
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(gx, gy);
        glEnd();
        // Velocity: blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(vx, vy);
        glEnd();

        glPopMatrix();
    }
private: // Handlers
    void hndPlayerPosition(BusDataInterface *data) {
        BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
        if (p != 0) {
            px = p->x;
            py = p->y;
            pa = p->angle;
        }
    }
    void hndPlayerGravity(BusDataInterface *data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            gx = v->x;
            gy = v->y;
        }
    }
    void hndPlayerVelocity(BusDataInterface *data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            vx = v->x;
            vy = v->y;
        }
    }
    void hndPlayerThrust(BusDataInterface *data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            tx = v->x;
            ty = v->y;
        }
    }
private:
    double px, py, pa;
    double gx, gy;
    double tx, ty;
    double vx, vy;
};

#endif // GENERIC_HUD_DEVICE_H_
