from mouv import handle_movement

def level_up(s, ia):
    level_1_to_2(s, ia)
    handle_movement(s, ia)

def go_level_8(s, ia):
    while ia.nb_linemate > 0:
        ia.steps.append('Set linemate')
        ia.nb_linemate -= 1
    while ia.nb_deraumere > 0:
        ia.steps.append('Set deraumere')
        ia.nb_deraumere -= 1
    while ia.nb_sibur > 0:
        ia.steps.append('Set sibur')
        ia.nb_sibur -= 1
    while ia.nb_mendiane > 0:
        ia.steps.append('Set mendiane')
        ia.nb_mendiane -= 1
    while ia.nb_phiras > 0:
        ia.steps.append('Set phiras')
        ia.nb_phiras -= 1
    while ia.nb_thystame > 0:
        ia.steps.append('Set thystame')
        ia.nb_thystame -= 1
    if ia.state != "MEET_UP_CHIEF" and ia.broadcast_is_sended == False:
        ia.broadcast_is_sended = True
        broadcast = f"Broadcast -{ia.teamname}-je_suis_pret-"
        ia.steps.append(broadcast)
    if ia.state == "MEET_UP_CHIEF" and ia.incantation == False and ia.player_ready >= 6:
        ia.incantation = True
        ia.steps.append('Incantation')
        ia.steps.append('Incantation')
        ia.steps.append('Incantation')
        ia.steps.append('Incantation')
        ia.steps.append('Incantation')
        ia.steps.append('Incantation')
        handle_movement(s, ia)

def level_1_to_2(s, ia):
    if ia.nb_linemate >= 1 and ia.level == 1:
        print("\nlevel up\n")
        ia.steps.append('Set linemate')
        ia.steps.append('Incantation')
        ia.increment_level()
        ia.nb_linemate -= 1
        ia.nb_global_linemate -= 1


def level_2_to_3(s, ia):
    if ia.nb_player >= 2 and ia.level == 2 and ia.nb_global_linemate == 1 and ia.nb_global_deraumere == 1 and ia.nb_global_sibur == 1:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_deraumere > 0:
            ia.steps.append('Set deraumere')
            ia.nb_deraumere -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        ia.steps.append('Incantation')
        ia.level += 1


def level_3_to_4(s, ia):
    if ia.nb_player >= 2 and ia.level == 3 and ia.nb_global_linemate == 2 and ia.nb_global_sibur == 1 and ia.nb_global_phiras == 2:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        while ia.nb_phiras > 0:
            ia.steps.append('Set phiras')
            ia.nb_phiras -= 1
        ia.steps.append('Incantation')
        ia.level += 1


def level_4_to_5(s, ia):
    if ia.nb_player >= 4 and ia.level == 4 and ia.nb_global_linemate == 1 and ia.nb_global_deraumere == 1 and ia.nb_global_sibur == 2 and ia.nb_global_phiras == 1:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_deraumere > 0:
            ia.steps.append('Set deraumere')
            ia.nb_deraumere -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        while ia.nb_phiras > 0:
            ia.steps.append('Set phiras')
            ia.nb_phiras -= 1
        ia.steps.append('Incantation')
        ia.level += 1


def level_5_to_6(s, ia):
    if ia.nb_player >= 4 and ia.level == 5 and ia.nb_global_linemate == 1 and ia.nb_global_deraumere == 2 and ia.nb_global_sibur == 1 and ia.nb_global_mendiane == 3:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_deraumere > 0:
            ia.steps.append('Set deraumere')
            ia.nb_deraumere -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        while ia.nb_mendiane > 0:
            ia.steps.append('Set mendiane')
            ia.nb_mendiane -= 1
        ia.steps.append('Incantation')
        ia.level += 1

def level_6_to_7(s, ia):
    if ia.nb_player >= 6 and ia.level == 6 and ia.nb_global_linemate == 1 and ia.nb_global_deraumere == 2 and ia.nb_global_sibur == 3 and ia.nb_global_phiras == 1:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_deraumere > 0:
            ia.steps.append('Set deraumere')
            ia.nb_deraumere -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        while ia.nb_phiras > 0:
            ia.steps.append('Set phiras')
            ia.nb_phiras -= 1
        ia.steps.append('Incantation')
        ia.level += 1

def level_7_to_8(s, ia):
    if ia.nb_player >= 6 and ia.level == 7 and ia.nb_global_linemate == 2 and ia.nb_global_deraumere == 2 and ia.nb_global_sibur == 2 and ia.nb_global_mendiane == 2 and ia.nb_global_phiras == 2 and ia.thystame == 1:
        print("\nlevel up\n")
        while ia.nb_linemate > 0:
            ia.steps.append('Set linemate')
            ia.nb_linemate -= 1
        while ia.nb_deraumere > 0:
            ia.steps.append('Set deraumere')
            ia.nb_deraumere -= 1
        while ia.nb_sibur > 0:
            ia.steps.append('Set sibur')
            ia.nb_sibur -= 1
        while ia.nb_mendiane > 0:
            ia.steps.append('Set mendiane')
            ia.nb_mendiane -= 1
        while ia.nb_phiras > 0:
            ia.steps.append('Set phiras')
            ia.nb_phiras -= 1
        while ia.nb_thystame > 0:
            ia.steps.append('Set thystame')
            ia.nb_thystame -= 1
        ia.steps.append('Incantation')
        ia.level += 1
