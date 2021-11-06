#include <dht.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x09, 0xA7 }; //physical mac address
byte ip[] = { 192, 168, 0, 208 }; // ip in lan
byte gateway[] = { 192, 168, 0, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString;

const int pinoDHT11 = 7;
int pin = 2; 
boolean ligado = true;

dht DHT;

//////////////////////

void setup(){

  pinMode(pin, OUTPUT); //pin selected to control
  //start Ethernet
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  //the pin for the servo co
  //enable serial data print
  Serial.begin(9600);
}

void loop(){
  DHT.read11(pinoDHT11);
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          //Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////////// control arduino pin
          Serial.println(readString); //print to serial monitor for debuging
          if(readString.indexOf("?ligar") > 0)//checks for on
          {
            digitalWrite(pin, HIGH);    // set pin 4 high
            Serial.println("On");
            ligado = false;
          }
          else{
            if(readString.indexOf("?desligar") >0)//checks for off
            {
              digitalWrite(pin, LOW);    // set pin 4 low
              Serial.println("Off");
              ligado = true;
            }
          }
          //clearing string for next read
          readString="";


          ///////////////

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<html>");
          client.println("<head>");
          client.println("<title>Mario Kennedy - Sistema</title>");
          client.println("<meta http-equiv='Content-Type' content='text/html; charset=ISO-8859-1'>");
          client.println("<link rel='stylesheet' type='text/css' href='https://raw.githubusercontent.com/mkennedysf/jssistema/main/style.css'/>");
          client.println("<script type='text/javascript' src='https://raw.githubusercontent.com/mkennedysf/jssistema/main/sistema.js'></script>");
          client.println("</head>");
          client.println("<body>");
          client.println("<div id='wrapper'>");
          client.println("Temperatura:");
          client.println(DHT.temperature, 0);
          client.println("C");
          client.println("</div>");
          client.println("<div>");
          client.println("Humidade:");
          client.println(DHT.humidity);
          client.print("%");
          client.println("</div>");
          client.print("<div id='estado' style='visibility: hidden;'>");
          client.print(ligado);
          client.println("</div>");
          client.println("<div id='rele'></div>");
          client.println("<div id='botao'></div>");
          client.println("</div>");
          client.println("<script>AlteraEstadoRele()</script>");
          client.println("</body>");
          client.println("</head>");

          delay(1);
          //stopping client
          client.stop();
        }
      }
    }
  }
}
