//--------------------------------------------------------------------//
//-------- Functions of the Android app-------------------------------//
//--------------------------------------------------------------------//
#pragma region AndroidFunctions

//Initialize the ouput of the arduino
void initialisationSorties(void){
    pinMode(LedR, OUTPUT);
    pinMode(LedG, OUTPUT);
    pinMode(LedB, OUTPUT);

  }
  // Function which handle the SoundMode
void TrameSound(void){

  int iBcl = 0; // Variable used for the sound mode
  long lRand; // Variable which will stock a random value for the sound mode
  char cLed[3]; //
  int iMin =32;
  int iMax = 64;
  int iVal = 0; // variable which will stock the analog value of the micro
  float fUcan = 0; // mesure of the analog pin in Volt

  iVal = analogRead(pinCAN);
  fUcan =  iVal*5.00/1023;
  Serial.println(fUcan);
  if(U_REF<fUcan){
    for(iBcl =0;iBcl<3;iBcl++){
     lRand= random(iMin,iMax);
     cLed[iBcl]=(char)lRand;
     GereLed(cLed[iBcl]);
     iMin = iMin +32;
     iMax = iMax +32;
   }
 }
}
// Function which set the value of the LEDs and activate the soundMode with the charactere received
void GereLed(char cBluetooth){

  unsigned char cAbs; // Value which store the absolute
                      // value of the charactere
  int iVal; // value whiche store the char send in an
            // integer to allow the control of the led
  bool bSoundMode=0;
  cAbs= (unsigned char) cBluetooth;
  iVal = (int) cAbs;

  if((32<=iVal)&&(64>iVal)){
    if(!bSoundMode){
    iVal = iVal-32;
    iVal = iVal*255/31;
     analogWrite(LedR, iVal);
     Serial.println("LedR");
    }
  }

  else if((64<=iVal)&&(iVal<96)){
    if(!bSoundMode){
     iVal = iVal-64;
    iVal = iVal*255/31;
     analogWrite(LedG, iVal);
      Serial.println("LedG");
    }
  }

  else if((96<=iVal)&&(iVal<128)){
    if(!bSoundMode){
     iVal = iVal-96;
     iVal =iVal*255/31;
     analogWrite(LedB, iVal);
     Serial.println("LedB");
    }
  }

  else if(iVal==1){
    analogWrite(LedB, 0);
    analogWrite(LedR, 0);
    analogWrite(LedG, 0);
  }

   else if(iVal==255){
    analogWrite(LedR, 255);
    analogWrite(LedG, 255);
    analogWrite(LedB, 255);

    }
    else if(iVal==128){
    TrameSound();//start sound mode
    bSoundMode=1;
    analogWrite(LedR, 255);
    analogWrite(LedG, 255);
    analogWrite(LedB, 255);
    }
    else if(iVal==192){
      bSoundMode=0;//quit sound mode
    analogWrite(LedB, 0);
    analogWrite(LedR, 0);
    analogWrite(LedG, 0);
      }
}

#pragma endregion

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//

//-----------------------------------------------------------------------//
//------------ Server functions -----------------------------------------//
//-----------------------------------------------------------------------//

#pragma region ServerFunctions

char StrContains(char *str, char *sfind){
  char found = 0;
  char index = 0;
  char len;
  len = strlen(str);
  if (strlen(sfind) > len) {
    return 0;
  }
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {
        return 1;
      }
    }
    else {
      found = 0;
    }
    index++;
  }
  return 0;
}
void sendBaseAnswer(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
}
void StrClear(char *str, char length){
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

#pragma endregion
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
