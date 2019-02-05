/*
Yamaha AV-Receiver  MQTT <-> RS232 controller based on the Homie MQTT convention
Optimized for RX-V1600 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Homie.h>
#include "main.hpp"


HomieNode serialNode("serial", "Serial", "serial");

/*-----Calculate CRC-----*/
int calculateCRC(const String& extcommand){

int crc = 0;
int len = extcommand.length();
char sumchar[len];
strcpy(sumchar, extcommand.c_str());
for(int i=0; i < len; i++){
    crc = crc + int(sumchar[i]);
}
return crc;
}

/*-----Set Standard Raw Command-----*/
bool commandHandler(const HomieRange& range, const String& command) {                  

const char *cmd = command.c_str();
Serial.printf("%c%s%c",STX,cmd,ETX); 
return true;
}

/*-----Set Extended Raw Command-----*/
bool commandExtHandler(const HomieRange& range, const String& extcommand) {                 

const char *cmd = extcommand.c_str();
Serial.printf("%c%s%02x%c",DC4,cmd,calculateCRC(extcommand) & 0xFF,ETX); 
return true;
}

/*------Send Read value ext command*/
bool sendReadExtCommand( char *command){
Serial.printf("%c%s%02x%c",DC4,command,calculateCRC(command)& 0xFF,ETX); 
return true; 
}

/*------Send 2 byte value ext command*/
bool sendTwoByteExtCommand( char *command,  int value){

char buffer [2];
itoa (value,buffer,16); 
if(value<16){
    strcat(command,"0");
}
strcat(command,buffer);
Serial.printf("%c%s%02x%c",DC4,command,calculateCRC(command)& 0xFF,ETX); 
return true; 
}

/*------Send 3 byte value ext command*/
bool sendThreeByteExtCommand( char *command,  int value){

char buffer [3];
itoa (value,buffer,16); 
if(value<16){
    strcat(command,"0");
}
if(value<256){
    strcat(command,"0");
}
strcat(command,buffer);
Serial.printf("%c%s%02x%c",DC4,command,calculateCRC(command)& 0xFF,ETX); 
return true; 
}



/*-----Get Main Volume----*/ 
void readMainVolume(uint8_t byte1, uint8_t byte2){     
char vols[2];                                          //ugly:(
vols[0]=char(byte1);
vols[1]=char(byte2);
int vol = (int) strtol(vols, 0, 16);
if(vol>0){    
    vol =vol-38;
    vol =vol/2;    
}
serialNode.setProperty("volume").send(String(vol));
Serial.printf("%c%s%c",STX,"22001",ETX);               //Get Text    
}

/*-----Set Volume----*/
bool volumeHandler(const HomieRange& range, const String& value) {
int input=value.toInt();
input = input*2+38;
Serial.printf("%c230%x%c",STX,input,ETX); 
return true;  
}


/*-----Get Bass----*/ 
void readBass(uint8_t byte1, uint8_t byte2){     
char basss[2];                                          //ugly:(
basss[0]=char(byte1);
basss[1]=char(byte2);
int bass = (int) strtol(basss, 0, 16);
if(bass>0){    
    bass =bass*4.1666666666666666;    
}
serialNode.setProperty("bass").send(String(bass));
}

/*-----Set Bass-----*/
bool bassHandler(const HomieRange& range, const String& value) {
int bass=value.toInt();
bass = bass/4.1666666666666666;
char command [15];
strcpy(command,"20090331001");
sendTwoByteExtCommand(command,bass);
strcpy(command,"2006033000");
sendReadExtCommand(command);
return true;  
}


/*-----Get Treble----*/ 
void readTreble(uint8_t byte1, uint8_t byte2){     
char trebs[2];                                          //ugly:(
trebs[0]=char(byte1);
trebs[1]=char(byte2);
int treb = (int) strtol(trebs, 0, 16);
if(treb>0){    
    treb =treb*4.1666666666666666;    
}
serialNode.setProperty("treble").send(String(treb));
}

/*-----Set Treble-----*/
bool trebleHandler(const HomieRange& range, const String& value) {
int treble=value.toInt();
treble = treble/4.1666666666666666;
char command [15];
strcpy(command,"20090331011");
sendTwoByteExtCommand(command,treble);
strcpy(command,"2006033001");
sendReadExtCommand(command);
return true;  
}
  

/*-----Get Center Distance----*/ 
void readCenterDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char cds[3];                                          
cds[0]=char(byte1);
cds[1]=char(byte2);
cds[2]=char(byte3);
int cd = (int) strtol(cds, 0, 16);
serialNode.setProperty("centerdist").send(String(cd));
}

/*-----Set Center Distance-----*/
bool centerDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"2009041100");
sendThreeByteExtCommand(command,dist);
strcpy(command,"2006041000"); 
sendReadExtCommand(command);
return true;  
}

