sudo docker buildx build --platform linux/amd64 . -t trx_tools
sudo docker run -it --rm -v .:/app --name run-trx trx_tools bash -c "cd /app ; ./dev.sh"
