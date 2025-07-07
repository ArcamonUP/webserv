#!/usr/bin/env python3
import os
import sys
import cgi
import html

def main():
    method = os.environ.get('REQUEST_METHOD', 'GET')

    print()

    print("""
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Réponse CGI</title>
        <style>
            body {
                font-family: 'Segoe UI', sans-serif;
                background: #f2f2f2;
                padding: 20px;
                color: #333;
            }
            .container {
                background: white;
                padding: 30px;
                border-radius: 10px;
                box-shadow: 0 0 10px rgba(0,0,0,0.1);
                max-width: 700px;
                margin: auto;
            }
            h1 {
                color: #3498db;
            }
            p {
                font-size: 1.1em;
            }
            .code {
                background: #eee;
                padding: 10px;
                border-radius: 5px;
                font-family: monospace;
                white-space: pre-wrap;
            }
        </style>
    </head>
    <body>
        <div class="container">
    """)

    if method == 'GET':
        print("<h1>✅ Requête GET reçue</h1>")
        query_string = os.environ.get('QUERY_STRING', '')
        if query_string:
            print("<h2>Paramètres GET :</h2>")
            print(f'<div class="code">{html.escape(query_string)}</div>')
        else:
            print("<p>Aucun paramètre fourni.</p>")

    elif method == 'POST':
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        post_data = sys.stdin.read(content_length) if content_length > 0 else ''
        print("<h1>📨 Requête POST reçue</h1>")
        if post_data:
            print("<h2>Données reçues :</h2>")
            print(f'<div class="code">{html.escape(post_data)}</div>')
        else:
            print("<p>Aucune donnée reçue.</p>")

    else:
        print("<h1>❌ Méthode inconnue</h1>")
        print(f"<p>Méthode détectée : {method}</p>")

    print("""
        </div>
    </body>
    </html>
    """)

if __name__ == '__main__':
    main()
