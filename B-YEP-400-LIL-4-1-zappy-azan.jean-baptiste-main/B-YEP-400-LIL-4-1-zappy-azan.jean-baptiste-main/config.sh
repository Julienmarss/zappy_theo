#!/bin/bash

# Vérifier si un argument de port est fourni
if [ -z "$1" ]; then
    echo "Usage: $0 <port> {open|close|status|clean}"
    exit 1
fi

PORT=$1
PF_ANCHOR_FILE="/etc/pf.anchors/zappy_pf.conf"
PF_CONF_FILE="/etc/pf.conf"
PORTS_FILE="/etc/pf.anchors/zappy_ports.conf"

# Créer le fichier de configuration pour pfctl
create_pf_config() {
    echo "Creating pfctl configuration for port $PORT..."
    sudo sh -c "echo '# Allow incoming connections on port $PORT\npass in proto tcp from any to any port $PORT' >> $PF_ANCHOR_FILE"
    echo $PORT | sudo tee -a $PORTS_FILE > /dev/null
}

# Modifier le fichier /etc/pf.conf pour inclure le fichier de configuration personnalisé
modify_pf_conf() {
    if ! grep -q "anchor \"zappy\"" $PF_CONF_FILE; then
        echo "Modifying $PF_CONF_FILE to include custom rules..."
        sudo sh -c "echo '\n# Load custom rules\nanchor \"zappy\"\nload anchor \"zappy\" from \"$PF_ANCHOR_FILE\"' >> $PF_CONF_FILE"
    fi
}

# Activer et recharger le pare-feu pfctl
enable_pf() {
    echo "Loading and enabling pfctl with new rules..."
    sudo pfctl -f $PF_CONF_FILE
    sudo pfctl -e
    sudo pfctl -sr
}

# Fermer le port spécifié pour des raisons de sécurité
close_port() {
    echo "Closing port $PORT..."
    sudo sed -i '' "/port $PORT/d" $PF_ANCHOR_FILE
    sudo sed -i '' "/$PORT/d" $PORTS_FILE
    sudo pfctl -f $PF_CONF_FILE
    sudo pfctl -sr
}

# Fermer tous les ports ouverts pour Zappy
clean_ports() {
    echo "Closing all ports opened for Zappy..."
    sudo sh -c "echo '' > $PF_ANCHOR_FILE"
    sudo sh -c "echo '' > $PORTS_FILE"
    sudo pfctl -f $PF_CONF_FILE
    sudo pfctl -sr
}

# Afficher l'état du pare-feu
check_pf() {
    sudo lsof -i :$PORT
}

# Menu pour l'utilisateur
case "$2" in
    open)
        create_pf_config
        modify_pf_conf
        enable_pf
        ;;
    close)
        close_port
        ;;
    status)
        check_pf
        ;;
    clean)
        clean_ports
        ;;
    *)
        echo "Usage: $0 <port> {open|close|status|clean}"
        exit 1
        ;;
esac

exit 0
