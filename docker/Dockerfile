# Zucks Server - Dockerfile
FROM alpine:3.20 AS builder
RUN apk add --no-cache cmake g++ git make linux-headers bash
WORKDIR /zucks
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DZUCKS_BUILD_TESTS=OFF \
    && cmake --build build -j

FROM alpine:3.20
RUN apk add --no-cache libstdc++
WORKDIR /zucks
COPY --from=builder /zucks/build/zucks-server .
COPY config/ ./config/
EXPOSE 28888
ENTRYPOINT ["./zucks-server"]
CMD ["config/zucks_config.json"]