/*-----Get Front L Distance----*/ 
void readFrontlDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char fls[3];                                          
fls[0]=char(byte1);
fls[1]=char(byte2);
fls[2]=char(byte3);
int fl = (int) strtol(fls, 0, 16);
serialNode.setProperty("frontldist").send(String(fl));
}

/*-----Set Front L Distance-----*/
bool frontLeftDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"2009041120");
sendThreeByteExtCommand(command,dist);
strcpy(command,"2006041020"); 
sendReadExtCommand(command);
return true;  
}

/*-----Get Front R Distance----*/ 
void readFrontrDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char frs[3];                                          
frs[0]=char(byte1);
frs[1]=char(byte2);
frs[2]=char(byte3);
int fr = (int) strtol(frs, 0, 16);
serialNode.setProperty("frontrdist").send(String(fr));
}

/*-----Set Front R Distance-----*/
bool frontRightDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"2009041130");
sendThreeByteExtCommand(command,dist);
strcpy(command,"2006041030"); 
sendReadExtCommand(command);
return true;
}

/*-----Get Surround L Distance----*/ 
void readSurrlDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char sls[3];                                          
sls[0]=char(byte1);
sls[1]=char(byte2);
sls[2]=char(byte3);
int sl = (int) strtol(sls, 0, 16);
serialNode.setProperty("surroundldist").send(String(sl));
}

/*-----Set Surround L Distance-----*/
bool surroundLeftDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"2009041140");
sendThreeByteExtCommand(command,dist); 
strcpy(command,"2006041040"); 
sendReadExtCommand(command);
return true;
}

/*-----Get Surround R Distance----*/ 
void readSurrrDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char srs[3];                                          
srs[0]=char(byte1);
srs[1]=char(byte2);
srs[2]=char(byte3);
int sr = (int) strtol(srs, 0, 16);
serialNode.setProperty("surroundrdist").send(String(sr));
}

/*-----Set Surround R Distance-----*/
bool surroundRightDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"2009041150");
sendThreeByteExtCommand(command,dist);
strcpy(command,"2006041050"); 
sendReadExtCommand(command);
return true;
}

/*-----Get Subwoofer Distance----*/ 
void readSubwooferDist(uint8_t byte1, uint8_t byte2, uint8_t byte3){     
char sws[3];                                          
sws[0]=char(byte1);
sws[1]=char(byte2);
sws[2]=char(byte3);
int sw = (int) strtol(sws, 0, 16);
serialNode.setProperty("subwooferrdist").send(String(sw));
}

/*-----Set Subwoofer Distance-----*/
bool subwooferDistanceHandler(const HomieRange& range, const String& value) {
int dist=value.toInt();
char command [15];
strcpy(command,"20090411A0");
sendThreeByteExtCommand(command,dist);
strcpy(command,"20060410A0"); 
sendReadExtCommand(command);
return true;
}


/*-----Get Fanmode-----*/
bool readFanMode(uint8_t fanmode){
    if(fanmode==ASCII_1){
      serialNode.setProperty("fanmode").send("ON");  
    }else {
        serialNode.setProperty("fanmode").send("OFF");  
    }
return true;
}
/*-----Get Fanmode-----*/

/*-----Set Fanmode-----*/
bool fanmodeHandler(const HomieRange& range, const String& value) {
if(value=="ON" || value =="1" || value =="true"){
    Serial.printf("%c%s%c", STX, "2B201", ETX) ; 
}
else{
    Serial.printf("%c%s%c", STX, "2B200", ETX) ;
}
return true;
}
/*-----Set Fanmode-----*/


/*-----Get Dimmer-----*/
bool readDimmer(uint8_t value){
char dimmers[2];                                          //ugly:(
dimmers[0]=char(ASCII_0);
dimmers[1]=char(value);
int dimmer = (int) strtol(dimmers, 0, 16);
dimmer= dimmer*25;
serialNode.setProperty("dimmer").send(String(dimmer));   
return true;
}
/*-----Get Dimmer-----*/

/*-----Set Dimmer-----*/
bool dimmerHandler(const HomieRange& range, const String& value) {
int dimmer=value.toInt();
dimmer = dimmer/25;
Serial.printf("%c2610%x%c",STX,dimmer,ETX); 
return true;
}
/*-----Set Dimmer-----*/

/*-----Get Pure Direct-----*/
bool readPureDirect(uint8_t puredirect){
if(puredirect==ASCII_1){
    serialNode.setProperty("puredirect").send("ON");
}else {
    serialNode.setProperty("puredirect").send("OFF");
}
return true;
}
/*-----Get Pure Direct-----*/

/*-----Set Pure Direct-----*/
bool puredirectHandler(const HomieRange& range, const String& value) {
if(value=="ON" || value =="1" || value =="true"){
    Serial.printf("%c%s%c", STX, "07E80", ETX) ;                                                   // PD ON
}
else{
    Serial.printf("%c%s%c", STX, "07E82", ETX) ;                                                   // PD OFF
}      
return true;  
}
/*-----Set Pure Direct-----*/


