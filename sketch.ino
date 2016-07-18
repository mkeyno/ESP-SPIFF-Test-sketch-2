#include "FS.h" 
static const char * testFile = "2.bin";
 static uint8_t * payload;
 static uint8_t * buffer;


String formatBytes(size_t bytes){
       if (bytes < 1024)                 return String(bytes)+"B"; 
  else if(bytes < (1024 * 1024))         return String(bytes/1024.0)+"KB";
  else if(bytes < (1024 * 1024 * 1024))  return String(bytes/1024.0/1024.0)+"MB";
  else                                   return String(bytes/1024.0/1024.0/1024.0)+"GB";
}
void DIR(void){
  uint32_t total=0;
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
              String fileName = dir.fileName();
              size_t fileSize = dir.fileSize();
                    total+=fileSize;
              Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
            }
    Serial.print("\t\t\tTotal:");Serial.println(total);
  
}
 
 void writeFile()
 { 
 Serial.println("Write File Test");
 uint32_t startTime = millis();
 File f = SPIFFS.open(testFile, "w");
 if(!f){ Serial.println("Could not open file for writing");
   return; 
   } 
 size_t i; for(i=0;i<256;i++){ 
               f.write(payload, 4096); 
               optimistic_yield(10000); 
               Serial.print(".");
               } 
 Serial.println();
 f.close();
 uint32_t timeTaken = millis() - startTime; 
 Serial.printf("Writing 1MB took %u ms\n", timeTaken); 
 }
 
 void readFile(){ 
 Serial.println("Read File Test"); 
 uint32_t startTime = millis();
 File f = SPIFFS.open(testFile, "r");
 if(!f) { Serial.println("Could not open file for reading"); return; } 
size_t i = f.size();
 Serial.printf("File size is %u Bytes\n", i); 
 while(i > 4096){
         f.read(buffer, 4096);
         optimistic_yield(10000); 
         Serial.print("."); i -= 4096;
         } 
 f.read(buffer, i); 
 Serial.println();
 f.close(); 
 uint32_t timeTaken = millis() - startTime;
 Serial.printf("Reading took %u ms\n", timeTaken); 
 } 
 
void setup()
 { 
 Serial.begin(115200);
 Serial.setDebugOutput(true);
        payload = (uint8_t *)malloc(4096); if(payload == NULL){ Serial.println("could not malloc payload"); while(1) delay(1000); }
 memset(payload, 'P', 4096); 
       buffer = (uint8_t *)malloc(4096);   if(buffer == NULL) { Serial.println("could not malloc buffer"); while(1) delay(1000); } 
memset(buffer, 0, 4096);
 delay(5000); 
 SPIFFS.begin();
 DIR(); 
 //Serial.println("format start");
 //SPIFFS.format();
 //Serial.println("format end"); 
 }
 
 void loop() 
 { 
 //writeFile();
 //delay(500);
 readFile(); 
 delay(500);
 }
