# -*- coding: utf-8 -*-
import os
import sys
from shutil import copyfile

import DirUnpackUi
from PIL import Image
from xml.etree import ElementTree

from PyQt5.QtCore import QThread
from PyQt5.QtWidgets import *

class ThreadRun(QThread):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.path = ''

    def setPath(self, path = ''):
        self.path = path

    def tree2Dict(self,tree):
        d = {}
        for index, item in enumerate(tree):
            if item.tag == 'key':
                if tree[index + 1].tag == 'string':
                    d[item.text] = tree[index + 1].text
                elif tree[index + 1].tag == 'true':
                    d[item.text] = True
                elif tree[index + 1].tag == 'false':
                    d[item.text] = False
                elif tree[index + 1].tag == 'integer':
                    d[item.text] = int(tree[index + 1].text)
                elif tree[index + 1].tag == 'real':
                    d[item.text] = float(tree[index + 1].text)
                elif tree[index + 1].tag == 'dict':
                    d[item.text] = self.tree2Dict(tree[index + 1])
        return d

    def fetchPlistData(self, filename):
        fdata = open(filename, encoding = 'utf8').read()
        root = ElementTree.fromstring(fdata)
        plist_dict = self.tree2Dict(root[0])
        to_list = lambda x: x.replace('{', '').replace('}', '').split(',')
        if 'frames' not in plist_dict.keys():
            return None
        frames = plist_dict['frames'].items()
        for k, v in frames:
            frame = v
            if(plist_dict["metadata"]["format"] == 3):
                frame['frame'] = frame['textureRect']
                frame['rotated'] = frame['textureRotated']
                frame['sourceSize'] = frame['spriteSourceSize']
                frame['offset'] = frame['spriteOffset']
            elif (plist_dict["metadata"]["format"] == 0):
                frame['frame'] = "{{{0},{1},{2},{3}}}".format(frame["x"], frame["y"], frame['width'], frame["height"])
                frame['sourceSize'] = "{{{0}, {1}}}".format(frame["originalWidth"], frame["originalHeight"])
                frame['offset'] = "{{{0}, {1}}}".format(frame['offsetX'], frame['offsetY'])
                frame['rotated'] = False

            rectlist = to_list(frame['frame'])
            width = int(float(rectlist[3] if frame['rotated'] else rectlist[2]))
            height = int(float(rectlist[2] if frame['rotated'] else rectlist[3]))
            frame['box'] = (
                int(float(rectlist[0])),
                int(float(rectlist[1])),
                int(float(rectlist[0])) + width,
                int(float(rectlist[1])) + height
            )
            real_rectlist = to_list(frame['sourceSize'])
            real_width = int(float(real_rectlist[1] if frame['rotated'] else real_rectlist[0]))
            real_height = int(float(real_rectlist[0] if frame['rotated'] else real_rectlist[1]))
            real_sizelist = [real_width, real_height]
            frame['real_sizelist'] = real_sizelist
            offsetlist = to_list(frame['offset'])
            offset_x = int(float(offsetlist[1] if frame['rotated'] else offsetlist[0]))
            offset_y = int(float(offsetlist[0] if frame['rotated'] else offsetlist[1]))

            if frame['rotated']:
                frame['result_box'] = (
                    int(float((real_sizelist[0] - width) / 2 + offset_x)),
                    int(float((real_sizelist[1] - height) / 2 + offset_y)),
                    int(float((real_sizelist[0] + width) / 2 + offset_x)),
                    int(float((real_sizelist[1] + height) / 2 + offset_y))
                )
            else:
                frame['result_box'] = (
                    int(float((real_sizelist[0] - width) / 2 + offset_x)),
                    int(float((real_sizelist[1] - height) / 2 - offset_y)),
                    int(float((real_sizelist[0] + width) / 2 + offset_x)),
                    int(float((real_sizelist[1] + height) / 2 - offset_y))
                )
        return frames

    def unpackPng(self, filename):
        ui.outputLog("unpack:"+ filename +".png\r\n")
        big_image = Image.open(filename + ".png")
        frames = self.fetchPlistData(filename + ".plist")
        if frames == None:
            return
        for k, v in frames:
            frame = v
            box = frame['box']
            rect_on_big = big_image.crop(box)
            real_sizelist = frame['real_sizelist']
            result_image = Image.new('RGBA', real_sizelist, (0, 0, 0, 0))
            result_box_1 = frame['result_box']

            result_box = (result_box_1[0], result_box_1[1], result_box_1[0] + rect_on_big.width, result_box_1[1] + rect_on_big.height)
            result_image.paste(rect_on_big, result_box, mask=0)
            if frame['rotated']:
                result_image = result_image.transpose(Image.ROTATE_90)
            # 输出到新目录
            if out_new_path != '':
                if not os.path.isdir(out_new_path):
                    os.mkdir(out_new_path)

                xidx = filename.rfind('/')
                xname = filename[xidx:]
                filename = out_new_path + xname

            if not os.path.isdir(filename):
                os.mkdir(filename)
            
            kidx = k.rfind('/')
            if kidx != -1:
                k = k[kidx+1:]
            outfile = (filename + '/' + k).replace('gift_', '')
            if not outfile.endswith('.png'):
                ui.outputLog("warn: expect png: " + outfile + "\r\n")
                outfile += '.png'

            while os.path.exists(outfile):
                outfile = outfile + ".png"

            result_image.save(outfile)

    def copyFile(self, filename):
        if out_new_path == '':
            return
        cidx = filename.rfind('/')
        foldername = filename[0:cidx]
        tag_fname = filename[cidx:]
        ccidx = foldername.rfind('/')
        tag_intername = foldername[ccidx:]
        if not os.path.isdir(out_new_path):
            os.mkdir(out_new_path)
        copypath = out_new_path + tag_intername
        if not os.path.isdir(copypath):
            os.mkdir(copypath)
        copypath = copypath + tag_fname
        strcopyf = 'copy {0} {1}'.format(filename, copypath)
        ui.outputLog(strcopyf + "\r\n")
        # subprocess.call(strcopyf, shell=True)
        copyfile(filename, copypath)

    def getSource(self, filename):
        dotidx = filename.rfind('.')
        if dotidx != -1:
            fname = filename[0:dotidx]
            ext = filename[dotidx:]
            if ext == ".png":
                pfile = fname + ".plist"
                if os.path.exists(pfile):
                    self.unpackPng(fname)
                else:
                    self.copyFile(filename)
            elif ext == ".jpg":
                self.copyFile(filename)
            elif ext == ".jpeg":
                self.copyFile(filename)


    def walkDir(self, path):
        files = os.listdir(path)
        for fi in files:
            # fi_d = os.path.join(path,fi)            
            fi_d = path + "/" + fi
            if os.path.isdir(fi_d):
                self.walkDir(fi_d)                  
            else:
                self.getSource(fi_d)

    def run(self):
        if self.path == '':
            ui.outputLog("error:请选择目录！\r\n")
            return
        ui.outputLog("start!\r\n")
        self.walkDir(self.path)
        ui.outputLog("finish!\r\n")
    
def startRun(path):
    start_run_thread.setPath(path)
    start_run_thread.start()

if __name__ == '__main__':
    out_new_path = 'd:/UI_doc/out_xigua'
    app = QApplication(sys.argv)
    mainwind = QMainWindow()
    ui = DirUnpackUi.Ui_mainui_dir()
    ui.setupUi(mainwind)
    ui.btn_unpack.clicked.connect(lambda:startRun(ui.textEdit.toPlainText()))
    mainwind.show()
    start_run_thread = ThreadRun()
    sys.exit(app.exec_())