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

#include <SparkFun_I2C_Expander_Arduino_Library.h>

#ifdef DEBUG_PCA95XX
#define PCA95XX_DEBUG(x)                                                                                               \
    if (_debugPort != NULL)                                                                                            \
    {                                                                                                                  \
        _debugPort->print(x);                                                                                          \
    }
#define PCA95XX_DEBUGLN(x)                                                                                             \
    if (_debugPort != NULL)                                                                                            \
    {                                                                                                                  \
        _debugPort->println(x);                                                                                        \
    }
#define STORAGE(x) (x)
#else
#define PCA95XX_DEBUG(x)
#define PCA95XX_DEBUGLN(x)
#define STORAGE(x) (x)
#endif

// Constructor
SFE_PCA95XX::SFE_PCA95XX(sfe_pca95xx_devices_e device)
{
    _i2cPort = NULL;
    _debugPort = NULL;

    switch (device)
    {
    default: // Default is the PCA9554
        _deviceAddress = PCA9554_ADDRESS_20;
        _numberOfGpio = 8;
        _deviceType = PCA95XX_PCA9554;
        break;

    case PCA95XX_PCA9534:
        _deviceAddress = PCA9534_ADDRESS_20;
        _numberOfGpio = 8;
        _deviceType = PCA95XX_PCA9534;
        break;

    case PCA95XX_PCA9536:
        _deviceAddress = PCA9536_ADDRESS;
        _numberOfGpio = 4;
        _deviceType = PCA95XX_PCA9536;
        break;

    case PCA95XX_PCA9537:
        _deviceAddress = PCA9537_ADDRESS;
        _numberOfGpio = 4;
        _deviceType = PCA95XX_PCA9537;
        break;

    case PCA95XX_PCA9554:
        _deviceAddress = PCA9554_ADDRESS_20;
        _numberOfGpio = 8;
        _deviceType = PCA95XX_PCA9554;
        break;

    case PCA95XX_PCA9556:
        _deviceAddress = PCA9556_ADDRESS_18;
        _numberOfGpio = 8;
        _deviceType = PCA95XX_PCA9556;
        break;

    case PCA95XX_PCA9557:
        _deviceAddress = PCA9557_ADDRESS_18;
        _numberOfGpio = 8;
        _deviceType = PCA95XX_PCA9557;
        break;
    }
}

bool SFE_PCA95XX::begin(byte deviceAddress, TwoWire &wirePort, byte numberOfGpio)
{
    _i2cPort = &wirePort;

    // Only allow deviceAddress to override constructor defaults if it is explicitly set
    if (deviceAddress != 255)
        _deviceAddress = (PCA95XX_Address_t)deviceAddress;

    // Only allow numberOfGpio to override constructor defaults if it is explicitly set
    if (numberOfGpio != 255)
        _numberOfGpio = numberOfGpio;

    return (isConnected());
}

// Returns true if a device ack's at given address
bool SFE_PCA95XX::isConnected(void)
{
    _i2cPort->beginTransmission((uint8_t)_deviceAddress);
    return (_i2cPort->endTransmission() == 0);
}

void SFE_PCA95XX::setDebugStream(Stream &debugPort)
{
    _debugPort = &debugPort;
}

PCA95XX_error_t SFE_PCA95XX::pinMode(uint8_t pin, uint8_t mode)
{
    PCA95XX_error_t err;
    uint8_t cfgRegister = 0;

    if (pin >= _numberOfGpio)
        return PCA95XX_ERROR_UNDEFINED;

    err = readI2CRegister(&cfgRegister, PCA95XX_REGISTER_CONFIGURATION);
    if (err != PCA95XX_ERROR_SUCCESS)
    {
        return err;
    }
    cfgRegister &= ~(1 << pin); // Clear pin bit
    if (mode == INPUT)          // Set the bit if it's being set to INPUT (opposite of Arduino)
    {
        cfgRegister |= (1 << pin);
    }
    return writeI2CRegister(cfgRegister, PCA95XX_REGISTER_CONFIGURATION);
}

PCA95XX_error_t SFE_PCA95XX::write(uint8_t pin, uint8_t value)
{
    PCA95XX_error_t err;
    uint8_t outputRegister = 0;

    if (pin >= _numberOfGpio)
        return PCA95XX_ERROR_UNDEFINED;

    err = readI2CRegister(&outputRegister, PCA95XX_REGISTER_OUTPUT_PORT);
    if (err != PCA95XX_ERROR_SUCCESS)
    {
        return err;
    }
    // TODO: Break out of here if it's already set correctly
    outputRegister &= ~(1 << pin); // Clear pin bit
    if (value == HIGH)             // Set the bit if it's being set to HIGH (opposite of Arduino)
    {
        outputRegister |= (1 << pin);
    }
    return writeI2CRegister(outputRegister, PCA95XX_REGISTER_OUTPUT_PORT);
}

PCA95XX_error_t SFE_PCA95XX::digitalWrite(uint8_t pin, uint8_t value)
{
    return write(pin, value);
}

// Safe reading of input register
PCA95XX_error_t SFE_PCA95XX::getInputRegister(uint8_t *destination)
{
    PCA95XX_error_t err;
    uint8_t inputRegister = 0;

    err = readI2CRegister(&inputRegister, PCA95XX_REGISTER_INPUT_PORT);
    if (err != PCA95XX_ERROR_SUCCESS)
        return err;

    *destination = inputRegister;
    return (err);
}

// Unsafe overload
uint8_t SFE_PCA95XX::getInputRegister()
{
    uint8_t val;
    if (getInputRegister(&val) == PCA95XX_ERROR_SUCCESS)
        return val;
    return 0; // Unsafe
}

