#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/* SETTINGS 
*  Comment to disable, Uncomment to enable
*/
#define COMMUNICATION_ENABLED
//#define REMOTE_CONTROL_ENABLED
#define DEBUGGING_ENABLED
#define USING_DISPLAY
#define AUTOMATED_DRIVING_ENABLED
//#define WIFI_ENABLED

/* SPEED LEVELS 0...1000 mm/s */
#define SPEED_HALT 0
#define SPEED_SLOW 300
#define SPEED_FAST 600

// cycle time
const float T_MS = 0.3f;

#endif



