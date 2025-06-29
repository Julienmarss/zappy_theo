#!/bin/bash

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration du port et de l'adresse du serveur
HOST="127.0.0.1"
PORT=4242
TEAM_NAME="lolo"

# Fonction pour vérifier si le serveur est en cours d'exécution
check_server_running() {
    if nc -z $HOST $PORT; then
        echo -e "${GREEN}Le serveur fonctionne sur le port $PORT${NC}"
    else
        echo -e "${RED}Le serveur n'est pas en cours d'exécution sur le port $PORT${NC}"
        exit 1
    fi
}

# Fonction pour envoyer une commande et attendre une réponse
send_command() {
    local command=$1
    echo -ne "$command\n" | nc $HOST $PORT
}

# Fonction pour tester la connexion du client IA
test_client_connection() {
    response=$(echo -ne "GRAPHIC\n" | nc $HOST $PORT)
    if [[ "$response" == "WELCOME" ]]; then
        echo -e "${GREEN}Connexion au serveur réussie.${NC}"
    else
        echo -e "${RED}Échec de la connexion au serveur.${NC}"
    fi
}

# Fonction pour tester les commandes basiques
test_basic_commands() {
    # Ouverture de la connexion au serveur
    exec 3<>/dev/tcp/$HOST/$PORT
    if [ $? -ne 0 ]; then
        echo -e "${RED}Échec de la connexion au serveur${NC}"
        exit 1
    fi

    # Envoi de la commande d'authentification de l'IA
    echo -ne "$TEAM_NAME\n" >&3
    read -u 3 response
    echo -e "${CYAN}Réponse du serveur : $response${NC}"

    if [[ "$response" != "WELCOME" ]]; then
        echo -e "${RED}Connexion au serveur échouée.${NC}"
        exit 1
    fi

    # Envoi de la commande TEAM-NAME
    echo -ne "$TEAM_NAME\n" >&3
    read -u 3 response
    echo -e "${CYAN}Réponse du serveur : $response${NC}"

    # Liste des commandes basiques à tester
    commands=("Forward" "Right" "Left" "Look" "Inventory" "Broadcast Hello" "Connect_nbr" "Fork" "Eject" "Take linemate" "Set linemate" "Incantation")

    for cmd in "${commands[@]}"; do
        echo -ne "$cmd\n" >&3
        read -u 3 response
        echo -e "${YELLOW}Commande: $cmd -> ${GREEN}Réponse: $response${NC}"
    done

    # Fermeture de la connexion
    exec 3<&-
    exec 3>&-
}

# Vérifier si le serveur fonctionne
check_server_running

# Tester les commandes basiques
test_basic_commands

echo -e "${BLUE}Tests terminés.${NC}"
