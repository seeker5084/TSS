/*
 * IRreceiver.ino
 * --------------------------------
 * 赤外線(IR)信号をRECV_PINより受信.
 * RAW(生)データをシリアルモニターに配列として出力する.
 * BUTTON_PINにつながったプッシュボタンが押されると,直前に受信した信号を再生する.
 * この時の出力はArduino UNOの場合Digital 3ピン(PWM)となる.
 * 受信/送信ともに38kHzで変調されたIR信号と仮定している.
 * 送信時の変調周波数はirsend.sendRaw(code, length, freq)のfreqをkHz単位で指定する.
 */



#include <IRremote.h>

int codeLen;
int lastButtonState;

int RECV_PIN = 11;
int BUTTON_PIN = 12;

unsigned int rawCodes[RAWBUF];

IRsend irsend;
IRrecv irrecv(RECV_PIN);

decode_results results;



void setup()
{
  // EthernetシールドのSDカードが悪さをしないように...
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // 初期設定
  Serial.begin(9600);
  irrecv.enableIRIn(); // 受信スタート
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}



void loop()
{
  // ボタンが押されたら(buttonStateがHIGHになったら),信号を送信する
  int buttonState = !digitalRead(BUTTON_PIN);
  
  if (lastButtonState == HIGH && buttonState == LOW) // 指がボタンから離れたら...
  {
    Serial.println("Button released. Re-enabling IR-in.");
    irrecv.enableIRIn(); // 受信機能の再有効化
  }

  if (buttonState == HIGH) // 信号の送信
  {
    Serial.println("Button pressed");
    irsend.sendRaw(rawCodes, codeLen, 38); // 38[kHz]として送信
    Serial.println("Sent RAW data");
    delay(50); // 再送信までに待ち時間を設ける
  }
  else if (irrecv.decode(&results)) // 信号を受信したら...
  {
    storeCode(&results);
    irrecv.resume(); // 受信再開
  }
  
  lastButtonState = buttonState;
}



void storeCode(decode_results *results)
{
  int count = results->rawlen;
  codeLen = results->rawlen - 1;

  Serial.print("{");

  for (int i = 1; i <= codeLen; i++)
  {
    if (i % 2)
    {
      // Mark
      rawCodes[i - 1] = results->rawbuf[i] * USECPERTICK - MARK_EXCESS;
    }
    else
    {
      // Space
      rawCodes[i - 1] = results->rawbuf[i] * USECPERTICK + MARK_EXCESS;
    }
    Serial.print(rawCodes[i - 1], DEC);

    if (i != codeLen)
    {
      Serial.print(", ");
    }
  }

  Serial.println("};");
}

