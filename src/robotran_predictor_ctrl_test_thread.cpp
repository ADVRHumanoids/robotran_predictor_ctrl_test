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

    // arm move
    //robot.left_arm.setPositionDirectMode(); // position control
    robot.left_arm.setTorqueMode(); // torque control

    return true;
}

void robotran_predictor_ctrl_test_thread::custom_release()
{
    return;
}


void robotran_predictor_ctrl_test_thread::run()
{
    // send a command (move arm)
    double time = getIterations()*0.05; // [s]
    double signal = ((int)round(time))%2-0.5; // step signal

    double left_offset[7] = {0};//, 45, 2, 330, 20, 22, 0};
    yarp::sig::Vector left_arm_offset(7,left_offset);

    left_offset[3] = signal; // position control

    //left_offset[3] = 10000*signal; // torque control (seems not available)

    robot.left_arm.move(left_arm_offset); // position control

    // update and send input state
    robot_position  = robot.sensePosition();
    robot_velocity  = robot.senseVelocity();
    robot_torque    = robot.senseTorque();
    robot_pos_ref   = robot.sensePositionRefFeedback();
        
    actual_state_input.link_pos     = robot_position;
    actual_state_input.link_vel     = robot_velocity;
    actual_state_input.motor_pos    = robot_position;  // currently no difference between motot and link side (TBD)
    actual_state_input.motor_vel    = robot_velocity;  // currently no difference between motot and link side (TBD)
    actual_state_input.torque       = robot_torque;
    actual_state_input.pos_ref      = robot_pos_ref;

    state_input.sendCommand(actual_state_input);

    if( (getIterations() % 50) == 0 )  // number of iterations (arbitrary)
    {
        // send a request
        actual_request.process_request = true;
        actual_request.prediction_time = 0.1; // [s]
        request.sendCommand(actual_request);
    }

    // get a prediction
    if (actual_request.process_request)
    {

        int seq_num = 1;

        if(prediction.getCommand(actual_prediction, seq_num))
        {
            std::cout << std::endl <<"received prediction, Lknee prediction = " << actual_prediction.left_knee_angle << std::endl<< std::endl;

            actual_request.process_request == false;
        }

    }




}    
