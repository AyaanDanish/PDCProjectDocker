FROM gcc:latest
COPY . /app
WORKDIR /app
CMD ["./compile.sh"]