#ifndef GENERIC_HUD_DEVICE_H_
#define GENERIC_HUD_DEVICE_H_

#include "HudSystemInterface.h"

#include <mutex>
#include <vector>

class DataBus;
class BusDataInterface;
struct DetectionObject;

class GenericHudDevice : public HudSystemInterface {
public:
    GenericHudDevice();
    ~GenericHudDevice();
    void Init(DataBus* bus);
    void Render();

private:
    void RenderHudDial();
    void RenderShipVectors();
    void RenderRadarDetections();
    void RenderFuelIndicator();
    void RenderDamageIndicator();
    void RenderArc(double center_angle, double horizon_angle);
    void AddDetections(int num_detections, double* detections);
    // Handlers
    void dbHandleShipPosition(BusDataInterface *data);
    void dbHandleShipAngle(BusDataInterface *data);
    void dbHandleShipGravity(BusDataInterface *data);
    void dbHandleShipVelocity(BusDataInterface *data);
    void dbHandleShipThrust(BusDataInterface *data);
    void dbHandleShipFuelQty(BusDataInterface *data);
    void dbHandleDetectionList(BusDataInterface *data);
    void dbHandleShipDamage(BusDataInterface *data);

private:
    std::mutex detection_mutex_;
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
    std::vector<DetectionObject *> detection_list_;
};

#endif // GENERIC_HUD_DEVICE_H_
