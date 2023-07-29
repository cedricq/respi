from tkinter import *
from tkinter import ttk
import serial
import serial.tools.list_ports
import datetime
import threading
import time

# for printing debugging messages in console
dbg = 0

gRoot = Tk()
gRoot.title("Serial Console")
sty = ttk.Style()
sty.theme_use("alt")
gRoot.columnconfigure(0, weight=1)
gRoot.rowconfigure(0, weight=1)
gFrame = ttk.LabelFrame(gRoot, text="Connection Setting", padding=10)
gFrame.grid(column=1, row=1, sticky=(W, E))

# Frame for COM messages
gFrame21 = ttk.Frame(gRoot, padding=10)
gFrame21.grid(column=2, row=1, sticky=(W))

for x in range(10):
    gFrame.columnconfigure(x, weight=x)
    gFrame.rowconfigure(x, weight=x)

label1 = ttk.Label(gFrame, text="Serial Console")
label1.grid(column=2, row=0)
gFrame.rowconfigure(0, weight=2)

sty.configure("label2.TLabel", borderwidth=4, relief="ridge", foreground="red", ipadx=10)
label2 = ttk.Label(gFrame, sty="label2.TLabel", text="Select Com Port")
label2.grid(column=1, row=1, sticky=(N, E, W, S))

"""
Com Port List
"""
# Start
ports = serial.tools.list_ports.comports()
com_port_list = [com[0] for com in ports]
com_port_list.insert(0, "Select an Option")
if dbg == 1:
    print(com_port_list)
# END
com_value_inside = StringVar()
baud_value_inside = StringVar()
baud_menu = ttk.OptionMenu(gFrame, baud_value_inside, "select baud rate", "9600",
                           '19200', '28800', '38400', '57600', '76800',
                           '115200', '128000', '153600', '230400', '256000', '460800', '921600')
baud_menu.grid(column=3, row=1, sticky=(E))

com_menu = ttk.OptionMenu(gFrame, com_value_inside, *com_port_list)
com_menu.grid(column=2, row=1, sticky=(E))
def com_port_list_update():
    global ports
    global com_port_list
    ports = serial.tools.list_ports.comports()
    com_port_list = [com[0] for com in ports]
    com_port_list.insert(0, "Select an Option")
    if dbg == 1:
        print(com_port_list)

    # Frame for the COM LIST
    gRoot_com_list = Toplevel(gRoot)
    x = gRoot.winfo_x()
    y = gRoot.winfo_y()
    gRoot_com_list.geometry("+%d+%d" % (x + 200, y + 200))
    gFrame01 = ttk.Frame(gRoot_com_list, padding=10)
    gFrame01.grid(column=0, row=1, sticky=(W))
    # Create a horizontal scrollbar
    scrollbar = ttk.Scrollbar(gFrame01, orient='horizontal')
    scrollbar.grid(column=1, row=2, sticky=W + E)

    Lb1 = Listbox(gFrame01, xscrollcommand=1, width=50, font=('Helvetica 8 bold'))
    counter = 0;
    for x in ports:
        Lb1.insert(counter, str(x))
    # print (counter)
    counter += 1
    Lb1.grid(column=1, row=1, sticky=W + E)
    Lb1.config(xscrollcommand=scrollbar.set)

    # Configure the scrollbar
    scrollbar.config(command=Lb1.xview)


def serial_print():
    global serFlag
    global ser
    global counter1
    global MotorTarget
    global ToSend
    x = ""
    # print("Task 1 assigned to thread: {}".format(threading.current_thread().name))
    # print("ID of process running task 1: {}".format(os.getpid()))
    if (serFlag):
        if (ser.in_waiting > 0):
            try:
                x = ser.read(ser.in_waiting)
                #y = str(counter1) + ": " + str(datetime.datetime.now()) + " -> " + str(x.decode())
                y = str(x.decode())
                ml = y.split('\n')
                for m in ml:
                    Lb2.insert(counter1, str(datetime.datetime.now()) + ' ' + str(m))
                    Lb2.see("end")
                    # print (counter1)
                    counter1 += 1
                # gFrame.after(100,serial_print)
                ser.flushInput()

            except:
                pass

        gFrame.after(100, serial_print)


