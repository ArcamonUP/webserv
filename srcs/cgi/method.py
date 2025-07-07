#!/usr/bin/env python3

import os
import sys
import cgi
import html


def create_response(title, content):
    return f"""
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>{title}</title>
    <style>
        body {{
            font-family: 'Segoe UI', sans-serif;
            margin: 0;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }}
        .container {{
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            padding: 40px;
            max-width: 700px;
            margin: 0 auto;
        }}
        h1 {{ color: #333; }}
        .data {{
            background: #f8f9fa;
            padding: 15px;
            border-radius: 10px;
            border-left: 4px solid #667eea;
            margin: 20px 0;
            font-family: monospace;
            white-space: pre-wrap;
        }}
        .btn {{
            display: inline-block;
            padding: 12px 24px;
            margin: 10px 5px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            text-decoration: none;
            border-radius: 25px;
            font-weight: 500;
        }}
        .btn:hover {{ transform: translateY(-2px); }}
        .navigation {{ text-align: center; margin-top: 30px; }}
    </style>
</head>
<body>
    <div class="container">
        {content}
        <div class="navigation">
            <a href="/index.html" class="btn">🏠 Retour à l'accueil</a>
        </div>
    </div>
</body>
</html>"""

method = os.environ.get('REQUEST_METHOD', 'GET')

if method == 'GET':
    query_string = os.environ.get('QUERY_STRING', '')
    content = f"""
        <h1>✅ Requête GET reçue</h1>
        <p>Méthode: <strong>{method}</strong></p>
        {f'<div class="data">{html.escape(query_string)}</div>' if query_string else '<p>Aucun paramètre fourni.</p>'}
    """
    
elif method == 'POST':
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    post_data = sys.stdin.read(content_length) if content_length > 0 else ''
    content = f"""
        <h1>📨 Requête POST reçue</h1>
        <p>Méthode: <strong>{method}</strong></p>
        {f'<div class="data">{html.escape(post_data)}</div>' if post_data else '<p>Aucune donnée reçue.</p>'}
    """
    
else:
    content = f"""
        <h1>❌ Méthode inconnue</h1>
        <p>Méthode détectée: <strong>{method}</strong></p>
    """

print(create_response("Script CGI", content))
