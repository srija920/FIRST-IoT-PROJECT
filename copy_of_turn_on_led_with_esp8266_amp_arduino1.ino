    /*Code By Srija
    Turn On LED via Web through ESP8266 with Arduino.
    ESP8266 HTML WEB SERVER.
    */
    
    #include <SoftwareSerial.h>
    #define DEBUG true
    SoftwareSerial esp8266(2,3);
     int LED=12; //LED PIN
     int itsONled[] = {0,0}; 
    void setup()
    {
      pinMode(LED, OUTPUT);
      Serial.begin(9600);
      esp8266.begin(115200); 
      sendData("AT+RST\r\n",2000,DEBUG); 
      //sendData("AT+CWMODE=1\r\n",1000,DEBUG); 
      sendData("AT+CWMODE=1\r\n",1000,DEBUG); 
      sendData("AT+CWJAP=\"yourssid\",\"pass\"\r\n", 6000, DEBUG); 
      sendData("AT+CIFSR\r\n",2000,DEBUG); 
      sendData("AT+CIPMUX=1\r\n",1000,DEBUG); 
      sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); 
    }

void loop()
{
     
  if(esp8266.available()) 
  {
    if(esp8266.find("+IPD,"))
    {

     int connectionId = esp8266.read()-48; 
     
     String msg;
     esp8266.find("?");
     delay(100);
     msg = esp8266.readStringUntil(' ');
     String command1 = msg.substring(0);
     // HTML START
     String webpage = "<html><head><title>ESP8266 WEB SWITCH</title>";
     webpage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>.button {background-color: red;border: none;color: white;padding: 15px 32px;text-align: center;display: inline-block;font-size: 16px;} .centre {text-align: center;}</style>";
     webpage += "</head><body class=\"centre\"><h1 class=\"centre\">ESP8266 WEB SWITCH</h1>";
     //COMMANDS TO TURN ON or OFF LED RECEIVE BY WEB
                  if (command1 == "T"){
                    if (itsONled[1] == 1) 
                    {
                      digitalWrite(LED, LOW);
                      itsONled[1] = 0; 
                      webpage += "<p>LED IS OFF</p>";
                    }
                    else
                    {
                      digitalWrite(LED, HIGH);
                      itsONled[1] = 1;
                      webpage += "<p>LED IS ON</p>";
                    }
                }
     webpage += "<a class=\"button\" href=\"?T\">CLICK</a></body></html>";
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     sendData(cipSend,500,DEBUG);
     sendData(webpage,500,DEBUG);
    
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; 
     closeCommand+="\r\n";
     sendData(closeCommand,500,DEBUG);
    }
  }
     
}


void sendData(String command, const int timeout, boolean debug)
{
    esp8266.print(command); 
    long int time = millis();

    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        Serial.write(esp8266.read());
      }  
    }
}
