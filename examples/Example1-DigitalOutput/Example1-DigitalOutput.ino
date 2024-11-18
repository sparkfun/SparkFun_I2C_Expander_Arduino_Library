/*
  Using the PCA9554 -- Digital Output
  By: Nathan Seidle
  SparkFun Electronics
  Date: October 28th, 2024
  
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17047 - Qwiic GPIO
  https://www.sparkfun.com/products/26784 - Qwiic Power Switch
  https://www.sparkfun.com/products/26851 - Qwiic Directional Pad
  
  This example demonstrates how to use pinMode and digitalWrite/write to set output values of the PCA95xx

  Hardware Connections:
  Plug the Qwiic Directional Pad into a Qwiic enabled board (SparkFun Thing Plus C, RedBoard, RedBoard IoT, etc)
  If you don't have a platform with a Qwiic connection use the SparkFun Qwiic Breadboard Jumper (https://www.sparkfun.com/products/17912)
  Open the serial monitor at 115200 baud to see the output
*/

#include <SparkFun_I2C_Expander_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_I2C_Expander_Arduino_Library

SFE_PCA95XX io; // Defaults to the PCA9554 at I2C address 0x20

//SFE_PCA95XX io(PCA95XX_PCA9534); // Create a PCA9534
//SFE_PCA95XX io(PCA95XX_PCA9536); // Create a PCA9536
//SFE_PCA95XX io(PCA95XX_PCA9537); // Create a PCA9537
//SFE_PCA95XX io(PCA95XX_PCA9554); // Create a PCA9554
//SFE_PCA95XX io(PCA95XX_PCA9556); // Create a PCA9556
//SFE_PCA95XX io(PCA95XX_PCA9557); // Create a PCA9557

void setup()
{
  Serial.begin(115200);
  delay(250);
  Serial.println("SparkFun PCA95XX example");

  Wire.begin();

  // Initialize the PCA9554 at its default address (0x20)
  if (io.begin() == false)
  {
    Serial.println("PCA95xx not detected. Please check wiring. Freezing...");
    while (true);
  }

  // pinMode can be used to set an I/O as OUTPUT or INPUT
  for (int i = 0; i < 4; i++)
  {
    io.pinMode(i, OUTPUT);
  }
}

void loop()
{
  for (int i = 0; i < 4; i++)
  {
    io.digitalWrite(i, HIGH); // Turn pin HIGH
    Serial.print("GPIO ");
    Serial.print(i);
    Serial.print(" is now high");
    delay(2000);

    io.digitalWrite(i, LOW); // Turn pin LOW
    Serial.print("GPIO ");
    Serial.print(i);
    Serial.print(" is now low");
    delay(2000);
  }
}