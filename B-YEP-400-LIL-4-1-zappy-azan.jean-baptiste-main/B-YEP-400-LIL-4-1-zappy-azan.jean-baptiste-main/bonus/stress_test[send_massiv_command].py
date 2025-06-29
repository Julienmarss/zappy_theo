import socket
import threading
import random
import string
import time

def connect_and_send_random_commands(host, port, num_commands, command_length, delay=0.1):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host, port))
        
        for _ in range(num_commands):
            command = ''.join(random.choices(string.ascii_letters + string.digits, k=command_length))
            
            sock.sendall(command.encode())
            print(f"Envoyé: {command}")
            
            time.sleep(delay)
        
        sock.close()
        print(f"Connexion au serveur {host}:{port} fermée.")
    except socket.error as e:
        print(f"Erreur lors de la connexion ou de l'envoi de commandes : {e}")
    except Exception as e:
        print(f"Erreur inattendue : {e}")

server_host = '127.0.0.1'  # Adresse IP ou nom du serveur
server_port = 4242          # Port du serveur
num_commands = 1000         # Nombre de commandes à envoyer
command_length = 500         # Longueur de chaque commande aléatoire
delay_between_commands = 0.1  # Délai (en secondes) entre l'envoi de chaque commande

t = threading.Thread(target=connect_and_send_random_commands, args=(server_host, server_port, num_commands, command_length, delay_between_commands))
t.start()

try:
    t.join()
except KeyboardInterrupt:
    print("Test de stress interrompu par l'utilisateur.")
