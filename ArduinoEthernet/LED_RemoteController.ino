/*
  シンプルWebサーバ
  ------------------------------------------
  HTTPリクエストの内容に応じてディジタル出力を行う.
  具体的にはGETメソッドのパラメタを調べる.
  Ethernet Shieldがディジタルピン10~13を使用中.
  SDカード制御用にディジタル4ピンも使用中.
  ------------------------------------------
 */

#include <SPI.h>
#include <Ethernet.h>

// LEDをつなげるディジタル出力ピン番号.
int ledPin =  9;

// IPアドレスは他のネットワーク機器と同じにならないように注意.
byte  ip[] = {192, 168, 11, 22};
// MACアドレスはEthernetシールドに記載されているので2桁づつココへ記入.
byte mac[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};

// ポート80(HTTP)で通信をするサーバを初期化.
EthernetServer server = EthernetServer(80);



void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  Ethernet.begin(mac, ip);
  server.begin();
  
  Serial.begin(9600);
  while(!Serial) ; // シリアルポートの接続が確立されるまでここで待機.
  Serial.print("I'm at ");
  Serial.println(Ethernet.localIP());
}



void loop()
{
  EthernetClient client = server.available();
  
  if (client) {
    Serial.println("Find a client!");
    connectingSequence(client);
  }
  
  // Webブラウザがデータを受信するまで少し待つ.
  delay(1);
  client.stop();
}



void connectingSequence(EthernetClient client)
{
  // HTTPリクエストの終わりは改行が2回連続で続く.
  boolean currentLineIsBlank = false;
  boolean getFirstLineOfRequest = false;
  String firstLineOfRequest = "";
  char c = ' ';
  
  while (client.connected()) {
    
    if (client.available()) {
      c = client.read();
      Serial.write(c);
      
      // 2回連続で改行したからリクエスト受信完了の合図だ.
      if (currentLineIsBlank && c == '\n') {
        // 直ぐにレスポンス(HTMLページ)を返そう. でないと通信不良だと勘違いされる.
        sendBackHttpResponse(client);
        // リクエスト先頭行にGETパラメタがある場合,そのパラメタに応じて出力ピンの状態を変える.
        // GETパラメタがない場合は何もしない.
        if (firstLineOfRequest.indexOf('?') != -1) {
          changeOutputPinsStatusBasedOn(firstLineOfRequest);
        }
        break;
      }
      
      if (c == '\n') {
        // 1回目の改行をしたぞ！
        currentLineIsBlank = true;
        getFirstLineOfRequest = true;
      }else if (c != '\r') {
        // 文字を取得したのでまだリクエストが続くんだな.
        currentLineIsBlank = false;
      }
      
      if (!getFirstLineOfRequest) {
        // GETパラメタの有無を調べるため,リクエストの先頭行を取得.
        firstLineOfRequest += c;
      }
    } // if文の閉じカッコ
    
  } // while文の閉じカッコ
}



void changeOutputPinsStatusBasedOn(String firstLineOfRequest)
{
  // '?'の次からがGETパラメタ.
  int getParamIndex = firstLineOfRequest.indexOf('?') + 1;
  // '='がGETパラメタのキーと値の境目.
  int getEqualIndex = firstLineOfRequest.indexOf('=');
  // 区切り文字' 'より1つ前までがGETパラメタ.
  int delimiterIndex = firstLineOfRequest.indexOf(' ', getParamIndex+1);
  
  String getKey = firstLineOfRequest.substring(getParamIndex, getEqualIndex);
  String getValue = firstLineOfRequest.substring(getEqualIndex+1, delimiterIndex);
  
  ledPin = getKey.toInt();
  
  Serial.print("key is '");
  Serial.print(getKey);
  Serial.print("' , value is '");
  Serial.print(getValue);
  Serial.println("'.");
  
  if (getValue == "ON" ) {digitalWrite(ledPin, HIGH); Serial.println("ON!");}
  if (getValue == "OFF") {digitalWrite(ledPin,  LOW); Serial.println("OFF");}
}



void sendBackHttpResponse(EthernetClient client)
{
  // HTTPヘッダ
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();
  
  // ペイロード
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=viewport content='width=80px, initial-scale=4, maximum-scale=4, user-scalable=no' />");
  client.println("<title>A Remote</title>");
  client.println("</head>");
  client.println("<body style='color:rgb(218,218,218);background-color:rgb(16,23,32);text-align:center;'>");
  client.println("Remote<br />");
  client.println("<form method=GET>");
  client.print("<input type=submit name="); client.print(ledPin);
  client.println(" value=ON  />");
  client.print("<input type=submit name="); client.print(ledPin);
  client.println(" value=OFF />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
}
