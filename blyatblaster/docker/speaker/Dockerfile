FROM ubuntu:22.04 as builder

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y nano ca-certificates git curl libc6 libc-bin
RUN apt-get install -y busybox
RUN apt-get install -y make gcc
RUN mkdir /code
WORKDIR /code
RUN curl -O https://dl.google.com/go/go1.17.linux-$(dpkg --print-architecture).tar.gz
RUN rm -rf /usr/local/go && tar -C /usr/local -xzf go1.17.linux-$(dpkg --print-architecture).tar.gz
ENV PATH="/usr/local/go/bin:$PATH"
COPY code/ /code/
WORKDIR /code
RUN go build
RUN git clone https://github.com/landley/toybox.git
WORKDIR /code/toybox
RUN ./configure
RUN make

FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get install -y busybox
RUN apt-get install -y curl
COPY --from=builder /code/midnightsun.ctf.blyat.blaster /
COPY --from=builder /code/toybox/toybox /bin/nc
COPY code/flag.ogg /
COPY code/sinot_aqua.webp /
COPY --from=builder /usr/bin/busybox /bin/sh
COPY --from=builder /usr/bin/busybox /bin/bash
COPY --from=builder /usr/bin/busybox /bin/dash

WORKDIR /
