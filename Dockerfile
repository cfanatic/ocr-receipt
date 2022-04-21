FROM ubuntu:jammy as ubuntu-base

ENV LD_LIBRARY_PATH=/usr/local/lib/

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get -y install \
    git \
    nano \
    wget \
    cmake \
    make \
    g++ \
    ca-certificates \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    libicu-dev \
    libpango1.0-dev

FROM ubuntu-base

RUN git clone --depth 1 --branch 1.82.0 https://github.com/DanBloomberg/leptonica.git /src/leptonica
RUN mkdir /src/leptonica/build
WORKDIR /src/leptonica/build
RUN cmake ..
RUN make -j8
RUN make install

RUN git clone --depth 1 --branch 5.1.0 https://github.com/tesseract-ocr/tesseract.git /src/tesseract
RUN mkdir /src/tesseract/build
WORKDIR /src/tesseract/build
RUN cmake ..
RUN make -j8
RUN make install
RUN ldconfig
RUN wget https://github.com/tesseract-ocr/tessdata_fast/raw/main/eng.traineddata \
    -P /usr/local/share/tessdata
RUN wget https://github.com/tesseract-ocr/tessdata_fast/raw/main/deu.traineddata \
    -P /usr/local/share/tessdata

RUN echo "alias c='clear'" >> /root/.bashrc
RUN echo "alias ll='ls -l'" >> /root/.bashrc
RUN echo "alias lla='ls -la'" >> /root/.bashrc
RUN echo "alias ..='cd ..'" >> /root/.bashrc
RUN . /root/.bashrc

WORKDIR /src/ocr
CMD [ "/bin/bash" ]
