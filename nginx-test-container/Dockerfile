FROM nginx:alpine

RUN apk add --no-cache python3 py3-pip fcgiwrap spawn-fcgi

COPY nginx.conf /etc/nginx/nginx.conf

COPY www/ /www/

COPY www/cgi-bin/ /www/cgi-bin/
RUN chmod +x /www/cgi-bin/*.py

EXPOSE 8080

COPY start.sh /start.sh
RUN chmod +x /start.sh

CMD ["/start.sh"]