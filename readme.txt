같은 보드로 실행하는 경우 client.c 파일에서 connect하는 ip를 127.0.0.1로 변경해야한다.

main.py를 실행하기 위해서는 python3.7 버전이 필요하며, 라이브러리로는 opencv, tesseract4, pytesseract의 설치가 필요하다. 
tesseract의 경우 https://github.com/thortex/rpi3-tesseract를 통해 설치하면 된다.
tessdata는 따로 찾아서 wget으로 받아야한다. 위의 링크에서도 다운로드 가능하지만 내용이 잘려서 다운된다.
나머지 라이브러리는 pip3을 통해 설치가능하다

server 실행 후 clinet를 실행시켜야 작동확인이 가능하다.
