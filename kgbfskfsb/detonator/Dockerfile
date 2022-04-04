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

FROM ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
COPY --from=builder /code/midnightsun.ctf.kgbfskfsb_detonator /
RUN mkdir -p /.do/you/see/the/answer/
COPY code/runme /.do/you/see/the/answer/runme
RUN chmod -rw /.do/you/see/the/answer/runme
USER nobody
