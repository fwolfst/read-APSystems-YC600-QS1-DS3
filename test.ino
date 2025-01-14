//void test_polling(int which) {
//    polled[which]=false; //nothing is displayed on webpage
//
//    if(diagNose) ws.textAll("polling"); //
//    char pollCommand[67] = {0};
//    char ecu_id_reverse[13];
//    ECU_REVERSE().toCharArray(ecu_id_reverse, 13);    
//
//
////    if(diagNose) ws.textAll("zb send poll cmd inverter " + String(which) + "  cmd:" + String(pollCommand));   
//      snprintf(pollCommand, sizeof(pollCommand), "1D2401%.2s%.2s1414060001000F13%sFBFB06BB000000000000C1FEFE", Inv_Prop[which].invID + 4, Inv_Prop[which].invID + 2, ecu_id_reverse);
//     //add checksum
//     strcpy(pollCommand, strncat(pollCommand, checkSum(pollCommand), sizeof(pollCommand) + sizeof(checkSum(pollCommand))));
//
//     Serial.println("pollCommand after len and checksum:" + String(pollCommand));
//}
//void test_polling(int which) {
//    polled[which]=false; //nothing is displayed on webpage
//
//    Serial.println("testpolling inv 0");
//
//    char pollCommand[254] = {0};
//    char normalOperationBaseCommand[][254]  =
//    {
//        "2401",                      //+ ..string.sub(inv_id[x],3,4)..string.sub(inv_id[x],1,2)..
//        "1414060001000F13",          //append (concatenate) _ecu_id_reverse
//         // "1414000601000F13",          //append (concatenate) _ecu_id_reverse
//        "FBFB06BB000000000000C1FEFE" // end of String
//    };
//    
//    //Serial.println("Inv_Prop[which].invID = " + String(Inv_Prop[which].invID) );
//    char inv_id[7];
//    memset(&inv_id[0], 0, sizeof(inv_id)); //zero out otherwise we get strange tokens
//    delayMicroseconds(250);
//    
//    strncpy(inv_id, Inv_Prop[which].invID, strlen(Inv_Prop[which].invID));
//    inv_id[7]='\0'; //terminate
//
//    char ecu_id_reverse[13];  
//    ECU_REVERSE().toCharArray(ecu_id_reverse, 13);
//    
//    strncpy(pollCommand, normalOperationBaseCommand[0], strlen(normalOperationBaseCommand[0])); // the 2401
//    Serial.println(" 1 pollCommand now = " + String(pollCommand));
//    strncat(pollCommand, inv_id + 4, 2 ); // ad the 2nd byte of inv_id
//    strncat(pollCommand, inv_id + 2, 2 );     // ad the 1st byte of inv_id
//    Serial.println("2 pollCommand after reversed inv id = " + String(pollCommand));
//    strncat(pollCommand, normalOperationBaseCommand[1], sizeof(normalOperationBaseCommand[1]));
//    Serial.println("3 pollCommand after 1414060001000F13 = " + String(pollCommand));         
//    strncat(pollCommand, ecu_id_reverse, sizeof(ecu_id_reverse));
//    Serial.println(" 4 pollCommand after reversed in id = " + String(pollCommand));
//    strncat(pollCommand, normalOperationBaseCommand[2], sizeof(normalOperationBaseCommand[2]));
//    //Serial.println("pollCommand after FBFB06BB000000000000C1FEFE = " + String(pollCommand));
//    
//    // calculate length and put this at beginning
//    strcpy(pollCommand, strncat(sLen(pollCommand), pollCommand, sizeof(sLen(pollCommand)) + sizeof(pollCommand))); //build command plus sln at the beginning
//    // put in the CRC at the end of the command
//    strcpy(pollCommand, strncat(pollCommand, checkSum(pollCommand), sizeof(pollCommand) + sizeof(checkSum(pollCommand))));
//    
//    Serial.println("pollCommand after len and checksum:" + String(pollCommand));
//}