/*-----Get Nightmode-----*/
void readNightMode(uint8_t byte1,uint8_t byte2){
if(byte1==ASCII_0){ 
    serialNode.setProperty("nightmode").send("0");
    }
    if(byte1==ASCII_1){ 
        switch(byte2){  
            case ASCII_0 : {
                serialNode.setProperty("nightmode").send("1");
            }
            case ASCII_1 : {
                serialNode.setProperty("nightmode").send("2");
            }
            case ASCII_2 : {
                serialNode.setProperty("nightmode").send("3");
            }     
        }                   
    }else if(byte1==ASCII_2){ 
        switch(byte2){
            case ASCII_0 : {
                serialNode.setProperty("nightmode").send("4");
            }
            case ASCII_1 : {
                serialNode.setProperty("nightmode").send("5");
            }
            case ASCII_2 : {
                serialNode.setProperty("nightmode").send("6");
            }
        }  
    }
}

/*-----Set Nightmode-----*/
bool nightmodeHandler(const HomieRange& range, const String& value) {
int input=value.toInt();
switch(input){
    case 0 : {
        Serial.printf("%c%s%c", STX, "28B00", ETX) ;   //OFF
        break;
    };
    case 1 : {
        Serial.printf("%c%s%c", STX, "28B10", ETX) ;   //Cinema Low
        break;
    };
    case 2 : {
        Serial.printf("%c%s%c", STX, "28B11", ETX) ;   //Cinema Mid
        break;
    };
    case 3 : {
        Serial.printf("%c%s%c", STX, "28B12", ETX) ;   //Cinema High
        break;
    };
    case 4 : {
        Serial.printf("%c%s%c", STX, "28B20", ETX) ;   //Music Low
        break;
    };
    case 5 : {
        Serial.printf("%c%s%c", STX, "28B21", ETX) ;   //Music Mid
        break;
    };
    case 6 : {
        Serial.printf("%c%s%c", STX, "28B22", ETX) ;   //Music High
        break;
    };  
}  
return true;  
}



/*-----Set Tuner Preset-----*/
bool presetHandler(const HomieRange& range, const String& value) {
int preset=value.toInt();
Serial.printf("%c%s%c", STX, "07A16", ETX) ;   //Tuner

switch(preset){
    case 1 : {
        Serial.printf("%c%s%c", STX, "07AE5", ETX) ;   //Preset1
        break;
    };
    case 2 : {
        Serial.printf("%c%s%c", STX, "07AE6", ETX) ;   //Preset2
        break;
    };
    case 3 : {
        Serial.printf("%c%s%c", STX, "07AE7", ETX) ;   //Preset3
        break;
    };
    case 4 : {
        Serial.printf("%c%s%c", STX, "07AE8", ETX) ;   //Preset4
        break;
    };
    case 5 : {
        Serial.printf("%c%s%c", STX, "07AE9", ETX) ;   //Preset5
        break;
    };
    case 6 : {
        Serial.printf("%c%s%c", STX, "07AEA", ETX) ;   //Preset6
        break;
    };
    case 7 : {
        Serial.printf("%c%s%c", STX, "07AEB", ETX) ;   //Preset7
        break;
    };
    case 8 : {
        Serial.printf("%c%s%c", STX, "07AEC", ETX) ;   //Preset8
        break;
    };
}  
return true;  
}



/*-----Get Programm-----*/
bool readProgram(uint8_t byte1,uint8_t byte2){
if(byte1==ASCII_0){
    switch(byte2){
        case ASCII_5 : {
            serialNode.setProperty("program").send("1");        //Vienna
            break;
        }        
        case ASCII_E : {
            serialNode.setProperty("program").send("2");        //Bottom Line
            break;
        }
    }
}else if(byte1==ASCII_1){
    switch(byte2){
        case ASCII_0 : {
            serialNode.setProperty("program").send("3");        //Roxy Theater
            break;
        }
        case ASCII_4 : {
            serialNode.setProperty("program").send("11");       //Disco
            break;
        }
        case ASCII_6 : {
            serialNode.setProperty("program").send("12");       //Game
            break;
        }
        case ASCII_7 : {
            serialNode.setProperty("program").send("4");        //7 Stereo
            break;
        }
        case ASCII_8 : {
            serialNode.setProperty("program").send("18");       //Music Pop
            break;
        }
    }    
}else if(byte1==ASCII_2){
switch(byte2){
    case ASCII_0 : {
        serialNode.setProperty("program").send("13");        //Mono Movie
        break;
    }
    case ASCII_1 : {
        serialNode.setProperty("program").send("17");        //Sports
        break;
    }
    case ASCII_4 : {
        serialNode.setProperty("program").send("14");        //Spactacle
        break;
    }
    case ASCII_5 : {
        serialNode.setProperty("program").send("15");        //Sci-Fi
        break;
    }
    case ASCII_8 : {
        serialNode.setProperty("program").send("16");        //Adventure
        break;
    }
    case ASCII_9 : {
        serialNode.setProperty("program").send("19");        //General
        break;
    }
    case ASCII_C : {
        serialNode.setProperty("program").send("9");         //Prologic
        break;
    }
    case ASCII_D : {
        serialNode.setProperty("program").send("10");        //Standard
        break;
    }
}
}else if(byte1==ASCII_3){
switch(byte2){
    case ASCII_4 : {
        serialNode.setProperty("program").send("8");        //2ch Stereo
        break;
        }
    case ASCII_6 : {
        serialNode.setProperty("program").send("6");        //THX Cinema
        break;
        }
    case ASCII_7 : {
        serialNode.setProperty("program").send("20");        //THX Music
        break;
        }
    case ASCII_C : {
        serialNode.setProperty("program").send("7");        //THX Game
        break;
    }
}
}else if(byte1==ASCII_8){
        serialNode.setProperty("program").send("5");        //STRAIGHT

/*}else if(byte1==ASCII_9){
        serialNode.setProperty("program").send("5");        //STRAIGHT 
}else if(byte1==ASCII_A){
        serialNode.setProperty("program").send("5");        //STRAIGHT*/  
}
return true;                                
}
/*-----Get Programm-----*/


