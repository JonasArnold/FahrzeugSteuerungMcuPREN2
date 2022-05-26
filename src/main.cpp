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
#include "state-machine.h"

#ifdef WIFI_ENABLED
  #include <ESPAsyncWebServer.h>
  #include <AsyncElegantOTA.h>
  #include "my-wifi.h"
  #include "credentials.h"
  // Create AsyncWebServer object on port 80
  AsyncWebServer server(80);
#endif

int16_t rpmL, rpmR;
uint8_t speedVal, batPct;
int16_t steeringVal;
bool connected;
int16_t deviationValue;
uint16_t sensorValues[2];
int16_t motorValues[2];

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

  Display_ShowInitText(String("Init deviation controller ..."));
  DeviationController_Init();
  delay(100);

  Display_ShowInitText(String("Init motors..."));
  Motors_Init();

  Display_ShowInitText(String("Init state machine..."));
  StateMachine_Init();
  delay(100);

#ifdef WIFI_ENABLED
  Display_ShowInitText(String("Init WiFi..."));
  MyWiFi_Init();
  bool connected = MyWiFi_Connect(WIFI_SSID, WIFI_PASSPHRASE, 10);
  if(connected) { Display_ShowInitText("WiFi: " + MyWiFi_GetIPv4String()); }
  else {Display_ShowInitText(String("Could not connect to WiFi"));}
  delay(1000);

  // Start ElegantOTA
  AsyncElegantOTA.setID("PREN_T9_ESP32");
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();
#endif


  Display_ShowInitText(String("Init done"));
  delay(2000);
  Display_Clear();

  // device ready
  StateMachine_SetCurrentState(DeviceState::Ready);
}

void loop() {

  // if there is a button event
  if(StartButton_GetEventHappened())
  {
    DeviceState currentState = StateMachine_GetCurrentState();
    // set to normal speed if device is ready or stopped
    if(currentState == DeviceState::Ready || currentState == DeviceState::Stopped)
    {
      StateMachine_SetCurrentState(DeviceState::RedSpeed);
    }
    // stop device if the button is pressed during drive
    if(currentState == DeviceState::NormSpeed || currentState ==  DeviceState::RedSpeed)
    {
      StateMachine_SetCurrentState(DeviceState::Stopped);
    }
    delay(100); // debounce
  }

  /* HANDLE INPUTS */
  Motors_Handle();
  rpmL = int16_t(Motors_GetMMpSL());
  rpmR = int16_t(Motors_GetMMpSR());

  batPct = BatteryMonitoring_GetPercent();
  //Helpers_SerialPrintLnAndVal("Battery: ", batPct);

  // get remote control data (motor test)
  speedVal = RemoteControl_GetThrottle();
  steeringVal = RemoteControl_GetSteering();
  connected = RemoteControl_GetConnectedState();

  /* HANDLE Communication */
  // update outgoing data (state / batteryLevel / speed)
  I2C_SetBatteryLevel((byte)batPct);
  I2C_SetDeviceState((byte)StateMachine_GetCurrentState());
  I2C_SetSetSpeed((byte)StateMachine_GetSpeedApropriateToState());  
#ifdef COMMUNICATION_ENABLED
  Command driveCommand = I2C_Handle();
  switch (driveCommand)
  {
  case SpeedDown:
    StateMachine_SetCurrentState(DeviceState::RedSpeed);
    break;

  case SpeedUp:
    StateMachine_SetCurrentState(DeviceState::NormSpeed);
    break;

  case Stop:    
    StateMachine_SetCurrentState(DeviceState::Stopped);
    break;
  case None:
  default:
    break;
  }
#endif

  // read sensor values and set motor speed 
  deviationValue = CoilSensor_ReadDeviation();
  //Helpers_SerialPrintLnAndVal("Read sensor deviation value: ", deviationValue);
#ifdef REMOTE_CONTROL_ENABLED
  Motors_ForwardAndSteering(speedVal, steeringVal); 
#endif


  CoilSensor_ReadIndividualValues(sensorValues);
#ifdef AUTOMATED_DRIVING_ENABLED
  DeviationController_CalcIndividualMotorPower(StateMachine_GetSpeedApropriateToState(), sensorValues, motorValues);
  Motors_LeftRightIndividual(motorValues[0], motorValues[1]);
#endif

  // update display
  Display_Clear();
  Display_SetupBase();
  Display_UpdateNewValues(StateMachine_GetCurrentState(), batPct, connected, deviationValue, GetUk(), sensorValues[0], sensorValues[1], GetUp(), GetUd());

  delayMicroseconds(400);
}