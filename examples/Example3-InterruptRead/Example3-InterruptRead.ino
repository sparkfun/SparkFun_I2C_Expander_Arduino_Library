/*
  Using the PCA9554 with Interrupts
  By: Nathan Seidle
  SparkFun Electronics
  Date: October 28th, 2024

  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17047 - Qwiic GPIO
  https://www.sparkfun.com/products/26784 - Qwiic Power Switch
  https://www.sparkfun.com/products/26851 - Qwiic Directional Pad

  This example demonstrates how to use interrupts to check for new pin changes on the Qwiic Directional Pad

  Hardware Connections:
  Plug the Qwiic Directional Pad into a Qwiic enabled board (SparkFun Thing Plus C, RedBoard, RedBoard IoT, etc)
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/17912)
  Connect a wire from a GPIO pin that is interrupt capable to the !INT pin on the Qwiic Directional Pad
  Open the serial monitor at 115200 baud to see the output
*/

#include <SparkFun_I2C_Expander_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_I2C_Expander_Arduino_Library

//SFE_PCA95XX io(PCA95XX_PCA9534); // Create a PCA9534 - Has interrupt pin
//SFE_PCA95XX io(PCA95XX_PCA9536); // Create a PCA9536 - Has optional interrupt pin
//SFE_PCA95XX io(PCA95XX_PCA9537); // Create a PCA9537 - Has interrupt pin
SFE_PCA95XX io(PCA95XX_PCA9554); // Create a PCA9554 - Has interrupt pin
//SFE_PCA95XX io(PCA95XX_PCA9556); // Create a PCA9556 - No interrupt pin / Not compatible with this code
//SFE_PCA95XX io(PCA95XX_PCA9557); // Create a PCA9557 - No interrupt pin / Not compatible with this code

int gpioExpander_up = 0;
int gpioExpander_down = 1;
int gpioExpander_right = 2;
int gpioExpander_left = 3;
int gpioExpander_center = 4;

int gpioExpander_interrupt = 14; //INT of PCA9554 is connected to ESP pin 14. Goes low when an input pin changes.

bool buttonPressed = false;

//Interrupt that is called when INT pin goes low
void IRAM_ATTR gpioExpanderISR()
{
  buttonPressed = true;
}

void setup()
{
  Serial.begin(115200);
  delay(250);
  Serial.println("Qwiic Directional Pad Example");

  Wire.begin();

  // Initialize the PCA95xx with its default I2C address
  if (io.begin() == false)
  {
    Serial.println("PCA9554 not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  pinMode(gpioExpander_interrupt, INPUT_PULLUP);

  io.pinMode(gpioExpander_up, INPUT);
  io.pinMode(gpioExpander_down, INPUT);
  io.pinMode(gpioExpander_left, INPUT);
  io.pinMode(gpioExpander_right, INPUT);
  io.pinMode(gpioExpander_center, INPUT);

  Serial.println("Qwiic Directional Pad online!");

  attachInterrupt(gpioExpander_interrupt, gpioExpanderISR, CHANGE);
}

void loop()
{
  //Wait for interrupt to fire
  if (buttonPressed == true)
  {
    Serial.print("Interrupt! ");
    //Get all the pins in one read to save time
    uint8_t currentState = io.getInputRegister();

    // Pins are pulled high so when we see low, button is being pressed
    if ((currentState & (1 << gpioExpander_up)) == 0)
      Serial.print("Up");
    if ((currentState & (1 << gpioExpander_down)) == 0)
      Serial.print("Down");
    if ((currentState & (1 << gpioExpander_left)) == 0)
      Serial.print("Left");
    if ((currentState & (1 << gpioExpander_right)) == 0)
      Serial.print("Right");
    if ((currentState & (1 << gpioExpander_center)) == 0)
      Serial.print("Center");

    Serial.println();

    buttonPressed = false;
  }
}