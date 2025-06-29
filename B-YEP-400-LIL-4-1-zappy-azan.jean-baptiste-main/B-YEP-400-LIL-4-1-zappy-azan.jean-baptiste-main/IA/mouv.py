def handle_movement(s, ia):
    if (ia.nb_command_send <10 and ia.steps):
        command = ia.steps[0].encode() + b'\n'
        s.sendall(command)
        ia.nb_command_send += 1
        print("commande exec : ", command)
        ia.steps.pop(0)
        handle_movement(s, ia)