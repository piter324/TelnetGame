'''
Usage:
python telnet_script [-u username] [-p password] [-l] [-inf]
-l Linux
-inf infinity: niekończąca się pętla akcji
'''
import getpass
import sys
import telnetlib
import random

HOST = "localhost"
tn = telnetlib.Telnet("192.168.0.172", port = 3130)
user = "1"
password = "1"
linux = False
inf = False

if '-u' in sys.argv:
    ind = sys.argv.index('-u')
    user = sys.argv[ind+1]
    user = str(user)
if '-p' in sys.argv:
    ind = sys.argv.index('-p')
    password = sys.argv[ind+1]
    password = str(password)
if '-l' in sys.argv:
    linux = True
if '-inf' in sys.argv:
    inf = True


print("user = " + user +"; password = " + password)

l = tn.read_until(b"Wyjdz")
print(l)
message = b"login\r\n"
if linux:
    message = b"login\n"
tn.write(message)
l = tn.read_until(b"uzytkownika:")
print(l)
message = bytes(user, 'utf-8') + b"\r\n"
if linux:
    message = bytes(user, 'utf-8') + b"\r\n"
#print([hex(i) for i in message])
tn.write(message)
l = tn.read_until(b"haslo:")
print(l)
message = bytes(password, 'utf-8') + b"\r\n"
if linux:
    message = bytes(password, 'utf-8') + b"\r\n"
tn.write(message)
l = tn.read_until(b"szatni.")
print(l)
#print("koniec")


possible_path = (
    (b"a", b"a", b"b", b"a", b"c", b"a", b"d", b"a", b"e", b"b", b"b", b"a", b"d", b"e" ), 
)

scenario_nr = random.randint(0, len(possible_path) - 1)
scenario = possible_path[scenario_nr]

count = 0

while count < 1:
    for i in scenario:
        message = i + b"\r\n"
        if linux:
            message = i + b"\r\n"
        tn.write(message)
        l = tn.read_until(b">")
        print(l)
    count += 1
    if inf:
        count = 0