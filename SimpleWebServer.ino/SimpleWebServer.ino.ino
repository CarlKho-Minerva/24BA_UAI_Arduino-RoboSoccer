#include <WiFi.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;          // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // by default the local IP address of will be 192.168.3.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  if (strlen(pass) < 8)
  {
    Serial.println("Creating access point failed");
    Serial.println("The Wi-Fi password must be at least 8 characters long");
    // don't continue
    while (true)
      ;
  }

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create the Access point
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed");
    // don't continue
    while (true)
      ;
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();
}

void loop()
{

  // compare the previous status to the current status
  if (status != WiFi.status())
  {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED)
    {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    }
    else
    {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available(); // listen for incoming clients

  if (client)
  {                               // if you get a client,
    Serial.println("new client"); // print a message out the serial port
    String currentLine = "";      // make a String to hold incoming data from the client

    while (client.connected())
    { // loop while the client's connected

      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>");
            // Enhanced CSS with animations and transitions
            client.print("*{margin:0;padding:0;box-sizing:border-box;}");
            client.print("body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#0f0f13;color:#fff;min-height:100vh;padding:20px;}");
            client.print(".container{max-width:600px;margin:0 auto;padding:20px;animation:fadeIn 0.5s ease-out;}");
            client.print(".header{text-align:center;margin-bottom:40px;padding:20px;}");
            client.print(".logo{font-size:28px;font-weight:700;color:#fff;margin-bottom:10px;text-shadow:0 0 20px rgba(255,255,255,0.3);}");
            client.print(".subtitle{color:#848484;font-size:16px;margin-bottom:5px;}");
            client.print(".branding{color:#666;font-size:12px;margin-top:30px;line-height:1.5;}");
            client.print(".control-group{background:#1d1e22;border-radius:20px;padding:24px;margin-bottom:24px;box-shadow:0 4px 6px rgba(0,0,0,0.2);transition:all 0.3s ease;}");
            client.print(".control-group:hover{transform:translateY(-2px);box-shadow:0 8px 12px rgba(0,0,0,0.3);}");
            client.print(".control-group h2{color:#848484;font-size:18px;margin-bottom:20px;text-transform:uppercase;letter-spacing:1px;}");
            client.print(".btn-group{display:grid;grid-template-columns:1fr 1fr;gap:8px;position:relative;padding:4px;background:#2a2a2a;border-radius:16px;}");
            client.print(".btn{display:block;padding:20px;border-radius:12px;text-decoration:none;text-align:center;font-weight:600;font-size:18px;position:relative;z-index:1;transition:all 0.3s ease;}");
            client.print(".btn{color:#999;text-shadow:0 1px 2px rgba(0,0,0,0.2);}");                  // Lighter inactive color
            client.print(".btn.active{color:#fff;text-shadow:0 1px 3px rgba(0,0,0,0.3);}");           // Brighter active color
            client.print(".btn:active{transform:scale(0.97);}");                                      // Button press effect
            client.print(".red .btn.active{color:#fff;text-shadow:0 0 10px rgba(255,255,255,0.5);}"); // Enhanced active state
            client.print(".green .btn.active{color:#fff;text-shadow:0 0 10px rgba(255,255,255,0.5);}");
            client.print(".blue .btn.active{color:#fff;text-shadow:0 0 10px rgba(255,255,255,0.5);}");
            client.print(".slider{position:absolute;top:4px;left:4px;width:calc(50% - 4px);height:calc(100% - 8px);border-radius:12px;transition:transform 0.3s cubic-bezier(0.4, 0, 0.2, 1);}");
            client.print(".red .slider{background:linear-gradient(135deg,#ff4444,#ff0000);}");
            client.print(".green .slider{background:linear-gradient(135deg,#44ff44,#00ff00);}");
            client.print(".blue .slider{background:linear-gradient(135deg,#4444ff,#0000ff);}");
            client.print(".slider.off{transform:translateX(calc(100% + 8px));}");
            client.print(".btn{color:#666;}");
            client.print(".btn.active{color:#fff;}");
            client.print("@keyframes fadeIn{from{opacity:0;transform:translateY(10px)}to{opacity:1;transform:translateY(0)}}");
            client.print("</style></head>");

            client.print("<body><div class='container'>");
            client.print("<div class='header'><div class='logo'>LED Control Hub</div>");
            client.print("<div class='subtitle'>Wireless LED Control System</div>");
            client.print("<div class='branding'>by Carl Kho<br>for CAETI @ UAI<br>using Arduino Portenta H7</div></div>");

            // Red LED controls
            client.print("<div class='control-group red'>");
            client.print("<h2>Red Channel</h2>");
            client.print("<div class='btn-group'>");
            client.print("<div class='slider'></div>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"red\", true, this); return false;'>ON</a>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"red\", false, this); return false;'>OFF</a>");
            client.print("</div></div>");

            // Green LED controls
            client.print("<div class='control-group green'>");
            client.print("<h2>Green Channel</h2>");
            client.print("<div class='btn-group'>");
            client.print("<div class='slider'></div>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"green\", true, this); return false;'>ON</a>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"green\", false, this); return false;'>OFF</a>");
            client.print("</div></div>");

            // Blue LED controls
            client.print("<div class='control-group blue'>");
            client.print("<h2>Blue Channel</h2>");
            client.print("<div class='btn-group'>");
            client.print("<div class='slider'></div>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"blue\", true, this); return false;'>ON</a>");
            client.print("<a href='#' class='btn' onclick='toggleLED(\"blue\", false, this); return false;'>OFF</a>");
            client.print("</div></div>");

            client.print("</div></body></html>");

            client.print("<script>");
            client.print("function toggleLED(color, isOn, btn) {");
            client.print("  const action = isOn ? 'H' : 'L';");
            client.print("  const colorCode = color[0].toLowerCase();");
            client.print("  fetch('/' + action + colorCode)");
            client.print("    .then(() => {");
            client.print("      updateState(btn, color, isOn);");
            client.print("    });");
            client.print("}");

            client.print("function updateState(btn, color, isOn) {");
            client.print("  const group = btn.closest('.control-group');");
            client.print("  const slider = group.querySelector('.slider');");
            client.print("  const btns = group.querySelectorAll('.btn');");
            client.print("  btns.forEach(b => b.classList.remove('active'));");
            client.print("  btn.classList.add('active');");
            client.print("  slider.classList.toggle('off', !isOn);");
            client.print("}");

            // Initialize states with current LED values
            client.print("document.querySelectorAll('.btn-group').forEach(group => {");
            client.print("  const firstBtn = group.querySelector('.btn');");
            client.print("  firstBtn.classList.add('active');");
            client.print("});");
            client.print("</script>");
            client.print("</div></body></html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /Hr"))
        {
          digitalWrite(LEDR, LOW); // GET /Hr turns the Red LED on
        }
        if (currentLine.endsWith("GET /Lr"))
        {
          digitalWrite(LEDR, HIGH); // GET /Lr turns the Red LED off
        }
        if (currentLine.endsWith("GET /Hg"))
        {
          digitalWrite(LEDG, LOW); // GET /Hg turns the Green LED on
        }
        if (currentLine.endsWith("GET /Lg"))
        {
          digitalWrite(LEDG, HIGH); // GET /Hg turns the Green LED on
        }
        if (currentLine.endsWith("GET /Hb"))
        {
          digitalWrite(LEDB, LOW); // GET /Hg turns the Green LED on
        }
        if (currentLine.endsWith("GET /Lb"))
        {
          digitalWrite(LEDB, HIGH); // GET /Hg turns the Green LED on
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your Wi-Fi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}