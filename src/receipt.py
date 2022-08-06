import easyocr
import cv2

class receipt():

    def __init__(self, path: str):
        self._reader = easyocr.Reader(["de", "en"])
        self._path = path
        self._img_cv = None
        self._detections = None
        self._articles = None

    def init(self):
        self._img_cv = cv2.imread(self._path)
        pass

    def preprocess(self):
        pass

    def extract(self) -> list:
        self._detections = self._reader.readtext(self._img_cv, detail=0)
        return self._detections

    def overlay(self):
        pass

    def process(self, detections: list) -> list:
        self._articles = detections
        return self._articles
