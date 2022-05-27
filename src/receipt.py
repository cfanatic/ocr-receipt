import easyocr

class receipt():

    def __init__(self, path: str):
        self.path = path
        self.reader = easyocr.Reader(["de", "en"])
        self.detections = None
        self.articles = None

    def init(self):
        pass

    def preprocess(self):
        pass

    def extract(self) -> list:
        self.detections = self.reader.readtext(self.path, detail=0)
        return self.detections

    def overlay(self):
        pass

    def process(self, detections: list) -> list:
        self.articles = detections
        return self.articles
