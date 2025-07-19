#!/usr/bin/env python3
import os
import sys
import urllib.parse

# IMPORTANT : Toujours afficher le Content-Type en premier
print("Content-Type: text/html")
print("")  # Ligne vide obligatoire entre headers et body

# Début du HTML
print("""<!DOCTYPE html>
<html>
<head>
    <title>Réponse du formulaire</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; }
        .container { max-width: 600px; margin: 0 auto; }
        .info { background: #f0f0f0; padding: 20px; border-radius: 8px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🎯 Formulaire traité avec succès</h1>""")

# Récupérer la méthode HTTP
method = os.environ.get('REQUEST_METHOD', 'GET')
print(f"        <p><strong>Méthode HTTP:</strong> {method}</p>")

# Traitement selon la méthode
if method == 'POST':
    try:
        # Lire les données POST
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        if content_length > 0:
            post_data = sys.stdin.read(content_length)
            print(f"        <p><strong>Données reçues:</strong> {post_data}</p>")
            
            # Parser les données
            params = urllib.parse.parse_qs(post_data)
            print("        <div class='info'>")
            print("        <h3>Paramètres du formulaire :</h3>")
            print("        <ul>")
            
            for key, values in params.items():
                for value in values:
                    print(f"            <li><strong>{key}:</strong> {value}</li>")
            
            print("        </ul>")
            print("        </div>")
            
            # Message personnalisé
            name = params.get('name', ['Anonyme'])[0] if 'name' in params else 'Anonyme'
            ville = params.get('ville', ['Non spécifiée'])[0] if 'ville' in params else 'Non spécifiée'
            
            print(f"        <h2>Bonjour {name} !</h2>")
            print(f"        <p>Nous avons bien reçu vos informations.</p>")
            print(f"        <p>Ville : {ville}</p>")
        else:
            print("        <p>⚠️ Aucune donnée POST reçue</p>")
    except Exception as e:
        print(f"        <p>❌ Erreur lors du traitement : {e}</p>")

elif method == 'GET':
    # Traitement GET (query string)
    query_string = os.environ.get('QUERY_STRING', '')
    if query_string:
        params = urllib.parse.parse_qs(query_string)
        print(f"        <p><strong>Query string:</strong> {query_string}</p>")
        print("        <ul>")
        for key, values in params.items():
            for value in values:
                print(f"            <li><strong>{key}:</strong> {value}</li>")
        print("        </ul>")
    else:
        print("        <p>Aucun paramètre GET reçu</p>")

# Variables d'environnement utiles
print("        <div class='info'>")
print("        <h3>Informations de la requête :</h3>")
print("        <ul>")
print(f"            <li><strong>REQUEST_METHOD:</strong> {os.environ.get('REQUEST_METHOD', 'N/A')}</li>")
print(f"            <li><strong>CONTENT_LENGTH:</strong> {os.environ.get('CONTENT_LENGTH', 'N/A')}</li>")
print(f"            <li><strong>CONTENT_TYPE:</strong> {os.environ.get('CONTENT_TYPE', 'N/A')}</li>")
print(f"            <li><strong>QUERY_STRING:</strong> {os.environ.get('QUERY_STRING', 'N/A')}</li>")
print(f"            <li><strong>SCRIPT_NAME:</strong> {os.environ.get('SCRIPT_NAME', 'N/A')}</li>")
print("        </ul>")
print("        </div>")

# Fin du HTML
print("""        <br>
        <a href="/">← Retour à l'accueil</a>
    </div>
</body>
</html>""")

# S'assurer que le script se termine proprement
sys.exit(0)
