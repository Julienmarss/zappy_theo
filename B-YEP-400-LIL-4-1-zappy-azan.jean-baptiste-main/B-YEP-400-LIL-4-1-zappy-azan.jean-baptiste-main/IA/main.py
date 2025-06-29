import socket
import time
import random
import sys
import select
from collections import deque
from take_object import take_action
from level_handling import level_up
from level_handling import go_level_8
from mouv import handle_movement

class MyIA:
    def __init__(self):
        self.nb_food = 0
        self.nb_linemate = 0
        self.teamname = ""
        self.nb_deraumere = 0
        self.nb_sibur = 0
        self.nb_mendiane = 0
        self.nb_phiras = 0
        self.nb_thystame = 0
        self.nb_global_food = 0
        self.nb_global_linemate = 0
        self.nb_command_send = 0
        self.nb_global_deraumere = 0
        self.nb_global_sibur = 0
        self.nb_global_mendiane = 0
        self.nb_global_phiras = 0
        self.steps = []
        self.nb_global_thystame = 0
        self.state = "FOOD_MOD"
        self.level = 1
        self.reponse = deque()
        self.nb_player = 1
        self.inventory_is_send = False
        self.client_num = -1
        self.random_num = random.randint(1, 100000000)
        self.joined = False
        self.is_chief = False
        self.not_chief = False
        self.chief_is_designed = False
        self.incantation = False
        self.meetup = False
        self.player_ready = 1
        self.broadcast_is_sended = False

    def increment_food(self):
        self.nb_food += 1

    def increment_linemate(self):
        self.nb_linemate += 1

    def increment_deraumere(self):
        self.nb_deraumere += 1

    def increment_sibur(self):
        self.nb_sibur += 1

    def increment_mendiane(self):
        self.nb_mendiane += 1

    def increment_phiras(self):
        self.nb_phiras += 1

    def increment_thystame(self):
        self.nb_thystame += 1

    def increment_level(self):
        self.level += 1

    def decrement_food(self):
        if self.nb_food > 0:
            self.nb_food -= 1



def login(ip, port, name, timeout, ia):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(timeout)
    try:
        s.connect((ip, port))
        reponse = s.recv(1024)
        print("Réponse : ", reponse.decode())
        ia.teamname = name
        s.sendall(name.encode() + b'\n')
        reponse = s.recv(1024)
        rep = reponse.decode().split("\n")
        ia.client_num = rep[0]
        print("CLIENT: ", ia.client_num)
        print("client num : ", reponse.decode())
        return s

    except socket.timeout:
        raise ConnectionError(f"Erreur : le serveur {ip} sur le port {port} n'a pas répondu dans le délai imparti.")
    except Exception as e:
        raise ConnectionError(f"Erreur lors de la connexion au serveur : {e}")

def choose_mov(s, ia, double_tableau):
    def get_steps_to_index(index):
        if index == 1:
            return ['Forward', 'Left', 'Forward']
        elif index == 2:
            return ['Forward']
        elif index == 3:
            return ['Forward', 'Right', 'Forward']
        elif index == 4:
            return ['Forward', 'Forward', 'Left', 'Forward', 'Forward']
        elif index == 5:
            return ['Forward', 'Forward', 'Left', 'Forward']
        elif index == 6:
            return ['Forward', 'Forward']
        elif index == 7:
            return ['Forward', 'Forward', 'Right', 'Forward']
        elif index == 8:
            return ['Forward', 'Forward', 'Right', 'Forward', 'Forward']
        else:
            return []

    target_index = -1
    max_food_count = 0
    max_linemate_count = 0
    max_deraumere_count = 0
    max_sibur_count = 0
    max_mendiane_count = 0
    max_phiras_count = 0
    max_thystame_count = 0

    for index, tab in enumerate(double_tableau):
        print("index:", index, "tab:", tab, " mod : ", ia.state)



        if ia.level == 1 and "linemate" in tab:
            target_index = index
            break
        elif "food" in tab and ia.state == "FOOD_MOD":
            food_count = tab.count("food")
            if food_count > max_food_count:
                max_food_count = food_count
                target_index = index
        elif "linemate" in tab and ia.state == "LINEMATE_MOD":
            linemate_count = tab.count("linemate")
            if linemate_count > max_linemate_count:
                max_linemate_count = linemate_count
                target_index = index
        elif "deraumere" in tab and ia.state == "DERAUMERE_MOD":
            deraumere_count = tab.count("deraumere")
            if deraumere_count > max_deraumere_count:
                max_deraumere_count = deraumere_count
                target_index = index
        elif "sibur" in tab and ia.state == "SIBUR_MOD":
            sibur_count = tab.count("sibur")
            if sibur_count > max_sibur_count:
                max_sibur_count = sibur_count
                target_index = index
        elif "mendiane" in tab and ia.state == "MENDIANE_MOD":
            mendiane_count = tab.count("mendiane")
            if mendiane_count > max_mendiane_count:
                max_mendiane_count = mendiane_count
                target_index = index
        elif "phiras" in tab and ia.state == "PHIRAS_MOD":
            phiras_count = tab.count("phiras")
            if phiras_count > max_phiras_count:
                max_phiras_count = phiras_count
                target_index = index
        elif "thystame" in tab and ia.state == "THYSTAME_MOD":
            thystame_count = tab.count("thystame")
            if thystame_count > max_thystame_count:
                max_thystame_count = thystame_count
                target_index = index

    if target_index != -1:
        step = get_steps_to_index(target_index)
        for ste in step:
            ia.steps.append(ste)
        handle_movement(s, ia)
        take_action(s, ia, double_tableau[target_index])
        level_up(s, ia)
    else:
        ia.steps.append('Forward')
        ia.steps.append('Forward')
        handle_movement(s, ia)



