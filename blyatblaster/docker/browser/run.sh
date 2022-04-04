#!/bin/sh
# docker container run -it --rm -v $(pwd):/usr/src/app zenika/alpine-chrome --no-sandbox
#--virtual-time-budget=30000 --screenshot --hide-scrollbars http://192.168.1.2:8000/exploit.html
while true
do
  echo "*Click*" | nc -w 1 -n -k -l -p 8888 > /tmp/url
  read line < /tmp/url
  if [[ ! -z "$line" ]]; then
    chromium-browser --no-sandbox --headless --virtual-time-budget=60000 -- "$line" &
  fi
done
