/*
  This is a library written for various 4 and 8-bit I2C I/O Expanders
  SparkFun sells these at its website: www.sparkfun.com
  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14733

  Written by Jim Lindblom @ SparkFun Electronics, May 4th, 2018
  Expanded (hah!) by Nathan Seidle @ SparkFun Electronics, October 28th, 2024

  The PCA9534/TCA9534 is a 8-bit I/O digital expander, and an INT output pin.
  The PCA9536/TCA9536 is a 4-bit I/O digital expander, and an optional INT output pin.
  The PCA9537/TCA9537 is a 4-bit I/O digital expander, and INT/RST pins.

  The PCA9554/TCA9554 is an 8-bit I/O digital expander, and an INT output pin.
  The PCA9556/TCA9556 is an 8-bit I/O digital expander, and a RST input pin.
  The PCA9557/TCA9557 is an 8-bit I/O digital expander, and a RST input pin.

  This library handles the initialization of the PCA95xx and supports standard
  pinMode, digitalRead, and digitalWrite functions.
  
  https://github.com/sparkfunX/SparkFun_I2C_Expander_Arduino_Library

  Development environment specifics:
  Arduino CLI v1.0.4

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SPARKFUN_I2C_EXPANDER_ARDUINO_LIBRARY_H
#define _SPARKFUN_I2C_EXPANDER_ARDUINO_LIBRARY_H

#include "Arduino.h"

#define DEBUG_PCA95XX

#include <Wire.h>

// Supported PCA variants
typedef enum {
  PCA95XX_PCA9534,
  PCA95XX_PCA9536,
  PCA95XX_PCA9537,
  PCA95XX_PCA9554,
  PCA95XX_PCA9556,
  PCA95XX_PCA9557,
} sfe_pca95xx_devices_e;

// Valid PCA95XX addresses
typedef enum
{
    PCA9534_ADDRESS_20 = 0x20,
    PCA9534_ADDRESS_21 = 0x21,
    PCA9534_ADDRESS_22 = 0x22,
    PCA9534_ADDRESS_23 = 0x23,
    PCA9534_ADDRESS_24 = 0x24,
    PCA9534_ADDRESS_25 = 0x25,
    PCA9534_ADDRESS_26 = 0x26,
    PCA9534_ADDRESS_27 = 0x27,

    PCA9536_ADDRESS = 0x41,

    PCA9537_ADDRESS = 0x49,

    PCA9554_ADDRESS_20 = 0x20,
    PCA9554_ADDRESS_21 = 0x21,
    PCA9554_ADDRESS_22 = 0x22,
    PCA9554_ADDRESS_23 = 0x23,
    PCA9554_ADDRESS_24 = 0x24,
    PCA9554_ADDRESS_25 = 0x25,
    PCA9554_ADDRESS_26 = 0x26,
    PCA9554_ADDRESS_27 = 0x27,

    PCA9556_ADDRESS_18 = 0x18,
    PCA9556_ADDRESS_19 = 0x19,
    PCA9556_ADDRESS_1A = 0x1A,
    PCA9556_ADDRESS_1B = 0x1B,
    PCA9556_ADDRESS_1C = 0x1C,
    PCA9556_ADDRESS_1D = 0x1D,
    PCA9556_ADDRESS_1E = 0x1E,
    PCA9556_ADDRESS_1F = 0x1F,

    PCA9557_ADDRESS_18 = 0x18,
    PCA9557_ADDRESS_19 = 0x19,
    PCA9557_ADDRESS_1A = 0x1A,
    PCA9557_ADDRESS_1B = 0x1B,
    PCA9557_ADDRESS_1C = 0x1C,
    PCA9557_ADDRESS_1D = 0x1D,
    PCA9557_ADDRESS_1E = 0x1E,
    PCA9557_ADDRESS_1F = 0x1F,

    PCA95XX_ADDRESS_INVALID = 0xFF
} PCA95XX_Address_t;

// PCA95XX registers:
typedef enum
{
    PCA95XX_REGISTER_INPUT_PORT = 0x00,
    PCA95XX_REGISTER_OUTPUT_PORT = 0x01,
    PCA95XX_REGISTER_POLARITY_INVERSION = 0x02,
    PCA95XX_REGISTER_CONFIGURATION = 0x03,
    PCA95XX_REGISTER_INVALID
} PCA95XX_REGISTER_t;

// PCA95XX error code returns:
typedef enum
{
    PCA95XX_ERROR_READ = -4,
    PCA95XX_ERROR_WRITE = -3,
    PCA95XX_ERROR_INVALID_ADDRESS = -2,
    PCA95XX_ERROR_UNDEFINED = -1,
    PCA95XX_ERROR_SUCCESS = 1
} PCA95XX_error_t;
const PCA95XX_error_t PCA95XX_SUCCESS = PCA95XX_ERROR_SUCCESS;

// PCA95XX invert/normal values:
typedef enum
{
    PCA95XX_RETAIN,    // 0
    PCA95XX_INVERT,    // 1
    PCA95XX_INVERT_END // 2
} PCA95XX_invert_t;

class SFE_PCA95XX
{
  public:
    SFE_PCA95XX(sfe_pca95xx_devices_e device = PCA95XX_PCA9554);

    bool begin(byte deviceAddress = 255, TwoWire &wirePort = Wire, byte numberOfGpio = 255);
    bool isConnected();

    // setDebugStream to enable library debug statements
    void setDebugStream(Stream &debugPort = Serial);

    // pinMode can set a pin (0-3) to INPUT or OUTPUT
    PCA95XX_error_t pinMode(uint8_t pin, uint8_t mode);

    // digitalWrite and write can be used to set a pin HIGH or LOW
    PCA95XX_error_t digitalWrite(uint8_t pin, uint8_t value);
    PCA95XX_error_t write(uint8_t pin, uint8_t value);

    // getInputRegister can be used to read the whole input register
    PCA95XX_error_t getInputRegister(uint8_t *destination); //Returns error if problem
    uint8_t getInputRegister(); //May return erroneous data if read fails

    // read and digitalRead can be used to read a pin
    PCA95XX_error_t read(uint8_t *destination, uint8_t pin);
    uint8_t read(uint8_t pin); //May return erroneous data if read fails

    PCA95XX_error_t digitalRead(uint8_t *destination, uint8_t pin);
    uint8_t digitalRead(uint8_t pin); //May return erroneous data if read fails

    // invert and revert can be used to invert (or not) the I/O logic during a read
    PCA95XX_error_t invert(uint8_t pin, PCA95XX_invert_t inversion = PCA95XX_INVERT);
    PCA95XX_error_t revert(uint8_t pin);

  private:
    TwoWire *_i2cPort; // The generic connection to user's chosen I2C hardware
    Stream *_debugPort;
    PCA95XX_Address_t _deviceAddress;
    byte _numberOfGpio;

    // I2C Read/Write
    PCA95XX_error_t readI2CBuffer(uint8_t *dest, PCA95XX_REGISTER_t startRegister, uint16_t len);
    PCA95XX_error_t writeI2CBuffer(uint8_t *src, PCA95XX_REGISTER_t startRegister, uint16_t len);
    PCA95XX_error_t readI2CRegister(uint8_t *dest, PCA95XX_REGISTER_t registerAddress);
    PCA95XX_error_t writeI2CRegister(uint8_t data, PCA95XX_REGISTER_t registerAddress);
};

#endif //_SPARKFUN_I2C_EXPANDER_ARDUINO_LIBRARY_H