/*-----Set Programm-----*/
bool programHandler(const HomieRange& range, const String& value) { 
int program=value.toInt();   
switch(program){
    case 1 : { 
        Serial.printf("%c%s%c", STX, "07EE5", ETX); //Vienna
        break;
        }
    case 2 : { 
        Serial.printf("%c%s%c", STX, "07EEC", ETX); //Bottom Line
        break;
        }
    case 3 : { 
        Serial.printf("%c%s%c", STX, "07EED", ETX); //Roxy Theater	
        break;					
        }		
    case 4 : { 
        Serial.printf("%c%s%c", STX, "07EFF", ETX); //7 Stereo				
        break;
        }	
    case 5 : { 
        Serial.printf("%c%s%c", STX, "07EE0", ETX); //Straight						
        break;
        }	
   case 6 : { 
       Serial.printf("%c%s%c", STX, "07EC2", ETX); //THX Cinema						
       break;
       }
    case 7 : { 
        Serial.printf("%c%s%c", STX, "07EC8", ETX); //THX Gamne				
        break;
        }	
    case 8 : { 
        Serial.printf("%c%s%c", STX, "07EC0", ETX); //2ch Stereo						
        break;
        }	
    case 9 : { 
        Serial.printf("%c%s%c", STX, "07EFD", ETX); //ProLogic						
        break;
        }	
    case 10 : { 
        Serial.printf("%c%s%c", STX, "07EFE", ETX); //Standard						
        break;
        }
    case 11 : { 
        Serial.printf("%c%s%c", STX, "07EF0", ETX); //Disco						
        break;
        }
    case 12 : { 
        Serial.printf("%c%s%c", STX, "07EF2", ETX); //Game						
        break;
        }
    case 13 : { 
        Serial.printf("%c%s%c", STX, "07EF7", ETX); //Mono Movie						
        break;
        }
    case 14 : { 
        Serial.printf("%c%s%c", STX, "07EF9", ETX); //Spactacle						
        break;
        }
    case 15 : { 
        Serial.printf("%c%s%c", STX, "07EFA", ETX); //Sci-Fi						
        break;
        }
    case 16 : { 
        Serial.printf("%c%s%c", STX, "07EFB", ETX); //Adventure						
        break;
        }
    case 17 : { 
        Serial.printf("%c%s%c", STX, "07EF8", ETX); //Sports						
        break;
        }
    case 18 : { 
        Serial.printf("%c%s%c", STX, "07EF3", ETX); //Music Pop						
        break;
        }  
    case 19 : { 
        Serial.printf("%c%s%c", STX, "07EFC", ETX); //General						
        break;
        }
    case 20 : { 
        Serial.printf("%c%s%c", STX, "07EC3", ETX); //THX Music						
        break;
        }
	}  
return true;  
}
/*-----Set Programm-----*/


/*-----Set Power-----*/
bool powerHandler(const HomieRange& range, const String& value) {
if(value=="ON" || value =="1" || value =="true"){
    Serial.printf("%c%s%c", DC1, "000", ETX) ; 
    Serial.printf("%c%s%c", STX, "20000", ETX) ; 
    Serial.printf("%c%s%c", STX, "20100", ETX) ; 
    Serial.printf("%c%s%c", STX, "07E7E", ETX) ;
}
else{
    Serial.printf("%c%s%c", STX, "07E7F", ETX) ;
}
return true;
}
/*-----Reset Receiver!!-----*/
bool resetHandler(const HomieRange& range, const String& value) {
if(value=="ON" || value =="1" || value =="true"){
    Serial.printf("%c%c%c%c%c", DC3, DEL, DEL, DEL, ETX) ;
}
return true;
}
/*-----Set Power-----*/


