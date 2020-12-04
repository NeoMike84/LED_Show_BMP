/*LED_Show_BMP

  Par Michael Simard

  Board : Wemos D1 mini Pro (ESP8266 based board)

  Description :

            Reads BMP files from the onboard SPIFFS storage and display them sequentially to
            a NeoMatrix (NeoPixel Matrix)

            Lit des fichiers BMP sur le stockage interne SPIFFS de la carte et les affiches sequentiellements sur une matrice NeoMatrix (Matrice de NeoPixel)

            Specs for BMP files :
            https://en.wikipedia.org/wiki/BMP_file_format

            Site pratique pour créer les animations :
            https://www.piskelapp.com/


*/

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include <FS.h>

#define LED_PIN D6
#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 10

//Creation de la variable pour la matrice -- Développer pour avoir les détails
//Creation of the matrix variable
//Initialization can vary according to your particular setup
//
//Documentation
// MATRIX DECLARATION:
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.
// Parameter 7 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
//   NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
// Example with three 10x8 matrices (created using NeoPixel flex strip --
// these grids are not a ready-made product).  In this application we'd
// like to arrange the three matrices side-by-side in a wide display.
// The first matrix (tile) will be at the left, and the first pixel within
// that matrix is at the top left.  The matrices use zig-zag line ordering.
// There's only one row here, so it doesn't matter if we declare it in row
// or column order.  The matrices use 800 KHz (v2) pixels that expect GRB
// color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, LED_PIN,
                            NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_RGB + NEO_KHZ800);

//Variable pointant vers le répertoire racine et le fichier courrant
Dir rootDir;
File currentFile;

//Fonction d'affichage d'un fichier BMP
//Le 2ième paramètre indique si l'image est en 24BPP ou en 32
void displayImage(File bmpFile, bool is24BPP); //Display image from BMP file in SPIFFS

void setup()
{
  //Initialize Serial console
  Serial.begin(115200);
  delay(10);


  //Initialize the matrix
  matrix.begin();
  matrix.setBrightness(255);
  matrix.show();

  //Initialisation du système de fichier
  //Initialize Filesystem
  Serial.print("Initializing FS...");

  if (SPIFFS.begin())
    Serial.println(" done!");
  else
  {
    Serial.println(" failed!");
  }

  //Ouvre le répertoire racine
  //Open the root directory
  rootDir = SPIFFS.openDir("/");
}


void loop()
{
  //Pour chaque fichier dans le répertoire racine
  //For each file in root directory
  while (rootDir.next())
  {
    //File is a bmp, seek to offset of pixel data
    currentFile = rootDir.openFile("r");

    //Extract begining of bitmap from file and position cursor on it
    //As per BMP file format spec, location is stored at offset 0x0A (see https://en.wikipedia.org/wiki/BMP_file_format)
    currentFile.seek(0x0A, SeekSet);
    char offset[4];
    currentFile.readBytes(offset, 4);

    //Extract BPP from file
    //As per BMP File Format Spec stored at 0x1C (See https://en.wikipedia.org/wiki/BMP_file_format)
    currentFile.seek(0x1C, SeekSet);
    char bpp[2];
    currentFile.readBytes(bpp, 2);

    //Sets cursor at the beginning of Pixel Data
    currentFile.seek(offset[0], SeekSet);

    bool is24BPP = (bpp[0] == 24);
    displayImage(currentFile, is24BPP);
    delay(125);
  }

  //Loop back to first file in folder
  //Retour au premier fichier du dossier
  rootDir.rewind();
}

//Affiche une image du fichier source (BMP)
//Le format des pixels est soit BBGGRR pour les 24BPP ou AABBGGRR pour les 32BPP
//
//Displays an image from a source file (BMP)
//Format for pixel is 0xAABBGGRR
void displayImage(File bmpFile, bool is24BPP)
{
  matrix.clear();

  //For Each row in the matrix
  for (int row = 0; row < MATRIX_HEIGHT; row++) //For each row
  {
    //Calcule combien d'octet il y a par ligne
    //Calculate hoe many bytes per line
    //Formula varies to take into account BPP (3 for 24BPP and 4 for 32BPP)
    int bytesInARow = (is24BPP ?
                       MATRIX_WIDTH * 3 + ((MATRIX_WIDTH * 3) % 4) //For 24BPP
                       : MATRIX_WIDTH * 4 + ((MATRIX_WIDTH * 4) % 4)) ; //For 32BPP


    char data[200];
    //Read a row from the file
    Serial.print("Read bytes : ");
    Serial.println(bmpFile.readBytes(data, bytesInARow));

    //For each column in the row
    //Pour chaque colone dans la ligne (essentiellement chaque pixel)
    for (int column = 0; column < MATRIX_WIDTH; column++)
    {
      //If 24BPP, there is 3 bytes per pixel if 32BPP, there is 4
      int offset = column * (is24BPP ? 3 : 4);

      //Each pixel is defined over 3 bytes BBGGRR (or 4 bytes BBGGRRAA)
      //Let's extract the RGB values
      byte b = data[offset]; //On lit le premier octet
      byte g = data[offset + 1]; //On lit le 2ieme octet
      byte r = data[offset + 2]; // On lit le 3ieme octet

      /*     Debug Code -- Shows pixel info on serial console

                Serial.print("Processing pixel X:");
                Serial.print(column);
                Serial.print(" Y: ");
                Serial.print(MATRIX_HEIGHT-row-1);
                Serial.print(" R : ");
                Serial.print(r);
                Serial.print(" G : ");
                Serial.print(g);
                Serial.print(" B : ");
                Serial.println(b);
      */

      //Now let's push them to the matrix
      //BMP format starts from bottom to up so we have to take that into account
      //Matrix is 0 indexed and MATRIX_HEIGHT is 1 indexed so we have to remove 1 from the value
      matrix.drawPixel(column, (MATRIX_HEIGHT - 1) - row, matrix.Color(r, g, b));
    }
  }
  matrix.show();
}
