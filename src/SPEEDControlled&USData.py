import tkinter as tk
import requests 
import time
esp8266_ip = "192.168.4.1"
ultrasonic_received=False

def sendSpeedRightForward():
    try:
        data_to_send= txtCMD1.get()
        url = f"http://{esp8266_ip}/rightSpeedForward"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD1.set("")
    except:
        print("Error has happened")
def sendSpeedRightBackward():
    try:
        data_to_send= txtCMD2.get()
        url = f"http://{esp8266_ip}/rightSpeedBackward"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD2.set("")
    except:
        print("Error has happened")

def sendSpeedLeftForward():
    try:
        data_to_send= txtCMD4.get()
        url = f"http://{esp8266_ip}/leftSpeedForward"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD4.set("")
    except:
        print("Error has happened")
def sendSpeedLeftBackward():
    try:
        data_to_send= txtCMD5.get()
        url = f"http://{esp8266_ip}/leftSpeedBackward"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD5.set("")
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
def sendServoAngle():
    try:
        data_to_send= txtCMD6.get()
        url = f"http://{esp8266_ip}/servoAngle"
        payload = {'data': data_to_send}
        response = requests.post(url,data=payload)
        txtCMD6.set("")
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
       
def updateData():
    global ultrasonic_received
    if ultrasonic_received:
      ultrasonicData=receivedData()
      if ultrasonicData: 
        text_widget.insert(tk.END, ultrasonicData + "\n")  # Insert data at the end of the text
        text_widget.see(tk.END)  # Scroll to show the latest data
      root.after(1000, updateData)  # Update every second (adjust as needed)

root = tk.Tk()
mf = tk.Frame(root)
mf.pack()
txtCMD1 = tk.StringVar()
txtCMD2 = tk.StringVar()
txtCMD3 = tk.StringVar()
txtCMD4 = tk.StringVar()
txtCMD5 = tk.StringVar()
txtCMD6 = tk.StringVar()
tk.Label(mf, text='RightSpeedMotorForward').grid(row=0, column=0)
tk.Label(mf, text='RightSpeedMotorBackward').grid(row=1, column=0)
tk.Label(mf, text='RotationSpeed').grid(row=2, column=0)
tk.Label(mf, text='LeftSpeedMotorForward').grid(row=3, column=0)
tk.Label(mf, text='LeftSpeedMotorBackward').grid(row=4, column=0)
tk.Label(mf, text='ServoAngle').grid(row=5, column=0)
tk.Entry(mf, textvariable=txtCMD1).grid(row=0, column=1)
tk.Entry(mf, textvariable=txtCMD2).grid(row=1, column=1)
tk.Entry(mf, textvariable=txtCMD3).grid(row=2, column=1)
tk.Entry(mf, textvariable=txtCMD4).grid(row=3, column=1)
tk.Entry(mf, textvariable=txtCMD5).grid(row=4, column=1)
tk.Entry(mf, textvariable=txtCMD6).grid(row=5, column=1)
tk.Button(mf, text='Send', command=sendSpeedRightForward).grid(row=0, column=2)
tk.Button(mf, text='Send', command=sendSpeedRightBackward).grid(row=1, column=2)
tk.Button(mf, text='Send', command=sendSpeedRot).grid(row=2, column=2)
tk.Button(mf, text='Send', command=sendSpeedLeftForward).grid(row=3, column=2)
tk.Button(mf, text='Send', command=sendSpeedLeftBackward).grid(row=4, column=2)
tk.Button(mf, text='Send', command=sendServoAngle).grid(row=5, column=2)
tk.Button(mf, text="Start Receiving US Data", command=startReceiving).grid(row=6, column=0)
tk.Button(mf, text="Stop Receiving US Data", command=stopReceiving).grid(row=6, column=1)

# Create a Text widget to display the data-------CGPT
text_widget = tk.Text(root, height=20, width=40)
text_widget.pack()
#------------------------------------------

root.title('Car Data')

root.mainloop()
