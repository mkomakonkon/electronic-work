#include <SoftwareSerial.h>  //MP3シリアル転送用
#include <DFPlayer_Mini_Mp3.h> //MP3再生用
#include <avr/pgmspace.h>      //ROMへのデータ退避用
#include <OneWire.h>          //温度センサーDS18B20制御用
#include <DallasTemperature.h>//温度センサーDS18B20制御用
#include <Wire.h>   //RTC用ライブラリ
#include <DS3231.h> //RTC用ライブラリ

byte val=0;
  int   Flg_hot ,Flg_warm ,Flg_kaiteki ,Flg_cold; //状態監視用フラグ
  int   RTC_h_buf ,RTC_m_buf ,hour_12h; //RTC用変数
  int ledPin = 8;  // LED on Pin of Arduino 人感で点灯
  int pirPin = 9; // Input for HC-S501　人感センサー
  int pirValue; // Place to store read PIR Value　人感センサー
  int   hWDT, hWDTflg; //人間ウォッチドッグタイマー関連（humanWDT）
    
SoftwareSerial mySerial(11, 12); // MP3シリアルRX, TX
int output = 2;     //IR制御用ピン
#define ONE_WIRE_BUS 10 // 温度センサーのピン定義 ★5 KΩでプルアップしないと動作しないことがある★
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

DS3231 clock;   //RTC用設定
RTCDateTime dt; //RTC用設定

