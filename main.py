#!/usr/bin/env python3

import sys
sys.dont_write_bytecode = True

from src.receipt import receipt

def main(argc, argv):
    path_i = argv[1]
    r = receipt(path_i)
    r.init()
    r.preprocess()
    detections = r.extract()
    articles = r.process(detections)
    print(articles)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
