import urllib.request
import json
import time

ConfigFile = 'config.json'
LogFile = 'log.txt'
DataFile = 'data.txt'
log = open(LogFile, 'a')
data = open(DataFile, 'w')
config = open(ConfigFile, 'r', encoding='utf-8')
ConfigText = json.load(config)
url = ConfigText["url"] + ConfigText["APIKey"]
res = urllib.request.urlopen(url)
s = res.read().decode('utf-8')
s = json.loads(s)
if s["infocode"] != "10000":
    log.write("[ERR] Error when get data, errcode = " + s["infocode"] + '\n')
else:
    log.write("[LOG] Get weather data successfully at " + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + '\n')
    data.write(s["lives"][0]["reporttime"] + '\n')
    data.write(s["lives"][0]["province"] + s["lives"][0]["city"] + '\n')
    data.write(s["lives"][0]["weather"] + '\n')
    data.write(s["lives"][0]["temperature"] + '\n')
    data.write(s["lives"][0]["winddirection"] + '\n')
    data.write(s["lives"][0]["windpower"] + '\n')
    data.write(s["lives"][0]["humidity"] + '\n')
