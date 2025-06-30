#!/usr/bin/env python3
import cgi
import os
import html

# 📁 Dossier où enregistrer les fichiers
UPLOAD_DIR = "./uploads"

# 🔧 Création du dossier s'il n'existe pas
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

# 🧾 En-tête HTTP (obligatoire avant tout HTML)
print("Content-Type: text/html\n")

# 📥 Traitement du formulaire
form = cgi.FieldStorage()

# Vérifie qu’un fichier a été envoyé avec le champ "monfichier"
if "monfichier" in form and form["monfichier"].filename:
    fileitem = form["monfichier"]
    filename = os.path.basename(fileitem.filename)

    # 🔐 Échappement HTML pour éviter les injections
    safe_filename = html.escape(filename)

    # 💾 Enregistrement du fichier dans UPLOAD_DIR
    filepath = os.path.join(UPLOAD_DIR, filename)
    with open(filepath, "wb") as f:
        f.write(fileitem.file.read())

    # ✅ Réponse HTML avec cadre rouge centré
    print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Fichier Uploadé</title>
    <style>
        .center-box {{
            width: 60%;
            margin: 100px auto;
            border: 3px solid red;
            padding: 20px;
            text-align: center;
            font-family: sans-serif;
            background-color: #ffecec;
        }}
    </style>
</head>
<body>
    <div class="center-box">
        <h2>✅ Fichier reçu avec succès !</h2>
        <p>Nom du fichier : <strong>{safe_filename}</strong></p>
        
    </div>
</body>
</html>""")

else:
    # ❌ Aucun fichier sélectionné
    print("""<!DOCTYPE html>
<html lang="fr">
<head><meta charset="UTF-8"><title>Erreur</title></head>
<body>
    <h2 style="color:red;">❌ Aucun fichier reçu</h2>
    <p>Retournez au formulaire et choisissez un fichier.</p>
</body>
</html>""")
