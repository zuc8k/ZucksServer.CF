FROM alpine:3.20 AS builder
RUN apk add --no-cache cmake g++ git make linux-headers
WORKDIR /zucks
COPY . .
RUN cmake -B build -DZUCKS_BUILD_TESTS=OFF && cmake --build build

FROM alpine:3.20
RUN apk add --no-cache libstdc++
WORKDIR /zucks
COPY --from=builder /zucks/build/zucks-server .
COPY config/ ./config/
EXPOSE 28888
CMD ["./zucks-server"]