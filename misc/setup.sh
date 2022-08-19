# Instructions for Docker to build the project
docker build -t ocr:latest .
docker run -t -d -e "TERM=xterm-256color" -v "$PWD":/src/ocr -e TESSDATA_PREFIX="/usr/local/share/tessdata" --name ocr-receipt ocr:latest bash
docker exec -it ocr-receipt ../misc/build.sh

# Instructions for Docker to attach to the container
docker exec -it ocr-receipt bash

# Instructions to perform OCR inside of the container
tesseract -l deu --tessdata-dir /usr/local/share/tessdata /src/ocr/misc/input/receipt_1.jpg stdout --oem 1 --psm 6

# Instructions to perform OCR outside of the container
docker exec -it ocr-receipt ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_1.jpg

# Instructions to pipe results into a file on the host machine
docker exec -t ocr-receipt ./ocr-receipt -c ../misc/config.json -i ../misc/input/receipt_2.jpg > results.txt
