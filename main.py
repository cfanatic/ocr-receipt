#!/usr/bin/env python3

import easyocr
import cv2
import sys
sys.dont_write_bytecode = True

class receipt():

    def __init__(self, path: str):
        self._reader = easyocr.Reader(["de", "en"])
        self._path = path
        self._img_cv = None

    def init(self):
        self._img_cv = cv2.imread(self._path)
        pass

    def preprocess(self, box: list):
        offset = 20
        x, y, w, h = box[0]+offset, box[1]+offset, box[2]-offset, box[3]-offset
        self._img_cv = self._img_cv[y:y+h, x:x+w]

    def extract(self) -> list:
        return self._reader.readtext(self._img_cv, detail=0)

    def overlay(self):
        pass

    def process(self, detections: list) -> list:
        articles = detections
        return articles

def main(argc, argv):
    try:
        path_i = argv[1]
        box_i = [int(c) for c in argv[2].split(",")]
        r = receipt(path_i)
        r.init()
        r.preprocess(box_i)
        detections = r.extract()
        articles = r.process(detections)
    except FileNotFoundError:
        print("Error: Cannot find {}".format(path_i))
    except IndexError:
        print("Error: No path to input file and/or box coordinates given")
    else:
        print(articles)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
