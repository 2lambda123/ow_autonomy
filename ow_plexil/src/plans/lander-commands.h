// The Notices and Disclaimers for Ocean Worlds Autonomy Testbed for Exploration
// Research and Simulation can be found in README.md in the root directory of
// this repository.

#ifndef Ow_commands_H
#define Ow_commands_H

// All available PLEXIL commands to the lander.  This is essentially the
// lander's command interface.

// Note that in PLEXIL, commands are asynchronous by design.
// Typically, OceanWATERS plans should not call the following commands
// directly, but instead use the Library interface (defined in
// plan-interface.h) that wraps these commands in a way that they
// called synchronously.

Command camera_set_exposure (Real seconds);
Command camera_capture ();

Command pan_tilt (Real pan_degrees, Real tilt_degrees);

Command arm_move_joint (Boolean relative,
                        Integer joint,
                        Real angle);

Command arm_move_joints (Boolean relative,
                         Real angles[6]);

Command scoop_circular (Integer frame,
                        Boolean relative,
                        Real x,
                        Real y,
                        Real z,
                        Real depth,
                        Boolean parallel); // true means parallel to lander arm

Command scoop_linear (Integer frame,
                      Boolean relative,
                      Real x,
                      Real y,
                      Real z,
                      Real depth,
                      Real length);

Command deliver ();

Command discard_sample (Integer frame,
                        Boolean relative,
                        Real x,
                        Real y,
                        Real z,
                        Real height);

Command grind (Real x,
               Real y,
               Real depth,
               Real length,
               Boolean parallel,
               Real ground_pos);

Command guarded_move (Real x,
                      Real y,
                      Real z,
                      Real dir_x,
                      Real dir_y,
                      Real dir_z,
                      Real search_distance);

// Move from stowed position to a "ready" position
Command unstow();

// Move from "ready" position to stowed position; requires unstow() first
Command stow();

// Processes number of images already taken with the stereo camera to
// find the 3d point to sample.  filter_type can either be "Dark" or
// "Brown".  (Dark chooses dark spots, brown chooses brown spots).
Real [3] Command identify_sample_location(Integer num_images, String filter_type);

// Set spotlight intensity
Command light_set_intensity (String side,     // "left" or "right"
			     Real intensity); // 0.0 to 1.0.  0 is off.

#endif
