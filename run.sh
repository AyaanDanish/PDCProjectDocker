clear
docker rm -vf $(docker ps -aq) 
docker rmi -f $(docker images -aq) 
docker build -t dockergcc . 
clear
docker run -it dockergcc