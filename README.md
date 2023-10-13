# ocr-receipt

This repository features an application which performs optical character recognition on receipts.

The input is an image recorded with a cell phone, and the output is a list of article/price pairs as in:

```cpp
struct article
{
    std::string name;
    float price;
};
std::vector<article> articles;
```

Character recognition is based on a two-step process: [Tesseract](https://github.com/tesseract-ocr/tesseract) is used to detect bounding boxes on receipts as shown below first, and then [EasyOCR](https://github.com/JaidedAI/EasyOCR) is used to extract the text within these bounding boxes.

Tesseract performs better in terms of coherent line-by-line detections, whereas EasyOCR is able to extract uncommon words with higher accuracy.

![Detection Results](https://media.githubusercontent.com/media/cfanatic/ocr-receipt/master/misc/output/receipt_2_overlay.jpg)

```text
$ ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg
Shop: unknown
Box[0]: x=1034, y=53, w=238, h=89, conf: 84, text: EUR
Box[1]: x=89, y=107, w=1253, h=105, conf: 71, text: 814373 Konfitüre Extra 1,29 A
Box[2]: x=120, y=173, w=1227, h=104, conf: 86, text: 44718 Feine Kleinkuchen 1,79 A
Box[3]: x=116, y=239, w=1237, h=106, conf: 76, text: 60819 Erdbeeren 500g 0,99 A
Box[4]: x=77, y=304, w=1280, h=111, conf: 92, text: 814989 Spargel grün 400g 2,99 A
Article=Konfitüre Extra, Price=1.29
Article=Feine Kleinkuchen, Price=1.79
Article=Erdbeeren 500g, Price=0.99
Article=Spargel Grün 400g, Price=2.99
```

```text
$ ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg --json
{
    "Shop": "unknown",
    "Articles": [
        {
            "Name": "Konfitüre Extra",
            "Price": "1.29"
        },
        {
            "Name": "Feine Kleinkuchen",
            "Price": "1.79"
        },
        {
            "Name": "Erdbeeren 500g",
            "Price": "0.99"
        },
        {
            "Name": "Spargel Grün 400g",
            "Price": "2.99"
        }
    ]
}
```

## Requirements

### General 

- OpenCV 4.5.4
- EasyOCR 1.5.0
- Boost 1.74.0

### Dependencies for Building with Docker (Optional)
The project itself is automatically built inside a Docker container based on following dependencies:

- Linux Based Distribution
- Tesseract 5.1.0
- Leptonica 1.82.0
- Docker 

### Dependencies when opting for Build without Docker (Optional)

- Curl
- Cmake 3.2.0
- CXX Compiler
- Git
- Libarchive
- Libpng
- Libjpeg
- Libgif
- Libtiff
- zlib
- libwebp (webp)
- libjp2k (openjpeg or JPEG 2000)

Leptonica and Tesseract are not needed in this case since they will be fetched
and compiled statically by the build script.

## Setup

### Without Docker
After cloning the repository go into the main project root and run the following :

#### Configure
```shell
mkdir build \
cd build 
```

```shell
cmake ..
```
this will trigger a docker-independent build only if Tesseract and Leptonica are not available
to force a static linkage of both libraries anyways, run the following instead : 

```shell
cmake -DDOCKERLESS_BUILD ..
```
The build script will fetch needed leptonica and tesseract libraries and link statically

#### Build 

```shell
cmake --build . --parallel ${nproc}
```
The binaries can then be found under the "build" folder

#### Run Unit Tests

```shell
ctest
```
#### set TESSDATA_PREFIX

The build script automatically fetches TESSDATA learningdata and provides information on where it is
after the configuration step . Please follow this for a proper execution of the software

### With Docker

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
      Start  1: configuration_test.shops
 1/13 Test  #1: configuration_test.shops .................   Passed    0.10 sec
      Start  2: configuration_test.filters
 2/13 Test  #2: configuration_test.filters ...............   Passed    0.16 sec
 ...
      Start 12: receipt_test.receipt_3
12/13 Test #12: receipt_test.receipt_3 ...................   Passed    2.36 sec
      Start 13: receipt_test.receipt_4
13/13 Test #13: receipt_test.receipt_4 ...................   Passed    2.41 sec

100% tests passed, 0 tests failed out of 13

Total Test time (real) =  23.33 sec
```

## Usage

Perform character recognition on [one of the input images](https://github.com/cfanatic/ocr-receipt/blob/master/misc/input/receipt_2.jpg) from the `misc/input/` folder:

```text
docker exec -it ocr-receipt ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg
```

You should be getting the results from above.

Add the `--json` switch if you want to serialize the results for further processing.
