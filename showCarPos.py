#!/usr/bin/python2.7
# -*- coding: utf-8 -*-


#from mpl_toolkits.basemap import Basemap
#import matplotlib.pyplot as plt
#from matplotlib.patches import Polygon
import MySQLdb
import time

import os,re,string
import sys
reload(sys)
sys.setdefaultencoding('utf8')
import MySQLdb


#def showCarPos(lon,lat): 
def showCarPos(): 
    dateT=time.strftime('%Y.%m.%d',time.localtime())
    db = MySQLdb.connect(host="localhost",db="carInfo",charset="utf8")
    cur = db.cursor()
    #sql="select last_pos from carPosInfo order by last_pos  and dateT="
    #sql="select last_pos from carPosInfo where dateT='"+dateT+"' order by last_pos limit 5"
    #sql="select dateT,last_pos from carPosInfo where dateT='"+dateT+"' order by last_pos"
    sql="select last_pos from carPosInfo where dateT='"+dateT+"' order by last_pos"
    
    rows=cur.execute(sql);
    a=cur.fetchall();
    latt=[]
    lonn=[]
    tmm=[]
    srcHtml=open('/var/www/sttics/showCarPos_mod.html','r')
    dstHtml=open("/var/www/sttics/showCarPos.html",'w+')
    for line in srcHtml.readlines():
        if line.find('carPosInfo.here')>0 :
            if rows > 0 :
                for row in range(0,rows):
                    print a[row][0]
                    bb=a[row][0].split(",")
                    #lat=bb[1].split(".")
                    lat=float(bb[1][0:2])+float(bb[1][2:])/60
                    lon=float(bb[3][0:3])+float(bb[3][3:])/60
                    tm=bb[0][0:4]
                    '''
                    print lat,lon
                    
                    lat=(int(lat)/10000000.)

                    lon=bb[3].split(".")
                    print bb[3][3:]
                    print lon[0]
                    print lon[1]
                    lon=lon[0]+lon[1]
                    lon=(int(lon)/10000000.)
                    #line='carPoslat['+str(row)+']="'+str(lat)+'"\n'
                    #dstHtml.write(line)
                    #line='carPoslon['+str(row)+']="'+str(lon)+'"\n'
                    #dstHtml.write(line)
                    '''
                    latt.append(lat);
                    lonn.append(lon);
                    tmm.append(tm);
            #    line="var len="+str(rows)+";\n"
            #    dstHtml.write(line)
            #else :
            #    line="\n"
            #    dstHtml.write(line)
        #elif line.find('carPosInfo.here')>0 :
            #if rows > 0 :
                for row in range(0,rows):
                    #line="        new BMap.Point("+str(lonn[row])+","+str(latt[row])+")"
                    line="    posLon["+str(row)+"]="+str(lonn[row])+";\n"+"    posLat["+str(row)+"]="+str(latt[row])+";\n"+"    time["+str(row)+"]="+str(tmm[row])+";\n"
                    #if row!=(rows-1):
                    #    line=line+","
                    #line=line+"\n"
                    dstHtml.write(line)
            else :
                line="        posLon[0]=121.657563;\n        posLat[0]=31.212064;\n"
                dstHtml.write(line)
        else :
            dstHtml.write(line)
            
    srcHtml.close()
    dstHtml.close()
    
    
       
    cur.close()
    db.commit()
    db.close()
    
if __name__ == "__main__":

    showCarPos()
    #app.run()