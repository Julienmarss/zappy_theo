from mouv import handle_movement

def take_action(s, ia, tab):
    if ia.meetup == True and "player" in tab:
        return
    for element in tab:
        if "food" == element:
            take_food(s, ia)
        if ("linemate" == element and ia.state == "FOOD_MOD") or ("linemate" == element and ia.state == "LINEMATE_MOD" and ia.nb_global_linemate < 8):
            take_linemate(s, ia)
        if ("deraumere" == element and ia.state == "FOOD_MOD") or ("deraumere" == element and ia.state == "DERAUMERE_MOD" and ia.nb_global_deraumere < 8):
            take_deraumere(s, ia)
        if ("sibur" == element and ia.state == "FOOD_MOD") or ("sibur" == element and ia.state == "SIBUR_MOD" and ia.nb_global_sibur < 10):
            take_sibur(s, ia)
        if ("mendiane" == element and ia.state == "FOOD_MOD") or ("mendiane" == element and ia.state == "MENDIANE_MOD" and ia.nb_global_mendiane < 5):
            take_mendiane(s, ia)
        if ("phiras" == element and ia.state == "FOOD_MOD") or ("phiras" == element and ia.state == "PHIRAS_MOD" and ia.nb_global_phiras < 6):
            take_phiras(s, ia)
        if ("thystame" == element and ia.state == "FOOD_MOD") or ("thystame" == element and ia.state == "THYSTAME_MOD" and ia.nb_global_thystame < 1):
            take_thystame(s, ia)
        handle_movement(s, ia)

def take_food(s, ia):
    ia.steps.append('Take food')
    print("reponse take food:")

def take_linemate(s, ia):
    ia.steps.append('Take linemate')
    print("reponse take linemate:")
    ia.nb_global_linemate += 1

def take_deraumere(s, ia):
    ia.steps.append('Take deraumere')
    print("reponse take deraumere:")
    ia.nb_global_deraumere += 1

def take_sibur(s, ia):
    ia.steps.append('Take sibur')
    print("reponse take sibur:")
    ia.nb_global_sibur += 1

def take_mendiane(s, ia):
    ia.steps.append('Take mendiane')
    print("reponse take mendiane:")
    ia.nb_global_mendiane += 1

def take_phiras(s, ia):
    ia.steps.append('Take phiras')
    print("reponse take phiras:")
    ia.nb_global_phiras += 1

def take_thystame(s, ia):
    ia.steps.append('Take thystame')
    print("reponse take thystame:")
    ia.nb_global_thystame += 1
