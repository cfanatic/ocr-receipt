# Instructions for Docker to run a container
docker build -t ocr:latest .
docker run -it -e "TERM=xterm-256color" -v "$PWD":/src/ocr --name ocr-receipt ocr:latest bash
