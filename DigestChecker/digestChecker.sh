#!/bin/bash

function printUsage () {
  echo 'USAGE: digestchecker [option] file checksum'
  echo 'Available options:'
  echo -e '\t--md5'
  echo -e '\t--sha1'
  echo -e '\t--sha256'
  exit 1
}

if [ $# -gt 2 ]; then

  # option(argument) specified
  file="$2" # File name
  checksum="$3"  # Checksum value

  if [ ! -f "$file" ] || [ -f "$checksum" ]; then
    printUsage # exit with code 1
  fi

  case $1 in
    --md5    ) declare -a md5=($(openssl dgst -md5 "$file")) ;;
    --sha1   ) declare -a sha1=($(openssl dgst -sha1 "$file")) ;;
    --sha256 ) declare -a sha256=($(openssl dgst -sha256 "$file")) ;;
    *        ) printUsage ;; # exit with code 1
  esac

else

  # no option(argument) found
  file="$1" # File name
  checksum="$2"  # Checksum value

  if [ ! -f "$file" ] || [ -f "$checksum" ]; then
    printUsage # exit with code 1
  fi

  # Each variables 'md5', 'sha1' and 'sha256' are arrays.
  # - The first item is a file name.
  # - The second item is a checksum.
  declare -a md5=($(openssl dgst -md5 "$file"))
  declare -a sha1=($(openssl dgst -sha1 "$file"))
  declare -a sha256=($(openssl dgst -sha256 "$file"))

fi

function matched () {
  echo $1
  echo '+------------+'
  echo '|  Matched!  |'
  echo '+------------+'
  exit 0
}

case "$checksum" in
    "${md5[1]}") matched 'MD5 Digest Algorithm' ;; # exit with code 0
    "${sha1[1]}") matched 'SHA-1 Digest Algorithm' ;; # exit with code 0
    "${sha256[1]}") matched 'SHA-256 Digest Algorithm' ;; # exit with code 0
esac

# Not matched...
echo '+-------------+'
echo '| Wrong File! |'
echo '+-------------+'
exit 2