//エアコンIRデータ
const PROGMEM int aircon_on[] =  {46,44,41,44,42,44,41,44,42,44,42,2529,346,172,42,130,42,44,41,44,42,44,41,131,42,45,40,47,39,44,41,47,39,128,44,47,39,130,41,128,45,44,42,133,39,130,42,131,42,130,42,130,42,45,41,44,41,131,42,44,42,44,41,45,41,41,45,46,38,47,39,44,42,43,42,44,42,44,41,130,42,45,41,130,42,47,39,44,41,42,44,131,41,131,42,47,39,45,41,44,42,46,39,44,41,47,38,44,41,44,42,46,40,44,42,44,41,44,42,46,39,44,42,44,41,44,41,131,41,134,38,131,41,45,41,131,41,46,39,131,42,130,42,2000,0,1462,347,176,37,131,41,47,39,44,41,47,38,131,41,46,40,44,41,44,42,44,41,131,41,44,42,135,37,131,41,46,40,131,42,131,41,131,42,131,41,132,41,44,42,44,42,130,42,45,41,44,41,47,39,44,41,44,41,47,38,44,42,46,40,44,42,44,41,44,42,131,41,44,42,47,38,45,41,44,41,131,41,44,42,41,45,47,38,44,42,44,41,44,42,44,41,44,42,44,41,44,42,44,42,41,44,45,40,44,42,44,42,44,41,44,41,45,41,44,42,131,42,44,41,131,42,44,42,131,40,44,41,2000,0,1462,347,168,44,132,41,44,42,44,41,44,41,136,37,44,41,49,37,44,41,41,45,130,42,44,42,131,41,130,42,48,38,135,37,131,41,132,41,128,44,131,42,45,41,44,41,128,44,44,42,49,36,44,42,49,37,47,39,44,41,47,39,49,37,44,41,48,37,44,42,44,41,47,39,49,36,44,41,50,36,44,42,44,41,131,41,46,40,48,37,135,37,131,41,131,41,49,37,47,38,49,36,45,41,135,37,44,42,135,37,127,45,45,40,50,36,42,44,45,41,48,37,44,41,44,42,41,44,46,39,49,36,136,37,136,36,130,42,128,45,135,36,131,41,41,45,41,45,48,37,49,37,44,41,41,44,45,42,44,41,41,45,49,37,44,41,46,40,47,39,41,44,49,37,44,42,47,38,41,44,47,38,128,44,128,45,49,37,48,37,45,41,49,36,49,37,41,44,44,42,41,45,41,45,41,45,133,39,135,37,44,41,44,41,44,42,41,45,44,41,45,41,44,42,44,41,44,42,42,44,49,36,41,45,41,45,47,38,45,42,44,41,44,42,135,37,45,40,49,37,49,37,44,41,49,37,131,41,136,37,47,39,44,42,41,44,48,38,49,36,50,36,44,41,44,41,49,37,44,41,49,37,44,42,44,41,41,45,44,41,49,37,130,42,49,37,135,37,44,41,44,42,131,41,128,44,128,44};
const PROGMEM int aircon_off[] =  {48,38,45,41,48,38,47,38,45,41,45,2525,350,168,44,128,47,39,44,42,47,39,47,125,47,39,46,38,48,38,44,41,47,126,44,41,47,125,45,128,44,42,47,125,47,125,45,128,47,125,47,125,45,44,41,41,47,125,47,39,44,41,47,38,48,39,47,39,47,38,47,38,48,38,48,38,48,38,48,125,45,41,47,125,48,41,44,38,48,38,47,125,47,125,47,39,47,38,48,38,47,38,48,38,48,38,48,38,47,39,47,39,47,38,47,38,48,38,47,39,46,38,48,38,48,38,47,125,44,128,47,125,44,41,48,125,47,39,48,124,48,125,47,2000,0,1456,350,168,48,125,47,40,46,38,47,38,48,124,47,39,44,42,47,38,47,39,48,124,48,38,47,125,47,126,47,39,46,128,45,125,47,125,47,128,45,125,47,38,48,38,48,125,48,39,47,38,47,38,48,38,47,38,48,38,47,39,45,41,48,38,47,38,48,38,48,124,47,38,48,38,47,39,47,38,48,124,48,38,48,38,48,38,48,38,47,41,45,38,48,38,47,38,47,39,47,38,47,39,47,38,48,38,47,38,47,39,47,38,47,39,47,38,48,38,47,125,44,41,47,125,47,38,47,126,46,41,45,2000,0,1455,350,168,47,125,48,39,46,38,48,38,48,124,48,39,44,41,48,38,48,38,48,125,48,38,45,128,47,125,48,39,47,124,48,125,48,125,47,125,47,125,47,39,46,38,47,125,48,39,44,41,47,39,48,38,47,39,47,38,47,38,47,38,48,38,47,38,48,38,47,38,48,38,47,39,48,37,48,38,48,41,45,38,48,38,48,38,47,38,48,126,46,125,47,125,47,38,48,38,47,38,48,38,47,125,48,38,47,125,47,125,47,39,47,38,48,38,48,38,47,39,47,38,48,39,47,39,47,38,47,38,48,124,47,125,47,125,47,125,47,125,47,125,48,39,46,39,47,38,44,41,45,44,44,39,45,41,47,39,47,38,45,44,44,39,44,41,47,38,45,41,44,44,44,38,48,38,48,38,47,39,47,124,45,128,47,38,45,41,47,39,47,38,48,38,47,39,47,38,47,39,47,39,47,38,44,128,47,125,47,38,47,39,44,41,47,39,45,41,45,41,48,38,45,41,45,42,44,41,45,41,44,41,45,41,47,39,44,41,44,42,47,39,44,128,47,38,45,42,44,41,45,41,47,39,45,127,47,125,44,41,45,41,47,38,45,41,47,39,44,41,45,41,44,42,44,42,44,42,44,41,45,41,44,42,44,41,45,41,44,41,45,41,45,41,44,128,44,42,44,41,45,128,44,128,44,128,45};

//電気点灯IRデータ→メモリ点灯に変更
//全灯　const PROGMEM int light_on[] =  {202,101,564,102,154,50,154,50,51,55,47,50,51,56,47,50,51,50,52,50,154,49,51,50,52,55,46,55,47,55,47,50,52,50,154,55,46,55,47,50,154,50,51,50,51,51,50,51,51,50,52,50,52,54,47,50,51,55,47,50,52,50,51,50,52,55,46,50,52,55,47,50,153,50,154,50,154,50,51,50,154,50,154,47,55};
const PROGMEM int light_on[] =  {206,103,562,102,154,51,154,50,51,52,49,53,48,50,52,52,50,54,46,53,152,52,48,51,51,53,50,52,49,53,49,52,49,55,149,50,51,52,152,55,46,53,48,53,49,52,49,53,49,50,52,53,48,50,51,53,49,52,49,53,48,51,50,53,49,50,51,51,51,51,153,50,50,54,151,51,153,55,47,52,152,51,153,48,54};

//電気消灯IRデータ
const PROGMEM int light_off[] =  {207,100,564,103,154,50,155,50,52,50,52,50,52,47,54,50,52,50,51,50,154,49,51,50,52,50,52,50,52,50,51,50,51,50,154,50,154,50,52,50,52,50,52,50,51,50,51,50,52,50,52,50,52,50,51,50,52,50,52,50,52,50,51,50,52,49,52,47,157,50,51,50,52,50,154,50,154,50,51,50,154,50,154,48,54};

