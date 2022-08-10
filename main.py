#!/usr/bin/env python3

import sys
sys.dont_write_bytecode = True

import easyocr
import cv2

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

def ocr(path, bounding_box):
    articles = []
    try:
        bbox = [int(c) for c in bounding_box.split(",")]
        r = receipt(path)
        r.init()
        r.preprocess(bbox)
        detections = r.extract()
        articles = r.process(detections)
    except FileNotFoundError:
        print("Error: Cannot find {}".format(path))
    except IndexError:
        print("Error: No path to input file and/or box coordinates given")
    return " ".join(articles)

if __name__ == "__main__":
    if len(sys.argv) == 3:
        path = sys.argv[1]
        bounding_box = sys.argv[2]
    else:
        path = "/src/ocr/misc/input/receipt_2.jpg"
        bounding_box = "77,304,1280,111"
    ocr(path, bounding_box)