/*-----Get Input-----*/
bool readInput(uint8_t input){
switch(input){
    case ASCII_0:{
        serialNode.setProperty("input").send("5");           //PHONO
        break;
    }
    case ASCII_1:{
        serialNode.setProperty("input").send("4");           //CD
        break;
    }
    case ASCII_2:{
        serialNode.setProperty("input").send("1");           //TUNER
        break;
    }
    case ASCII_3:{
        serialNode.setProperty("input").send("6");
        break;
    }
    case ASCII_4:{
        serialNode.setProperty("input").send("7");           //MD
        break;
    }
    case ASCII_5:{
        serialNode.setProperty("input").send("3");           //DVD
        break;
    }
    case ASCII_6:{
        serialNode.setProperty("input").send("8");
        break;
    }
    case ASCII_7:{
        serialNode.setProperty("input").send("2");
        break;
    }
    case ASCII_9:{
        serialNode.setProperty("input").send("9");
        break;
    }
    case ASCII_A:{
        serialNode.setProperty("input").send("10");
        break;
    }
    case ASCII_C:{
        serialNode.setProperty("input").send("11");         //V_AUX
        break;
    }                        
} 
return true;  
}

/*-----Set Input-----*/
bool inputHandler(const HomieRange& range, const String& value) {
int input=value.toInt();
switch(input){
    case 1 : {
        Serial.printf("%c%s%c", STX, "07A16", ETX) ;   //Tuner
        break;
    };
    case 2 : {
        Serial.printf("%c%s%c", STX, "07AC0", ETX) ;   //CBL/Sat
        break;
    };
    case 3 : {
        Serial.printf("%c%s%c", STX, "07AC1", ETX) ;   //DVD
        break;
    };
    case 4 : {
        Serial.printf("%c%s%c", STX, "07A15", ETX) ;   //CD
        break;
    };
    case 5 : {
        Serial.printf("%c%s%c", STX, "07A14", ETX) ;   //PHONO
        break;
    };
    case 6 : {
        Serial.printf("%c%s%c", STX, "07A19", ETX) ;   //CD-R
        break;
    };
    case 7 : {
        Serial.printf("%c%s%c", STX, "07A18", ETX) ;   //MD
        break;
    };
    case 8 : {
        Serial.printf("%c%s%c", STX, "07A54", ETX) ;   //DTV
        break;
    };
    case 9 : {
        Serial.printf("%c%s%c", STX, "07A0F", ETX) ;   //VCR1
        break;
    };
    case 10 : {
        Serial.printf("%c%s%c", STX, "07A13", ETX) ;   //DVR
        break;
    };
    case 11 : {
        Serial.printf("%c%s%c", STX, "07A55", ETX) ;   //VAUX
        break;
    };
}
return true;
}
/*-----Set Input-----*/


/*-----Get Audio Select-----*/
bool readAudioSelect(uint8_t audioSelect){
switch(audioSelect){
    case ASCII_0 : {
        serialNode.setProperty("audioselect").send("1");        //Auto
        break;
    }
    case ASCII_3 : {
        serialNode.setProperty("audioselect").send("2");        //COAX/OPT
        break;
    }
    case ASCII_4 : {
        serialNode.setProperty("audioselect").send("3");        //ANALOG
        break;
    }
    case ASCII_8 : {
        serialNode.setProperty("audioselect").send("4");        //HDMI
        break;
    }
}
return true;
}
/*-----Get Audio Select-----*/

/*-----Set Audio Select-----*/
bool audioselectHandler(const HomieRange& range, const String& value) { 
int audioselect=value.toInt();
switch(audioselect){
    case 1 : {
        Serial.printf("%c%s%c", STX, "07EA6", ETX) ;   //Auto
        break;
    };
    case 2 : {
        Serial.printf("%c%s%c", STX, "07EA9", ETX) ;   //COAX
        break;
    };
    case 3 : {
        Serial.printf("%c%s%c", STX, "07EAA", ETX) ;   //Analog
        break;
    };
    case 4 : {
        Serial.printf("%c%s%c", STX, "07EDA", ETX) ;   //HDMI
        break;
    };
}
return true;  
}
/*-----Set Audio Select-----*/

/*-----Get Audio mute-----*/
bool readAudioMute(uint8_t audioMute){
if(audioMute==ASCII_1){
    serialNode.setProperty("mute").send("ON");
}else {
    serialNode.setProperty("mute").send("OFF");
}
return true;
}
/*-----Get Audio mute-----*/

/*-----Set Audio mute-----*/
bool muteHandler(const HomieRange& range, const String& value) {
    serialNode.setProperty("debug").send(value);
if(value=="ON" || value =="1" || value =="true"){
    Serial.printf("%c%s%c", STX, "07EA2", ETX) ;                                                   // Mute ON
}
else{
    Serial.printf("%c%s%c", STX, "07EA3", ETX) ;                                                   // Mute OFF
}      
return true;  
}
/*-----Set Audio mute-----*/


