docker run --name testContainer -d --rm --mount type=bind,source="C:\Users\garre\CS_STUFF\cs3505\project\pictures",target="/pictures" myffmpeg bash /cs3505/thing.sh
docker container wait testContainer
TIMEOUT /T 1800
