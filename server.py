#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

'''
the program is used to start server
'''
import socket
import sys
#import MySQLdb
import updateCarPos

def start_tcp_server(ip, port):

  
  
  #create socket
  sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # TCP SOCK_STREAM, UDP SOCK_DGRAM
  server_address = (ip, port)
  #bind port
  print 'starting listen on ip %s, port %s'%server_address
  sock.bind(server_address)
  #starting listening, allow only one connection
  try:
    sock.listen(1)
  except socket.error, e:
    print "fail to listen on port %s"%e
    
    sys.exit(1)
  while True:
    print "waiting for connection"
    client,addr = sock.accept()

    print 'having a connection'
    data=client.recv(1024)
    #if data=='yaoyao':
    #    print 'ha1'
    #elif data=='yaoyao2':
    #    print 'ha2'
    #else:
    print len(data)
    print data
    

    #print (str(data, 'utf8'))
    client.sendall('Gotit!')
    #client.send('bye')
    client.close()
    
    if (len(data) > 0):
        updateCarPos.updatePos(data)
        
if __name__ == '__main__':
  start_tcp_server('192.168.1.222', 32451)