/*-----Get Bitrate------*/
bool readBitrate(uint8_t bitrate){
switch(bitrate){
    case ASCII_0: {
        serialNode.setProperty("fs").send("Analog");
        break;
    }
    case ASCII_1: {
        serialNode.setProperty("fs").send("32kHz");
        break;
    }
    case ASCII_2: {
        serialNode.setProperty("fs").send("44.1kHz");
        break;
    }
    case ASCII_3: {
        serialNode.setProperty("fs").send("48kHz");
        break;
    }
    case ASCII_4: {
        serialNode.setProperty("fs").send("64kHz");
        break;
    }
    case ASCII_5: {
        serialNode.setProperty("fs").send("88.2kHz");
        break;
    }
    case ASCII_6: {
        serialNode.setProperty("fs").send("96kHz");
        break;
    }
    case ASCII_7: {
        serialNode.setProperty("fs").send("Unknown");
        break;
    }
    case ASCII_8: {
        serialNode.setProperty("fs").send("128.0 kHz");
        break;
    }
    case ASCII_9: {
        serialNode.setProperty("fs").send("176.4 kHz");
        break;
    }
    case ASCII_A: {
        serialNode.setProperty("fs").send("192.0 kHz");
        break;
    }
    case ASCII_B: {
        serialNode.setProperty("fs").send("48kHz");
        break;
    }
}
return true;
}
/*-----Get Bitrate------*/


/*-------Get Format-----------*/
bool readFormat(uint8_t format){
switch(format){
    case ASCII_0: {
        serialNode.setProperty("format").send("6CH Input");
        break;
    }
    case ASCII_1: {
        serialNode.setProperty("format").send("Analog");
        break;
    }
    case ASCII_2: {
        serialNode.setProperty("format").send("PCM");
        break;
    }
    case ASCII_3: {
        serialNode.setProperty("format").send("Dolby Digital ");
        break;
    }
    case ASCII_4: {
        serialNode.setProperty("format").send("Dolby Digital 2.0");
        break;
    }
    case ASCII_5: {
        serialNode.setProperty("format").send("D.D.karaoke");
        break;
    }
    case ASCII_6: {
        serialNode.setProperty("format").send("D.D.EX");
        break;
    }
    case ASCII_7: {
        serialNode.setProperty("format").send("DTS");
        break;
    }
    case ASCII_8: {
        serialNode.setProperty("format").send("DTS ES");
        break;
    }
    case ASCII_9: {
        serialNode.setProperty("format").send("Other Digital");
        break;
    }
}
return true;
}
/*-------Get Format-----------*/


