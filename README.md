# ocr-receipt

This repository features an application which performs optical character recognition on receipts.

The input is an image recorded with a cell phone, and the output is a list of article/price pairs as in:

```cpp
struct article
{
    std::string name;
    float price;
};
std::vector<article> results;
```

![Detection Results](https://media.githubusercontent.com/media/cfanatic/ocr-receipt/master/misc/output/receipt_2_overlay.jpg)

```text
$ ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg

Shop: unknown
Box[0]: x=1034, y=53, w=238, h=89, conf: 84, text: EUR
Box[1]: x=89, y=107, w=1253, h=105, conf: 71, text: 814373 Konfitüre Extra 1,29 A
Box[2]: x=120, y=173, w=1227, h=104, conf: 86, text: 44718 Feine Kleinkuchen 1,79 A
Box[3]: x=116, y=239, w=1237, h=106, conf: 76, text: 60819 Erdbeeren 5009 0,99 A
Box[4]: x=77, y=304, w=1280, h=111, conf: 92, text: 814989 Spargel grün 4009 2,99 A
Article=Konfitüre Extra, Price=1.29
Article=Feine Kleinkuchen, Price=1.79
Article=Erdbeeren 5009, Price=0.99
Article=Spargel Grün 4009, Price=2.99
```

## Requirements

The project itself is automatically built inside a Docker container based on following dependencies:

- Ubuntu 22.04
- Tesseract 5.1.0
- Leptonica 1.82.0
- Boost 1.74.0
- OpenCV 4.5.4
- Python 3.10.4

## Setup

Build the image:

```text
docker build -t ocr:latest .
```

Run a detached container:

```text
docker run -t -d -e "TERM=xterm-256color" -v "$PWD":/src/ocr \
-e TESSDATA_PREFIX="/usr/local/share/tessdata" --name ocr-receipt ocr:latest bash
```

Build this project:

```text
docker exec -it ocr-receipt ../misc/build.sh
```

The build script executes unit tests in the end, so hopefully you are seeing:

```text
Test project /src/ocr/build
    Start 1: receipt_test.receipt_1
1/3 Test #1: receipt_test.receipt_1 ...........   Passed    1.85 sec
    Start 2: receipt_test.receipt_2
2/3 Test #2: receipt_test.receipt_2 ...........   Passed    1.57 sec
    Start 3: receipt_test.receipt_3
3/3 Test #3: receipt_test.receipt_3 ...........   Passed    1.80 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   5.25 sec
```

## Usage

Perform character recognition on [one of the input images](https://github.com/cfanatic/ocr-receipt/blob/master/misc/input/receipt_2.jpg) from the `misc/input/` folder:

```text
docker exec -it ocr-receipt ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg
```

You should be getting the results shown in the top.
