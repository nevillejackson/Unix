# Use the latest Alpine parent image
FROM alpine
# set th working directory inside the container
WORKDIR /home/Folli
# copy current project directory to workdir
COPY . /home/Folli
# install support packages
RUN apk add make && \
    apk add gcc && \
    apk add libc-dev && \
    make 
# run the test script
CMD ["sh -ex /home/Folli/folli.scr >&/home/Folli/out"]

