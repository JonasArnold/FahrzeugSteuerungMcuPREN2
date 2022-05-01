#include <Arduino.h>
#include "configuration.h"
#include "helpers.h"
#include "display.h"
#include "coil-sensor.h"
#include "motors.h"
#include "i2c.h"
#include "remote-control.h"
#include "start-button.h"
#include "battery-monitoring.h"
#include "deviation-controller.h"

int16_t sensorValue, rpmL, rpmR;
uint8_t speedVal, batPct;
int8_t steeringVal;
uint8_t driveCommand;
bool startButtonPressed = false;
bool connected;

void setup() {
  // put your setup code here, to run once:
#ifdef DEBUGGING_ENABLED
  Serial.begin(115200);
#endif
  Display_Init();
  CoilSensor_Init();

#ifdef COMMUNICATION_ENABLED
  Display_ShowInitText(String("Init i2c communication..."));
  I2C_Init();
#endif

  Display_ShowInitText(String("Init battery monitoring..."));
  BatteryMonitoring_Init();
  delay(100);

  Display_ShowInitText(String("Init start button..."));
  StartButton_Init();
  delay(100);

  Display_ShowInitText(String("Init remote control..."));
  RemoteControl_Init();
  delay(100);

  Display_ShowInitText(String("Init motors..."));
  Motors_Init();

  Display_ShowInitText(String("Init done"));
  delay(2000);
  Display_Clear();

  // TEST_I2C VAR
  set_state(Ready); set_batteryLevel(0); set_speed(1000);
}

void loop() {

  // ausgehende Daten aktualisieren (state/batterylevel/speed)

  // while(!startButtonPressed) {
  //   // ausgehende Daten aktualisieren (state/batterylevel/speed)
  //   if(StartButton_GetState() == true) {
  //     startButtonPressed = true;
  //     Display_ShowText(15, 15, String("Start-Button pressed"));
  //     delay(5000);
  //     Display_Clear();
  //   }
  // }
  

#ifdef COMMUNICATION_ENABLED
  // handle all periodic stuff
  driveCommand = I2C_Handle();
  if(driveCommand) {
    /* transfer command immediately to drive controller */
    Display_ShowText(15, 15, String("Current command: " + driveCommand));
    delay(5000); // NUR ZUM TESTEN!!!
  }

  /* update outgoing data (state / batteryLevel / speed) */
  // Test_I2C
  set_state(get_state());
  set_batteryLevel(get_batteryLevel() + 2);
  set_speed(get_speed());
  delay(2000);
  // TEST_I2C END
#endif
  
  Motors_Handle();
  rpmL = int16_t(Motors_GetMMpSL());
  rpmR = int16_t(Motors_GetMMpSR());

  batPct = BatteryMonitoring_GetPercent();
  Helpers_SerialPrintLnAndVal("Battery: ", batPct);

  // read coil sensor
  sensorValue = CoilSensor_Read();
  Helpers_SerialPrintLnAndVal("Read sensor value: ", sensorValue);

  // get remote control data (motor test)
  speedVal = RemoteControl_GetThrottle();
  steeringVal = RemoteControl_GetSteering();
  connected = RemoteControl_GetConnectedState();

  // set motor speed
  Motors_ForwardAndSteering(speedVal, DeviationController_CalcSteering(sensorValue));  

  // update display
  Display_Clear();
  Display_SetupBase();
  Display_UpdateNewValues(String(DeviceState::Ready), batPct, connected, sensorValue, speedVal, steeringVal, rpmL, rpmR);

  delay(50);
}