from tkinter import *
import subprocess
import os

def center_window(w, h, window):
    # 获取屏幕 宽、高
    ws = window.winfo_screenwidth()
    hs = window.winfo_screenheight()
    # 计算 x, y 位置
    x = (ws / 2) - (w / 2)
    y = (hs / 2) - (h / 2) - 30
    window.geometry('%dx%d+%d+%d' % (w, h, x, y))

window = Tk()
window.title('Letter generator')
center_window(800, 500, window)
frame1 = Frame(window, bg='blue')
frame1.pack()

frame10 = Frame(frame1)
frame10.pack()
l0 = Label(frame10, text='文件名', width=10)
l0.pack(side='left')
e0 = Entry(frame10, width=40)
e0.insert(END, '请输入文件名')
e0.pack(side='right')

frame11 = Frame(frame1)
frame11.pack()

frame12 = Frame(frame1)
frame12.pack()

l1 = Label(frame12, text='文件详情', width=10)
l1.pack()
t1 = Text(frame12, width=80, height=30)
t1.pack()


def generate():
    t1.delete(1.0, END)
    fj = open('input.txt', 'w')
    fj.write(e0.get())
    fj.close()
    out_bytes = subprocess.check_output(['mult_generate.exe', '-a'])
    out_text = out_bytes.decode(encoding="utf-8", errors="ignore")
    t1.insert(END, out_text)
    if os.path.exists('input.txt'):
        os.remove('input.txt')

#****    button
button = Button(frame11, text='一键生成', width=10, font=('Ariai',12), command=generate)
button.pack(side='right')

window.mainloop()
