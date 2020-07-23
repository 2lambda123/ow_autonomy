#ifndef Ow_Interface_H
#define Ow_Interface_H

// Copyright (c) 2018-2020, United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration. All
// rights reserved.

// Interface to lander simulator.  Singleton, because only once instance will
// ever be needed in the current autonomy scheme, which has one autonomy
// executive per lander.

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <ow_autonomy/GuardedMoveAction.h>
#include <control_msgs/JointControllerState.h>
#include <sensor_msgs/JointState.h>
#include <string>

enum class AntennaState;

class OwInterface
{
 public:
  static OwInterface* instance();
  OwInterface ();
  ~OwInterface ();
  OwInterface (const OwInterface&) = delete;
  OwInterface& operator= (const OwInterface&) = delete;
  void initialize ();

  // "Demo" functions, perhaps temporary.
  void digCircularDemo();
  void guardedMoveDemo();
  void guardedMoveActionDemo(); // temporary, proof of concept
  void publishTrajectoryDemo();

  // Operational interface

  // The defaults currently match those of the activity.  When all are used,
  // this function matches guardedMoveDemo above.
  void guardedMove (double target_x = 2,
                    double target_y = 0,
                    double target_z = 0.02,
                    double surf_norm_x = 0,
                    double surf_norm_y = 0,
                    double surf_norm_z = 1,
                    double offset_dist = 0.2,
                    double overdrive_dist = 0.2,
                    bool delete_prev_traj = false,
                    bool retract = false);

  // Temporary, until guardedMove is just a ROS action.
  void guardedMoveAction (double target_x = 2,
                          double target_y = 0,
                          double target_z = 0.02,
                          double surf_norm_x = 0,
                          double surf_norm_y = 0,
                          double surf_norm_z = 1,
                          double offset_dist = 0.2,
                          double overdrive_dist = 0.2,
                          bool delete_prev_traj = false,
                          bool retract = false);

  bool tiltAntenna (double degrees);
  bool panAntenna (double degrees);
  void takePicture ();
  void digLinear (double x, double y, double z, double depth,
                  double length, double width, double pitch, double yaw,
                  double dumpx, double dumpy, double dumpz);
  void takePanorama (double elev_lo, double elev_hi,
                     double lat_overlap, double vert_overlap);

  // State interface
  double getTilt () const;
  double getPanDegrees () const;
  double getPanVelocity () const;
  double getTiltVelocity () const;
  bool imageReceived () const;

  // These methods apply to all "lander operations", and hide their ROS
  // implementation, which could be services, actions, or ad hoc messaging.
  bool running (const std::string& name) const;
  bool finished (const std::string& name) const;
  bool hardTorqueLimitReached (const std::string& joint_name) const;
  bool softTorqueLimitReached (const std::string& joint_name) const;
  void stopOperation (const std::string& name) const;

 private:
  // temporary, proof of concept
  void guardedMoveActionAux (double target_x,
                             double target_y,
                             double target_z,
                             double surf_norm_x,
                             double surf_norm_y,
                             double surf_norm_z,
                             double offset_dist,
                             double overdrive_dist,
                             bool delete_prev_traj,
                             bool retract);
  bool operationRunning (const std::string& name) const;
  bool operationFinished (const std::string& name) const;

  static void jointStatesCallback (const sensor_msgs::JointState::ConstPtr&);
  void tiltCallback (const control_msgs::JointControllerState::ConstPtr& msg);
  void panCallback (const control_msgs::JointControllerState::ConstPtr& msg);

  static OwInterface* m_instance;
  ros::NodeHandle* m_genericNodeHandle;

  // Publishers and subscribers

  ros::Publisher*  m_antennaTiltPublisher;
  ros::Publisher*  m_antennaPanPublisher;
  ros::Publisher*  m_leftImageTriggerPublisher;

  ros::Subscriber* m_antennaPanSubscriber;
  ros::Subscriber* m_antennaTiltSubscriber;
  ros::Subscriber* m_jointStatesSubscriber;
  ros::Subscriber* m_cameraSubscriber;

  // Action clients
  actionlib::SimpleActionClient<ow_autonomy::GuardedMoveAction>
    m_guardedMoveClient;

  // Antenna state - note that pan and tilt can be concurrent.
  double m_currentPan, m_currentTilt;
  double m_goalPan, m_goalTilt; // commanded pan, tilt values
  bool m_prePan, m_preTilt;     // pan, tilt just commanded
  bool m_panning, m_tilting;    // pan, tilt in progress
  Ros::Time m_panStart, m_tiltStart;
};

#endif