//一段階暗くするIRデータ→未使用に変更
//const PROGMEM int light_down[] =  {202,101,564,108,149,54,150,55,46,53,49,55,47,55,46,55,47,55,47,55,150,49,52,55,47,50,52,55,47,55,47,55,47,50,154,50,51,50,52,54,47,50,52,50,52,55,46,51,51,50,154,55,47,50,51,51,51,55,47,50,52,50,52,50,52,50,51,51,52,54,47,55,47,55,149,54,150,54,47,55,150,55,46,52,50};

//一段階明るくするIRデータ→未使用に変更
//const PROGMEM int light_up[] =  {206,100,564,102,155,50,154,50,51,51,51,50,52,50,51,50,51,50,52,55,150,49,51,50,52,49,52,50,52,50,51,50,52,47,157,50,51,50,51,55,47,55,47,49,52,50,52,50,154,50,51,56,46,55,46,51,50,55,47,55,46,51,50,55,47,51,50,51,51,50,51,55,47,55,149,51,153,55,47,55,46,55,150,52,50};

int last = 0;
unsigned long us = micros();
  
void setup() { 
  pinMode(output, OUTPUT);
  Serial.begin (9600);  //コンソール用シリアル定義
  mySerial.begin (9600);  //mp3 module用シリアル定義
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (30); //mp3 moduleの音量を３０（最大）に
  sensors.begin(); //温度センサー定義
  pinMode(ledPin, OUTPUT);  //人感センサー
  pinMode(pirPin, INPUT);   //人感センサー
  digitalWrite(ledPin, LOW);//人感センサー

  // Initialize DS3231
  clock.begin();
//  clock.setDateTime(__DATE__, __TIME__);// コンパイル時の時刻がRTCに設定される。★設定時以外はコメントアウト★

  // Manual (YYYY, MM, DD, HH, II, SS
  // clock.setDateTime(2016, 12, 9, 11, 46, 00);
}


void send(int data[]) {  //エアコンIR制御
  int dataSize = 588;
  for (int cnt = 0; cnt < dataSize; cnt++) {
    unsigned long len = pgm_read_word(data + cnt)*10;
    unsigned long us = micros();
    do {
      digitalWrite(output, 1 - (cnt&1)); 
      delayMicroseconds(8); 
      digitalWrite(output, 0);
      delayMicroseconds(7);
    } while (long(us + len - micros()) > 0);
  }
}

void send_l(int data[]) {  //照明IR制御
  int dataSize = 85;
  for (int cnt = 0; cnt < dataSize; cnt++) {
    unsigned long len = pgm_read_word(data + cnt)*10;
    unsigned long us = micros();
    do {
      digitalWrite(output, 1 - (cnt&1)); 
      delayMicroseconds(8); 
      digitalWrite(output, 0);
      delayMicroseconds(7);
    } while (long(us + len - micros()) > 0);
  }
}

//音声リスト
//0001_aircon-off.mp3
//0002_aircon-on.mp3
//0003_kaiteki.mp3
//0004_light-off-check.mp3
//0005_light-on-check.mp3
//0006_oha.mp3
//0007_okiteru.mp3
//0008_okiteta.mp3
//0009_sleep.mp3
//0010_samui.mp3
//0011_atsui.mp3
//0012_danger.mp3

