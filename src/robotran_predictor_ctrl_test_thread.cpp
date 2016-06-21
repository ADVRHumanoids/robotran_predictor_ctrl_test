#include <yarp/os/all.h>

#include "robotran_predictor_ctrl_test_thread.h"
#include "robotran_predictor_ctrl_test_constants.h"

robotran_predictor_ctrl_test_thread::robotran_predictor_ctrl_test_thread( std::string module_prefix, 
                                                                          yarp::os::ResourceFinder rf, 
                                                                          std::shared_ptr< paramHelp::ParamHelperServer > ph) :
    control_thread( module_prefix, rf, ph ),
    state_input("robotran_predictor", "/state_input"),
    request("robotran_predictor", "/request"),
    prediction("robotran_predictor", "/prediction:i")
{
    // TODO: skeleton constructor
}

bool robotran_predictor_ctrl_test_thread::custom_init()
{
    // state input
    actual_state_input.joint_num = robot.getNumberOfJoints();
    
    actual_state_input.link_pos.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.motor_pos.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.link_vel.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.motor_vel.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.torque.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.pos_ref.resize(actual_state_input.joint_num , 0.0);
    actual_state_input.max_temperature.resize(actual_state_input.joint_num , 0);
    actual_state_input.fault.resize(actual_state_input.joint_num , 0);
    actual_state_input.rtt.resize(actual_state_input.joint_num , 0);
    actual_state_input.op_idx_ack.resize(actual_state_input.joint_num , 0);
    actual_state_input.aux.resize(actual_state_input.joint_num , 0.0);

    // request
    actual_request.process_request = false;  // initially no requests are made
    actual_request.prediction_time = 0.;


    return true;
}

void robotran_predictor_ctrl_test_thread::custom_release()
{
    return;
}


void robotran_predictor_ctrl_test_thread::run()
{
    // update and send input state
    robot_position = robot.sensePosition();   
    robot_torque = robot.senseTorque();
    robot_pos_ref = robot.sensePositionRefFeedback();
        
    actual_state_input.link_pos = robot_position;      // NOTE just a test, i leave empty the rest
    actual_state_input.torque = robot_torque;       // NOTE just a test, i leave empty the rest
    actual_state_input.pos_ref = robot_pos_ref;
    state_input.sendCommand(actual_state_input);

    // if t = ... sends request
    // update and send input state
    request.sendCommand(actual_request);
}    
