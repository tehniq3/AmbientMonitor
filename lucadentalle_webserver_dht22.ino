// original from http://www.lucadentella.it/2015/02/14/ambientmonitor-client-w5100/
// adapted by niq_ro from http://arduinotehniq.blogspot.com/
// use just DHT22 sensor (AM2302) and W5100 shield

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 20 }; 
EthernetServer server(80);
 
#include <DHT.h>
#define DHTPIN A2     // what pin we're connected the DHT output
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22 
DHT dht(DHTPIN, DHTTYPE); 
 
void setup() {
 dht.begin();  

    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients

}
 
void loop() {

	EthernetClient client = server.available();

	if (client) {

		boolean currentLineIsBlank = true;
		
		while (client.connected()) {
		
			if (client.available()) {
			
				char c = client.read();
				if (c == '\n' && currentLineIsBlank) {
					client.println("HTTP/1.1 200 OK");
					client.println("Content-Type: text/html");
					client.println("Server: Arduino");
					client.println("Connnection: close");
					client.println();
				        float temperatura = dht.readTemperature();
					client.print("myCB({'temperature':");
					client.print(temperatura);
					client.print(",'humidity':");
                                        float umiditate = dht.readHumidity();					
                                        client.print(umiditate);
					client.print("})");
					client.println();
					break;
				}
				
				if (c == '\n') currentLineIsBlank = true;
				else if (c != '\r') currentLineIsBlank = false;
			}
		}
		
		delay(1);
		client.stop();
	}

}
