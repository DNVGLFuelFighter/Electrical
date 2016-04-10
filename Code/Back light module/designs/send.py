import socket
import signal
import sys
 

def signal_handler(signal, frame):
    print'You pressed Ctrl+C!'
    sock.close()
    sys.exit(0)
 

signal.signal(signal.SIGINT, signal_handler)
 
ECHO_SERVER_ADDRESS = '192.168.1.2'
ECHO_PORT = 7


send_str = "16711680,16711680,16711680,0,0,0,0,0,0,0,0,0,0,16711680,16711680,16711680,16711680,16773120,16711680,16711680,0,0,0,0,0,0,0,0,16711680,16711680,16773120,16711680,16711680,16773120,16773120,16711680,16711680,0,0,0,0,0,0,16711680,16711680,16773120,16773120,16711680,16711680,16773120,16773120,16773120,16711680,16711680,0,0,0,0,16711680,16711680,16773120,16773120,16773120,16711680,16711680,16773120,16773120,16773120,16773120,16711680,16711680,0,0,16711680,16711680,16773120,16773120,0,16773120,16711680,16711680,16773120,0,16773120,16773120,16773120,16711680,16711680,16711680,16711680,16773120,16773120,0,0,16773120,16711680,16711680,16773120,0,0,16773120,16773120,16773120,16711680,16711680,16773120,16773120,0,0,0,16773120,16711680,16711680,16773120,0,0,0,16773120,16773120,16773120,16773120,16773120,0,0,0,0,16773120,16711680,16711680,16773120,0,0,0,0,16773120,16773120,16773120,0,0,0,0,0,16773120,16711680,16711680,16773120,0,0,0,0,0,16773120,0,0,0,0,0,0,16773120,16711680,16711680,16773120,0,0,0,0,0,0,0,0,0,0,0,0,16773120,16711680,16711680,16773120,0,0,0,0,0,0,0,0,0,0,0,0,16773120,16711680,16711680,16773120,0,0,0,0,0,0,0,0,0,0,0,0,0,16711680,16711680,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16773120,16773120,0,0,0,0,0,0,0,0,0,0,0,0,0,16711680,16773120,16773120,16711680,0,0,0,0,0,0,0,0,0,0,0,0,16711680,16773120,16773120,16711680,0,0,0,0,0,0,0,0,0,0,0,0,16711680,16773120,16773120,16711680,0,0,0,0,0,0,16711680,0,0,0,0,0,16711680,16773120,16773120,16711680,0,0,0,0,0,16711680,16711680,16711680,0,0,0,0,16711680,16773120,16773120,16711680,0,0,0,0,16711680,16711680,16773120,16711680,16711680,0,0,0,16711680,16773120,16773120,16711680,0,0,0,16711680,16711680,16773120,16773120,16773120,16711680,16711680,0,0,16711680,16773120,16773120,16711680,0,0,16711680,16711680,16773120,16773120,16773120,16773120,16773120,16711680,16711680,0,16711680,16773120,16773120,16711680,0,16711680,16711680,16773120,16773120,0,0,16773120,16773120,16773120,16711680,16711680,16711680,16773120,16773120,16711680,16711680,16711680,16773120,16773120,0,0,0,0,16773120,16773120,16773120,16711680,16711680,16773120,16773120,16711680,16711680,16773120,16773120,0,0,0,0,0,0,16773120,16773120,16773120,16711680,16773120,16773120,16711680,16773120,16773120,0,0,0,0,0,0,0,0,16773120,16773120,16773120,16773120,16773120,16773120,16773120,0,0,0,0,0,0,0,0,0,0,16773120,16773120,16773120,"

if(len(sys.argv) > 1):
    print "I have input"
    send_str = raw_input("")

print len(send_str)
print send_str

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(send_str, (ECHO_SERVER_ADDRESS, ECHO_PORT))
#response = sock.recv(256)
sock.close()
 
#print response