/*-----Get serial Data in Loop-----*/
void loopHandler() {
uint8_t serial_buf[BUFFER_SIZE];
size_t bytes_read=0;
String rcmd ="";   

while(Serial.available() && bytes_read < BUFFER_SIZE) {
    serial_buf[bytes_read] = Serial.read();
    #ifdef DEBUG_MODE 
        rcmd.concat(char(serial_buf[bytes_read]));
    #endif
    if(serial_buf[bytes_read]==DC1 || serial_buf[bytes_read]==DC2 || serial_buf[bytes_read]==DC3 || serial_buf[bytes_read]==DC4 || serial_buf[bytes_read]==STX ){
        bytes_read = 0;
    }
    if(serial_buf[bytes_read]==ETX){
        #ifdef DEBUG_MODE  
            serialNode.setProperty("debug").send(rcmd);
        #endif
        if(serial_buf[0]==DC1) {
            if(serial_buf[1]==ASCII_0){
                if(serial_buf[2]==ASCII_1){
                     serialNode.setProperty("volumetext").send(rcmd.substring(4,bytes_read));
                } 
            } 

        }else if(serial_buf[0]==DC2) {
            if(serial_buf[17]!=ASCII_0 && bytes_read > 133){            //Power On?          Report20       
                readInput(serial_buf[18]);                              //Get Input          Report21
                readAudioSelect(serial_buf[20]);                        //Get AudioSelect    Report22
                readAudioMute(serial_buf[21]);                          //Get AudioMute      Report23
                readMainVolume(serial_buf[24],serial_buf[25]);          //Get MainVolume     Report26
                readProgram(serial_buf[28],serial_buf[29]);             //Get Program        Report28
                readNightMode(serial_buf[36],serial_buf[37]);           //Get Nightmode      Report8B
                readDimmer(serial_buf[94]);                             //Get Dimmer         Report61
                readFanMode(serial_buf[132]);                           //Get FanMode        ReportB2
                readPureDirect(serial_buf[135]);                        //Get PureDirect     Report8C
                delay(100);
                char command [10];
                strcpy(command,"20050000F");                            //Get Type
                sendReadExtCommand(command);
                strcpy(command,"2006033000");                           //Get Bass
                sendReadExtCommand(command);
                strcpy(command,"2006033001");                           //Get Treble
                sendReadExtCommand(command);
                strcpy(command,"2006041000");                           //Get Center Distance
                sendReadExtCommand(command);
                strcpy(command,"2006041020");                           //Get Fromt L Distance
                sendReadExtCommand(command);
                strcpy(command,"2006041030");                           //Get Front R Distance
                sendReadExtCommand(command);
                strcpy(command,"2006041040");                           //Get Surr L Distance                    
                sendReadExtCommand(command);
                strcpy(command,"2006041050");                           //Get Surr R Distance                    
                sendReadExtCommand(command);
                strcpy(command,"20060410A0");                           //Get  Sub Distance                    
                sendReadExtCommand(command);
            }    
        }else if(serial_buf[0]==DC3) {
        }else if(serial_buf[0]==DC4) {
            if(serial_buf[6]==ASCII_0){
                if(serial_buf[7]==ASCII_0){
                   if(serial_buf[8]==ASCII_0){
                       if(serial_buf[9]==ASCII_F){
                           serialNode.setProperty("type").send(rcmd.substring(12,bytes_read-2));        //Get Type
                        }
                    } 
                }
            }else if(serial_buf[6]==ASCII_3){
                if(serial_buf[7]==ASCII_3){
                   if(serial_buf[8]==ASCII_0){
                       if(serial_buf[9]==ASCII_0){                                                      //0= Speaker, 1=Headphone
                            if(serial_buf[10]==ASCII_0){                                                //Get Bass
                                readBass(serial_buf[12],serial_buf[13]);       
                            }else if(serial_buf[10]==ASCII_1){                                          //Get Treble
                                readTreble(serial_buf[12],serial_buf[13]);    
                            }                                               
                        }
                    } 
                }
            } else if(serial_buf[6]==ASCII_4){
                if(serial_buf[7]==ASCII_1){
                   if(serial_buf[8]==ASCII_0){
                        if(serial_buf[10]==ASCII_0){                                                    //Get Unit ID must be meter
                            if(serial_buf[9]==ASCII_0){                                                 //0= Center
                                readCenterDist(serial_buf[11],serial_buf[12],serial_buf[13]);       
                            }else if(serial_buf[9]==ASCII_2){                                          
                                readFrontlDist(serial_buf[11],serial_buf[12],serial_buf[13]);           //2= Front L      
                            }else if(serial_buf[9]==ASCII_3){                                          
                                readFrontrDist(serial_buf[11],serial_buf[12],serial_buf[13]);           //3= Front R      
                            }else if(serial_buf[9]==ASCII_4){                                          
                                readSurrlDist(serial_buf[11],serial_buf[12],serial_buf[13]);            //4= Surr L      
                            }else if(serial_buf[9]==ASCII_5){                                          
                                readSurrrDist(serial_buf[11],serial_buf[12],serial_buf[13]);            //5= Surr R      
                            }else if(serial_buf[9]==ASCII_A){                                          
                                readSubwooferDist(serial_buf[11],serial_buf[12],serial_buf[13]);        //A= Sub     
                            }                                                   
                        }else if(serial_buf[10]==ASCII_1){
                            serialNode.setProperty("debug").send("Please setup your receiver to meters");        
                        }
                    } 
                }
            }     
        }else if(serial_buf[0]==STX) {                                      //Reportcommand
            if(serial_buf[3]==ASCII_0){                                     //RCMD0=0 System Status Reports
                if(serial_buf[4]==ASCII_0){                                 //RCMD1=0  system

                    if(serial_buf[6]==ASCII_0){                             //0=OK
                        //serialNode.setProperty.send("power","ON");
                    }else if (serial_buf[6]==ASCII_2) {                     //2=Standby
                        //serialNode.setProperty.send("power","OFF");
                    }
                }
            }else if(serial_buf[3]==ASCII_1){                               //RCMD0=1 Playback Status Reports
                if(serial_buf[4]==ASCII_0){                                 //RCMD1=0 Format      Report10
                    readFormat(serial_buf[6]);
                }else if(serial_buf[4]==ASCII_1){                                  //RCMD1=1 Fs                Report11
                    readBitrate(serial_buf[6]);
                }else if(serial_buf[4]==ASCII_2){                                  //RCMD1=2                   Report12
                    
                }else if(serial_buf[4]==ASCII_3){                                  //RCMD1=3                   Report13
                    
                }else if(serial_buf[4]==ASCII_4){                                  //RCMD1=4                   Report14
                    
                }else if(serial_buf[4]==ASCII_5){                                  //RCMD1=5                   Report15
                    
                }
            }else if(serial_buf[3]==ASCII_2){
                if(serial_buf[4]==ASCII_0){                                       //RCMD1=0 0=Power            Report20
                    if(serial_buf[6]==ASCII_0){
                        serialNode.setProperty("power").send("OFF");
                    }else if(serial_buf[6]==ASCII_2){
                        serialNode.setProperty("power").send("ON");
                    }
                }else if(serial_buf[4]==ASCII_1){                                  //RCMD1=1 1=Input           Report21
                    readInput(serial_buf[6]);             
                }else if(serial_buf[4]==ASCII_2){                                  //RCMD1=1 3=Audio Select    Report22
                    readAudioSelect(serial_buf[6]);
                }else if(serial_buf[4]==ASCII_3){                                  //RCMD1=1 3=Audio Mute      Report23
                    readAudioMute(serial_buf[6]);
                }else if(serial_buf[4]==ASCII_6){                                  //RCMD1=1 6=Main Volume     Report26
                    readMainVolume(serial_buf[5],serial_buf[6]);                                   
                }else if(serial_buf[4]==ASCII_8){                                  //RCMD1=1 8=Program         Report28
                    readProgram(serial_buf[5],serial_buf[6]); 
                }
            }else if(serial_buf[3]==ASCII_6){
                if(serial_buf[4]==ASCII_1){                                        //RCMD=6  1=Dimmer          Report61
                    readDimmer(serial_buf[6]);
                }
            }else if(serial_buf[3]==ASCII_8){
                if(serial_buf[4]==ASCII_B){                                        //RCMD1=8 B=Nightmode       Report8B
                    readNightMode(serial_buf[5],serial_buf[6]);
                }
                if(serial_buf[4]==ASCII_C){                                        //RCMD1=8 C=Pure Direct     Report8C
                    readPureDirect(serial_buf[6]);
                }   
            }
        }
        bytes_read=0;
        rcmd ="";
    }else {
        bytes_read++;
    }
    delay(2);
}
}

