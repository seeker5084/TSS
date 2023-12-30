# 正規化UNIX時間(Normalized UNIX Time)に対応する月ごとの平均値を算出するためのスクリプト

## 概要

ブログ記事「UNIX時間による季節判定」で使用したJavaScriptコードです。ここではUNIX時間だけを使って季節判定を試みています。

## このコードについて

このスクリプトをNode.jsで実行すると2次元配列が出力されます。

これは1970年から2038年の範囲で、1月から12月のはじめ(0時0分0秒000)とおわり(23時59分59秒999)を表す数値です。親配列は12個あり、これが各月に対応します。小配列の2つの数値が“はじめ”と“おわり”に対応します。

ここで示されている数値はわたしが勝手に「正規化UNIX時間」と呼ぶ数値に対応するものです。これはあるUNIX時間を`31557286.9565217`で割った数値を`Yf`としたとき、`Yf-Math.floor(Yf)`で求まる数値のことです。

この正規化UNIX時間を用いることで、UNIX時間だけでもある程度の精度で四季、強いては何月ごろかを判定することが可能です。

詳しくは下記ブログ記事を参照ください。

## 記事を読む

[UNIX時間による季節判定 (https://www.storange.jp/2022/04/blog-post.html)](https://www.storange.jp/2022/04/blog-post.html)