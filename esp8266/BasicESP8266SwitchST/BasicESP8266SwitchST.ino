/**
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 *  in compliance with the License. You may obtain a copy of the License at:
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
 *  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
 *  for the specific language governing permissions and limitations under the License.
 *
 * A Basic ESP8266 Device to Integrate with Samsung Smart Things
 * 
 *
 * Author: Jacob Schreiver 
 * Date: 2016-02-21
 *
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266SSDP.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards

const char* ssid = "PLACE SSID HERE";
const char* password = "PLACE PASSWORD HERE";


ESP8266WebServer server(80);

int state = 0;

void handleRoot() {
  if (state == 1)
    server.send(200, "text/html", "<html><body>the device is on</body></html>");
  else
    server.send(200, "text/html", "<html><body>the device is off</body></html>");     
}

void handleOn() {
  //*****************ADD Custom Code Here**********************
  int pos;
  if (state == 0)
     {  
        for(pos = 5; pos < 175; pos += 1) // goes from 0 degrees to 180 degrees 
        {                                  // in steps of 1 degree 
          myservo.write(pos);              // tell servo to go to position in variable 'pos' 
          delay(5);                       // waits 15ms for the servo to reach the position 
        } 
     }
     
  state = 1;
  //****************End Turn on Device Code********************
  server.send(200, "text/html", "<html><body>on</body></html>");
  
}


void handleOff() {
  int pos;
  //*****************ADD Custom Code Here**********************
  if (state == 1)
     {
        for(pos = 175; pos>0; pos-=1)     // goes from 180 degrees to 0 degrees 
        {                                  // in steps of 1 degree 
          myservo.write(pos);              // tell servo to go to position in variable 'pos' 
          delay(5);                       // waits 15ms for the servo to reach the position 
        } 
     }
     
  state = 0;
  //****************End Turn off Device Code********************
  server.send(200, "text/html", "<html><body>off</body></html>");
}

void handlePoll() {
  if (state == 1)
    server.send(200, "text/html", "<html><body>on</body></html>");
  else
    server.send(200, "text/html", "<html><body>off</body></html>");
}

void handleNotFound(){
  server.send(404, "text/html", "<html><body>Error! Page Not Found!</body></html>");
}

void setup(void){
  
  myservo.attach(2);  // attaches the servo on GIO2 to the servo object 
  WiFi.begin(ssid, password);
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/poll", handlePoll);

  server.on("/esp8266.xml", HTTP_GET, [](){
    SSDP.schema(server.client());
  });
  
  server.onNotFound(handleNotFound);

  server.begin();

  SSDP.setSchemaURL("esp8266.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName("ESP8266 Basic Switch");
  SSDP.setSerialNumber("0112358132134");
  SSDP.setURL("index.html");
  SSDP.setModelName("SmartThingsESP8266 Basic Switch");
  SSDP.setModelNumber("1");
  SSDP.setModelURL("https://github.com/SchreiverJ/SmartThingsESP8266/");
  SSDP.setManufacturer("Jacob Schreiver");
  SSDP.setManufacturerURL("https://github.com/SchreiverJ/SmartThingsESP8266/");
    
  SSDP.begin();
  
  digitalWrite(BUILTIN_LED, LOW);
}

void loop(void){
  server.handleClient();
  delay(10);
}