def look_and_parse(s, ia):
    decoded_response = ia.reponse[0]
    ia.reponse.popleft()
    print("Réponse brute:", decoded_response)
    elements = decoded_response.strip()[1:-1].split(",")
    print("Elements:", elements)
    
    double_tableau = []
    for case in elements:
        objets = case.strip().split(" ")
        if not objets[0]:
            objets = ["NONE"]
        double_tableau.append(objets)
    print("Double tableau:")
    for case in double_tableau:
        print(case)
    return double_tableau

def get_and_update_inventory(s, ia):
    inventory_after = ia.reponse[0]
    ia.reponse.popleft()
    print("after inv:", inventory_after)
    
    # Removing the brackets and splitting the items
    items = inventory_after[1:-1].split(", ")
    inventory_dict = {}
    
    for item in items:
        parts = item.split()
        if len(parts) > 1:
            # Handle cases where there are spaces
            name = parts[0]
            count = parts[-1]
            # Check if count is a number, otherwise set it to 1
            count = int(count) if count.isdigit() else 1
            inventory_dict[name] = count

    
    # Updating the IA's attributes
    ia.nb_food = inventory_dict.get("food", 0)
    ia.nb_linemate = inventory_dict.get("linemate", 0)
    ia.nb_deraumere = inventory_dict.get("deraumere", 0)
    ia.nb_sibur = inventory_dict.get("sibur", 0)
    ia.nb_mendiane = inventory_dict.get("mendiane", 0)
    ia.nb_phiras = inventory_dict.get("phiras", 0)
    ia.nb_thystame = inventory_dict.get("thystame", 0)

def increment_global_inventory(s, ia):
    rep = ia.reponse[0].split(",")
    message = rep[1].split("-")
    if message[2] == "chief" and message[1] == ia.teamname:
        ia.not_chief = True
        ia.chief_is_designed = True
        ia.reponse.popleft()
        return
    if message[2] == "jesuisla" and ia.state == "MEET_UP_CHIEF" and ia.teamname == message[1]:
        ia.nb_player += 1
        print("\n\n\n\n\nIL YYYYYYYYYYYY AAAAAAAAAAAAAAAAAA:    ", ia.nb_player)
        print("\n\n\n\n\n\n")
        ia.reponse.popleft()
        return
    elif message[2] == "jesuisla" and ia.teamname == message[1] and ia.state != "MEET_UP_CHIEF":
        ia.nb_player += 1
        ia.reponse.popleft()
        return
    if message[2] == "je_suis_pret" and message[1] == ia.teamname and ia.state == "MEET_UP_CHIEF":
        ia.player_ready += 1
        ia.reponse.popleft()
        print("\n\n\n\n JOUEUR PRET A INCANTER:   ", ia.player_ready)
        print("\n\n\n\n")
        return
    if message[2] == "je_suis_pret" and message[1] == ia.teamname and ia.state == "MEETUP":
        ia.player_ready += 1
        ia.reponse.popleft()
        return
    if message[1] == ia.teamname:
        ia.nb_global_linemate += int(message[2])
        ia.nb_global_deraumere += int(message[3])
        ia.nb_global_sibur += int(message[4])
        ia.nb_global_mendiane += int(message[5])
        ia.nb_global_phiras += int(message[6])
        ia.nb_global_thystame += int(message[7])
        print("\n global linemate : ", ia.nb_global_linemate)
        print("\n global deraumere : ", ia.nb_global_deraumere)
        print("\n global sibur : ", ia.nb_global_sibur)
        print("\n global mendiane : ", ia.nb_global_mendiane)
        print("\n global phiras : ", ia.nb_global_phiras)
        print("\n global thystame : ", ia.nb_global_thystame)
    ia.reponse.popleft()