ser = serial.Serial()
serFlag = 0


def serial_connect(com_port, baud_rate):
    global ser
    ser.baudrate = baud_rate
    ser.port = com_port
    ser.timeout = 1
    ser.write_timeout = 1
    ser._xonxoff = 0
    ser.bytesize = serial.EIGHTBITS
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.open()
    global serFlag
    serFlag = 1

    t1 = threading.Thread(target=serial_print, args=(), daemon=1)
    t1.start()
    # t1.join()
    """
    P1 = multiprocessing.Process(target = serial_print, args=())
    P1.start()
    P1.join()
    """
    # serial_print()


counter1 = 0;


def serial_close():
    global ser
    global serFlag
    serFlag = 0
    ser.close()


def submit_value():
    if dbg == 1:
        print("selected option: {}".format(com_value_inside.get()))
        print(" Baud Rate {}".format(baud_value_inside.get()))
    serial_connect(com_value_inside.get(), baud_value_inside.get())

Lb2 = Listbox(gFrame21, width=100, xscrollcommand=1)
Lb2.grid(column=1, row=1, sticky=W + E)
Sb2 = ttk.Scrollbar(gFrame21, orient='vertical')
Sb2.config(command=Lb2.yview)
Sb2.grid(column=2, row=1, sticky=N + S)
Sb2v = ttk.Scrollbar(gFrame21, orient='horizontal')
Sb2v.grid(column=1, row=2, sticky=W + E)
Sb2v.config(command=Lb2.xview)
Lb2.configure(xscrollcommand=Sb2v.set, yscrollcommand=Sb2.set)

w1 = Scale(gFrame, from_=0, to=1000, orient=HORIZONTAL, resolution=200, tickinterval=10, length=120, label='Motor Target')
w1.grid(column=1, row=3, sticky=S)
w1.set(0)

w2 = Scale(gFrame, from_=0, to=1000, orient=HORIZONTAL, resolution=200, tickinterval=10, length=120, label='Peep Motor Target')
w2.grid(column=2, row=3, sticky=S)
w2.set(0)

w3 = Scale(gFrame, from_=0, to=1, orient=HORIZONTAL, resolution=1, length=120, label='I/E Valve')
w3.grid(column=3, row=3, sticky=S)
w3.set(0)

MotorTarget = 0
PeepMotorTarget = 0
IEValveTarget = 0
ToSend = False

def write_value(data_id, value):
    global ser

    str_out = str(data_id) + ':' + str(value) + '\r\n'
    # ser.write(str_out.encode("utf-8"))
    print(str_out.encode())
    ser.write(str_out.encode())
    ser.flushOutput()
    time.sleep(0.1)

def send_value():

    global MotorTarget, PeepMotorTarget, IEValveTarget

    MotorTarget = int(w1.get())
    write_value(6, MotorTarget)

    PeepMotorTarget = int(w2.get())
    write_value(7, PeepMotorTarget)

    IEValveTarget = int(w3.get())
    write_value(8, IEValveTarget)


sendBtn = ttk.Button(gFrame, text="send", command=send_value)
sendBtn.grid(column=4, row=3, sticky=(E))



def clear_listbox():
    Lb2.delete(0, END)


subBtn = ttk.Button(gFrame, text="submit", command=submit_value)
subBtn.grid(column=4, row=1, sticky=(E))

RefreshBtn = ttk.Button(gFrame, text="Get List", command=com_port_list_update)
RefreshBtn.grid(column=2, row=2, sticky=(E))

closeBtn = ttk.Button(gFrame, text="Disconnect", command=serial_close)
closeBtn.grid(column=4, row=2, sticky=(E))

clearBtn = ttk.Button(gFrame, text="Clear Messages", command=clear_listbox)
clearBtn.grid(column=3, row=2, sticky=(E))

"""
#Add a Listbox Widget
listbox = Listbox(win, width= 350, font= ('Helvetica 15 bold'))
listbox.pack(side= LEFT, fill= BOTH)

#Add values to the Listbox
for values in range(1,101):
   listbox.insert(END, values)
"""

gRoot.mainloop()
