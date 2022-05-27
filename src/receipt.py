import easyocr

class receipt():

    def __init__(self, path):
        self.path = path
        self.reader = easyocr.Reader(["de", "en"])
        self.detections = None
        self.articles = None

    def init(self):
        pass

    def preprocess(self):
        pass

    def extract(self):
        self.detections = self.reader.readtext(self.path, detail=0)
        return self.detections

    def overlay(self):
        pass

    def process(self, detections):
        self.articles = detections
        return self.articles
