#ifndef GENERIC_HUD_DEVICE_H_
#define GENERIC_HUD_DEVICE_H_

#include <GLFW/glfw3.h>

#include <mutex>

#include "ship_device.h"

class GenericHudDevice : public ShipDevice {
private:
    struct Detection {
        Detection(double c, double h)
        : center(c)
        , horizon(h)
        {}
        double center, horizon;
    };
    std::mutex d_mutex_;
public:
    GenericHudDevice()
    : ShipDevice()
    {
        gx = gy = 0.0;
        thrust = 0.0;
        vx = vy = 0.0;
        px = py = pa = 0.0;
    }
    ~GenericHudDevice() {
    }
    void Init() {
        using std::placeholders::_1;
        bus_->Subscribe(db_PlayerPosition,
            std::bind(&GenericHudDevice::hndPlayerPosition, this, _1));
        bus_->Subscribe(db_PlayerAngle,
            std::bind(&GenericHudDevice::hndPlayerAngle, this, _1));
        bus_->Subscribe(db_PlayerGravity,
            std::bind(&GenericHudDevice::hndPlayerGravity, this, _1));
        bus_->Subscribe(db_PlayerVelocity,
            std::bind(&GenericHudDevice::hndPlayerVelocity, this, _1));
        bus_->Subscribe(db_PlayerThrust,
            std::bind(&GenericHudDevice::hndPlayerThrust, this, _1));
        bus_->Subscribe(db_PlayerFuel,
            std::bind(&GenericHudDevice::hndFuelQuantity, this, _1));
        bus_->Subscribe(db_DetectionList,
            std::bind(&GenericHudDevice::hndRadarDetections, this, _1));

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

        // Platform/Ownship Vectors
        glPushMatrix();
        glScaled(vector_scale_, vector_scale_, 1.0);
        glLineWidth(2.0);
        // Thrust  : green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex2d(0.0, 0.0);
            glVertex2d(thrust * cos(0.5 * M_PI + pa * M_PI / 180.0), thrust * sin(0.5 * M_PI + pa * M_PI / 180.0));
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

        // Detections
        {
            glPushMatrix();
            glScaled(hud_size_, hud_size_, 1.0);
            std::lock_guard<std::mutex> lock(d_mutex_);
            for (auto d : detection_list_) {
                RenderArc(d->center, d->horizon);
            }
            glPopMatrix();
        }

        glPopMatrix();

        // Fuel Gauge
        int fw = 120;
        int fh = 20;
        int t = 30;
        int fl = fw * fuel;

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
private:
    double detection_size_;
    double detection_u_;
    double detection_v_;
    void AddDetections(int num_detections, double* detections) {
        std::lock_guard<std::mutex> lock(d_mutex_);
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
private: // Handlers
    void hndPlayerPosition(BusDataInterface *data) {
        BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
        if (p != 0) {
            px = p->x;
            py = p->y;
        }
    }
    void hndPlayerAngle(BusDataInterface *data) {
        BD_Scalar *a = static_cast<BD_Scalar *>(data);
        if (a != 0) {
            pa = a->value;
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
        BD_Scalar *s = static_cast<BD_Scalar *>(data);
        if (s != 0) {
            thrust = s->value;
        }
    }
    void hndFuelQuantity(BusDataInterface *data) {
        BD_Scalar *f = static_cast<BD_Scalar *>(data);
        if (f != 0) {
            fuel = f->value;
        }
    }
    void hndRadarDetections(BusDataInterface *data) {
        BD_RadarDetectionList *d = static_cast<BD_RadarDetectionList *>(data);
        if (d != 0) {
            AddDetections(d->num_detections, d->data);
        }
    }
private:
    double fuel;
    double px, py, pa;
    double gx, gy;
    double thrust;
    double vx, vy;
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
