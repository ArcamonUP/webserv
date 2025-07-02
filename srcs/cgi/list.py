#!/usr/bin/env python3
# list.cgi

import os

UPLOAD_DIR = "./uploads"

print("Content-Type: text/html\n")  # Header CGI obligatoire
print("<html><head><title>Fichiers uploadés</title></head><body>")
print("<h1>Liste des fichiers uploadés</h1>")

if os.path.exists(UPLOAD_DIR):
    files = os.listdir(UPLOAD_DIR)
    if files:
        print("<ul>")
        for f in files:
            print(f'<li><a href="/uploads/{f}">{f}</a></li>')
        print("</ul>")
    else:
        print("<p>Aucun fichier uploadé pour le moment.</p>")
else:
    print("<p>Le dossier d'upload n'existe pas.</p>")

print('<p><a href="/">Retour à l\'upload</a></p>')
print("</body></html>")
