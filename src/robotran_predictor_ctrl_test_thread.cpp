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

    
    return true;
}

void robotran_predictor_ctrl_test_thread::run()
{   
    left_arm_position = robot.left_arm.sensePosition();   
    left_arm_torque = robot.left_arm.senseTorque();
    
    actual_state_input.link_pos = left_arm_position[0];      // NOTE just a test, i leave empty the rest
    actual_state_input.torque = left_arm_torque[0];       // NOTE just a test, i leave empty the rest
    state_input.sendCommand(actual_state_input);
}    
