#!/usr/bin/env python3

import os
import html
import re
import sys

def get_upload_path():
    if len(sys.argv) >= 4:
        upload_status = sys.argv[2]
        upload_path = sys.argv[3]
        if upload_status == "on":
            return upload_path
    try:
        with open("./default.conf", 'r') as f:
            content = f.read()
        match = re.search(r'upload\s+on\s+([^;]+);', content)
        return match.group(1).strip() if match else "./srcs/www/uploads/"
    except:
        return "./srcs/www/uploads/"

def format_size(size):
    for unit in ['B', 'KB', 'MB', 'GB']:
        if size < 1024:
            return f"{size:.1f} {unit}"
        size /= 1024
    return f"{size:.1f} TB"

upload_dir = get_upload_path()

print(f"""\n
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Fichiers Uploadés</title>
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
            max-width: 800px;
            margin: 0 auto;
        }}
        h1 {{ color: #333; text-align: center; }}
        .file-item {{
            background: #f8f9fa;
            border: 1px solid #e9ecef;
            border-radius: 10px;
            padding: 15px;
            margin: 10px 0;
            border-left: 4px solid #667eea;
        }}
        .btn {{
            display: inline-block;
            padding: 10px 20px;
            margin: 5px;
            background: linear-gradient(45deg, #667eea, #764ba2);
            color: white;
            text-decoration: none;
            border-radius: 25px;
            font-weight: 500;
            cursor: pointer;
        }}
        .btn:hover {{ transform: translateY(-2px); }}
        .navigation {{ text-align: center; margin-top: 30px; }}
    </style>
</head>
<body>
    <div class="container">
        <h1>📂 Fichiers Uploadés</h1>
""")

if os.path.exists(upload_dir):
    files = [f for f in os.listdir(upload_dir) if os.path.isfile(os.path.join(upload_dir, f))]
    
    if files:
        for filename in files:
            filepath = os.path.join(upload_dir, filename)
            try:
                size = format_size(os.path.getsize(filepath))
                safe_name = html.escape(filename)
                encoded_name = html.escape(filename.replace('"', '\\"'))  # pour JS
                print(f"""
        <div class="file-item" id="file-{safe_name}">
            <strong>📄 {safe_name}</strong><br>
            Taille: {size}
            <div style="margin-top: 10px;">
                <a href="/uploads/{safe_name}?download=1" class="btn">⬇️ Télécharger</a>
                <button class="btn" onclick="deleteFile('{encoded_name}')">🗑 Supprimer</button>
            </div>
        </div>""")
            except:
                pass
    else:
        print('<p style="text-align: center; color: #666;">📭 Aucun fichier trouvé</p>')
else:
    print('<p style="text-align: center; color: #666;">❌ Dossier d\'upload inexistant</p>')

print("""
        <div class="navigation">
            <a href="/" class="btn">🏠 Accueil</a>
        </div>
    </div>

    <script>
        function deleteFile(filename) {
            if (!confirm("❌ Supprimer ce fichier ?")) return;

            fetch("/uploads/" + encodeURIComponent(filename), {
                method: "DELETE"
            })
            .then(res => {
                if (res.ok) {
                    const element = document.getElementById("file-" + filename);
                    if (element) element.remove();
                } else {
                    alert("Erreur lors de la suppression !");
                }
            })
            .catch(() => alert("Erreur réseau."));
        }
    </script>
</body>
</html>""")
