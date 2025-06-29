import socket
import threading

# Fonction pour gérer chaque thread de connexion avec un buffer immense
def connect_and_send_large_buffer(host, port, buffer_size):
    try:
        # Créer un socket TCP/IP
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Se connecter au serveur
        sock.connect((host, port))

        sock.sendall(b'bite\n')
        
        # Construire un buffer de données (rempli de 'A')
        huge_data = b'A' * buffer_size
        
        # Envoyer le buffer de données
        sock.sendall(huge_data)
        
        # Fermer la connexion
        sock.close()
        
        print(f"Envoi de {buffer_size} octets vers {host}:{port} terminé.")
    except Exception as e:
        print(f"Erreur lors de la connexion au serveur : {e}")

# Paramètres du serveur à tester
server_host = '127.0.0.1'  # Adresse IP ou nom du serveur
server_port = 4242         # Port du serveur
buffer_size = 1000   # Taille du buffer à envoyer (10 Mo)

# Créer un thread pour envoyer un buffer immense
t = threading.Thread(target=connect_and_send_large_buffer, args=(server_host, server_port, buffer_size))
t.start()
t.join()

print(f"Test d'envoi de buffer immense terminé vers {server_host}:{server_port}.")