void setup() {
    Homie.disableLogging();
    Serial.begin(SERIAL_BAUD);

    Homie_setFirmware(FW_NAME, FW_VERSION);
	Homie.setLoopFunction(loopHandler);
	//Homie.disableLedFeedback(); 
    Homie.setup();

    serialNode.advertise("command").setDatatype("string").settable(commandHandler).setName("command");
    serialNode.advertise("commandext").setDatatype("string").settable(commandExtHandler).setName("commandext");
    serialNode.advertise("format").setDatatype("string").setName("format");
    serialNode.advertise("debug").setDatatype("string").setName("debug");
    serialNode.advertise("fs").setDatatype("string").setName("bitrate");
    serialNode.advertise("input").settable(inputHandler).setDatatype("integer").setName("input");
    serialNode.advertise("power").setDatatype("boolean").settable(powerHandler).setName("power");
    serialNode.advertise("volume").settable(volumeHandler).setDatatype("integer").setName("volume");
    serialNode.advertise("volumetext").setDatatype("string").setName("volumetext");
    serialNode.advertise("nightmode").settable(nightmodeHandler).setDatatype("integer").setName("nightmode");
    serialNode.advertise("preset").settable(presetHandler).setDatatype("integer").setName("preset");
    serialNode.advertise("program").settable(programHandler).setDatatype("integer").setName("program");
    serialNode.advertise("mute").setDatatype("boolean").settable(muteHandler).setName("mute");
    serialNode.advertise("audioselect").settable(audioselectHandler).setDatatype("integer").setName("audioselect");
    serialNode.advertise("fanmode").setDatatype("boolean").settable(fanmodeHandler).setName("fanmode");
    serialNode.advertise("bass").setDatatype("integer").settable(bassHandler).setName("bass");
    serialNode.advertise("treble").setDatatype("integer").settable(trebleHandler).setName("treble");
    serialNode.advertise("centerdist").setDatatype("integer").setUnit("m").settable(centerDistanceHandler).setName("centerdist");
    serialNode.advertise("frontldist").setDatatype("integer").setUnit("m").settable(frontLeftDistanceHandler).setName("centerdist");
    serialNode.advertise("frontrdist").setDatatype("integer").setUnit("m").settable(frontRightDistanceHandler).setName("frontrdist");
    serialNode.advertise("surroundldist").setDatatype("integer").setUnit("m").settable(surroundLeftDistanceHandler).setName("surroundldist");
    serialNode.advertise("surroundrdist").setDatatype("integer").setUnit("m").settable(surroundRightDistanceHandler).setName("surroundrdist");
    serialNode.advertise("subwooferrdist").setDatatype("integer").setUnit("m").settable(subwooferDistanceHandler).setName("subwooferrdist");
    serialNode.advertise("dimmer").setDatatype("integer").settable(dimmerHandler).setName("dimmer"); 
    serialNode.advertise("type").setDatatype("string").setName("type");
    serialNode.advertise("reset").setDatatype("boolean").settable(resetHandler).setName("reset"); 
    serialNode.advertise("puredirect").setDatatype("boolean").settable(puredirectHandler).setName("puredirect"); 
}

void loop() {
    Homie.loop();
}

