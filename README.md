# LED_Show_BMP
WeMos D1 BMP Show to NeoMatrix

This project uses the SPIFFS filesystem of the WeMos D1 to store BMP files and display them on a NeoPixel Matrix
This was built on a 10x10 matrix with limited testing on other configuration

## Configuration options
I tired to keep most parameters as #define at the top of the code
  * LED_PIN indicates the pin connected to the Neopixel Matrix Data Line
  * MODE_SWITCH
  * MODE_LED
  * MATRIX_WIDTH is self explanatory
  * MATRIX_WIDTH is set in as a \#define
  * MATRIX_HEIGHT is set in as a \#define
  * LED_PIN is set in as a \#define
  * MATRIX_LAYOUT_FLAGS: add together as needed *(e.g. NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG)*
    * NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:     
     Position of the FIRST LED in the FIRST MATRIX;   
     Pick two
       
    * NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS :   
     LEDs within matrix are arranged in horizontal rows or in vertical columns, respectively;  
     Pick one or the other.     
       
    * NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG:   
      all rows/columns WITHIN EACH MATRIX proceed in the same order, or alternate lines reverse direction;  
      Pick one.
        
    * NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:  
      Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY;  
      Pick two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.

    * NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG:  
      The ROWS/COLUMS OF MATRICES(tiles) in the OVERALL DISPLAY proceed in the same order for every line, or alternate lines reverse direction;  
      When using zig-zag order, the orientation of the matrices in alternate rows will be rotated 180 degrees (this is normal -- simplifies wiring).  
      Pick one.  
  * MATRIX_TYPE_FLAGS : add together as needeed *(e.g. NEO_RGB + NEO_KHZ800)*
    * NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
    * NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
    * NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
    * NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

## Operating modes
There is to operating mode controlled by a switch on the breadboard

By default, the code enters display mode
To enter FTP Mode, the mode button has to be held while the module is reset 

### Display mode
This mode reads the SPIFFS filesystem sequentially and displays the BMP images to the Neopixel Matrix

### FTP Mode
This mode enables the onboard wifi and connects to the last saved network by default.
If connections fails (either the network is not there or no saved network are found), the WiFi Manager interface loads up and allow the user to connect through wifi and connect to a network.

#### WiFi Manager
On connection failure, the Wifi manager will fallback to AP mode and initialise a network with a SSID with the module serial number.

It is then possible to connect and connect to the module IP adress to get a web interface for wifi configuration.
The IP address is output to the Serial console.

#### Reseting stored WiFi credentials
It is possible to reset the stored wifi credential by holding the mode switch for 5 seconds while resetting the wemos.

The onboard LED will flash until credential are erased. It will then turn back on to confirm reset is done.

#### FTP Connection
Any standard FTP software can be used to connect to the wemos.

However the following things must be taken into account  
  * **Transfer must be done in PASV mode** 
  * **FTP client must be set to allow only 1 connection at a time or transfer will fail (connection will drop)**

#### File naming convention
Files are simply read sequentially from the SPIFS file system so your naming convention should put the files in the proper order.

I use the n_ prefix to make it simple.

*Example* 

Each animations frames are named *animation_name_n.bmp* where *n* is the frame number.
I simply add the position of the animation of the loop as a prefix to the filenames.

In windows 10 the following command does the job : 
> for /f "tokens=*" %a in ('dir /b') do ren "%a" "*prefix*_%a"

## Animation creation

### Suggested website
I use https://www.piskelapp.com/ to create my animations.

This website allows to export the animation as .ZIP which contains all frames as .PNG files.
It is then simple to batch convert the PNG as .BMP for direct transfer to the module.

Also, a prefix can be added to the filenames in the export process.

### Conversion from PNG to BMP
I included a copy of a simple batch conversion program in the ImageBatch folder.


## Breadboard schematic
![Breadboard Schematic](/Schema%20Fritz_bb.png)

## Circuit diagram
![Circuit Diagram](/Schema%20Fritz_diagrame%20circuit.png)

## References to libraries used
https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
https://github.com/tzapu/WiFiManager
http://www.imagebatch.org/


