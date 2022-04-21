# Instructions for Docker to run a container
docker build -t ocr:latest .
docker run -it -e "TERM=xterm-256color" -v "$PWD":/src/ocr --name ocr-receipt ocr:latest bash

# Instructions for Tesseract to perform OCR
tesseract -l deu --tessdata-dir /usr/local/share/tessdata /src/ocr/misc/input/receipt_1.jpg stdout --oem 1 --psm 6
