import requests

x = requests.get('http://192.168.101.187')
print(x.text)