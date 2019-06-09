#ifndef BASIC_SHIP_MODEL_H_
#define BASIC_SHIP_MODEL_H_

#include "ShipModelInterface.h"

#include <Box2D.h>

#define NUM_PART_VERTICES 5

class StationInterface;

class BasicShipModel : public ShipModelInterface {
public: // Ship model interface
    void Init(b2World * world, void * user_data);   // SMI_01
    void Update(double delta_time);                 // SMI_02
    void Render();                                  // SMI_03
    bool Dock(StationInterface * station);          // SMI_04
    bool Undock(StationInterface * station);        // SMI_05
    void LandingGear(uint8_t down);                 // SMI_06
    double GetAngle();                              // SMI_07
    void SetAngle(double angle);                    // SMI_08
    b2Vec2 const & GetPosition();                   // SMI_09
    void SetPosition(double x, double y);           // SMI_10
    double GetSpeed();                              // SMI_11
    void GetVelocity(double & x, double & y);       // SMI_12
    double GetMass();                               // SMI_13
    double GetAngularVelocity();                    // SMI_14
    void GetGravity(double & x, double & y);        // SMI_15
    b2Body * GetEngineMount();                      // SMI_16

public:
    BasicShipModel();
    ~BasicShipModel();

private:
    void UpdateGravity();
    void CheckJoints(double delta_time);
    void NormalizeAngle(b2Body* b);
    void NormalizeAngles();

private:
    // Body rotation angles
    // -- main body angle
    double angle_main_;
    // -- upper body angle
    double angle_upper_;
    // -- left landing gear 'bay' body angle
    double angle_left_;
    // -- right landing gear 'bay' body angle
    double angle_right_;
    // -- left landing gear body angle
    double angle_llg_;
    // -- right landing gear body angle
    double angle_rlg_;

    // NOTE : Ship material density (not used with multiple bodies)
    double density_;

    // Ship angular velocity.
    double angular_velocity_;

    // Body positions
    // -- main body position
    b2Vec2 pos_main_;
    // -- upper body position
    b2Vec2 pos_upper_;
    // -- left landing gear bay position
    b2Vec2 pos_left_;
    // -- right landing gear bay position
    b2Vec2 pos_right_;
    // -- left landing gear position
    b2Vec2 pos_llg_;
    // -- right landing gear position
    b2Vec2 pos_rlg_;

    // Velocity vector for ship.
    b2Vec2 velocity_;
    // Box2D world reference.
    b2World * world_;
    // Main part of ship body; camera fixed on this.
    b2Body * body_main_;
    // Upper part of ship body; ship systems (proposal) reside in this.
    b2Body * body_upper_;
    // Left landing gear bay; left landing gear resides here when retracted.
    b2Body * body_left_;
    // Right landing gear bay; right landing gear resides here when retracted.
    b2Body * body_right_;
    // Left Landing Gear itself.
    b2Body * body_left_gear_;
    // Right Landing Gear itself.
    b2Body * body_right_gear_;

    // Vertices for main ship body.
    b2Vec2 v_main_body_[NUM_PART_VERTICES];
    // Vertices for upper ship body.
    b2Vec2 v_upper_body_[NUM_PART_VERTICES];
    // Vertices for left landing gear bay.
    b2Vec2 v_left_body_[NUM_PART_VERTICES];
    // Vertices for right landing gear bay.
    b2Vec2 v_right_body_[NUM_PART_VERTICES];
    // Vertices for left landing gear.
    b2Vec2 v_left_gear_[NUM_PART_VERTICES];
    // Vertices for right landing gear.
    b2Vec2 v_right_gear_[NUM_PART_VERTICES];
    // Joint between main & upper body parts.
    b2WeldJoint *j_upper_;
    // Joint between main & left gear bay body parts.
    b2WeldJoint *j_left_;
    // Joint between main & right gear bay body parts.
    b2WeldJoint *j_right_;
    // Joint between left & right gear bay body parts.
    b2WeldJoint *j_lr_;
    // Suspension joint for left landing gear.
    b2PrismaticJoint *j_llg_;
    // Suspension joint for right landing gear.
    b2PrismaticJoint *j_rlg_;

    // Landing gear up/down control variable.
    // uint8_t landing_gear_state_;
    // StationInterface* station_iface_;
    // bool ship_anchored_;
    b2Joint *anchor_;
    // Ship color.
    float color_[3];

    b2Vec2 g_total_;
};

#endif // BASIC_SHIP_MODEL_H_
