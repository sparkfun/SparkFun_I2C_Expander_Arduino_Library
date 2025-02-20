SparkFun I2C Expander Library
==============================

<table class="table table-hover table-striped table-bordered">
  <tr align="center">
   <td><a href="https://www.sparkfun.com/products/26784"><img src="https://cdn.sparkfun.com/r/455-455/assets/parts/2/7/8/6/7/PRT-26784-Qwiic-Power-Switch-Feature.jpg"></a></td>
   <td><a href="https://www.sparkfun.com/products/17047"><img src="https://cdn.sparkfun.com/r/455-455/assets/parts/1/5/9/9/3/17047-SparkFun_Qwiic_GPIO-01.jpg"></a></td>
    <td><a href="https://www.sparkfun.com/products/14733"><img src="https://cdn.sparkfun.com/r/455-455/assets/parts/1/2/9/6/0/14733-RGB_Sensor__Qwiic__-_BH1749NUC-01.jpg"></a></td>
    <td><a href="https://www.sparkfun.com/products/26851"><img src="https://cdn.sparkfun.com/r/455-455/assets/parts/2/7/9/6/9/26851-Qwiic-Directional-Pad-Feature.jpg"></a></td>
    <td><a href="https://www.sparkfun.com/sparkfun-qwiic-navigation-switch.html"><img src="https://www.sparkfun.com/media/catalog/product/cache/a793f13fd3d678cea13d28206895ba0c/p/r/prt-27576-qwiic-navigation-switch-feature.jpg" width="455px"></a></td>
  </tr>
  <tr align="center">
    <td><a href="https://www.sparkfun.com/products/26784"><i>SparkFun Qwiic Power Switch<br />(PRT-26784)</i></a></td>
    <td><a href="https://www.sparkfun.com/products/17047"><i>SparkFun Qwiic GPIO<br />(DEV-17047)</i></a></td>
    <td><a href="https://www.sparkfun.com/products/14733"><i>SparkFun RGB Sensor - BH1749NUC<br />(SPX-14733)</i></a></td>
    <td><a href="https://www.sparkfun.com/products/26851"><i>SparkFun Qwiic Directional Pad<br />(PRT-26851)</i></a></td>
    <td><a href="https://www.sparkfun.com/sparkfun-qwiic-navigation-switch.html"><i>SparkFun Qwiic Navigational Switch<br />(PRT-27576)</i></a></td>
  </tr>
</table>

The **SparkFun I2C Expander Library** makes working with 4-bit and 8-bit I2C digital I/O expanders very easy. These expanders can read or write four or eight separate I/O.

The following ICs are supported: 

* PCA9534/TCA9534 
* PCA9536/TCA9536
* PCA9537/TCA9537
* PCA9554/TCA9554
* PCA9556/TCA9556
* PCA9557/TCA9557
* And maybe others... 

This library supports using alternate Wire ports (ie, Wire1, etc) as well as alternate I2C addresses and includes support for interrupt pin errata (PCA9554).

This library supports standard `pinMode`, `digitalRead`, and `digitalWrite` functions.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Products That Use This Library
-------------------
* [PRT-26784](https://www.sparkfun.com/products/26784) - SparkFun Qwiic Power Switch
* [DEV-17047](https://www.sparkfun.com/products/17047) - SparkFun Qwiic GPIO
* [SPX-14733](https://www.sparkfun.com/products/14733) - SparkFun RGB Sensor - BH1749NUC
* [PRT-26851](https://www.sparkfun.com/products/26851) - SparkFun Qwiic Directional Pad
* [PRT-27576](https://www.sparkfun.com/sparkfun-qwiic-navigation-switch.html) - SparkFun Qwiic Navigational Switch

License Information
-------------------

This product is _**open source**_! 

Various bits of the code have different licenses applied. Anything SparkFun wrote is beerware; if you see me (or any other SparkFun employee) at the local, and you've found our code helpful, please buy us a round!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release anything derivative under the same license.

Distributed as-is; no warranty is given.

- Your friends at SparkFun.
