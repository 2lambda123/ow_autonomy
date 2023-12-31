// The Notices and Disclaimers for Ocean Worlds Autonomy Testbed for Exploration
// Research and Simulation can be found in README.md in the root directory of
// this repository.

#ifndef Ow_Plan_Interface_H
#define Ow_Plan_Interface_H

// PLEXIL interface to lander: commands, lookups, library plans, PLEXIL utilities

#include "lander-commands.h"

// Reference frames
#define BASE_LINK 0
#define SCOOP_TIP 1

// Joint names, defined by their indices in the /joint_states ROS message.

#define ANTENNA_PAN    0
#define ANTENNA_TILT   1
#define DISTAL_PITCH   2
#define GRINDER        3
#define HAND_YAW       4
#define PROXIMAL_PITCH 5
#define SCOOP_YAW      6
#define SHOULDER_PITCH 7
#define SHOULDER_YAW   8


// Utility commands; issue ROS_INFO, ROS_WARN, and ROS_ERROR, respectively.
Command log_info (...);
Command log_warning (...);
Command log_error (...);
Command log_debug (...);


// PLEXIL library for lander operations.

LibraryAction ArmFindSurface (In Integer Frame,
                              In Boolean Relative,
                              In Real PosX,
                              In Real PosY,
                              In Real PosZ,
                              In Real NormX,
                              In Real NormY,
                              In Real NormZ,
                              In Real Distance,
                              In Real Overdrive,
                              In Real ForceThreshold,
                              In Real TorqueThreshold);

LibraryAction ArmMoveCartesian (In Integer Frame,
				In Boolean Relative,
				In Real X, In Real Y, In Real Z, // Euler angle
				In Real OrientX,
				In Real OrientY,
				In Real OrientZ);

// Quaternion version of previous.
LibraryAction ArmMoveCartesian_Q (In Integer Frame,
				  In Boolean Relative,
				  In Real X, In Real Y, In Real Z,
				  In Real OrientX, In Real OrientY,
				  In Real OrientZ, In Real OrientW);

LibraryAction ArmMoveCartesianGuarded (In Integer Frame,
                                       In Boolean Relative,
                                       // Euler angle:
                                       In Real X, In Real Y, In Real Z,
                                       In Real OrientX,
                                       In Real OrientY,
                                       In Real OrientZ,
                                       In Real ForceThreshold,
                                       In Real TorqueThreshold);

// Quaternion version of previous.
LibraryAction ArmMoveCartesianGuarded_Q (In Integer Frame,
                                         In Boolean Relative,
                                         In Real X, In Real Y, In Real Z,
                                         In Real OrientX, In Real OrientY,
                                         In Real OrientZ, In Real OrientW,
                                         In Real ForceThreshold,
                                         In Real TorqueThreshold);

LibraryAction LightSetIntensity (In String Side, In Real Intensity);
LibraryAction Pan (In Real Degrees);
LibraryAction Tilt (In Real Degrees);
LibraryAction PanTiltMoveJoints (In Real PanDegrees, In Real TiltDegrees);
LibraryAction PanTiltMoveCartesian  (In Integer Frame,
                                     In Real X, In Real Y, In Real Z);
LibraryAction ArmStow ();
LibraryAction ArmUnstow ();
LibraryAction GuardedMove (In Real X,
                           In Real Y,
                           In Real Z,
                           In Real DirX,
                           In Real DirY,
                           In Real DirZ,
                           In Real SearchDistance);

LibraryAction ArmMoveJoint (In Boolean Relative,
                            In Integer Joint,
                            In Real Angle);

LibraryAction ArmMoveJoints (In Boolean Relative,
                             In Real Angles[6]);

LibraryAction ArmMoveJointsGuarded (In Boolean Relative,
                                    In Real Angles[6],
                                    In Real ForceThreshold,
                                    In Real TorqueThreshold);

LibraryAction ArmStop ();

LibraryAction DockIngestSample ();

LibraryAction TaskGrind (In Real X,
                         In Real Y,
                         In Real Depth,
                         In Real Length,
                         In Boolean Parallel,
                         In Real GroundPos);

LibraryAction TaskScoopCircular (In Integer Frame,
                                 In Boolean Relative,
                                 In Real X,
                                 In Real Y,
                                 In Real Z,
                                 In Real Depth,
                                 In Boolean Parallel);

LibraryAction TaskScoopLinear (In Integer Frame,
                               In Boolean Relative,
                               In Real X,
                               In Real Y,
                               In Real Z,
                               In Real Depth,
                               In Real Length);

LibraryAction TaskDeliverSample ();

LibraryAction TaskDiscardSample (In Integer Frame,
                                 In Boolean Relative,
                                 In Real X,
                                 In Real Y,
                                 In Real Z,
                                 In Real Height);

LibraryAction CameraSetExposure (In Real Seconds);
LibraryAction CameraCapture ();

// Lander queries

Real Lookup StateOfCharge;
Real Lookup RemainingUsefulLife;
Real Lookup BatteryTemperature;
Boolean Lookup HardTorqueLimitReached (String joint_name);
Boolean Lookup SoftTorqueLimitReached (String joint_name);

// Faults
Boolean Lookup SystemFault;
Boolean Lookup AntennaFault;
Boolean Lookup AntennaPanFault;
Boolean Lookup AntennaTiltFault;
Boolean Lookup ArmFault;
Boolean Lookup PowerFault;
Boolean Lookup CameraFault;

// Relevant with GuardedMove only:
Boolean Lookup GroundFound;
Real    Lookup GroundPosition;

// Antenna
Real Lookup PanRadians;
Real Lookup PanDegrees;
Real Lookup TiltRadians;
Real Lookup TiltDegrees;

// Joints
Real Lookup JointAcceleration (Integer joint);
Real Lookup JointVelocity     (Integer joint);
Real Lookup JointPosition     (Integer joint);
Real Lookup JointEffort       (Integer joint);

// Misc

Real[6] Lookup ArmEndEffectorForceTorque;
Real[7] Lookup ArmPose;
Boolean Lookup UsingOceanWATERS;
Boolean Lookup UsingOWLAT;

// Query whether a given operation is running.  Uses the operation names as
// defined in OwInterface.cpp.  Generally not needed, but supports more
// fine-grained control of concurrency.
Boolean Lookup Running (String operation_name);

// Query the goal status of the ROS action corresponding to a given library action
Integer Lookup ActionGoalStatus (String action_name);

// Function
Boolean Lookup AnglesEquivalent (Real deg1, Real deg2, Real tolerance);


//////// PLEXIL Utilities

// Predefined, PLEXIL variable for current time.
Real Lookup time;

// String operations
String Lookup ToString(...);
Boolean Lookup StringToBoolean(String);
Integer Lookup StringToInteger(String);
Integer Lookup StringToReal(String);
String Lookup substr(...);
Integer Lookup find_first_of(...);
Integer Lookup find_last_of(...);

// Checkpointing interface
Command set_checkpoint(String,Boolean,String);
Command flush_checkpoints;
Command set_boot_ok();
Integer Lookup CheckpointWhen(String);
Integer Lookup NumberOfUnhandledBoots;
Boolean Lookup IsBootOK(Integer);
Boolean Lookup DidCrash;

#endif
