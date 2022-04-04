# docker container run -it --rm -v $(pwd):/usr/src/app zenika/alpine-chrome --no-sandbox --virtual-time-budget=30000 --screenshot --hide-scrollbars http://192.168.1.2:8000/exploit.html
FROM zenika/alpine-chrome:89
COPY run.sh /

ENTRYPOINT ["/run.sh"]