void testMessage() {
char sendCmd[100];
int len = strlen(txBuffer);

DebugPrint("len of txBuffer :  "); DebugPrintln(String(txBuffer));

      for(int i=0; i<len; i++) 
      {
        sendCmd[i] = txBuffer[i+7];
      }
        
       //len at the begin
       strcpy(sendCmd, strcat(sLen(sendCmd), sendCmd)); //build command plus sln at the beginning
       //strcpy(sendCmd, strncat(sLen(sendCmd), sendCmd, sizeof(sLen(sendCmd)) + sizeof(sendCmd))); //build command plus sln at the beginning
       // CRC at the end done by sendZigbee

       ws.textAll("sendCmd (ex crc) = FE" + String(sendCmd));
       //now we send this command

       sendZigbee(sendCmd);
       if ( waitSerialAvailable() ) { readZigbee(); } else { readCounter = 0;}
//       #ifdef DEBUG
//       swap_to_usb();
//       Serial.println("received : " + String(inMessage));
//       #endif
       ws.textAll("answer " + String(inMessage));
}


#ifdef TEST
void testDecode() {
// we always test inverter 0, depending on the type we test the right string
int type = Inv_Prop[0].invType; //
// we define an inmessage first
// the time in the message = 36887
//we want a timespan of 300 

//en_saved[0][0] = 245;


switch(type) {
 case 0: // yc600
   strncpy(inMessage, "FE0164010064FE034480001401D2FE0345C43A1000A8FE724481000006013A101414007100B57CFA00005E408000158215FBFB51B103D40F4117000074CF00000076706A73D06B0496000000000000000172072D88017862E8201F00030555073F0303030100000100000000000000000000000000000000000000000000000000000000000000FEFE3A100E76",300);
   break;
 case 1: // test qs1 johan
   strncpy(inMessage, "FE0164010064FE034480001401D2FE72448100000601C0051414005E00905D5B00005E801000085070FBFB51B103EB0F419300CAF069D9F068C7C068C1206804B868E0000006A80001BB38134D01CCE90E0A01FD1E052201D967D0641F0003055400000000000000000000000000000000000000000000000000002B2A0000FEFEC0050E55",300);
   break;
 case 2: // ds3
   if(testCounter == 0) {
// from npeters tracefile at 9:53 and 9:57
   strncpy(inMessage, "FE0164010064FE034480001401D2FE0345C4226C00CCFE0345C4226C00CCFE0345C43A1000A8FE724481000006013A101414007100B57CFA00005E703000021300fbfb5cbbbb2000fc0001ffff000000000000000006e506ee00E200EA036e13882D5F01480026ffff052508430049F8C40048C77C00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff3896fefe",300);   
//                                                                                                                                           >                                                   26                      tttt                    100     108       
   } else { //yc600
   strncpy(inMessage, "FE0164010064FE034480001401D2FE0345C4226C00CCFE0345C4226C00CCFE0345C43A1000A8FE724481000006013A101414007100B57CFA00005E703000021300fbfb5cbbbb2000fc0001ffff000000000000000006e506ee015900EF010313882E8B01480026ffff052508430050BF39004F849C00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff3896fefe",300);   
//                                                                                                                                           >                                                   26                      tttt                    100     108       
   }
break;
}
//703000021300 FB FB 5C BB BB 20 00 FC 00 01 FF FF 00 00 00 00 00 00 00 00 06 83 06 8A 02 31 02 3A 03 68 13 87   2C 9B  01 
//703000021300 fb fb 5c bb bb 20 00 02 00 e6 ff ff 00 00 00 00 00 00 00 00 06 f5 06 f9 00 2e 00 34 03 60 13 8a | 17 a7 |00 
//0             6  7  8  9 10 11 12 13 14 15                21 22                      30                      | time  |40                            50       53 54 55

//BB 00 2A FF FF 05 6B 08 D8 01 1C 4E 79 01 1D BD 26 00 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3830FEFE226C70E
//24 00 1f ff ff 05 42 06 90 00 16 f6 2b 00 18 e4 51 ff ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff3969fefe
//41          45             50             55  

//ws.textAll("going to test for invType: " + String(type) + "\n");
decodePollAnswer(0);

polled[0]=true;
testCounter += 1; // for the next round we have new string
mqttPoll(0);
}
#endif
