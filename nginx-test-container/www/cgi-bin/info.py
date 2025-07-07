#!/usr/bin/env python3
import os
import sys
from datetime import datetime

print("Content-Type: text/html\n")
print("""<!DOCTYPE html>
<html><head><title>CGI Info</title></head><body>
<h1>Informations CGI</h1>""")

print(f"<p><strong>Date:</strong> {datetime.now()}</p>")
print(f"<p><strong>Python Version:</strong> {sys.version}</p>")
print(f"<p><strong>Working Directory:</strong> {os.getcwd()}</p>")

print("<h2>Variables d'environnement</h2><ul>")
for key, value in sorted(os.environ.items()):
    print(f"<li><strong>{key}:</strong> {value}</li>")
print("</ul></body></html>")