// Safe reading of a pin
PCA95XX_error_t SFE_PCA95XX::read(uint8_t *destination, uint8_t pin)
{
    PCA95XX_error_t err;
    uint8_t inputRegister = 0;

    if (pin >= _numberOfGpio)
        return PCA95XX_ERROR_UNDEFINED;

    err = readI2CRegister(&inputRegister, PCA95XX_REGISTER_INPUT_PORT);
    if (err != PCA95XX_ERROR_SUCCESS)
        return err;

    *destination = (inputRegister & (1 << pin)) >> pin;
    return (err);
}

// Unsafe overload
uint8_t SFE_PCA95XX::read(uint8_t pin)
{
    uint8_t val;
    if (read(&val, pin) == PCA95XX_ERROR_SUCCESS)
        return val;
    return 0; // Unsafe
}

// Safe reading of a pin
PCA95XX_error_t SFE_PCA95XX::digitalRead(uint8_t *destination, uint8_t pin)
{
    return (read(destination, pin));
}

// Unsafe overload
uint8_t SFE_PCA95XX::digitalRead(uint8_t pin)
{
    uint8_t val;
    if (digitalRead(&val, pin) == PCA95XX_ERROR_SUCCESS)
        return val;
    return 0; // Unsafe
}

PCA95XX_error_t SFE_PCA95XX::invert(uint8_t pin, PCA95XX_invert_t inversion)
{
    PCA95XX_error_t err;
    uint8_t invertRegister = 0;

    if (pin >= _numberOfGpio)
        return PCA95XX_ERROR_UNDEFINED;

    err = readI2CRegister(&invertRegister, PCA95XX_REGISTER_POLARITY_INVERSION);
    if (err != PCA95XX_ERROR_SUCCESS)
    {
        return err;
    }
    // TODO: Break out of here if it's already set correctly
    invertRegister &= ~(1 << pin);   // Clear pin bit
    if (inversion == PCA95XX_INVERT) // Set the bit if it's being set to inverted
    {
        invertRegister |= (1 << pin);
    }
    return writeI2CRegister(invertRegister, PCA95XX_REGISTER_POLARITY_INVERSION);
}

PCA95XX_error_t SFE_PCA95XX::revert(uint8_t pin)
{
    return invert(pin, PCA95XX_RETAIN);
}

PCA95XX_error_t SFE_PCA95XX::readI2CBuffer(uint8_t *dest, PCA95XX_REGISTER_t startRegister, uint16_t len)
{
    PCA95XX_DEBUGLN((STORAGE("(readI2CBuffer): read ") + String(len) + STORAGE(" @ 0x") + String(startRegister, HEX)));
    if (_deviceAddress == PCA95XX_ADDRESS_INVALID)
    {
        PCA95XX_DEBUGLN(STORAGE("    ERR (readI2CBuffer): Invalid address"));
        return PCA95XX_ERROR_INVALID_ADDRESS;
    }
    _i2cPort->beginTransmission((uint8_t)_deviceAddress);
    _i2cPort->write(startRegister);
    if (_i2cPort->endTransmission(false) != 0)
    {
        PCA95XX_DEBUGLN(STORAGE("    ERR (readI2CBuffer): End transmission"));
        return PCA95XX_ERROR_READ;
    }

    _i2cPort->requestFrom((uint8_t)_deviceAddress, (uint8_t)len);
    for (int i = 0; i < len; i++)
    {
        dest[i] = _i2cPort->read();
        PCA95XX_DEBUGLN((STORAGE("    ") + String(i) + STORAGE(": 0x") + String(dest[i], HEX)));
    }

    return PCA95XX_ERROR_SUCCESS;
}

PCA95XX_error_t SFE_PCA95XX::writeI2CBuffer(uint8_t *src, PCA95XX_REGISTER_t startRegister, uint16_t len)
{
    if (_deviceAddress == PCA95XX_ADDRESS_INVALID)
    {
        PCA95XX_DEBUGLN(STORAGE("ERR (readI2CBuffer): Invalid address"));
        return PCA95XX_ERROR_INVALID_ADDRESS;
    }
    _i2cPort->beginTransmission((uint8_t)_deviceAddress);
    _i2cPort->write(startRegister);
    for (int i = 0; i < len; i++)
    {
        _i2cPort->write(src[i]);
    }
    if (_i2cPort->endTransmission(true) != 0)
    {
        return PCA95XX_ERROR_WRITE;
    }
    return PCA95XX_ERROR_SUCCESS;
}

PCA95XX_error_t SFE_PCA95XX::readI2CRegister(uint8_t *dest, PCA95XX_REGISTER_t registerAddress)
{
    PCA95XX_error_t result = readI2CBuffer(dest, registerAddress, 1);

    // Interrupt Errata: User must change command byte to something besides 00h after a Read operation
    if (_deviceType == PCA95XX_PCA9554 && registerAddress == PCA95XX_REGISTER_INPUT_PORT)
    {
        // If we've just read the INPUT Port register, we must change command byte
        writeI2CBuffer(0, PCA95XX_REGISTER_OUTPUT_PORT, 0); // Write the command register, but no data
    }
    return result;
}

PCA95XX_error_t SFE_PCA95XX::writeI2CRegister(uint8_t data, PCA95XX_REGISTER_t registerAddress)
{
    PCA95XX_error_t result = writeI2CBuffer(&data, registerAddress, 1);

    // Interrupt Errata: User must change command byte to something besides 00h after a Read operation
    if (_deviceType == PCA95XX_PCA9554 && registerAddress == PCA95XX_REGISTER_INPUT_PORT)
    {
        // If we've just read the INPUT Port register, we must change command byte
        writeI2CBuffer(0, PCA95XX_REGISTER_OUTPUT_PORT, 0); // Write the command register, but no data
    }
    return result;
}
