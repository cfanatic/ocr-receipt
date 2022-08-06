#!/usr/bin/env python3

import sys
sys.dont_write_bytecode = True

from src.receipt import receipt

def main(argc, argv):
    try:
        path_i = argv[1]
        r = receipt(path_i)
        r.init()
        r.preprocess()
        detections = r.extract()
        articles = r.process(detections)
    except FileNotFoundError:
        print("Error: Cannot find {}".format(path_i))
    except IndexError:
        print("Error: No path to input file given")
    else:
        print(articles)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
