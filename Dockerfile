FROM ubuntu:jammy as ubuntu-base

ENV LD_LIBRARY_PATH=/usr/local/lib/
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get -y install \
    git \
    nano \
    wget \
    automake \
    make \
    ca-certificates \
    g++ \
    libtool \
    libleptonica-dev \
    pkg-config

FROM ubuntu-base

RUN git clone --depth 1  https://github.com/tesseract-ocr/tesseract.git /src/tesseract
WORKDIR /src/tesseract
RUN ./autogen.sh
RUN ./configure
RUN make -j8
RUN make install
RUN ldconfig
RUN wget https://github.com/tesseract-ocr/tessdata_fast/raw/main/eng.traineddata \
    -P /usr/local/share/tessdata
RUN wget https://github.com/tesseract-ocr/tessdata_fast/raw/main/deu.traineddata \
    -P /usr/local/share/tessdata

CMD [ "/bin/bash" ]
