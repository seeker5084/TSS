/*
 * Ethernetシールドに関わる処理:
 *  Arduinoへのアクセスを検知したら,相手のHTTPリクエストを解析する.
 *  リクエスト内のURLにGETパラメタがある場合はそのキーと値を読み取る(getKeyとgetValue).
 *  GETパラメタが無い場合はLightの状態だけを返す.
 */



void connectingSequence(EthernetClient client)
{
  bool currentLineIsBlank = false;
  bool getFirstLineOfRequest = false;

  char c = ' ';
  String firstLineOfRequest = "";

  while (client.connected())
  {
    if (client.available())
    {
      c = client.read();
      Serial.write(c);

      if (currentLineIsBlank && c == '\n')
      {
        if (firstLineOfRequest.indexOf('?') != -1)
        {
          changeOutputPinStatusBasedOn(firstLineOfRequest);
        }

        sendBackHttpResponse(client);

        break;
      }

      if (c == '\n')
      {
        currentLineIsBlank = true;
        getFirstLineOfRequest = true;
      }
      else if (c != '\r')
      {
        currentLineIsBlank = false;
      }

      if (!getFirstLineOfRequest)
      {
        firstLineOfRequest += c;
      }

    }
  }
}



void changeOutputPinStatusBasedOn(String firstLineOfRequest)
{
  // '?'の次からがGETパラメタ.
  int getParamIndex = firstLineOfRequest.indexOf('?') + 1;
  // '='がGETパラメタのキーと値の境目.
  int getEqualIndex = firstLineOfRequest.indexOf('=');
  // 区切り文字' 'より1つ前までがGETパラメタ.
  int delimiterIndex = firstLineOfRequest.indexOf(' ', getParamIndex + 1);

  String getKey = firstLineOfRequest.substring(getParamIndex, getEqualIndex);
  String getValue = firstLineOfRequest.substring(getEqualIndex + 1, delimiterIndex);

  Serial.print("key is '");
  Serial.print(getKey);
  Serial.print("' , value is '");
  Serial.print(getValue);
  Serial.println("'.");

  if (getKey == "Light") // GETのキーが"Light"の場合は赤外線コードを送信
  {
    sendIRcode(getValue);
  }
}



void sendBackHttpResponse(EthernetClient client)
{
  // HTTPヘッダ
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("Connection: close");
  client.println();

  // ペイロード (Lightの状態を0か1かで返信)
  Serial.println(lightState);
  client.println(lightState);
}



