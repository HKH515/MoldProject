from datetime import datetime
import paho.mqtt.client as mqttClient
import time
import requests 
def on_connect(client, userdata, flags, rc):
 
    if rc == 0:
 
        print("Connected to broker")
 
        global Connected                #Use global variable
        Connected = True                #Signal connection 
 
    else:
 
        print("Connection failed", rc)
 
def on_message(client, userdata, message):
	try:
		print("payload", message.payload)
		message = message.payload
		now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
		print("datetime",now)
		#timestamp = datetime.timestamp(now)
		#print(timestamp)	
		url = 'http://127.0.0.1:5000/api/submit'
		myobj = {"humidity_value":message, "device_id": '1' ,"timestamp": now}
		x = requests.post(url, data = myobj)
#	print("after request")
#	print(x.test)
#	print("client", client)
	#print ("Message received: "  + message.payload)
	except Exception as e:
		print(e)
 
Connected = False   #global variable for the state of the connection
 
broker_address= "127.0.0.1"  #Broker address
port = 1883                         #Broker port
user = "alex"                    #Connection username
password = "testing"            #Connection password
 
client = mqttClient.Client("Python")               #create new instance
client.username_pw_set(user, password=password)    #set username and password
client.on_connect= on_connect                      #attach function to callback
client.on_message= on_message                      #attach function to callback
 
client.connect(broker_address, port=port)          #connect to broker
 
client.loop_start()        #start the loop
 
while Connected != True:    #Wait for connection
	time.sleep(0.1)
#	print("while not connected")

client.subscribe("humidity_S1")
print("subscribed to humidity_S1")
 
try:
	while True:
		time.sleep(1)
 
except KeyboardInterrupt:
    print ("exiting")
    client.disconnect()
    client.loop_stop()
