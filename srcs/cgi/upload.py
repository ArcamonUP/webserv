# #!/usr/bin/env python3
# import cgi
# import os
# import html

# # 📁 Dossier où enregistrer les fichiers
# UPLOAD_DIR = "./uploads"

# # 🔧 Création du dossier s'il n'existe pas
# if not os.path.exists(UPLOAD_DIR):
#     os.makedirs(UPLOAD_DIR)

# # 🧾 En-tête HTTP (obligatoire avant tout HTML)
# print("Content-Type: text/html\n")

# # 📥 Traitement du formulaire
# form = cgi.FieldStorage()

# # Vérifie que le champ "nom" et "valeur" existent (on attend ces champs dans le formulaire)
# if "nom" in form and "valeur" in form:
#     # Récupérer le nom (ex: "monfichier") et nettoyer
#     raw_name = form.getfirst("nom", "").strip()
#     safe_name = html.escape(raw_name)

#     # Ajouter automatiquement .txt
#     filename = safe_name + ".txt"

#     # Chemin complet vers le fichier
#     filepath = os.path.join(UPLOAD_DIR, filename)

#     # Vérifier si fichier existe déjà
#     if os.path.exists(filepath):
#         # Afficher erreur : fichier existe déjà
#         print(f"""<!DOCTYPE html>
# <html lang="fr">
# <head>
#     <meta charset="UTF-8">
#     <title>Erreur</title>
#     <style>
#         .center-box {{
#             width: 60%;
#             margin: 100px auto;
#             border: 3px solid red;
#             padding: 20px;
#             text-align: center;
#             font-family: sans-serif;
#             background-color: #ffecec;
#         }}
#     </style>
# </head>
# <body>
#     <div class="center-box">
#         <h2>❌ Impossible de créer le fichier</h2>
#         <p>Le fichier <strong>{filename}</strong> existe déjà.</p>
#     </div>
# </body>
# </html>""")
#     else:
#         # Récupérer la valeur à écrire dans le fichier
#         raw_value = form.getfirst("valeur", "")
#         safe_value = html.escape(raw_value)

#         # Créer et écrire dans le fichier
#         with open(filepath, "w", encoding="utf-8") as f:
#             f.write(safe_value)

#         # Réponse succès
#         print(f"""<!DOCTYPE html>
# <html lang="fr">
# <head>
#     <meta charset="UTF-8">
#     <title>Fichier Créé</title>
#     <style>
#         .center-box {{
#             width: 60%;
#             margin: 100px auto;
#             border: 3px solid green;
#             padding: 20px;
#             text-align: center;
#             font-family: sans-serif;
#             background-color: #eaffea;
#         }}
#     </style>
# </head>
# <body>
#     <div class="center-box">
#         <h2>✅ Fichier créé avec succès !</h2>
#         <p>Nom du fichier : <strong>{filename}</strong></p>
#         <p>Contenu écrit : <em>{safe_value}</em></p>
#     </div>
# </body>
# </html>""")

# else:
#     # ❌ Champs manquants
#     print("""<!DOCTYPE html>
# <html lang="fr">
# <head><meta charset="UTF-8"><title>Erreur</title></head>
# <body>
#     <h2 style="color:red;">❌ Veuillez fournir un nom et une valeur.</h2>
#     <p>Retournez au formulaire et remplissez tous les champs.</p>
# </body>
# </html>""")




# !/usr/bin/env python3
# upload.cgi

import cgi
import os

UPLOAD_DIR = "./uploads"

print("Content-Type: text/html\n")  # Header CGI obligatoire

form = cgi.FieldStorage()
fileitem = form['file_upload']

if fileitem.filename:
    filename = os.path.basename(fileitem.filename)
    filepath = os.path.join(UPLOAD_DIR, filename)

    if not os.path.exists(UPLOAD_DIR):
        os.makedirs(UPLOAD_DIR)

    if os.path.exists(filepath):
        print(f"<h2>Erreur : Le fichier '{filename}' existe deja.</h2>")
    else:
        with open(filepath, 'wb') as f:
            f.write(fileitem.file.read())
        print(f"<h2>Fichier '{filename}' upload avec succes !</h2>")
else:
    print("<h2>Erreur : Aucun fichier reçu.</h2>")

print('<p><a href="/">Retour a l\'upload</a></p>')
print('<p><a href="/list.py">Voir les fichiers uploades</a></p>')
