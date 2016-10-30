/*
   このプログラムについて:
     HTTPリクエストを受け取る.
     受け取ったリクエストの内容に対応した赤外線信号を送信する.

   プロジェクトの目標:
     SiriによるIR信号の送信.
     Raspberry Pi上のHomeBridgeサーバからHTTPリクエストが送られる.
     アクセス先URL(正確にはGETパラメタ)により対応するIRコマンドを送信する.
     例1) http://192.168.11.5?Light=ON LightのONコマンドを送信.
     例2) http://192.168.11.5?Light=OFF LightのOFFコマンドを送信.
 */



#define VERSION "1.00"

#include <IRremote.h>

#include <SPI.h>
#include <Ethernet.h>

int lightState = 0; // Lightの状態

byte ip [] = {192, 168, 11, 5}; // TODO: Arduinoに割り当てたいIPアドレスを入力(Homebridgeのon_urlなどと合わせる)
byte mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // TODO: EthernetシールドのMacアドレスを2桁づつ入力

IRsend irsend;

EthernetServer server = EthernetServer(80);



void setup()
{
  // EthernetシールドのSDカードが悪さをしないように...
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  Ethernet.begin(mac, ip);
  server.begin();

  Serial.begin(9600);
  while (!Serial) ; // シリアル通信が確立されるまで待つ...
  Serial.print("I'm at ");
  Serial.println(Ethernet.localIP());
}



void loop()
{
  EthernetClient client = server.available();

  if (client) // Arduinoへのアクセスを検知したらif文の中へ
  {
    Serial.println("Find a client!");
    connectingSequence(client);
  }

  delay(1);
  client.stop();
}



