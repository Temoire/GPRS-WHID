import socket, sys
import signal
def handler(*args):
      # 6) Fermeture de la connexion :
    connexion.send("exit".encode("Utf8"))
    print("Connexion interrompue.")
    connexion.close()
    #mySocket.close()
    sys.exit()




HOST = 'localhost'
PORT = 5000
counter =0	 # compteur de connexions actives
 
# 1) création du socket :
mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
 
# 2) liaison du socket à une adresse précise :
try:
    mySocket.bind((HOST, PORT))
except socket.error:
    print("La liaison du socket à l'adresse choisie a échoué.")
    sys.exit()
 
while 1:
    # 3) Attente de la requête de connexion d'un client :
    print("Serveur prêt, en attente de requêtes ...")
    mySocket.listen(2)
    
    # 4) Etablissement de la connexion :
    connexion, adresse = mySocket.accept()
    counter +=1
    print("Client connecté, adresse IP %s, port %s" % (adresse[0], adresse[1]))
    
    # 5) Dialogue avec le client :
    signal.signal(signal.SIGINT, handler)
    msgClient = connexion.recv(1024).decode("Utf8")
    while 1:


        print(msgClient,end ='')
        if msgClient[len(msgClient)-2]=='>' : 
            msgServeur = input("S> ")
            for i in range (int(len(msgServeur)/64)+1):
                print (i)
                print(int(len(msgServeur)/64))
                connexion.send(msgServeur[64*i:64+64*i].encode("Utf8"))
                print(msgServeur[64*i:64+64*i].encode("Utf8"))
        msgClient = connexion.recv(1024).decode("Utf8")
 

        
   