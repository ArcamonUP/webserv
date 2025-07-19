#!/usr/bin/env python3

import cgi
import os
import html
import sys
import re

def get_upload_status_and_path():
    default_status = "off"
    default_path = os.getenv("UPLOAD_PATH", None)

    if len(sys.argv) >= 3:
        upload_status = sys.argv[1].lower()
        if upload_status not in ("on", "off"):
            return default_status, default_path

        upload_path = sys.argv[2] if upload_status == "on" else None
        return upload_status, upload_path

    return default_status, default_path

def get_upload_path():
    status, path = get_upload_status_and_path()
    return path 

def create_response(title, message, color="#667eea"):
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
            padding: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
        }}
        .container {{
            background: white;
            border-radius: 15px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            padding: 40px;
            max-width: 500px;
            text-align: center;
            border-left: 5px solid {color};
        }}
        h1 {{ color: #333; }}
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
    </style>
</head>
<body>
    <div class="container">
        {message}
        <div>
            <a href="/" class="btn">🏠 Accueil</a>
            <a href="/list" class="btn">📂 Fichiers</a>
        </div>
    </div>
</body>
</html>"""

upload_status, upload_dir = get_upload_status_and_path()

try:
    if upload_status != "on":
        html_content = f"""
            <h1>🚫 Upload désactivé</h1>
            <p>L'upload de fichiers n'est pas autorisé sur ce serveur.</p>
            <p><strong>Statut actuel :</strong> {upload_status}</p>
        """
        print(create_response("Upload désactivé", html_content, "#e74c3c"))
        sys.exit(0)

    if not upload_dir:
        print(create_response(
            "Erreur de configuration",
            "<h1>⚙️ Erreur de configuration</h1><p>Le répertoire d'upload n'est pas configuré.</p>",
            "#e74c3c"
        ))
        sys.exit(0)

    if not os.path.exists(upload_dir):
        os.makedirs(upload_dir, mode=0o755)

    form = cgi.FieldStorage()

    if 'file_upload' not in form:
        print(create_response(
            "Erreur",
            "<h1>❌ Aucun champ file_upload trouvé</h1>",
            "#e74c3c"
        ))
    else:
        fileitem = form['file_upload']
        if isinstance(fileitem, list):
            if len(fileitem) > 0:
                fileitem = fileitem[0]
            else:
                print(create_response(
                    "Erreur",
                    "<h1>❌ Champ file_upload vide</h1>",
                    "#e74c3c"
                ))
                sys.exit(0)

        if not hasattr(fileitem, 'filename') or not fileitem.filename:
            print(create_response(
                "Erreur",
                "<h1>❌ Aucun fichier reçu</h1>",
                "#e74c3c"
            ))
        else:
            filename = os.path.basename(fileitem.filename)
            filepath = os.path.join(upload_dir, filename)

            if os.path.exists(filepath):
                print(create_response(
                    "Erreur",
                    f"<h1>🚫 Fichier existant</h1><p>Le fichier <strong>{html.escape(filename)}</strong> existe déjà.</p>",
                    "#e74c3c"
                ))
            else:
                with open(filepath, 'wb') as f:
                    f.write(fileitem.file.read())
                size = os.path.getsize(filepath)
                size_kb = size / 1024
                print(create_response(
                    "Succès",
                    f"<h1>✅ Upload réussi!</h1><p>Fichier: <strong>{html.escape(filename)}</strong></p><p>Taille: {size_kb:.1f} KB</p>",
                    "#27ae60"
                ))
except Exception as e:
    print(create_response(
        "Erreur",
        f"<h1>🐛 Erreur</h1><p>{html.escape(str(e))}</p>",
        "#e74c3c"
    ))