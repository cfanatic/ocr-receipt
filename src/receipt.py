import easyocr

class receipt():

    def __init__(self, path: str):
        self._path = path
        self._reader = easyocr.Reader(["de", "en"])
        self._detections = None
        self._articles = None

    def init(self):
        pass

    def preprocess(self):
        pass

    def extract(self) -> list:
        self._detections = self._reader.readtext(self._path, detail=0)
        return self._detections

    def overlay(self):
        pass

    def process(self, detections: list) -> list:
        self._articles = detections
        return self._articles
