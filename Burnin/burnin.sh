#!/bin/bash

# BURNIN
# -----------------------------------------------
# Description: Burn in subtitles to a movie file.
# Supported subtitle formats: SRT, ASS, MKV
# Usage: burnin [movie] [subtitles]
# Author: Hideyuki Tabata
# License: MIT License

# Output format
OUT='m4v'

MOV=$1
SBT=$2

function encode {
    echo -e "- Output\t: ${MOV}.${OUT}"
    ffmpeg -i "$MOV" -vf $1="${SBT//\'/\\\\\'}" -y "${MOV}.${OUT}"
}

function showUsage {
    echo -e '- Usage\t\t: burnin [movie] [subtitle(srt,mkv,ass)]'
}

# Show usage if no arguments
[[ $@ ]] || { showUsage; exit 1; }

echo 'BURNIN SCRIPT, POWERED BY FFMPEG.'
echo -e "+ Movie\t\t: $MOV"
echo -e "+ Subtitles\t: $SBT"

if [[ !(-e "$MOV" && -e "$SBT") ]]; then
    echo -e '! Error\t\t: Invalid argument(s).'
    showUsage
    exit 2
fi

if [[ "$SBT" == *".srt" || "$SBT" == *".mkv" ]]; then
    encode subtitles
elif [[ "$SBT" == *".ass" ]]; then
    encode ass
fi
exit 0
