FROM        artifactorycn.netcracker.com:17035/ubuntu

COPY        coturn/deb /var/coturn/deb
RUN         dpkg -i /var/coturn/deb/*.deb

COPY        nodejs/deb /var/nodejs/deb
RUN         dpkg -i /var/nodejs/deb/*.deb
COPY        src /var/http/signal-server

EXPOSE      8080/tcp
EXPOSE      3478/udp

ENTRYPOINT  /usr/bin/turnserver -v & exec nodejs /var/http/signal-server/server.js