Challenge:
- speaker system is insecure but not exposed to the internet
- a yacht passenger receives a link and *clicks* it


Teams were only given a binary for blyatblaster which made this a RE challenge as well

===

Running:

- when deploying, set an env variable PIN different than the binary

On port 8888 we want to receive a URL that will be run, and port 8080 should *not* be exposed
