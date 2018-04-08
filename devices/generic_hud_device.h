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

        DISPLAY.GetSize(scr_width_, scr_height_);
        hud_position_x_ = scr_width_ >> 1;
        hud_position_y_ = scr_height_ >> 1;
        hud_size_ = scr_height_ * 0.3;
        big_marker_size_ = 0.02 * scr_height_;
        small_marker_size_ = 0.01 * scr_height_;
        vector_scale_ = 0.01 * scr_height_;
    }
    void Render() {
        glPushMatrix();
        glLoadIdentity();

        glTranslated(hud_position_x_, hud_position_y_, 0);
        glScaled(1.0, -1.0, 1.0);
        glRotated(pa, 0.0, 0.0, -1.0);

        // HUD Circle
        glLineWidth(4.0);
        glColor4f(0.4, 1.0, 0.4, 0.5);
        glBegin(GL_LINE_LOOP);
        for (double a = 0.0; a < 2.0 * M_PI; a+=0.1) {
            glVertex2d(hud_size_ * cos(a), hud_size_ * sin(a));
        }
        glEnd();

        // HUD Markers - Big
        glLineWidth(10.0);
        glBegin(GL_LINES);
            glVertex2i(0.0, hud_size_ + 1);
            glVertex2i(0.0, hud_size_ + big_marker_size_);
        glEnd();
        // HUD Markers - Small
        glLineWidth(4.0);
        glBegin(GL_LINES);
            glVertex2i(0.0, -hud_size_ - small_marker_size_);
            glVertex2i(0.0, -hud_size_ - 1);
            glVertex2i(-hud_size_ - 1, 0.0);
            glVertex2i(-hud_size_ - small_marker_size_, 0.0);
            glVertex2i(hud_size_ + small_marker_size_, 0.0);
            glVertex2i(hud_size_ + 1, 0.0);
        glEnd();

        glPushMatrix();
        glScaled(vector_scale_, vector_scale_, 1.0);
        glLineWidth(2.0);
        // Thrust  : green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(tx, ty);
        glEnd();
        // Gravity : yellow
        glColor3f(1.0, 0.5, 0.0);
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
    int scr_width_;
    int scr_height_;
    int hud_position_x_;
    int hud_position_y_;
    int hud_size_;
    int big_marker_size_;
    int small_marker_size_;
    int vector_scale_;
};

#endif // GENERIC_HUD_DEVICE_H_