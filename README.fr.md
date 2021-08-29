# LED_Show_BMP
WeMos D1 BMP Animation sur une matrice NeoPixel

Ce projet utilise le systeme d efichier SPIFFS du WeMos D1 pour tocker des fichiers BMP et les afficher sur une matrice de NeoPixel
Il a été bati sur une matrice de 10x10 avec très peu d'essais sur d'autres configurations

## Options de configuration
J'ai essayé de garder la plupart des paramètres dans les #define au début du code
  * LED_PIN indique sur quelle broche est branché la lugne de donnée de la matrice de NeoPixel
  * MODE_SWITCH
  * MODE_LED
  * MATRIX_WIDTH Largeur de la matrice
  * MATRIX_HEIGHT hauteur de la matrice
  * MATRIX_LAYOUT_FLAGS:Parametres de disposition de la matrice divisé en 2 parties : Additionnez les éléments requis pour définir la première LED de la matrice
    *Position de la première LED (En choisir 2 )
    *(e.g. NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG)
    * NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT
    * Disposition de la matrice en ligne ou colone, en choisir 1
    * NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS :   
    * Les lignes de la matrice sont progressive (123,456), ou en zigzag (123,654,789)
    * NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG:   
      
  * MATRIX_TYPE_FLAGS : Type de NeoPixels add together as needeed *(e.g. NEO_RGB + NEO_KHZ800)*
    * NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
    * NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
    * NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
    * NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

## Modes d'opération
Il y a 2 modes d'opérations sélectionné par un interupteur sur le circuit.

Par défaut, le code se place en mode affichage
Pour sélectionner le mode FTP, il faut appuyer sur le boutton de sélection pendant que le module s'initialise

### Mode Affichage
Ce mode lit le système de fichier SPIFFS séquentiellement et affiche les images BMP sur la matrice de NeoPixels

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


