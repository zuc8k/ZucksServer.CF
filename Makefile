# Zucks Server - Makefile
.PHONY: build run test clean docker

build:
	./scripts/build.sh

run:
	./scripts/run.sh

test:
	ctest --test-dir build --output-on-failure

clean:
	./scripts/clean.sh

docker:
	docker compose -f docker/docker-compose.yml up -d --build