void loop() {
  pirValue = digitalRead(pirPin); //  人感センサー
  digitalWrite(ledPin, pirValue); //  人感センサー
  Serial.println(pirValue);
  int WDTchk1, WDTchk2, WDTchk3;

  WDTchk1=1100;  //起きてるか聞くまでの時間
  WDTchk2=1200;  //照明を消すまでの時間
  WDTchk3=20;  //起床を判定する時間

/*
  WDTchk1=3;  //起きてるか聞くまでの時間(デバッグ用)
  WDTchk2=6;  //照明を消すまでの時間(デバッグ用)
  WDTchk3=20;  //起床を判定する時間(デバッグ用)
*/
   
  switch (hWDTflg) {
    case 0:  //消灯中ではない場合
    if (pirValue==1){  // 人感センサーの反応があればウォッチドッグリセット
        if (hWDT>=WDTchk1){
          mp3_play (8);  // 起きてるか聞いた後は「起きてたのね？」と言う
        }
      hWDT=0;
    }else{
      hWDT=hWDT+1;
    }
    break;

    default: //消灯中の場合
    if (pirValue==1){  // 人感センサーの反応があればタイマースタート
        if (hWDTflg==1){
          hWDTflg==2; //フラグを2へ
          mp3_play (7);  // 「起きてる？」と聞く
        }else if (hWDT>=WDTchk3){
          mp3_play (9);  // 1分応答が無ければ「ただの屍のようだ」と言う
          hWDTflg=1;  //フラグを戻す
          hWDT=0;
        }else if (hWDTflg==3){
           mp3_play (6);  // 「おはよう」と言う
           send_l(light_on); //照明をつける
           hWDTflg=0;  //フラグを0(起床モード)にする
           hWDT=0;  
        }
    }else if (pirValue==0){ // 一度人感センサーの反応が消えることを確認する
      hWDTflg=3; //フラグを3へ
    }else if (hWDTflg>1){ //フラグが2以上ならタイマーを進める
      hWDT=hWDT+1;
    }
  }

  if (hWDT==WDTchk1){  // WDTchk1x3秒間人感センサーが反応しなければ起きているか聞く
    mp3_play (7);
  }else if (hWDT==WDTchk2){ // WDTchk2x3秒間人感センサーが反応しなければ照明を消す
    mp3_play (4);
    send_l(light_off);
    hWDT=0;     //消灯中はWDTタイマーは止める
    hWDTflg=1; //消灯中はhWDTflgをセットする
  }

  float Tc_100;  //浮動小数点型の宣言
      
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures
  Tc_100=sensors.getTempCByIndex(0);
  Serial.print("Now temperature is ");
  Serial.println(Tc_100);
    
  if (Tc_100>=28){
    if (Flg_hot==0){
    Flg_hot=1;Flg_warm=0;Flg_kaiteki=0;Flg_cold=0;
   mp3_play (12);
   delay(3000);
   mp3_play (2);
   delay(3000);
   send(aircon_on);}
   
  }else if (Tc_100>27.5){
    if (Flg_warm==0){
    Flg_hot=0;Flg_warm=1;Flg_kaiteki=0;Flg_cold=0;
    mp3_play (11);
   delay(3000);
   mp3_play (2);
   delay(3000);
   send(aircon_on);}
   
  }else if (Tc_100<26.5){
    if (Flg_cold==0){
    Flg_hot=0;Flg_warm=0;Flg_kaiteki=0;Flg_cold=1;
    mp3_play (10);
   delay(3000);
   mp3_play (1);
   delay(3000);
   send(aircon_off);}
   
  }else{
    if (Flg_kaiteki==0){
    Flg_hot=0;Flg_warm=0;Flg_kaiteki=1;Flg_cold=0;
    mp3_play (3);
   delay(3000);}
  }

  int   RTC_hour ,RTC_min;
  dt = clock.getDateTime();

  RTC_hour=dt.hour;
  if (RTC_hour >12){
    hour_12h=RTC_hour-12;  // 12時間表示
  }else if (RTC_hour == 0){
    hour_12h=12;           // 0時→12時
  }else{
    hour_12h=RTC_hour;  // 12時間表示
  }
  RTC_min=dt.minute;

  if (RTC_h_buf!=RTC_hour){
  mp3_play (100+hour_12h);
  delay(800);
  mp3_play (100);
  }else if (RTC_min==30 && RTC_m_buf==29){
  mp3_play (100+hour_12h);
  delay(800);
  mp3_play (130);
  delay(1000);
  mp3_play (100);
  }

  Serial.print("Now time is ");
  Serial.print(RTC_hour);
  Serial.print(":");
  Serial.println(RTC_min);
  Serial.print("Buffer time is ");
  Serial.print(RTC_h_buf);
  Serial.print(":");
  Serial.println(RTC_m_buf);  

  // The temperature registers are updated after every 64-second conversion.
  // If you want force temperature conversion use forceConversion()
  clock.forceConversion();

  Serial.print("RTC Temperature is ");
  Serial.println(clock.readTemperature());
  Serial.println("----------------------------------------");
  RTC_h_buf=RTC_hour;
  RTC_m_buf=RTC_min;
  delay(3000);
}

