import tkinter
from tkinter import *
import subprocess
import os
from tkinter import filedialog

from tkinter import *
from tkinter.filedialog import askopenfilename
global path_

def center_window(w, h, window):
    # 获取屏幕 宽、高
    ws = window.winfo_screenwidth()
    hs = window.winfo_screenheight()
    # 计算 x, y 位置
    x = (ws / 2) - (w / 2)
    y = (hs / 2) - (h / 2) - 30
    window.geometry('%dx%d+%d+%d' % (w, h, x, y))

window = Tk()
window.title('Letter counter')
center_window(800, 500, window)
frame1 = Frame(window, bg='blue')
frame1.pack()

path = StringVar()

frame10 = Frame(frame1)
frame10.pack()
l0 = Label(frame10, text='文件路径', font=('Ariai',12), width=10)
l0.pack(side='left')
e0 = Entry(frame10, textvariable = path, font=('Ariai',11), width=50)
e0.pack(side='right')

frame11 = Frame(frame1)
frame11.pack()

frame12 = Frame(frame1)
frame12.pack()

l1 = Label(frame12, text='文件详情', font=('Ariai',12), width=10)
l1.pack()
t1 = Text(frame12, width=80, height=30)
t1.pack()

def selectPath():
    global path_
    path_ = askopenfilename()
    path.set(path_)
    t1.delete(1.0, END)

def analyze():
    global path_
    fj = open('input.txt', 'w')
    fj.write(path_)
    fj.close()
    out_bytes = subprocess.check_output(['2buf_4thread.exe', '-a'])
    out_text = out_bytes.decode(encoding="utf-8", errors="ignore")
    t1.insert(END, out_text)
    if os.path.exists('input.txt'):
        os.remove('input.txt')

#****    button
button0 = Button(frame11, text = "文件选择", width=10, font=('Ariai',12), command = selectPath)
button0.pack(side='left')
button = Button(frame11, text='一键解析', width=10, font=('Ariai',12), command=analyze)
button.pack(side='right')

window.mainloop()
