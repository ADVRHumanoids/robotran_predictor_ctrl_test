#include <yarp/os/all.h>
#include <GYM/control_module.hpp>
#include <cstdlib>

#include "robotran_predictor_ctrl_test_module.hpp"

// default module period
#define MODULE_PERIOD 1000 //[millisec]

int main(int argc, char* argv[])
{
    // yarp network declaration and check
    yarp::os::Network yarp;
    if(!yarp.checkNetwork()){
        std::cerr <<"yarpserver not running - run yarpserver"<< std::endl;
        exit(EXIT_FAILURE);
    }
    // yarp network initialization
    yarp.init();

    // create rf
    yarp::os::ResourceFinder rf;
    rf.setVerbose(true);
    // set robotran_predictor_ctrl_test_initial_config.ini as default
    // to specify another config file, run with this arg: --from your_config_file.ini 
    rf.setDefaultConfigFile( "robotran_predictor_ctrl_test_initial_config.ini" ); 
    rf.setDefaultContext( "robotran_predictor_ctrl_test" );  
    rf.configure(argc, argv);
    // create my module
    robotran_predictor_ctrl_test_module robotran_predictor_ctrl_test_mod = robotran_predictor_ctrl_test_module( argc, argv, "robotran_predictor_ctrl_test", MODULE_PERIOD, rf );
    
    // run the module
    robotran_predictor_ctrl_test_mod.runModule( rf );
    
    // yarp network deinitialization
    yarp.fini();
    
    exit(EXIT_SUCCESS);
}
