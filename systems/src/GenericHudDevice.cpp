#include "GenericHudDevice.h"
#include "DetectionObject.h"
#include "BusDataTypes.h"
#include "DataBus.h"
#include "Display.h"

#include <math.h>

GenericHudDevice::GenericHudDevice() {
    name_ = "hud";
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
    star_list_.clear();
    selected_star_ = -1;

    bus_ = 0;
    bus_connection_ = 0;
}

GenericHudDevice::~GenericHudDevice() {
    for (auto d : detection_list_) delete d;
    for (auto s : star_list_) delete s;
}

void GenericHudDevice::Init(DataBus* bus) {
    ShipSystemBase::Init(bus);

    if (bus_connection_ != 0) {
        DB_SUBSCRIBE(GenericHudDevice, ShipPosition);
        DB_SUBSCRIBE(GenericHudDevice, ShipAngle);
        DB_SUBSCRIBE(GenericHudDevice, ShipGravity);
        DB_SUBSCRIBE(GenericHudDevice, ShipVelocity);
        DB_SUBSCRIBE(GenericHudDevice, ShipThrust);
        DB_SUBSCRIBE(GenericHudDevice, ShipFuelQty);
        DB_SUBSCRIBE(GenericHudDevice, DetectionList);
        DB_SUBSCRIBE(GenericHudDevice, StarList);
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

void GenericHudDevice::Render() {
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
    RenderStarDetections();

    glPopMatrix();

    RenderFuelIndicator();
    RenderDamageIndicator();
}

void GenericHudDevice::RenderHudDial() {
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
void GenericHudDevice::RenderShipVectors() {
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
void GenericHudDevice::RenderRadarDetections() {
    // Detections
    float c_radar_dot[4] = { 1.0, 1.0, 1.0, 0.8 };
    float c_radar_arc[4] = { 0.0, 1.0, 0.0, 0.5 };
    glPushMatrix();
    glScaled(hud_size_, hud_size_, 1.0);
    std::lock_guard<std::mutex> lock(detection_mutex_);
    for (auto d : detection_list_) {
        RenderArc(d->center, d->span, c_radar_dot, c_radar_arc);
    }
    glPopMatrix();
}

void GenericHudDevice::RenderStarDetections() {
    // Stars
    float s_color[4] = { 1.0, 0.5, 0.0, 0.3 };
    float sel_color[4] = { 1.0, 1.0, 1.0, 0.8 };
    glScaled(hud_size_, hud_size_, 1.0);
    std::lock_guard<std::mutex> lock(detection_mutex_);
    for (int i=0; i<star_list_.size(); ++i) {
        if (i == selected_star_) {
            RenderDot(star_list_[i]->center, sel_color, 12.0, 0.98);
        } else {
            RenderDot(star_list_[i]->center, s_color, 8.0, 0.98);
        }
    }
    glPopMatrix();
}

void GenericHudDevice::RenderFuelIndicator() {
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

void GenericHudDevice::RenderDamageIndicator() {
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

void GenericHudDevice::RenderDot(
    double center_angle,
    float * color,
    float size,
    double scale)
{
    glPointSize(size);
    glColor4fv(color);
    glBegin(GL_POINTS);
    glVertex2d(scale * cos(center_angle), scale * sin(center_angle));
    glEnd();
}


void GenericHudDevice::RenderArc(
    double center_angle, double span_angle,
    float * dot_color, float * arc_color,
    double scale)
{
    double a_begin = (center_angle - span_angle);
    double a_end = (center_angle + span_angle);
    const double a_step = 0.01;

    if ((a_end - a_begin) < a_step) {

        glPointSize(5.0);
        // glColor4f(1.0, 1.0, 1.0, 0.8);
        glColor4fv(dot_color);
        glBegin(GL_POINTS);
            glVertex2d(scale * cos(a_begin), scale * sin(a_begin));
        glEnd();
    }
    else {
        glLineWidth(5.0);
        // glColor4f(0.0, 1.0, 0.0, 0.5);
        glColor4fv(arc_color);
        glBegin(GL_LINE_STRIP);
        for (double a=a_begin; a<=a_end; a+=a_step) {
            glVertex2d(scale * cos(a), scale * sin(a));
        }
        glEnd();
    }
}

void GenericHudDevice::AddDetections(int num_detections, double* detections) {
    std::lock_guard<std::mutex> lock(detection_mutex_);
    for (auto d : detection_list_) delete d;
    detection_list_.clear();
    DetectionObject * d = 0;
    for (int i=0; i<num_detections; ++i) {
        d = new DetectionObject(
            detections[2*i + 0],
            detections[2*i + 1]
        );
        detection_list_.push_back(d);
    }
    delete [] detections;
}

void GenericHudDevice::AddStars(int num_detections, double* detections) {
    std::lock_guard<std::mutex> lock(detection_mutex_);
    for (auto s : star_list_) delete s;
    star_list_.clear();
    DetectionObject * d = 0;
    for (int i=0; i<num_detections; ++i) {
        d = new DetectionObject(
            detections[4*i + 0],
            0.0
        );
        star_list_.push_back(d);
    }
    delete [] detections;
}

// Handlers
void GenericHudDevice::dbHandleShipPosition(BusDataInterface *data) {
    BD_BasicPosition *p = static_cast<BD_BasicPosition *>(data);
    if (p != 0) {
        ship_x_ = p->x;
        ship_y_ = p->y;
    }
}

void GenericHudDevice::dbHandleShipAngle(BusDataInterface *data) {
    BD_Scalar *a = static_cast<BD_Scalar *>(data);
    if (a != 0) {
        ship_angle_ = a->value;
    }
}

void GenericHudDevice::dbHandleShipGravity(BusDataInterface *data) {
    BD_Vector *v = static_cast<BD_Vector *>(data);
    if (v != 0) {
        g_x_ = v->x;
        g_y_ = v->y;
    }
}

void GenericHudDevice::dbHandleShipVelocity(BusDataInterface *data) {
    BD_Vector *v = static_cast<BD_Vector *>(data);
    if (v != 0) {
        ship_velocity_x_ = v->x;
        ship_velocity_y_ = v->y;
    }
}

void GenericHudDevice::dbHandleShipThrust(BusDataInterface *data) {
    BD_Scalar *s = static_cast<BD_Scalar *>(data);
    if (s != 0) {
        ship_thrust_force_ = s->value;
    }
}

void GenericHudDevice::dbHandleShipFuelQty(BusDataInterface *data) {
    BD_Scalar *f = static_cast<BD_Scalar *>(data);
    if (f != 0) {
        ship_fuel_percent_ = f->value;
    }
}

void GenericHudDevice::dbHandleDetectionList(BusDataInterface *data) {
    BD_RadarDetectionList *d = static_cast<BD_RadarDetectionList *>(data);
    if (d != 0) {
        AddDetections(d->num_detections, d->data);
    }
}

void GenericHudDevice::dbHandleStarList(BusDataInterface * data) {
    BD_StarDetectionList *d = static_cast<BD_StarDetectionList *>(data);
    if (d != 0) {
        selected_star_ = d->selected_index;
        AddStars(d->num_detections, d->data);
    }
}

void GenericHudDevice::dbHandleShipDamage(BusDataInterface *data) {
    BD_Scalar *dr = static_cast<BD_Scalar *>(data);
    if (dr != 0) {
        damage_ratio_ = dr->value;
    }
}
