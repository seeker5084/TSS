/*
 * IR通信に関わる処理:
 *  予め取得したIR信号をunsigned int配列として用意する.
 *  HTTPリクエストのGETパラメタに対応したIR信号を送信する.
 */



unsigned int on_code[] = {}; // TODO: ここにON信号をペースト
unsigned int off_code[] = {}; // TODO: ここにOFF信号をペースト



void sendIRcode(String command)
{
  if (command == "ON") // GETパラメタがONのとき...
  {
    irsend.sendRaw(on_code, sizeof(on_code)/sizeof(on_code[0]), 38);
    Serial.print("Sent ON signal.");
    lightState = 1;
  }
  else
  // if (command == "OFF") // GETパラメタがOFFのとき...
  {
    irsend.sendRaw(off_code, sizeof(off_code)/sizeof(off_code[0]), 38);
    Serial.print("Sent OFF signal.");
    lightState = 0;
  }
}



