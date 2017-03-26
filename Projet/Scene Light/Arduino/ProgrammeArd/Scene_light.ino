#define TAILLE_BUF 10
#define U_REF 2
#define BLUETOOTH Serial
#include <string.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#define DHT11_PIN 7
#define REQ_BUF_SZ 40

#pragma region Androidapp
//initialization of the pins of the LEDs

const int LedR = 3;
const int LedG = 5;
const int LedB = 6;
const int pinCAN = 1;

void AndroidApp(){
  char cOctet = 0; // variable which will receive the character send by the phone

  static bool bSound=1;
  // number of characters received in the frame
  static int iBclReception = 0;
  // counter sent to the phone
  static int iBcl=0;

  static char strTrame[TAILLE_BUF]="_________";
   if(BLUETOOTH.available() > 0)
   {
      strTrame[iBclReception] = BLUETOOTH.read(); //the frame is read character per character (not the complete frame at the same time)
      if(strTrame[iBclReception] == '\0')
      {  // search end of frame
      // display received message for debug
      // Serial.println(strTrame);
      cOctet = strTrame[0];
      iBclReception=0;
      }else{
      iBclReception++;
      }
   }
   GereLed(cOctet);
}
#pragma endregion

#pragma region Webserver

File webFile;
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0; // index into HTTP_req buffer

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x45, 0xD9};   //Chez Mik
IPAddress ip(192, 168, 1, 40);
//byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x85, 0xA2}; // A l'iu-miel
//IPAddress ip(192, 168, 0, 200);

EthernetServer server(80);

int web_pin3;
int web_pin5;
int web_pin6;

bool web_bStrob = 0;

void ServerWeb(){
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (req_index < (REQ_BUF_SZ - 1)) {
          HTTP_req[req_index] = c; // save HTTP request character
          req_index++;
        }
        if (c == '\n' && currentLineIsBlank) {
          if (StrContains(HTTP_req, "GET / ") || StrContains(HTTP_req, "GET /index.htm")) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connnection: close");
            client.println();
            webFile = SD.open("index.htm");

          } else if (StrContains(HTTP_req, "GET /my.css")) {
            webFile = SD.open("my.css");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println();
            }

          }else if (StrContains(HTTP_req, "GET /Ch_des_c.png")) {
            webFile = SD.open("Ch_des_c.png");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println();
            }
          }else if (StrContains(HTTP_req, "setpin?pin=1")) {
            web_bStrob = !web_bStrob;

          }else if (StrContains(HTTP_req, "GET /Led_rgb.png")) {
            webFile = SD.open("Led_rgb.png");
            if (webFile) {
              client.println("HTTP/1.1 200 OK");
              client.println();
            }
          }else if (StrContains(HTTP_req, "ajax_flame")) {
            sendBaseAnswer(client);

            client.print(web_pin3);
            client.print(web_pin5);
            client.print(web_pin6);
          }
           else if (StrContains(HTTP_req, "setpin?pin=3")) {
            String input = HTTP_req;
            int posStart = input.indexOf("value=");
            int posEnd = input.indexOf(' ', posStart);
            String param = input.substring(posStart + 6, posEnd + 1);
            web_pin3 = param.toInt();
            if(web_bStrob){
              analogWrite(3, web_pin3);
              delay(75);
            }else{
            analogWrite(3, web_pin3);
            }
            sendBaseAnswer(client);
          }
           else if (StrContains(HTTP_req, "setpin?pin=5")) {
            String input = HTTP_req;
            int posStart = input.indexOf("value=");
            int posEnd = input.indexOf(' ', posStart);
            String param = input.substring(posStart + 6, posEnd + 1);
            web_pin5 = param.toInt();
            if(web_bStrob){
              analogWrite(5, web_pin5);
              delay(75);
            }else{
            analogWrite(5, web_pin5);
            }

            sendBaseAnswer(client);

          }
           else if (StrContains(HTTP_req, "setpin?pin=6")) {
            String input = HTTP_req;
            int posStart = input.indexOf("value=");
            int posEnd = input.indexOf(' ', posStart);
            String param = input.substring(posStart + 6, posEnd + 1);
            web_pin6 = param.toInt();
            if(web_bStrob){
              analogWrite(6, web_pin6);
              delay(75);
              Serial.println(web_pin6);
            }else{
            analogWrite(6, web_pin6);
            }
            sendBaseAnswer(client);

          }

          if (webFile) {
            while (webFile.available()) {
              client.write(webFile.read()); // send web page to client
            }
            webFile.close();
          }
          req_index = 0;
          StrClear(HTTP_req, REQ_BUF_SZ);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
#pragma endregion

void setup(){
//inizialization of the arduino pins
  initialisationSorties();

//Initialization of bluetooth adapter
  BLUETOOTH.begin(9600);

  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  SD.begin(4);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);

}

void loop() {
  AndroidApp();
  ServerWeb();
}