def meetup(s, ia):
    
    tab = ia.reponse[0].split(",")
    tab2 = tab[0].split(" ")
    dir = tab2[1]
    print("je bouge : -", dir, "-")
    tab3 = tab[1].split("_")
    nb = int(tab3[3])
    if nb != ia.random_num:
        return
    if dir.find("0") != -1:
        ia.steps = []
        if ia.joined == False:
            meetup = f"Broadcast -{ia.teamname}-jesuisla-"
            ia.steps.append(meetup)
            ia.joined = True
            ia.nb_player += 1
        print("\n\n\n ON EST ENSEMBLE \n\n\n")
        handle_movement(s, ia)
        return
    elif dir.find("3") != -1:
        ia.steps.append("Left")
    elif dir.find("4") != -1:
        ia.steps.append("Left")
    elif dir.find("5") != -1:
        ia.steps.append("Left")
        ia.steps.append("Left")
    elif dir.find("6") != -1:
        ia.steps.append("Right")
    elif dir.find("7") != -1:
        ia.steps.append("Right")
    ia.steps.append("Forward")
    gooo = f"Broadcast _{ia.teamname}_goo_{ia.random_num}"
    ia.steps.append(gooo)
    handle_movement(s, ia)


def main_game(s, ia):
    try:
        is_look = False
        is_inventory = False
        passeed = False
        fork = True
        while True:
            sockets_to_read = [s]

            readable, _, _ = select.select(sockets_to_read, [], [], 0.5)
            
            for s in readable:
                if s is s:
                    data = s.recv(1024)
                    if data:
                        print(f"Message reçu: {data.decode()}")
                        dbl_tab = data.decode().split("\n")
                        for elem in dbl_tab:
                            ia.reponse.append(elem)
                        while len(ia.reponse) > 0:
                            if (ia.reponse[0].find("food", 0, 6) != -1):
                                ia.nb_command_send -= 1
                                get_and_update_inventory(s, ia)
                                passeed = False
                            elif (ia.reponse[0].find("player", 0,8) != -1) and ia.state != "MEET_UP_CHIEF":
                                ia.nb_command_send -= 1
                                double_tableau = look_and_parse(s, ia)
                                choose_mov(s, ia, double_tableau)
                                passeed = True
                            elif (ia.reponse[0].find("message", 0, 9) != -1):
                                rep = ia.reponse[0].split(",")
                                if rep[1].find("readymeetup") != -1 and ia.state != "MEET_UP_CHIEF":
                                    current_team = rep[1].split("_")
                                    if current_team[1] == ia.teamname:
                                        gooo = f"Broadcast _{ia.teamname}_goo_{ia.random_num}"
                                        ia.steps.append(gooo)
                                        ia.reponse.popleft()
                                        handle_movement(s, ia)
                                    else:
                                        ia.meetup = True
                                        ia.reponse.popleft()
                                elif rep[1].find("readymeetup") != -1 and ia.state == "MEET_UP_CHIEF":
                                    ia.reponse.popleft()

                                if rep[1].find("_goo_") != -1 and ia.state == "MEET_UP_CHIEF":
                                    tab3 = rep[1].split("_")
                                    if tab3[1] == ia.teamname:
                                        meetup_perso = f"Broadcast _{ia.teamname}_meetup_{tab3[3]}"
                                        ia.steps.append(meetup_perso)
                                        ia.reponse.popleft()
                                        handle_movement(s, ia)
                                    else:
                                        ia.reponse.popleft()
                                elif rep[1].find("_goo_") != -1 and ia.state != "MEET_UP_CHIEF":
                                    ia.reponse.popleft()
                                if rep[1].find("_meetup_") != -1 and ia.state != "MEET_UP_CHIEF":
                                    mess = rep[1].split("_")
                                    if mess[1] == ia.teamname:
                                        ia.state = "MEETUP"
                                        meetup(s, ia)
                                        ia.reponse.popleft()
                                    else:
                                        ia.reponse.popleft()
                                elif rep[1].find("_meetup_") != -1 and ia.state == "MEET_UP_CHIEF":
                                    ia.reponse.popleft()
                                elif rep[1].find("-") != -1:
                                    increment_global_inventory(s, ia)
                            elif (ia.reponse[0].find("ok", 0, 4) != -1 or ia.reponse[0].find("ko", 0, 4) != -1):
                                ia.nb_command_send -= 1
                                if ia.reponse[0].find("ko", 0, 4) != -1 and ia.nb_player < 6:
                                    ia.steps.append("Left")
                                ia.reponse.popleft()
                            else:
                                ia.reponse.popleft()
                    else:
                        print("Le serveur a fermé la connexion")
                        s.close()
                        sys.exit()
            if ia.state != "MEETUP" and ia.state != "MEET_UP_CHIEF":
                if ia.nb_food < 20:
                    ia.state = "FOOD_MOD"
                if ia.nb_food > 70:
                    if ia.chief_is_designed == False:
                        print("\n\n\n\n\n\n\n\nJE SUUUIS LE KIIIIIIIIIIIIIIIIIIIIIIIIING\n\n\n\n\n")
                        ia.chief_is_designed = True
                        ia.is_chief = True
                        ia.steps = []
                        chief = f"Broadcast -{ia.teamname}-chief"
                        ia.steps.append(chief)
                        handle_movement(s, ia)
                    if ia.inventory_is_send == False:
                        ia.inventory_is_send = True
                        inv = f"Broadcast -{ia.teamname}-{ia.nb_linemate}-{ia.nb_deraumere}-{ia.nb_sibur}-{ia.nb_mendiane}-{ia.nb_phiras}-{ia.nb_thystame}"
                        ia.steps.append(inv)
                    if ia.state == "FOOD_MOD":
                        ia.state = "OUAIS"
                if ia.state != "FOOD_MOD" and ia.state != "MEETUP":
                    if fork == True:
                        fork = False
                        for i in range(5):
                            ia.steps.append('Fork')
                            handle_movement(s, ia)

                    if ia.nb_global_linemate < 8:
                        ia.state = "LINEMATE_MOD"
                    elif ia.nb_global_deraumere < 8:
                        ia.state = "DERAUMERE_MOD"
                    elif ia.nb_global_sibur < 10:
                        ia.state = "SIBUR_MOD"
                    elif ia.nb_global_mendiane < 5:
                        ia.state = "MENDIANE_MOD"
                    elif ia.nb_global_phiras < 6:
                        ia.state = "PHIRAS_MOD"
                    elif ia.nb_global_thystame < 1:
                        ia.state = "THYSTAME_MOD"
                    
                    elif ia.state != "MEETUP":
                        if ia.is_chief == True and ia.not_chief == False:
                            ia.state = "MEET_UP_CHIEF"
                            phrase = f"Broadcast _{ia.teamname}_readymeetup"
                            ia.steps.append(phrase)
                if passeed == False and is_look == False and ia.state != "MEETUP" and ia.state != "MEET_UP_CHIEF":
                    ia.steps.append('Look')
                    is_look = True
                    is_inventory = False
                    handle_movement(s, ia)
                elif passeed == True and is_inventory == False and ia.state != "MEETUP" and ia.state != "MEET_UP_CHIEF":
                    ia.steps.append('Inventory')
                    handle_movement(s, ia)
                    is_look = False
                    is_inventory = True
            if ia.nb_player >= 6:
                go_level_8(s, ia)
            handle_movement(s, ia)

    except KeyboardInterrupt:
        print("Interruption par l'utilisateur")
        s.close()
        sys.exit()

def main():
    if len(sys.argv) < 7 or '-h' not in sys.argv:
        print("Usage: ./zappy_ai -p <port> -n <name> -h <host>")
        sys.exit(1)

    host = None
    port = None
    name = None

    i = 1
    while i < len(sys.argv):
        if sys.argv[i] == '-p':
            port = int(sys.argv[i + 1])
            i += 1
        elif sys.argv[i] == '-n':
            name = sys.argv[i + 1]
            i += 1
        elif sys.argv[i] == '-h':
            host = sys.argv[i + 1]
            i += 1
        i += 1

    if port is None or name is None or host is None:
        print("Missing required arguments: -p <port>, -n <name>, -h <host>")
        sys.exit(1)

    my_ia = MyIA()
    try:
        socket = login(host, port, name, 5, my_ia)
    except ConnectionError as e:
        print(e)
        sys.exit(1)

    if socket:
        return main_game(socket, my_ia)
    else:
        sys.exit(1)

if __name__ == "__main__":
    main()
