# Instructions for Docker to run a container
docker build -t ocr:latest .
docker run -it -e "TERM=xterm-256color" -v "$PWD":/src/ocr -e TESSDATA_PREFIX="/usr/local/share/tessdata" --name ocr-receipt ocr:latest bash

# Instructions to perform OCR inside of a container
tesseract -l deu --tessdata-dir /usr/local/share/tessdata /src/ocr/misc/input/receipt_1.jpg stdout --oem 1 --psm 6

# Instructions to perform OCR outside of a container
docker exec -it ocr-receipt ./ocr-receipt /src/ocr/misc/input/receipt_1.jpg
