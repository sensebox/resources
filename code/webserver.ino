#include <Ethernet.h>
#include <TextFinder.h>
#include <SD.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC-Adresse des Ethernet-Shield              
IPAddress ip(192, 168, 0, 42);                       // Lokale IP zum aufrufen des Webservers
byte sdPin = 4;                                      // Pin der SD-Karte
 
EthernetServer server(80);                           // Server port
File webFile;
boolean testmode = false;// Auf true setzen falls etwas nicht funktioniert. Dann werden Informationen üebr die serielle Schnittstelle ausgegeben.
  void setup()
  {  
    if(testmode)Serial.begin(9600);     
    Ethernet.begin(mac, ip); // Client starten  
    server.begin();          // Server starten  
  
      if(testmode)Serial.println("Initialisiere SD-Karte...");
    if (!SD.begin(sdPin)) 
    {
      if(testmode)Serial.println("Initialisierung der SD-Karte fehlgeschlagen!");
      return;
    }
    if(testmode)Serial.println("SD-Karte erfolgreich initialisiert.");
   
    if (!SD.exists("index.htm")) 
    {
      if(testmode) Serial.println("Datei (index.htm) wurde nicht gefunden!");
      return;
    }
    if(testmode){
      Serial.println("Datei (index.htm) wurde gefunden.");
      Serial.println("Verbraucher schalten");
    }  
  }
 
void loop()
{
  EthernetClient client = server.available(); // Auf Anfrage warten 
  if(client)
  {   
    TextFinder finder(client);
 
    if(finder.find("GET"))
    {
      while(finder.findUntil("pin", "\n\r"))
      {
        char typ = char(client.read());
        int  pin = int(finder.getValue());
        int  val = int(finder.getValue());
 
        if(typ == 'D')
        {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, val);
          if(testmode)Serial.print(" - D"+String(pin));            
        }
        else if(typ == 'A')
        {
            analogWrite(pin, val);
            if(testmode)Serial.print(" - A"+String(pin));
                    
        }
        else if(typ == 'a'){// a -> Sensorwert auslesen und ausgeben
           val = analogRead(pin);
           if(testmode)Serial.print(" - a"+String(pin)); 
        }
        else{
           if(testmode)Serial.print(" - Falscher Typ");                   
        }
        //Hier können neue Befehle definiert werden die aus dem Browser gestartet werden.
         if(testmode){
          if(val==1) Serial.println(" ein");
          if(val==0) Serial.println(" aus");
          else Serial.println(" "+ val);
          }        
       
      }
    }
 
    /************************
    * Webformular anzeigen  *
    ************************/
    boolean current_line_is_blank = true;       // eine HTTP-Anfrage endet mit einer Leerzeile und einer neuen Zeile
    String htmlline = "";
    while (client.connected()) 
    {
      if (client.available())                   // Wenn Daten vom Server empfangen werden
      {
        
        char c = client.read();                 // empfangene Zeichen einlesen
        if (c == '\n' && current_line_is_blank) // wenn neue Zeile und Leerzeile empfangen
        { // Standard HTTP Header senden
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
 
          // Website von SD-Karte laden
          webFile = SD.open("index.htm");  // Website laden
          if (webFile)
          {
             while(webFile.available()){                          
              char temp = char(webFile.read());      
              if((int(temp) != 10)&&(int(temp) != 32)&&(int(temp) != 13)){ //Bei jedem Leerzeichen wird ein neuer String generiert, da Arduino String nur 165 Zeichen lang ist
                htmlline = htmlline + char(temp);
              }else{
                  htmlline = filter(htmlline); //Ersetzt Keywords zum Beispiel durch Sensorwerte
                  client.println(htmlline); 
                  if(testmode)Serial.println(htmlline);         
                  htmlline = ""; 
              }              
            }
            webFile.close();
          }
          break;
        }
        if (c == '\n') 
        {
          current_line_is_blank = true;
        } 
        else if (c != '\r') 
        {
          current_line_is_blank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }  
}

//Ersetzt Keywords zum Beispiel durch Sensorwerte
// Es können nach belieben neue Keywords definerit und durch andern Text ersätzt werden 
String filter(String htmlline){
    htmlline.replace("sa0en",String(analogRead(A0))); 
    htmlline.replace("sa1en","No sensor");
    htmlline.replace("sa2en","No sensor");
    htmlline.replace("sa3en","No sensor");
    htmlline.replace("sa4en","No sensor");   
    htmlline.replace("sa5en","No sensor");
    return htmlline;
}
