#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <DHT.h>    
int sensorValue; 
#define DHTPIN 3     
#define DHTTYPE DHT22
ThreeWire myWire(6, 7, 5);
DHT dht(DHTPIN, DHTTYPE);
RtcDS1302<ThreeWire> Rtc(myWire);
void setup ()
{
  //設定鮑率為115200
  Serial.begin(115200);
  //初始化Rtc
  Rtc.Begin();
  //確認時間是否正確和矯正時間
  if (Rtc.GetDateTime() != RtcDateTime(__DATE__, __TIME__))
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    //獲取正常時間
    Rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
  }
  pinMode(A0,INPUT);
  //初始化dht22 
  dht.begin();
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}
 
void loop ()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  RtcDateTime now = Rtc.GetDateTime();
  String t1="/", t2=":", t3="  "; 
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }
  if (!now.IsValid()){ 
    Serial.println("RTC lost confidence in the DateTime!");
  }
  else{ 
    //燈條的繼電器的PIN腳
    digitalWrite(11,HIGH);
    sensorValue=analogRead(A0); //讀取感測器回傳值
    String timeNow=now.Year()+t1+now.Month()+t1+now.Day()+t3+now.Hour()+t2+now.Minute()+t2+now.Second();
    if( now.Minute() == 00 && now.Second() == 00){
    while(sensorValue <= 360){
        //土壤檢測器的PIN腳(回傳數值)
        sensorValue=analogRead(A0);
        //抽水馬達的繼電器的PIN腳
        digitalWrite(10,HIGH);
        Serial.println(sensorValue);
        delay(1000);
      }
    }
    //抽水馬達的繼電器的PIN腳
    digitalWrite(10,LOW);
    Serial.println(timeNow);
    Serial.print("value:");
    Serial.println(sensorValue);
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
  }
  delay(1000);  
}
