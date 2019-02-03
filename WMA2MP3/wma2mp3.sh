#!/bin/bash

bitrate=256
samplerate=44100

alias vlc="/Applications/VLC.app/Contents/MacOS/VLC"

for file in *.wma
do
  echo $file
  out="$(tr -d '\(.wma)$\i' <<< ${file})"
  vlc -I dummy "${file}" --sout "#transcode{vcodec=none,acodec=mp3,ab=$bitrate,channels=2,samplerate=$samplerate}:file{dst=${out}.mp3}" vlc://quit
done
