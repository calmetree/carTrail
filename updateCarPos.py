#!/usr/bin/python2.7
# -*- coding: utf-8 -*-

import MySQLdb
import showCarPos
import time

def updatePos(data):
    db = MySQLdb.connect(host="localhost",db="carInfo",charset="utf8")
    cur = db.cursor()
    
    # cur.execute("create table carPosInfo(last_pos varchar(60))")
    # cur.execute("drop table carPosInfo") ##delete table
    '''
    dateT=time.strftime('%Y.%m.%d',time.localtime())
    rows=cur.execute('select * from carPosInfo');
    a=cur.fetchall();
    for row in range(0,rows):
        #print a[row][1]
        #x=str(float(a[row][1][0:9])+80000)+a[row][1][10:]
        x=str((int(float(a[row][1][0:9]))+80000)%240000)+a[row][1][10:]
        #print x
        sql="insert into carPosInfo(dateT,last_pos) values('"
        sql=sql+dateT+"','"+x+"')";
        print sql
        cur.execute(sql)
    '''
    # cur.execute("create table carPosInfo(dataT varchar(15),last_pos varchar(60))")
    # cur.execute("drop table carPosInfo") ##delete table
    dateT=time.strftime('%Y.%m.%d',time.localtime())
    b=data.split('GPGGA,')[1]
    bb=b.split(',')[0]
    #print len(bb)   
    #print "dddd"
    #print b
    if (len(bb)>0 and bb[0]!='' and len(b)>40 and b.find('N')>0 and b.find('E')>0 ) :
        #sql="insert into carPosInfo(last_pos) values('"
        sql="insert into carPosInfo(dateT,last_pos) values('"
        #sql=sql+b+"')";
        x=str((int(float(b[0:9]))+80000)%240000)+b[10:]
        sql=sql+dateT+"','"+x+"')";
        cur.execute(sql)

        #bb=b.split(",")
        #lat=bb[1].split(".")
        #lat=lat[0]+lat[1]
        #lat=int(lat)/10000000.

        #lon=bb[3].split(".")
        #lon=lon[0]+lon[1]
        #lon=int(lon)/10000000.
        #showCarPos.showCarPos(lon,lat)
        showCarPos.showCarPos()
    #print "OK"
    cur.close()
    db.commit()
    db.close()
    
    


if __name__ == "__main__":

    updatePos("fd,GPGGA,060724,3112.58036,N,12138.87877,E,1,03,3.0,")
    #updatePos("+GPSRD:$GPGGA,,,,,,0,00,,,M,,M,,0000*66 \
    #$GPGSA,A,1,,,,,,,,,,")
    #updatePos("fd,GPGGA,115559,,,,,0,00,,,M,,,,,,,,,,,,,,,,,M,")

