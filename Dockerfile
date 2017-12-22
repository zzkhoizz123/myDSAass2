FROM rikorose/gcc-cmake

COPY . /dsa171a2/

# get zip to make it work with script
RUN   cd /dsa171a2/ \
      # get all files needed to build
      && curl "https://raw.githubusercontent.com/saitamandd/DSA171-A02/master/main.cpp" > main.cpp \
      && g++ main.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ dbLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ requestLib.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      && g++ processData.cpp -c -std=c++11 -DAVL_USE_HEIGHT \
      # link the built files
      && g++ main.o dbLib.o requestLib.o processData.o -std=c++11 -DAVL_USE_HEIGHT -o /dsa171a2/dsa171a2 \
	  && curl "https://media.githubusercontent.com/media/nganhkhoa/CTDL-GT-ASS1-FINAL/master/resource/data_origin.csv" > data.csv \
      && ./dsa171a2 event.txt data.csv

ENTRYPOINT ["/dsa171a2/dsa171a2"]

CMD [ "/dsa171a2/in/request.txt", "/dsa171a2/in/data.csv" ]
