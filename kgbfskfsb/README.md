# kgbfskfsb

This challenge contained the hint:

   "info": "DeNiS Sergeev wants a secret mission payload...",

Note that D N and S are capitalized (DNS). 

You can build and run the docker containers yourself if you'd like to try it out.
NOTE: you need a /home/ctf/detonations/ folder owned by the nobody user, which is bind mounted by docker.

## Challenge design:

This challenge contains two services. A scanner and a detonator.

The scanner is exposed on :15555. After some special operation checks, the scanner will decide to "scan" a URL.

It also has a second endpoint, "detonate", which will accept requests only from localhost and download a URL to the /detonations/ folder.

The second service is a "detonator". It exposes a "detonate" endpoint which will execute a provided binary filename from the /detonations folder.
