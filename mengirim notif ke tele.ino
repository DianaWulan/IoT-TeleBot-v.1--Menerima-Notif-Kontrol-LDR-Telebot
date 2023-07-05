//library
#include "CTBot.h"

//konfig wifi
String ssid = "";
String pass = "";

//konfig tele
String token = "";
const int id = ;

//var
int ldr = A0;
int sensorldr;
int ledmerah = D0;  //indikator pembacaan ldr
int ledhijau = D1;  //indikator tele
CTBot myBot;
int aktif = 1;

void bacaldr(){
  sensorldr = analogRead(ldr);
}

void setup() {
  Serial.begin(9600);
  pinMode(ldr, OUTPUT);
  pinMode(ledmerah, OUTPUT);
  pinMode(ledhijau, OUTPUT);
  
  //koneksi tele dari wifi
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  //uji koneksi tele
  while(myBot.testConnection()==false){
    Serial.print(".");
    delay(500);
  }
  Serial.println("Koneksi Berhasil");
  digitalWrite(ledhijau, HIGH);
  /*    
  if(myBot.testConnection()){
    Serial.println("Koneksi Berhasil");
    digitalWrite(ledhijau, HIGH);
  }else{
    Serial.println("Koneksi Gagal");
    digitalWrite(ledhijau, LOW);
  }*/

}

void loop() {
  //pembacaan nilai sensor ldr
  bacaldr();
  //Serial.println(sensorldr);
  
  //baca pesan yg diterima di tele
  TBMessage msg;
  if(myBot.getNewMessage(msg)){
    //baca pesan
    String pesan = msg.text;
    //uji on/off
    if(pesan == "ON"){
      aktif = 1;
      //mengatur pesan balasan ke user
      myBot.sendMessage(msg.sender.id, "sensor sudah diaktifkan");
    }else if(pesan == "OFF"){
      aktif = 0;
      //mengatur pesan balasan ke user
      myBot.sendMessage(msg.sender.id, "sensor sudah dinonaktifkan");
    }else if(pesan = "STATUS"){
      String kata = "Intensitas Cahaya : ";
      String chat = kata += String(sensorldr);
      myBot.sendMessage(msg.sender.id, chat);
    }
  }
  
  //aktivasi melalui tele
  if(aktif==1){
    if(sensorldr >=800){
      digitalWrite(ledmerah, HIGH);
      //mengirim notif ke tele
      myBot.sendMessage(id, "Ruangan Terlalu Terang!"); 
      String kata = "Intensitas Cahaya : ";
      String chat = kata += String(sensorldr);         
      myBot.sendMessage(id, chat);  
             
    }else{
    digitalWrite(ledmerah, LOW);
    }
  }else if(aktif==0){
    digitalWrite(ledmerah, LOW);
  }
  

  delay(1000);
}
