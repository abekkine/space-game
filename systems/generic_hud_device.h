#ifndef GENERIC_HUD_DEVICE_H_
#define GENERIC_HUD_DEVICE_H_

#include <GLFW/glfw3.h>
#include <assert.h>

#include <mutex>

#include "hud_system_interface.h"

class GenericHudDevice : public HudSystemInterface {
private:
    struct Detection {
        Detection(double c, double h)
        : center(c)
        , horizon(h)
        {}
        double center, horizon;
    };
public:
    GenericHudDevice() {
        detection_size_ = 0.0;
        detection_u_ = 0.0;
        detection_v_ = 0.0;
        // detection_mutex_;
        active_ = true;
        ship_fuel_percent_ = 0.0;
        damage_ratio_ = 1.0;
        ship_x_ = 0.0;
        ship_y_ = 0.0;
        ship_angle_ = 0.0;
        g_x_ = 0.0;
        g_y_ = 0.0;
        ship_thrust_force_ = 0.0;
        ship_velocity_x_ = 0.0;
        ship_velocity_y_ = 0.0;
        scr_width_ = 0;
        scr_height_ = 0;
        hud_position_x_ = 0;
        hud_position_y_ = 0;
        hud_size_ = 0;
        big_marker_size_ = 0;
        small_marker_size_ = 0;
        vector_scale_ = 0;
        detection_list_.clear();

        bus_ = 0;
        bus_connection_ = 0;
    }
    ~GenericHudDevice() {
    }
    void Disable() {
        active_ = false;
    }
    void Init(DataBus* bus) {

        HudSystemInterface::Init(bus);

        if (bus_connection_ != 0) {
            DB_SUBSCRIBE(GenericHudDevice, ShipPosition);
            DB_SUBSCRIBE(GenericHudDevice, ShipAngle);
            DB_SUBSCRIBE(GenericHudDevice, ShipGravity);
            DB_SUBSCRIBE(GenericHudDevice, ShipVelocity);
            DB_SUBSCRIBE(GenericHudDevice, ShipThrust);
            DB_SUBSCRIBE(GenericHudDevice, ShipFuelQty);
            DB_SUBSCRIBE(GenericHudDevice, DetectionList);
            DB_SUBSCRIBE(GenericHudDevice, ShipDamage);
        }

        DISPLAY.GetSize(scr_width_, scr_height_);
        hud_position_x_ = scr_width_ >> 1;
        hud_position_y_ = scr_height_ >> 1;
        hud_size_ = scr_height_ * 0.3;
        big_marker_size_ = 0.02 * scr_height_;
        small_marker_size_ = 0.01 * scr_height_;
        vector_scale_ = 0.01 * scr_height_;
    }
    void Render() {
        if (!active_) {
            return;
        }

        glPushMatrix();
        glLoadIdentity();

        glTranslated(hud_position_x_, hud_position_y_, 0);
        glScaled(1.0, -1.0, 1.0);
        glRotated(ship_angle_, 0.0, 0.0, -1.0);

        RenderHudDial();
        RenderShipVectors();
        RenderRadarDetections();

        glPopMatrix();

        RenderFuelIndicator();
        RenderDamageIndicator();
    }

private:
    void RenderHudDial() {
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
    }
    void RenderShipVectors() {
        glPushMatrix();
        glScaled(vector_scale_, vector_scale_, 1.0);
        glLineWidth(2.0);
        // Thrust  : green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(ship_thrust_force_ * cos(0.5 * M_PI + ship_angle_ * M_PI / 180.0), ship_thrust_force_ * sin(0.5 * M_PI + ship_angle_ * M_PI / 180.0));
        glEnd();
        // Gravity : yellow
        glColor3f(1.0, 0.5, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(g_x_, g_y_);
        glEnd();
        // Velocity: blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(ship_velocity_x_, ship_velocity_y_);
        glEnd();
        glPopMatrix();
    }
    void RenderRadarDetections() {
        // Detections
        glPushMatrix();
        glScaled(hud_size_, hud_size_, 1.0);
        std::lock_guard<std::mutex> lock(detection_mutex_);
        for (auto d : detection_list_) {
            RenderArc(d->center, d->horizon);
        }
        glPopMatrix();
    }
    void RenderFuelIndicator() {
        int fw = 120;
        int fh = 20;
        int t = 30;
        int fl = fw * ship_fuel_percent_;

        glPushMatrix();
        glTranslated(scr_width_ - fw -t, t, 0.0);
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(fl, 0);
        glVertex2i(fl, fh);
        glVertex2i(0, fh);
        glEnd();
        glPopMatrix();

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(scr_width_ - fw - t, t);
        glVertex2i(scr_width_ - t, t);
        glVertex2i(scr_width_ - t, t + fh);
        glVertex2i(scr_width_ - fw - t, t + fh);
        glEnd();
    }
    void RenderDamageIndicator() {
        int dw = 120;
        int fh = 20;
        int dh = 20;
        int t = 30;
        int dl = dw * damage_ratio_;

        glPushMatrix();
        glTranslated(scr_width_ - dw -t, 2*t + fh, 0.0);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex2i(0, 0);
        glVertex2i(dl, 0);
        glVertex2i(dl, dh);
        glVertex2i(0, dh);
        glEnd();
        glPopMatrix();

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(2.0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(scr_width_ - dw - t, 2*t + fh);
        glVertex2i(scr_width_ - t, 2*t + fh);
        glVertex2i(scr_width_ - t, 2*t + fh + dh);
        glVertex2i(scr_width_ - dw - t, 2*t + fh + dh);
        glEnd();

    }
    void RenderArc(double center_angle, double horizon_angle) {
        double a_begin = (center_angle - horizon_angle);
        double a_end = (center_angle + horizon_angle);
        const double a_step = 0.01;

        if ((a_end - a_begin) < a_step) {

            glPointSize(5.0);
            glColor4f(1.0, 1.0, 1.0, 0.8);
            glBegin(GL_POINTS);
                glVertex2d(cos(a_begin), sin(a_begin));
            glEnd();
        }
        else {
            glLineWidth(5.0);
            glColor4f(0.0, 1.0, 0.0, 0.5);
            glBegin(GL_LINE_STRIP);
            for (double a=a_begin; a<=a_end; a+=a_step) {
                glVertex2d(cos(a), sin(a));
            }
            glEnd();
        }
    }
    void AddDetections(int num_detections, double* detections) {
        std::lock_guard<std::mutex> lock(detection_mutex_);
        for (auto d : detection_list_) delete d;
        detection_list_.clear();
        Detection * d = 0;
        for (int i=0; i<num_detections; ++i) {
            d = new Detection(
                detections[2*i + 0],
                detections[2*i + 1]
            );
            detection_list_.push_back(d);
        }
        delete [] detections;
    }
    // Handlers
    void dbHandleShipPosition(BusDataInterface *data) {
        BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
        if (p != 0) {
            ship_x_ = p->x;
            ship_y_ = p->y;
        }
    }
    void dbHandleShipAngle(BusDataInterface *data) {
        BD_Scalar *a = static_cast<BD_Scalar *>(data);
        if (a != 0) {
            ship_angle_ = a->value;
        }
    }
    void dbHandleShipGravity(BusDataInterface *data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            g_x_ = v->x;
            g_y_ = v->y;
        }
    }
    void dbHandleShipVelocity(BusDataInterface *data) {
        BD_Vector *v = static_cast<BD_Vector *>(data);
        if (v != 0) {
            ship_velocity_x_ = v->x;
            ship_velocity_y_ = v->y;
        }
    }
    void dbHandleShipThrust(BusDataInterface *data) {
        BD_Scalar *s = static_cast<BD_Scalar *>(data);
        if (s != 0) {
            ship_thrust_force_ = s->value;
        }
    }
    void dbHandleShipFuelQty(BusDataInterface *data) {
        BD_Scalar *f = static_cast<BD_Scalar *>(data);
        if (f != 0) {
            ship_fuel_percent_ = f->value;
        }
    }
    void dbHandleDetectionList(BusDataInterface *data) {
        BD_RadarDetectionList *d = static_cast<BD_RadarDetectionList *>(data);
        if (d != 0) {
            AddDetections(d->num_detections, d->data);
        }
    }
    void dbHandleShipDamage(BusDataInterface *data) {
        BD_Scalar *dr = static_cast<BD_Scalar *>(data);
        if (dr != 0) {
            damage_ratio_ = dr->value;
        }
    }

private:
    double detection_size_;
    double detection_u_;
    double detection_v_;
    std::mutex detection_mutex_;
    bool active_;
    double ship_fuel_percent_;
    double damage_ratio_;
    double ship_x_;
    double ship_y_;
    double ship_angle_;
    double g_x_;
    double g_y_;
    double ship_thrust_force_;
    double ship_velocity_x_;
    double ship_velocity_y_;
    int scr_width_;
    int scr_height_;
    int hud_position_x_;
    int hud_position_y_;
    int hud_size_;
    int big_marker_size_;
    int small_marker_size_;
    int vector_scale_;
    std::vector<Detection *> detection_list_;
};

#endif // GENERIC_HUD_DEVICE_H_
