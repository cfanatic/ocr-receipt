#!/usr/bin/env python3

import sys
sys.dont_write_bytecode = True

import easyocr
import cv2

class engine_easyocr():

    def __init__(self):
        self._reader = easyocr.Reader(["de", "en"], verbose=False)
        self._img = None
        self._conf = 0.0

    def init(self, path: str, box: str):
        self._img = cv2.imread(path)
        x, y, w, h = box[0], box[1], box[2], box[3]
        self._img = self._img[y:y+h, x:x+w]

    def text(self) -> str:
        article = []
        confidence = 0
        detections = self._reader.readtext(self._img, detail=1)
        for d in detections:
            article.append(d[-2])
            confidence += d[-1]
        self._conf = confidence / len(detections)
        return " ".join(article)

    def conf(self) -> float:
        return self._conf

    def overlay(self):
        pass

def ocr(path, bounding_box) -> str:
    article = ""
    try:
        bounding_box = [int(c) for c in bounding_box.split(",")]
        e = engine_easyocr()
        e.init(path, bounding_box)
        article = e.text()
    except FileNotFoundError:
        print("Error: Cannot find {}".format(path))
    except (IndexError, TypeError, ValueError):
        print("Error: No path to input file and/or box coordinates given")
    return article

def ocr_short(path, bounding_box) -> str:
    reader = easyocr.Reader(["de", "en"], verbose=False)
    img = cv2.imread(path)
    box = [int(c) for c in bounding_box.split(",")]
    x, y, w, h = box[0], box[1], box[2], box[3]
    img = img[y:y+h, x:x+w]
    article = []
    detections = reader.readtext(img, detail=1)
    for d in detections:
        article.append(d[-2])
    return " ".join(article)

if __name__ == "__main__":
    if len(sys.argv) == 3:
        path = sys.argv[1]
        bounding_box = sys.argv[2]
    else:
        path = "/src/ocr/misc/input/receipt_2.jpg"
        bounding_box = "92,319,1265,96"
    result = ocr(path, bounding_box)
    print(result)
