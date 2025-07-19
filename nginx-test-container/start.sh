# Demarrer fcgiwrap pour les CGI
spawn-fcgi -s /var/run/fcgiwrap.socket -F 1 -U nginx -G nginx /usr/bin/fcgiwrap

# Demarrer nginx
nginx -g "daemon off;"
