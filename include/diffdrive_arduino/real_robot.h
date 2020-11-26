#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <serial/serial.h>
#include "arduino_comms.h"
#include <cstring>

class RealRobot : public hardware_interface::RobotHW
{

public:
  struct Config
  {
    std::string left_wheel_name;
    std::string right_wheel_name;
    float loop_rate = 30;
    std::string device = "/dev/ttyUSB0";
    int baud_rate = 57600;
    int timeout = 1000;
  };



private:

  struct Wheel
  {
    int enc;
    double cmd;
    double pos;
    double vel;
    double eff;
    double velSetPt;
    double rads_per_count = 2*M_PI/1920;
    std::string name;


    void Update(int NewEncVal)
    {
      enc = NewEncVal;
      pos = enc * rads_per_count;

    }

    void UpdatePosFromEnc()
    {
      pos = enc * rads_per_count;
    }

  };



public:
  RealRobot(const Config &Cfg);

  void read();
  void write();


  const ros::Time &get_time() { return time; }
  const ros::Duration &get_period() { return period; }

private:
  hardware_interface::JointStateInterface jnt_state_interface;
  hardware_interface::VelocityJointInterface jnt_vel_interface;
  ArduinoComms arduino;

  Wheel l_wheel;
  Wheel r_wheel;

  ros::Time time;
  ros::Duration period;

  float loop_rate;
  
};