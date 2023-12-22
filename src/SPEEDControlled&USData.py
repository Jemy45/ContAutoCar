import tkinter as tk
import requests 
import time
esp8266_ip = "192.168.4.1" 
ultrasonic_received=False

def sendSpeedRight():
    try:
        data_to_send= txtCMD1.get()
        url = f"http://{esp8266_ip}/rightSpeed"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD1.set("")
    except:
        print("Error has happened")
def sendSpeedLeft():
    try:
        data_to_send= txtCMD2.get()
        url = f"http://{esp8266_ip}/leftSpeed"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD2.set("")
    except:
        print("Error has happened")
def sendSpeedRot():
    try:
        data_to_send= txtCMD3.get()
        url = f"http://{esp8266_ip}/rotSpeed"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD3.set("")
    except:
        print("Error has happened")
def receivedData():   
  try:
    url = f"http://{esp8266_ip}/sendData"  
    response = requests.get(url)
    data_received = response.text
    return data_received
  except:
    print("Error has happened")
def startReceiving():
    global ultrasonic_received
    ultrasonic_received = True
    updateData()

def stopReceiving():
    global ultrasonic_received
    ultrasonic_received = False
 #--------------------CGPT--------------------       
def updateData():
    global ultrasonic_received
    if ultrasonic_received:
      ultrasonicData=receivedData()
      if ultrasonicData: 
        text_widget.insert(tk.END, ultrasonicData + "\n")  # Insert data at the end of the text
        text_widget.see(tk.END)  # Scroll to show the latest data
      root.after(1000, updateData)  # Update every second (adjust as needed)
#-------------------CGPT----------------------
root = tk.Tk()
mf = tk.Frame(root)
mf.pack()
txtCMD1 = tk.StringVar()
txtCMD2 = tk.StringVar()
txtCMD3 = tk.StringVar()
tk.Label(mf, text='RightSpeedMotor').grid(row=0, column=0)
tk.Label(mf, text='LeftSpeedMotor').grid(row=1, column=0)
tk.Label(mf, text='RotationSpeed').grid(row=2, column=0)
tk.Entry(mf, textvariable=txtCMD1).grid(row=0, column=1)
tk.Entry(mf, textvariable=txtCMD2).grid(row=1, column=1)
tk.Entry(mf, textvariable=txtCMD3).grid(row=2, column=1)
tk.Button(mf, text='Send', command=sendSpeedRight).grid(row=0, column=2)
tk.Button(mf, text='Send', command=sendSpeedLeft).grid(row=1, column=2)
tk.Button(mf, text='Send', command=sendSpeedRot).grid(row=2, column=2)
tk.Button(mf, text="Start Receiving US Data", command=startReceiving).grid(row=3, column=0)
tk.Button(mf, text="Stop Receiving US Data", command=stopReceiving).grid(row=3, column=1)

# Create a Text widget to display the data-------CGPT
text_widget = tk.Text(root, height=20, width=40)
text_widget.pack()
#------------------------------------------

root.title('Car Data')

root.mainloop()
