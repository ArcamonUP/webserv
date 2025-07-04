#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

UPLOAD_DIR = "./uploads"

print("Content-Type: text/html; charset=utf-8\n")  # En‑tête HTTP complet
print("""
<html>
<head>
    <meta charset="utf-8">
    <title>Fichiers uploadés</title>
</head>
<body>
<h1>Liste des fichiers uploadés</h1>
""")

if os.path.exists(UPLOAD_DIR):
    files = os.listdir(UPLOAD_DIR)
    if files:
        print("<ul>")
        for f in files:
            print(f'<li><a/{f}">{f}</a></li>')
        print("</ul>")
    else:
        print("<p>Aucun fichier uploadé pour le moment.</p>")
else:
    print("<p>Le dossier d\'upload n\'existe pas.</p>")

print("""
<p><a href="/">Retour à l'upload</a></p>
</body>
</html>
""")
