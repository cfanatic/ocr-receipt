#!/usr/bin/env python3

import sys
sys.dont_write_bytecode = True

import easyocr
import cv2

class engine_easyocr():

    def __init__(self):
        self._reader = easyocr.Reader(["de", "en"], verbose=False)
        self._img_cv = None

    def init(self, path: str, box: str):
        self._img_cv = cv2.imread(path)
        offset = 20
        x, y, w, h = box[0]+offset, box[1]+offset, box[2]-offset, box[3]-offset
        self._img_cv = self._img_cv[y:y+h, x:x+w]

    def text(self) -> list:
        return self._reader.readtext(self._img_cv, detail=0)

    def conf(self) -> int:
        pass

    def overlay(self):
        pass

def ocr(path, bounding_box):
    article = []
    try:
        bounding_box = [int(c) for c in bounding_box.split(",")]
        e = engine_easyocr()
        e.init(path, bounding_box)
        article = e.text()
    except FileNotFoundError:
        print("Error: Cannot find {}".format(path))
    except IndexError:
        print("Error: No path to input file and/or box coordinates given")
    return " ".join(article)

if __name__ == "__main__":
    if len(sys.argv) == 3:
        path = sys.argv[1]
        bounding_box = sys.argv[2]
    else:
        path = "/src/ocr/misc/input/receipt_2.jpg"
        bounding_box = "77,304,1280,111"
    ocr(path, bounding_box)
