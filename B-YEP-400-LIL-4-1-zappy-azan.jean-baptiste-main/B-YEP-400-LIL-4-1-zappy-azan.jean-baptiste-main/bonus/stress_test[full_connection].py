import socket
import threading

def connect_and_send_large_buffer(host, port, buffer_size):
    try:
        # Créer un socket TCP/IP
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Se connecter au serveur
        sock.connect((host, port))
        
        # Construire un buffer de données (rempli de 'A')
        huge_data = b'A' * buffer_size
        
        # Envoyer le buffer de données
        sock.sendall(huge_data)
        
        # Fermer la connexion
        sock.close()
        
        print(f"Envoi de {buffer_size} octets vers {host}:{port} terminé.")
    except Exception as e:
        print(f"Erreur lors de la connexion au serveur : {e}")

# Fonction pour gérer chaque thread de connexion
def connect_and_send(host, port, data):
    try:
        # Créer un socket TCP/IP
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Se connecter au serveur
        sock.connect((host, port))
        
        # Envoyer des données (modifier selon vos besoins)
        sock.sendall(data.encode())
        
        # Attendre la réponse si nécessaire
        # response = sock.recv(1024)
        # print("Réponse du serveur:", response.decode())
        
        # Fermer la connexion
        sock.close()
    except Exception as e:
        print(f"Erreur lors de la connexion au serveur : {e}")

# Paramètres du serveur à tester
server_host = 'localhost'  # Adresse IP ou nom du serveur
server_port = 4242         # Port du serveur
num_connections = 100000       # Nombre de connexions à simuler
message = "Hello, server!"  # Message à envoyer (modifiable selon vos besoins)

# Liste pour stocker les threads
threads = []

# Créer les threads pour simuler plusieurs connexions
for i in range(num_connections):
    t = threading.Thread(target=connect_and_send, args=(server_host, server_port, message))
    threads.append(t)
    t.start()

# Attendre que tous les threads se terminent
for t in threads:
    t.join()

print(f"Test de stress terminé. {num_connections} connexions ont été envoyées vers {server_host}:{server_port}.")
