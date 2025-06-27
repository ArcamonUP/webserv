#!/usr/bin/env python3
import os
import sys

def main():
    method = os.environ.get('REQUEST_METHOD', 'GET')

    print("Content-Type: text/html")
    print()

    if method == 'GET':
        print('<html><head><meta charset="UTF-8"></head><body><h1>Requête GET reçue</h1></body></html>')
    elif method == 'POST':
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        post_data = sys.stdin.read(content_length) if content_length > 0 else ''
        print("<html><body>")
        print("<h1>Requête POST reçue</h1>")
        print(f"<p>Données reçues : {post_data}</p>")
        print("</body></html>")
    else:
        print("<html><body><h1>Méthode inconnue</h1></body></html>")

if __name__ == '__main__':
    main()
