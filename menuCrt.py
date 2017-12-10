#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
import urllib,cookielib;
import json;
import urllib2,requests;

my_appid = 'wxe44f7baed944b4e2'
my_secret = 'a78880d525f5d8afaa8b28ac544b77e8'

#设置要请求的头，让服务器不会以为你是机器人
header = {'Content-Type': 'application/json'}

url='https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid='+my_appid+'&secret='+my_secret

r=requests.get(url,headers=header);

a=r.json()
accessToken=str(a[a.keys()[0]])
print accessToken
#post方式时候要发送的数据
#values={'name':'admin','password':'123456'};
#values={"button":[#{"type":"click","name":"历史天气曲线","key":"V1001","sub_button":[]},
                  #{"type":"view","name":"历史天气曲线","url":"http://web.calmet.space/static/weather.png","sub_button":[]},
                  #{"type":"view","name":"历史天气曲线","url":"http://calmtree.cn/images/weather.png","sub_button":[]},
                  #{"type":"click","name":"历史天气曲线","key":"V1001",
                                  #"sub_button":
                                  #   [{"type":"view","name":"matpltPng","url":"http://calmtree.cn/images/weather.png"},
                                  #    {"type":"view","name":"plotly","url":"https://plot.ly/~calmet/0"}]},
                  #{"type":"click","name":"附近车位","key":"V1002","sub_button":[]},
                  #{"type":"view","name":"blog","url":"http://calmet.space/","sub_button":[]}]}
                  #{"type":"view","name":"blog","url":"http://calmtree.cn/","sub_button":[]}]}
#values={"button":[{"type":"click","name":"test","key":"V1001_TODAY_MUSIC","sub_button":[]}]}
values={"button":[
        {
          "type":"view",
          "name":"MyBlog",
          "url":"http://calmtree.cn/",
          "sub_button":[]
        },      
        {
          "type":"view",
          "name":"car",
          "url":"http://calmet.iask.in/sttic/showCarPos.html",
          "sub_button":[]
        },      
        {
          "type":"view",
          "name":"天气历史",
          "url":"http://calmet.iask.in/sttic/weather.png",
          "sub_button":[]
        }] 
      }
#对发送的数据进行编码
datas=urllib.urlencode(values);

url='https://api.weixin.qq.com/cgi-bin/menu/create?access_token='+accessToken
#发送一个http请求
request=urllib2.Request(url,headers=header,data=json.dumps(values,ensure_ascii=False));

#获得回送的数据
response=urllib2.urlopen(request);

print response.read();
